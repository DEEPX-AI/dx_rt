/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/common.h"
#include "dxrt/fixed_size_buffer.h"
#include <chrono>
#include <cstdlib>
#include <stdexcept>
#include <cstring>  // for strerror
#ifdef USE_VNPU
#include "rk_mpi_sys.h"  // For RK_MPI_SYS_MmzFlushCache
#include "rk_mpi_mmz.h"  // For RK_MPI_MMZ_FlushCache* functions
#include "rk_type.h"
#include "cc_pcie_type.h"  // For EBuff_Type, E_SEND, E_RECV

extern "C" {
    #include "rk_pcie_ep.h"

    // rk_pcie_import_dma_fd is provided by libdx_vnpu_core_ep.so but not declared in headers
    RK_S32 rk_pcie_import_dma_fd(int task_id, EBuff_Type dir, int index, int fd);
}
#endif

static constexpr int MEM_ALIGN_VALUE = 4096;

namespace dxrt {

namespace {
std::chrono::seconds GetBufferWaitTimeout()
{
    const char* env = std::getenv("DXRT_FIXEDSIZEBUFFER_WAIT_TIMEOUT_SEC");
    if (env == nullptr) {
        return std::chrono::seconds(3600);
    }

    char* end = nullptr;
    const long value = std::strtol(env, &end, 10);
    if (end == env || value < 0) {
        return std::chrono::seconds(3600);
    }
    return std::chrono::seconds(value);
}
}  // namespace

#ifndef USE_VNPU
FixedSizeBuffer::FixedSizeBuffer(int64_t size, int buffer_count)
:  _count(buffer_count), _size(size)
{
    std::unique_lock<std::mutex> lock(_lock);

    _pointers.reserve(_count);
    for (int i = 0; i < _count; i++)
    {
        void* ptr = nullptr;
#ifdef __linux__
        int result = posix_memalign(&ptr, MEM_ALIGN_VALUE, size);
        if (result != 0) {
            LOG_DXRT_ERR("Failed to posix_memalign: error=" << result << " (" << strerror(result) << ")"
                         << ", buffer_index=" << i << "/" << _count
                         << ", size=" << size << " bytes (" << (size / 1024.0 / 1024.0) << " MB)"
                         << ", alignment=" << MEM_ALIGN_VALUE);
            DXRT_ASSERT(false, "Memory allocation failed - check system memory availability");
        }
#elif _WIN32
        ptr = _aligned_malloc(size, MEM_ALIGN_VALUE);
        if (ptr == nullptr) {
            LOG_DXRT_ERR("Failed to _aligned_malloc: buffer_index=" << i << "/" << _count
                         << ", size=" << size << " bytes (" << (size / 1024.0 / 1024.0) << " MB)"
                         << ", alignment=" << MEM_ALIGN_VALUE);
            DXRT_ASSERT(false, "Memory allocation failed - check system memory availability");
        }
#else
        ptr = aligned_alloc(MEM_ALIGN_VALUE, size);
        if (ptr == nullptr) {
            LOG_DXRT_ERR("Failed to aligned_alloc: buffer_index=" << i << "/" << _count
                         << ", size=" << size << " bytes (" << (size / 1024.0 / 1024.0) << " MB)"
                         << ", alignment=" << MEM_ALIGN_VALUE);
            DXRT_ASSERT(false, "Memory allocation failed - check system memory availability");
        }
#endif
        _data.push_back(ptr);
        _pointers.push_back(ptr);
    }
}

FixedSizeBuffer::~FixedSizeBuffer()
{
    std::unique_lock<std::mutex> lock(_lock);
    _pointers.clear();  // Clear to signal destruction to waiting threads
    _cv.notify_all();  // Wake all waiting threads before cleanup
    for (void* ptr : _data)
    {
#ifdef __linux__
        free(ptr);
#elif _WIN32
        _aligned_free(ptr);
#else
        free(ptr);
#endif
    }
}

#else
FixedSizeBuffer::FixedSizeBuffer(int64_t size, int buffer_count, BufferAllocType allocType, BufferDirection direction)
:  _allocType(allocType), _direction(direction), _count(buffer_count), _size(size)
{
    LOG_DXRT_DBG << "[FixedSizeBuffer Constructor] size=" << size << ", count=" << buffer_count
             << ", allocType=" << (allocType == BufferAllocType::HEAP ? "HEAP" : "CMA_DMA")
             << ", direction=" << (direction == BufferDirection::INPUT ? "INPUT" : "OUTPUT") << std::endl;

    std::unique_lock<std::mutex> lock(_lock);
    _pointers.reserve(_count);

    if (_allocType == BufferAllocType::HEAP)
    {
        allocateHeapBuffers();
    }
    else
    {
        allocateCMABuffers();
    }
}

void FixedSizeBuffer::allocateHeapBuffers()
{
    LOG_DXRT_DBG << "[allocateHeapBuffers] CALLED! Allocating " << _count << " heap buffers of "
                 << (_size / 1024.0 / 1024.0) << " MB each" << std::endl;

    for (int i = 0; i < _count; i++)
    {
        void* ptr = nullptr;
#ifdef __linux__
        int result = posix_memalign(&ptr, MEM_ALIGN_VALUE, _size);
        if (result != 0) {
            LOG_DXRT_ERR("Failed to posix_memalign: error=" << result << " (" << strerror(result) << ")"
                         << ", buffer_index=" << i << "/" << _count
                         << ", size=" << _size << " bytes (" << (_size / 1024.0 / 1024.0) << " MB)"
                         << ", alignment=" << MEM_ALIGN_VALUE);
            DXRT_ASSERT(false, "Memory allocation failed - check system memory availability");
        }
#elif _WIN32
        ptr = _aligned_malloc(_size, MEM_ALIGN_VALUE);
        if (ptr == nullptr) {
            LOG_DXRT_ERR("Failed to _aligned_malloc: buffer_index=" << i << "/" << _count
                         << ", size=" << _size << " bytes (" << (_size / 1024.0 / 1024.0) << " MB)"
                         << ", alignment=" << MEM_ALIGN_VALUE);
            DXRT_ASSERT(false, "Memory allocation failed - check system memory availability");
        }
#else
        ptr = aligned_alloc(MEM_ALIGN_VALUE, _size);
        if (ptr == nullptr) {
            LOG_DXRT_ERR("Failed to aligned_alloc: buffer_index=" << i << "/" << _count
                         << ", size=" << _size << " bytes (" << (_size / 1024.0 / 1024.0) << " MB)"
                         << ", alignment=" << MEM_ALIGN_VALUE);
            DXRT_ASSERT(false, "Memory allocation failed - check system memory availability");
        }
#endif
        _data.push_back(ptr);
        _pointers.push_back(ptr);
    }
}

void FixedSizeBuffer::allocateCMABuffers()
{
    LOG_DXRT_DBG << "[allocateCMABuffers] CALLED! Allocating " << _count << " CMA DMA buffers of "
             << (_size / 1024.0 / 1024.0) << " MB each" << std::endl;

    // Create CMA pool
    MB_POOL_CONFIG_S poolConfig = {};
    poolConfig.u64MBSize = _size;
    poolConfig.u32MBCnt = _count;
    poolConfig.enRemapMode = MB_REMAP_MODE_NONE;
    poolConfig.enAllocType = MB_ALLOC_TYPE_DMA;
    poolConfig.enDmaType = MB_DMA_TYPE_CMA;  // Physically contiguous
    poolConfig.bPreAlloc = RK_TRUE;  // Pre-allocate all buffers

    _cmaPoolId = RK_MPI_MB_CreatePool(&poolConfig);
    if (_cmaPoolId == MB_INVALID_POOLID) {
        LOG_DXRT_ERR("RK_MPI_MB_CreatePool failed: size=" << _size << ", count=" << _count);
        throw std::runtime_error("Failed to create CMA buffer pool");  // NOSONAR:S112
    }

    LOG_DXRT_DBG << "CMA pool created: pool_id=" << _cmaPoolId << std::endl;

    // Get both virtual and physical addresses, keep MB handles for cache flush
    _cmaPhysAddrs.reserve(_count);
    _cmaHandles.reserve(_count);

    for (int i = 0; i < _count; i++)
    {
        MB_BLK mb = RK_MPI_MB_GetMB(_cmaPoolId, _size, RK_TRUE);
        if (mb == MB_INVALID_HANDLE) {
            LOG_DXRT_ERR("RK_MPI_MB_GetMB failed: pool=" << _cmaPoolId << ", index=" << i);
            throw std::runtime_error("Failed to get MB from CMA pool");  // NOSONAR:S112
        }

        // Extract dmabuf fd for PCIe registration
        RK_S32 fd = RK_MPI_MB_Handle2Fd(mb);
        if (fd < 0) {
            LOG_DXRT_ERR("RK_MPI_MB_Handle2Fd failed: idx=" << i);
            RK_MPI_MB_ReleaseMB(mb);
            throw std::runtime_error("Failed to get fd from MB handle");  // NOSONAR:S112
        }

        // Register CMA buffer PCIe Import
#if 0
        std::cout << " _taskId=" << _RK_taskId << " , direction="
                  << ((_direction == BufferDirection::OUTPUT) ? "OUTPUT" : "INPUT")
                  << ", index=" << i << ", fd=" << fd << std::endl;

        if (_RK_taskId >= 0) {
            EBuff_Type dma_dir = (_direction == BufferDirection::OUTPUT) ? E_SEND : E_RECV;
            RK_S32 ret = rk_pcie_import_dma_fd(_RK_taskId, dma_dir, i, fd);
            if (ret != 0) {
                LOG_DXRT_ERR("rk_pcie_import_dma_fd (Host↔EP) failed: task=" << _RK_taskId
                            << ", dir=" << (dma_dir == E_SEND ? "E_SEND" : "E_RECV")
                            << ", idx=" << i << ", fd=" << fd << ", ret=" << ret);
                RK_MPI_MB_ReleaseMB(mb);
                throw std::runtime_error("Failed to register DMA buffer with Host↔EP PCIe");  // NOSONAR:S112
            }
            LOG_DXRT_DBG << "PCIe DMA registered (Host↔EP): task=" << _RK_taskId
                     << ", dir=" << (dma_dir == E_SEND ? "E_SEND" : "E_RECV")
                     << ", idx=" << i << ", fd=" << fd << std::endl;

            // Register CMA buffer with PCIe DMA for EP ↔ M1 using the same taskId
            // Direction is REVERSED from M1's perspective:
            // INPUT buffer (host→EP→M1): EP sends to M1, so E_SEND for M1
            // OUTPUT buffer (M1→EP→host): EP receives from M1, so E_RECV for M1
            // 2. EP <-> M1 등록
            EBuff_Type m1_dma_dir = (_direction == BufferDirection::OUTPUT) ? E_RECV : E_SEND;
            ret = rk_pcie_import_dma_fd(_RK_taskId, m1_dma_dir, i, fd);
            if (ret != 0) {
                LOG_DXRT_ERR("rk_pcie_import_dma_fd (EP↔M1) failed: task=" << _RK_taskId
                            << ", dir=" << (m1_dma_dir == E_SEND ? "E_SEND" : "E_RECV")
                            << ", idx=" << i << ", fd=" << fd << ", ret=" << ret);
                RK_MPI_MB_ReleaseMB(mb);
                throw std::runtime_error("Failed to register DMA buffer with EP↔M1 PCIe");   // NOSONAR:S112
            }
            LOG_DXRT_DBG << "PCIe DMA registered (EP↔M1): task=" << _RK_taskId
                     << ", dir=" << (m1_dma_dir == E_SEND ? "E_SEND" : "E_RECV")
                     << ", idx=" << i << ", fd=" << fd << std::endl;
        }
#endif

        // Get virtual address for CPU access (read/write verification)
        void* vaddr = RK_MPI_MB_Handle2VirAddr(mb);
        if (vaddr == nullptr) {
            LOG_DXRT_ERR("RK_MPI_MB_Handle2VirAddr failed: index=" << i);
            RK_MPI_MB_ReleaseMB(mb);
            throw std::runtime_error("Failed to get virtual address from MB");  // NOSONAR:S112
        }

        // Get physical address for DMA operations
        uint64_t phyAddr = RK_MPI_MB_Handle2PhysAddr(mb);

        LOG_DXRT_DBG << "CMA buffer[" << i << "]: vaddr=" << vaddr
                 << ", phy=0x" << std::hex << phyAddr << std::dec
                 << ", size=" << _size << std::endl;

        // Keep MB handle for cache flush operations (do NOT release)
        _cmaHandles.push_back(mb);

        _cmaPhysAddrs.push_back(phyAddr);
        // Store virtual address for CPU access
        _data.push_back(vaddr);
        _pointers.push_back(vaddr);
    }

     LOG_DXRT_DBG<< "Successfully allocated " << _count << " CMA DMA buffers" << std::endl;
}

FixedSizeBuffer::~FixedSizeBuffer()
{
    if (_allocType == BufferAllocType::HEAP) {
        releaseHeapBuffers();
    } else {
        releaseCMABuffers();
    }
}

void FixedSizeBuffer::releaseHeapBuffers()
{
    for (void* ptr : _data)
    {
#ifdef __linux__
        free(ptr);
#elif _WIN32
        _aligned_free(ptr);
#else
        free(ptr);
#endif
    }
}

void FixedSizeBuffer::releaseCMABuffers()
{
    LOG_DXRT_DBG << "Releasing CMA buffer pool" << std::endl;

    // Release all MB handles first
    for (auto mb : _cmaHandles) {
        if (mb != MB_INVALID_HANDLE) {
            RK_MPI_MB_ReleaseMB(mb);
        }
    }
    _cmaHandles.clear();

    // Then destroy the pool
    if (_cmaPoolId != MB_INVALID_POOLID) {
        RK_S32 ret = RK_MPI_MB_DestroyPool(_cmaPoolId);
        if (ret != 0) {
            LOG_DXRT_ERR("RK_MPI_MB_DestroyPool failed: pool_id=" << _cmaPoolId << ", ret=" << ret);
        } else {
            LOG_DXRT_DBG << "CMA pool destroyed: pool_id=" << _cmaPoolId << std::endl;
        }
    }

    _cmaPhysAddrs.clear();
}
#endif // USE_VNPU


void* FixedSizeBuffer::getBuffer()
{
    std::unique_lock<std::mutex> lock(_lock);

    if (_data.empty() || _count <= 0) {
        LOG_DXRT_DBG << "FixedSizeBuffer: Invalid state - empty data or invalid count" << std::endl;
        return nullptr;
    }

    // Timeout can be overridden in tests through DXRT_FIXEDSIZEBUFFER_WAIT_TIMEOUT_SEC.
    bool success = _cv.wait_for(lock, GetBufferWaitTimeout(), [this] { return !_pointers.empty(); });

    if (!success) {
        LOG_DXRT_ERR("FixedSizeBuffer: Timeout waiting for buffer. Available: " << _pointers.size() << ", Total: " << _count);
        throw std::runtime_error("Buffer allocation timeout - possible deadlock detected");  // NOSONAR:S112
    }

    // Check if destructor was called while we were waiting
    if (_pointers.empty()) {
        throw std::runtime_error("FixedSizeBuffer is being destroyed");  // NOSONAR:S112
    }

    void* retval = _pointers.back();
    _pointers.pop_back();
    LOG_DXRT_DBG << "FixedSizeBuffer: Buffer acquired. Remaining: " << _pointers.size() << std::endl;
    return retval;
}

void FixedSizeBuffer::releaseBuffer(void* ptr)
{
    if (ptr == nullptr) {
        LOG_DXRT_DBG << "FixedSizeBuffer: Attempted to release nullptr buffer" << std::endl;
        return;
    }

    std::unique_lock<std::mutex> lock(_lock);

    // 1. Check if it's a valid buffer
    bool isExist = false;
    for (const auto& x : _data)
    {
        if (x == ptr)
        {
            isExist = true;
            break;
        }
    }

    DXRT_ASSERT(isExist, "RETURNED outputs different than output");

    // 2. check if the buffer is already freed (to avoid duplicate frees)
    for (const auto& x : _pointers)
    {
        if (x == ptr)
        {
            LOG_DXRT_ERR("FixedSizeBuffer: Attempted to release buffer " << ptr << " that is already released (double release detected)");
            return; // avoid duplicate frees
        }
    }

    // 3. release the buffer
    _pointers.push_back(ptr);
    LOG_DXRT_DBG << "FixedSizeBuffer: Buffer released. Available: " << _pointers.size() << "/" << _count << std::endl;
    _cv.notify_one();
}

bool FixedSizeBuffer::hasBuffer()
{
    std::unique_lock<std::mutex> lock(_lock);
    return _pointers.empty() == false;
}

#ifdef USE_VNPU
uint64_t FixedSizeBuffer::getPhysicalAddress(void* ptr)
{
    if (_allocType != BufferAllocType::CMA_DMA) {
        LOG_DXRT_DBG << "[getPhysicalAddress] Not a CMA buffer, returning 0" << std::endl;
        return 0;  // Not a CMA buffer, no physical address
    }

    if (ptr == nullptr) {
        LOG_DXRT_DBG << "[getPhysicalAddress] ERROR: called with nullptr" << std::endl;
        return 0;
    }

    // CMA buffers: find the virtual address in _data and return corresponding physical address
    for (size_t i = 0; i < _data.size(); i++) {
        if (_data[i] == ptr) {
            LOG_DXRT_DBG << "[getPhysicalAddress] Found: vaddr=" << ptr << " -> phy=0x"
                     << std::hex << _cmaPhysAddrs[i] << std::dec << std::endl;
            return _cmaPhysAddrs[i];
        }
    }

    LOG_DXRT_DBG << "[getPhysicalAddress] ERROR: buffer not found: " << ptr << std::endl;
    return 0;
}

void FixedSizeBuffer::flushCache(void* ptr, uint32_t size, bool invalidate)
{
    LOG_DXRT_DBG << "[flushCache] Called: ptr=" << ptr << ", invalidate=" << invalidate << std::endl;

    if (_allocType != BufferAllocType::CMA_DMA) {
        LOG_DXRT_DBG << "[flushCache] Not a CMA buffer, skipping" << std::endl;
        return;  // Only for CMA buffers
    }

    if (ptr == nullptr) {
        LOG_DXRT_DBG << "[flushCache] ERROR: called with nullptr" << std::endl;
        return;
    }

    // Find the MB handle for this buffer
    MB_BLK mb = MB_INVALID_HANDLE;
    for (size_t i = 0; i < _data.size(); i++) {
        if (_data[i] == ptr) {
            if (i < _cmaHandles.size()) {
                mb = _cmaHandles[i];
            }
            break;
        }
    }

    if (mb == MB_INVALID_HANDLE) {
        LOG_DXRT_DBG << "[flushCache] ERROR: MB handle not found for buffer " << ptr << std::endl;
        return;
    }

    LOG_DXRT_DBG << "[flushCache] Found MB handle, calling RK_MPI_SYS_MmzFlushCache..." << std::endl;

    if (invalidate) {
        RK_MPI_MMZ_FlushCacheVaddrStart(ptr, size, RK_MMZ_SYNC_RW);
    } else {
        RK_MPI_MMZ_FlushCacheVaddrEnd(ptr, size, RK_MMZ_SYNC_RW);
    }
}
#endif // USE_VNPU

}  // namespace dxrt
