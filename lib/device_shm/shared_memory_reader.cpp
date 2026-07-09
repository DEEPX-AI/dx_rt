/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "shared_memory_reader.h"
#include "dxrt/common.h"
#include <cstdint>
#include <cerrno>

#ifdef __linux__
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <csignal>
#elif _WIN32
#include <windows.h>
#endif

namespace dxrt {

constexpr int MAX_SEQLOCK_RETRIES = 1000;


SharedMemoryReader::SharedMemoryReader() = default;

SharedMemoryReader::~SharedMemoryReader()
{
    Close();
}

bool SharedMemoryReader::Open()
{
    if (_opened)
    {
        return true;
    }

#ifdef __linux__
    const char* shm_name = GetMonitorShmName();

    // Preferred path: attach to existing SHM as read-only.
    _shm_fd = shm_open(shm_name, O_RDONLY, 0);

    // reader-first fallback: create and initialize SHM once if it doesn't exist.
    if (_shm_fd == -1 && errno == ENOENT)
    {
        int bootstrap_fd = shm_open(shm_name, O_CREAT | O_EXCL | O_RDWR, MONITOR_SHM_PERMS);
        if (bootstrap_fd != -1)
        {
            // shm_open honors umask, so force the intended permissions.
            // Failure is non-fatal here (bootstrap still proceeds), but log it
            // so we can diagnose subsequent EACCES from other readers/writers.
            if (fchmod(bootstrap_fd, MONITOR_SHM_PERMS) == -1)
            {
                LOG_DXRT_DBG << "fchmod on bootstrap shared memory failed: "
                             << shm_name << " (" << strerror(errno) << ")";
            }
            if (ftruncate(bootstrap_fd, sizeof(MonitorSharedMemory)) == -1)
            {
                LOG_DXRT_ERR("Failed to set shared memory size during bootstrap: "
                             << shm_name << " (" << strerror(errno) << ")");
                close(bootstrap_fd);
                return false;
            }

            void* bootstrap_ptr = mmap(nullptr, sizeof(MonitorSharedMemory),
                                       PROT_READ | PROT_WRITE, MAP_SHARED, bootstrap_fd, 0);
            if (bootstrap_ptr == MAP_FAILED)
            {
                LOG_DXRT_ERR("Failed to map shared memory during bootstrap: "
                             << shm_name << " (" << strerror(errno) << ")");
                close(bootstrap_fd);
                return false;
            }

            auto* bootstrap_shm = static_cast<MonitorSharedMemory*>(bootstrap_ptr);
            new (bootstrap_shm) MonitorSharedMemory();

            munmap(bootstrap_ptr, sizeof(MonitorSharedMemory));
            close(bootstrap_fd);

            _shm_fd = shm_open(shm_name, O_RDONLY, 0);
        }
        else if (errno == EEXIST)
        {
            // Another process created it first; attach read-only.
            _shm_fd = shm_open(shm_name, O_RDONLY, 0);
        }
    }

    if (_shm_fd == -1)
    {
        if (errno != ENOENT)
        {
            LOG_DXRT_ERR("Failed to open shared memory: " << shm_name
                         << " (" << strerror(errno) << ")");
        }
        return false;
    }

    // Map to memory (read-only)
    _shm_ptr = mmap(nullptr, sizeof(MonitorSharedMemory),
                    PROT_READ, MAP_SHARED, _shm_fd, 0);

    if (_shm_ptr == MAP_FAILED)
    {
        LOG_DXRT_ERR("Failed to map shared memory");
        close(_shm_fd);
        _shm_fd = -1;
        _shm_ptr = nullptr;
        return false;
    }

    // Verify magic number and version
    auto* shm = static_cast<const MonitorSharedMemory*>(_shm_ptr);
    if (shm->magic != MONITOR_SHM_MAGIC)
    {
        LOG_DXRT_ERR("Invalid shared memory magic number");
        munmap(_shm_ptr, sizeof(MonitorSharedMemory));
        close(_shm_fd);
        _shm_fd = -1;
        _shm_ptr = nullptr;
        return false;
    }
    if (shm->version != MONITOR_SHM_VERSION)
    {
        LOG_DXRT_ERR("Invalid shared memory version");
        munmap(_shm_ptr, sizeof(MonitorSharedMemory));
        close(_shm_fd);
        _shm_fd = -1;
        _shm_ptr = nullptr;
        return false;
    }

    _opened = true;
    return true;

#elif _WIN32
    // Opening an existing Global\ object needs no special privilege (just FILE_MAP_READ
    // on its DACL), so try Global\ first — this is the common case when the writer is a
    // service/SYSTEM process.  Fall back to Local\ if the Global\ segment doesn't exist.
    const char* env_override = std::getenv("DXRT_MONITOR_SHM_NAME");
    const bool use_env = (env_override != nullptr && env_override[0] != '\0');

    HANDLE ro_handle = nullptr;
    if (use_env)
    {
        ro_handle = OpenFileMappingA(FILE_MAP_READ, FALSE, env_override);
    }
    else
    {
        ro_handle = OpenFileMappingA(FILE_MAP_READ, FALSE, MONITOR_SHM_NAME_WIN_GLOBAL);
        if (ro_handle == nullptr)
            ro_handle = OpenFileMappingA(FILE_MAP_READ, FALSE, MONITOR_SHM_NAME_WIN_LOCAL);
    }

    if (ro_handle == nullptr)
    {
        // dxtop-first 시나리오: Writer 없이 Reader가 먼저 시작된 경우.
        // Bootstrap with Global\ if we have privilege, otherwise Local\.
        LOG_DXRT_DBG << "Shared memory doesn't exist, creating placeholder for Writer..."
                     << std::endl;

        const char* create_name = use_env ? env_override : MONITOR_SHM_NAME_WIN_GLOBAL;
        HANDLE init_handle = CreateFileMappingA(
            INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE,
            0, static_cast<DWORD>(sizeof(MonitorSharedMemory)),
            create_name);

        if (init_handle == nullptr && !use_env)
        {
            // Fall back to Local\ for bootstrap placeholder.
            create_name = MONITOR_SHM_NAME_WIN_LOCAL;
            init_handle = CreateFileMappingA(
                INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE,
                0, static_cast<DWORD>(sizeof(MonitorSharedMemory)),
                create_name);
        }

        if (init_handle == nullptr)
        {
            LOG_DXRT_ERR("Failed to create shared memory placeholder, error=" << GetLastError());
            return false;
        }

        auto* init_ptr = static_cast<MonitorSharedMemory*>(
            MapViewOfFile(init_handle, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(MonitorSharedMemory)));

        if (init_ptr == nullptr)
        {
            LOG_DXRT_ERR("Failed to map shared memory for initialization, error=" << GetLastError());
            CloseHandle(init_handle);
            return false;
        }

        // writer_pid = 0: 아직 Writer 없음
        new (init_ptr) MonitorSharedMemory();
        UnmapViewOfFile(init_ptr);

        // init_handle이 열린 상태에서 read-only 핸들 획득 후 init_handle 해제
        ro_handle = OpenFileMappingA(FILE_MAP_READ, FALSE, create_name);
        CloseHandle(init_handle);

        if (ro_handle == nullptr)
        {
            LOG_DXRT_ERR("Failed to reopen shared memory as read-only, error=" << GetLastError());
            return false;
        }
    }

    _shm_handle = ro_handle;

    _shm_ptr = MapViewOfFile(
        static_cast<HANDLE>(_shm_handle),
        FILE_MAP_READ,
        0, 0,
        sizeof(MonitorSharedMemory)
    );

    if (_shm_ptr == nullptr)
    {
        LOG_DXRT_ERR("Failed to map view of shared memory, error=" << GetLastError());
        CloseHandle(static_cast<HANDLE>(_shm_handle));
        _shm_handle = nullptr;
        return false;
    }

    // magic/version 검증
    auto* shm = static_cast<const MonitorSharedMemory*>(_shm_ptr);
    if (shm->magic != MONITOR_SHM_MAGIC)
    {
        LOG_DXRT_ERR("Invalid shared memory magic number");
        UnmapViewOfFile(_shm_ptr);
        CloseHandle(static_cast<HANDLE>(_shm_handle));
        _shm_handle = nullptr;
        _shm_ptr = nullptr;
        return false;
    }
    if (shm->version != MONITOR_SHM_VERSION)
    {
        LOG_DXRT_ERR("Invalid shared memory version");
        UnmapViewOfFile(_shm_ptr);
        CloseHandle(static_cast<HANDLE>(_shm_handle));
        _shm_handle = nullptr;
        _shm_ptr = nullptr;
        return false;
    }

    _opened = true;
    return true;

#else
    return false;
#endif
}

void SharedMemoryReader::Close()
{
    if (!_opened)
    {
        return;
    }

#ifdef __linux__
    if (_shm_ptr != nullptr && _shm_ptr != MAP_FAILED)
    {
        munmap(_shm_ptr, sizeof(MonitorSharedMemory));
        _shm_ptr = nullptr;
    }

    if (_shm_fd != -1)
    {
        close(_shm_fd);
        _shm_fd = -1;
    }
#elif _WIN32
    if (_shm_ptr != nullptr)
    {
        UnmapViewOfFile(_shm_ptr);
        _shm_ptr = nullptr;
    }

    if (_shm_handle != nullptr)
    {
        CloseHandle(static_cast<HANDLE>(_shm_handle));
        _shm_handle = nullptr;
    }
    // Windows: 모든 핸들/뷰 해제 시 자동 소멸 (shm_unlink 불필요)
#endif

    _opened = false;
}

bool SharedMemoryReader::ReadDeviceData(int deviceId, MonitorDeviceData& outData) const
{
    if (!_opened || _shm_ptr == nullptr)
    {
        return false;
    }

    auto* shm = static_cast<const MonitorSharedMemory*>(_shm_ptr);

    // Use sequence lock to ensure consistent read
    uint64_t seq1;
    uint64_t seq2;
    int retries = 0;
    do {
        // Read sequence before data
        seq1 = shm->sequence.load(std::memory_order_acquire);

        // If sequence is odd, writer is updating - retry
        if (seq1 & 1)
        {
            if (++retries > MAX_SEQLOCK_RETRIES)
            {
                LOG_DXRT_DBG << "ReadDeviceData: seqlock retry limit reached, writer may have crashed" << std::endl;
                return false;
            }
            continue;
        }
        retries = 0;

        // Find and copy device data
        bool found = false;
        for (uint32_t i = 0; i < shm->device_count; ++i)
        {
            if (shm->devices[i].device_id == static_cast<uint32_t>(deviceId))
            {
                outData = shm->devices[i];
                found = true;
                break;
            }
        }

        // Read sequence after data
        seq2 = shm->sequence.load(std::memory_order_acquire);

        // If sequences match and even, data is consistent
        if (seq1 == seq2)
        {
            return found;
        }

        // Otherwise, writer updated during read - retry
    } while (true);
}

bool SharedMemoryReader::GetAllDevices(MonitorDeviceData* outDevices, uint32_t& outCount, uint32_t maxCount) const
{
    if (!_opened || _shm_ptr == nullptr || outDevices == nullptr)
    {
        return false;
    }

    auto* shm = static_cast<const MonitorSharedMemory*>(_shm_ptr);

    // Use sequence lock to ensure consistent read
    uint64_t seq1;
    uint64_t seq2;
    int retries = 0;
    do {
        // Read sequence before data
        seq1 = shm->sequence.load(std::memory_order_acquire);

        // If sequence is odd, writer is updating - retry
        if (seq1 & 1)
        {
            if (++retries > MAX_SEQLOCK_RETRIES)
            {
                LOG_DXRT_DBG << "GetAllDevices: seqlock retry limit reached, writer may have crashed" << std::endl;
                return false;
            }
            continue;
        }
        retries = 0;

        // Copy device data
        auto count = shm->device_count;
        if (count > maxCount)
        {
            count = maxCount;
        }

        for (uint32_t i = 0; i < count; ++i)
        {
            outDevices[i] = shm->devices[i];
        }

        // Read sequence after data
        seq2 = shm->sequence.load(std::memory_order_acquire);

        // If sequences match and even, data is consistent
        if (seq1 == seq2)
        {
            outCount = count;
            return true;
        }

        // Otherwise, writer updated during read - retry
    } while (true);

}

uint32_t SharedMemoryReader::GetDeviceCount() const
{
    if (!_opened || _shm_ptr == nullptr)
    {
        return 0;
    }

    auto* shm = static_cast<const MonitorSharedMemory*>(_shm_ptr);

    uint64_t seq1;
    uint64_t seq2 = 0;
    uint32_t count = 0;
    int retries = 0;
    do
    {
        seq1 = shm->sequence.load(std::memory_order_acquire);
        if (seq1 & 1)
        {
            if (++retries > MAX_SEQLOCK_RETRIES)
            {
                LOG_DXRT_DBG << "GetDeviceCount: seqlock retry limit reached, writer may have crashed" << std::endl;
                return 0;
            }
            continue;
        }
        retries = 0;

        count = shm->device_count;

        seq2 = shm->sequence.load(std::memory_order_acquire);
    } while (seq1 != seq2);

    return count;
}

bool SharedMemoryReader::IsWriterAlive() const
{
    if (!_opened || _shm_ptr == nullptr)
    {
        return false;
    }

#ifdef __linux__
    auto* shm = static_cast<const MonitorSharedMemory*>(_shm_ptr);
    // Check if writer process is still running
    auto writer_pid = static_cast<pid_t>(shm->writer_pid);
    if (writer_pid == 0)
    {
        return false;
    }

    // Send signal 0 to check if process exists
    if (kill(writer_pid, 0) == 0)
    {
        return true;
    }

    // EPERM means process exists but we lack permission (e.g., root-owned service)
    if (errno == EPERM)
    {
        return true;
    }

    return false;
#elif _WIN32
    auto* shm = static_cast<const MonitorSharedMemory*>(_shm_ptr);
    DWORD writer_pid = static_cast<DWORD>(shm->writer_pid);
    if (writer_pid == 0)
    {
        return false;
    }

    HANDLE process_handle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, writer_pid);
    if (process_handle == nullptr)
    {
        return false;
    }

    DWORD exit_code = 0;
    bool alive = GetExitCodeProcess(process_handle, &exit_code) && (exit_code == STILL_ACTIVE);
    CloseHandle(process_handle);
    return alive;
#else
    return false;
#endif
}

uint32_t SharedMemoryReader::GetWriterPid() const
{
    if (!_opened || _shm_ptr == nullptr)
    {
        return 0;
    }

    auto* shm = static_cast<const MonitorSharedMemory*>(_shm_ptr);
    return shm->writer_pid;
}

uint64_t SharedMemoryReader::GetUpdateCount() const
{
    if (!_opened || _shm_ptr == nullptr)
    {
        return 0;
    }

    auto* shm = static_cast<const MonitorSharedMemory*>(_shm_ptr);
    return shm->update_count;
}

} // namespace dxrt
