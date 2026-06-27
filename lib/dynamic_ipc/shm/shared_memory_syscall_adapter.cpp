/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */
#include "shared_memory_syscall_adapter.h"
#include "shm_error.h"
#include <cstdio>
#include "dxrt/common.h"

// ============================================================
// Windows implementation (CreateFileMapping / MapViewOfFile)
// ============================================================
#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <string>
#include <atomic>

namespace {

// Query the size of a file-mapping section using NtQuerySection.
// ntdll.dll is always loaded in Win32 processes, so dynamic lookup is safe.
// (The official winternl.h declaration matches this struct layout.)
size_t NtGetSectionSize(HANDLE hMapping) {
    typedef LONG (WINAPI *PFNNtQuerySection)(HANDLE, int, PVOID, ULONG, PULONG);
    static PFNNtQuerySection pfn = nullptr;
    static bool loaded = false;
    if (!loaded) {
        HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
        if (hNtdll) {
            pfn = reinterpret_cast<PFNNtQuerySection>(
                GetProcAddress(hNtdll, "NtQuerySection"));
        }
        loaded = true;
    }
    if (!pfn) return 0;

    // SECTION_BASIC_INFORMATION (SectionBasicInformation = 0)
    struct SectionBasicInfo {
        PVOID         BaseAddress;
        ULONG         AllocationAttributes;
        LARGE_INTEGER MaximumSize;
    } info = {};
    ULONG retLen = 0;
    if (pfn(hMapping, 0, &info, static_cast<ULONG>(sizeof(info)), &retLen) != 0) {
        return 0;
    }
    return static_cast<size_t>(info.MaximumSize.QuadPart);
}

inline HANDLE ToWinHandle(dxrt::shm::MemFDHandle fd) {
    return reinterpret_cast<HANDLE>(static_cast<LONG_PTR>(fd));
}

} // anonymous namespace

namespace dxrt {
namespace shm {

MemFDHandle SharedMemorySyscallAdapter::CreateMemFD(size_t size, int /*flags*/) {
    if (size == 0 || size > (static_cast<size_t>(1) << 40)) {
        throw MemFDException(MemFDErrorCode::INVALID_SIZE, "Invalid size: 0 or > 1TB");
    }

    DWORD high = static_cast<DWORD>(size >> 32);
    DWORD low  = static_cast<DWORD>(size & 0xFFFFFFFFUL);

    // INVALID_HANDLE_VALUE -> pagefile-backed anonymous section (no file backing).
    // The flags argument has no meaning on Windows, so it is ignored.
    HANDLE hMapping = CreateFileMappingA(
        INVALID_HANDLE_VALUE,   // pagefile-backed
        nullptr,                // default security attributes
        PAGE_READWRITE,
        high, low,
        nullptr                 // no name (anonymous)
    );
    if (hMapping == nullptr || hMapping == INVALID_HANDLE_VALUE) {
        throw MemFDException(MemFDErrorCode::CREATE_FAILED,
            "CreateFileMapping failed, error=" + std::to_string(GetLastError()));
    }

    return static_cast<MemFDHandle>(reinterpret_cast<LONG_PTR>(hMapping));
}

bool SharedMemorySyscallAdapter::OpenMemFD(MemFDHandle fd) {
    if (fd == kInvalidMemFDHandle) return false;
    DWORD flags = 0;
    return GetHandleInformation(ToWinHandle(fd), &flags) != FALSE;
}

void* SharedMemorySyscallAdapter::MapMemFD(MemFDHandle fd, size_t size, size_t offset) {
    if (fd == kInvalidMemFDHandle || size == 0) {
        throw MemFDException(MemFDErrorCode::INVALID_ADDRESS, "Invalid handle or size");
    }

    DWORD offsetHigh = static_cast<DWORD>(offset >> 32);
    DWORD offsetLow  = static_cast<DWORD>(offset & 0xFFFFFFFFUL);

    void* addr = MapViewOfFile(
        ToWinHandle(fd),
        FILE_MAP_ALL_ACCESS,
        offsetHigh, offsetLow,
        size
    );
    if (!addr) {
        throw MemFDException(MemFDErrorCode::MMAP_FAILED,
            "MapViewOfFile failed, error=" + std::to_string(GetLastError()));
    }
    return addr;
}

bool SharedMemorySyscallAdapter::UnmapMemFD(void* addr, size_t /*size*/) {
    if (!addr) return false;
    if (!UnmapViewOfFile(addr)) {
        LOG_DXRT_ERR("UnmapViewOfFile failed: " + std::to_string(GetLastError()));
        return false;
    }
    return true;
}

bool SharedMemorySyscallAdapter::CloseMemFD(MemFDHandle fd) {
    if (fd == kInvalidMemFDHandle) return false;
    if (!CloseHandle(ToWinHandle(fd))) {
        LOG_DXRT_ERR("CloseHandle failed: " + std::to_string(GetLastError()));
        return false;
    }
    return true;
}

bool SharedMemorySyscallAdapter::ResizeMemFD(MemFDHandle /*fd*/, size_t /*new_size*/) {
    // Sections created by CreateFileMapping cannot be resized.
    // Recreating would invalidate existing MapViewOfFile pointers, so this layer does not support it.
    LOG_DXRT_ERR("ResizeMemFD: not supported on Windows");
    return false;
}

bool SharedMemorySyscallAdapter::SealMemFD(MemFDHandle /*fd*/) {
    // Windows has no API equivalent to memfd sealing.
    // Since CreateFileMapping fixes the size at creation, F_SEAL_GROW|F_SEAL_SHRINK
    // are effectively already enforced.
    return true;
}

size_t SharedMemorySyscallAdapter::GetMemFDSize(MemFDHandle fd) {
    if (fd == kInvalidMemFDHandle) return 0;
    return NtGetSectionSize(ToWinHandle(fd));
}

bool SharedMemorySyscallAdapter::SyncMemory(void* addr, size_t size) {
    // FlushViewOfFile flushes dirty pages to the backing store (pagefile).
    // On x86_64/ARM64 Windows, DMA is coherent, so extra cache flush is unnecessary.
    if (!addr || size == 0) return false;
    return FlushViewOfFile(addr, size) != FALSE;
}

bool SharedMemorySyscallAdapter::InvalidateMemory(void* addr, size_t size) {
    // Windows memory model is coherent; explicit invalidation is unnecessary.
    (void)addr;
    (void)size;
    return true;
}

} // namespace shm
} // namespace dxrt

// ============================================================
// Linux / POSIX implementation (memfd_create / mmap)
// ============================================================
#elif defined(__linux__)

#include <atomic>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <string>
#include <atomic>
#include <iostream>
#include <cstdlib>

#if __has_include(<linux/memfd.h>)
#include <linux/memfd.h>
#endif

namespace {
bool shouldFault(const char *name)
{
    const char *value = std::getenv(name);
    return value != nullptr && value[0] != '\0' && value[0] != '0';
}

int MemfdCreateCompat(const char* name, unsigned int flags) {
#if defined(SYS_memfd_create)
    return static_cast<int>(syscall(SYS_memfd_create, name, flags));
#else
    (void)name;
    (void)flags;
    errno = ENOSYS;
    return -1;
#endif
}
} // anonymous namespace

namespace dxrt {
namespace shm {

MemFDHandle SharedMemorySyscallAdapter::CreateMemFD(size_t size, int flags) {
    if (size == 0 || size > (1UL << 40)) {
        throw MemFDException(MemFDErrorCode::INVALID_SIZE, "Invalid size: 0 or > 1TB");
    }

    if (shouldFault("DXRT_FAULT_MEMFD_CREATE")) {
        errno = EIO;
        throw MemFDException(MemFDErrorCode::CREATE_FAILED, errno);
    }


    int mfd_flags = MFD_CLOEXEC | MFD_ALLOW_SEALING;
#ifdef MFD_NOEXEC_SEAL
    static std::atomic<bool> noexec_supported{true};
    if (noexec_supported.load()) {
        mfd_flags |= MFD_NOEXEC_SEAL;
    }
#endif
    if (flags != 0) {
        mfd_flags |= flags;
    }

    // Generate a unique name (for debugging; visible under /proc/pid/fd/).
    static std::atomic<int> counter{0};
    std::string name = "memfd_" + std::to_string(getpid()) + "_" +
                       std::to_string(counter.fetch_add(1));

    int fd = MemfdCreateCompat(name.c_str(), static_cast<unsigned int>(mfd_flags));
#ifdef MFD_NOEXEC_SEAL
    if (((fd == -1) && (errno == EINVAL) && (noexec_supported.load() == true)))
    {
        // failback: retry without MFD_NOEXEC_SEAL
        LOG_DXRT_DBG << "memfd_create with MFD_NOEXEC_SEAL failed, retrying without it" << std::endl;
        noexec_supported.store(false);
        mfd_flags &= ~MFD_NOEXEC_SEAL;
        fd = MemfdCreateCompat(name.c_str(), static_cast<unsigned int>(mfd_flags));
    }
#endif
    if (fd < 0) {
        throw MemFDException(MemFDErrorCode::CREATE_FAILED, errno);
    }

    if (ftruncate(fd, static_cast<off_t>(size)) < 0) {
        int err = errno;
        close(fd);
        throw MemFDException(MemFDErrorCode::RESIZE_FAILED, err);
    }

    if (shouldFault("DXRT_FAULT_MEMFD_FTRUNCATE")) {
        int err = EIO;
        close(fd);
        throw MemFDException(MemFDErrorCode::RESIZE_FAILED, err);
    }

    return static_cast<MemFDHandle>(fd);
}

bool SharedMemorySyscallAdapter::OpenMemFD(MemFDHandle fd) {
    if (fd < 0) return false;
    if (shouldFault("DXRT_FAULT_MEMFD_OPEN")) {
        return false;
    }
    struct stat sb;
    return fstat(static_cast<int>(fd), &sb) == 0;
}

void* SharedMemorySyscallAdapter::MapMemFD(MemFDHandle fd, size_t size, size_t offset) {
    if (fd < 0 || size == 0) {
        throw MemFDException(MemFDErrorCode::INVALID_ADDRESS, "Invalid fd or size");
    }

    if (shouldFault("DXRT_FAULT_MEMFD_MMAP")) {
        errno = ENOMEM;
        throw MemFDException(MemFDErrorCode::MMAP_FAILED, errno);
    }

    void* addr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED,
                      static_cast<int>(fd), static_cast<off_t>(offset));
    if (addr == MAP_FAILED) {
        throw MemFDException(MemFDErrorCode::MMAP_FAILED, errno);
    }
    return addr;
}

bool SharedMemorySyscallAdapter::UnmapMemFD(void* addr, size_t size) {
    if (!addr || size == 0) return false;
    if (shouldFault("DXRT_FAULT_MEMFD_MUNMAP")) {
        errno = EIO;
        return false;
    }
    if (munmap(addr, size) < 0) {
        LOG_DXRT_ERR(std::string("munmap failed: ") + strerror(errno));
        return false;
    }
    return true;
}

bool SharedMemorySyscallAdapter::CloseMemFD(MemFDHandle fd) {
    if (fd < 0) return false;
    if (shouldFault("DXRT_FAULT_MEMFD_CLOSE")) {
        errno = EBADF;
        return false;
    }
    if (close(static_cast<int>(fd)) < 0) {
        LOG_DXRT_ERR(std::string("close failed: ") + strerror(errno));
        return false;
    }
    return true;
}

bool SharedMemorySyscallAdapter::ResizeMemFD(MemFDHandle fd, size_t new_size) {
    if (fd < 0 || new_size == 0) return false;
    if (shouldFault("DXRT_FAULT_MEMFD_RESIZE")) {
        errno = EIO;
        return false;
    }
    if (ftruncate(static_cast<int>(fd), static_cast<off_t>(new_size)) < 0) {
        LOG_DXRT_ERR(std::string("ftruncate failed: ") + strerror(errno));
        return false;
    }
    return true;
}

bool SharedMemorySyscallAdapter::SealMemFD(MemFDHandle fd){
    constexpr unsigned int seals = F_SEAL_GROW | F_SEAL_SHRINK;
    if (fd < 0) return false;
    if (shouldFault("DXRT_FAULT_MEMFD_SEAL")) {
        errno = EIO;
        return false;
    }
    if (fcntl(static_cast<int>(fd), F_ADD_SEALS, seals) < 0) {
        LOG_DXRT_ERR(std::string("fcntl F_ADD_SEALS failed: ") + strerror(errno));
        return false;
    }
    return true;
}

size_t SharedMemorySyscallAdapter::GetMemFDSize(MemFDHandle fd) {
    if (fd < 0) return 0;
    if (shouldFault("DXRT_FAULT_MEMFD_SIZE")) {
        return 0;
    }
    struct stat sb;
    if (fstat(static_cast<int>(fd), &sb) < 0) return 0;
    return static_cast<size_t>(sb.st_size);
}

bool SharedMemorySyscallAdapter::SyncMemory(void* addr, size_t size) {
    // CPU cache -> main memory direction (call before NPU DMA reads CPU-written data).
    //
    // msync() flushes kernel page cache -> file backing storage,
    // which is unrelated to CPU L1/L2 cache -> main memory. Because memfd is tmpfs-based,
    // msync() is effectively a no-op here.
    //
    //   x86_64: DMA is cache-coherent (IOMMU), so explicit flush is unnecessary.
    //   ARM non-coherent NPU: cacheflush(2) or dma-buf IOCTL sync is required.
    (void)addr;
    (void)size;
    return true;
}

bool SharedMemorySyscallAdapter::InvalidateMemory(void* addr, size_t size) {
    // Invalidate CPU cache (call before CPU reads data written by NPU DMA).
    // Symmetric with SyncMemory; same platform dependencies apply.
    (void)addr;
    (void)size;
    return true;
}

}  // namespace shm
}  // namespace dxrt

#else
#error "Unsupported platform"
#endif  // platform check
