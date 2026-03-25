/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <cstdint>
#include <vector>
#include <mutex>
#include <condition_variable>
#include "dxrt/common.h"

#ifdef USE_VNPU
#include "rk_mpi_mb.h"
#include "rk_comm_mb.h"
#endif

namespace dxrt {

#ifdef USE_VNPU
enum class BufferAllocType {
    HEAP,      // Regular heap memory (posix_memalign)
    CMA_DMA    // CMA-based DMA memory (zero-copy capable, physically contiguous)
};

enum class BufferDirection {
    INPUT,     // Input buffer: E_RECV (host → EP)
    OUTPUT     // Output buffer: E_SEND (EP → host)
};
#endif // USE_VNPU

class DXRT_API FixedSizeBuffer
{
 public:
#ifndef USE_VNPU
    explicit FixedSizeBuffer(int64_t size, int buffer_count);
#else
    explicit FixedSizeBuffer(int64_t size, int buffer_count, BufferAllocType allocType = BufferAllocType::HEAP, BufferDirection direction = BufferDirection::INPUT);
    uint64_t getPhysicalAddress(void* ptr);  // Get physical address for given buffer (CMA only)
    void flushCache(void* ptr, uint32_t size, bool invalidate);  // Flush/invalidate CPU cache for DMA (CMA only)
    int count() { return _count;}
#endif // !USE_VNPU
    void* getBuffer();
    void releaseBuffer(void* ptr);
    bool hasBuffer();
    int64_t size() const { return _size;}
    ~FixedSizeBuffer();
    FixedSizeBuffer(const FixedSizeBuffer&) = delete;
    FixedSizeBuffer& operator=(const FixedSizeBuffer&) = delete;
    FixedSizeBuffer(FixedSizeBuffer&&) = delete;
    FixedSizeBuffer& operator=(FixedSizeBuffer&&) = delete;

 private:
 #ifdef USE_VNPU
    void allocateHeapBuffers();
    void allocateCMABuffers();
    void releaseHeapBuffers();
    void releaseCMABuffers();

    // Member variables (order matches initialization list)
    BufferAllocType _allocType;
    BufferDirection _direction;   // INPUT (E_RECV) or OUTPUT (E_SEND)
    // CMA-specific members
    MB_POOL _cmaPoolId;
    std::vector<MB_BLK> _cmaHandles;
    std::vector<uint64_t> _cmaPhysAddrs;  // Physical addresses for zero-copy DMA
#endif // USE_VNPU
    std::vector<void*> _data;
    std::vector<void*> _pointers;

    int _count;
    int64_t _size;
    std::mutex _lock;
    std::condition_variable _cv;
};


}  // namespace dxrt
