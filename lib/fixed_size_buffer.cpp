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
#include <stdexcept>

static constexpr int MEM_ALIGN_VALUE = 4096;

namespace dxrt {

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
        DXRT_ASSERT(result == 0, "Failed to posix_memalign " + std::to_string(result));
#elif _WIN32
        ptr = _aligned_malloc(size, MEM_ALIGN_VALUE);
        DXRT_ASSERT(ptr != nullptr, "Failed to windows aligned_malloc");
#else
        ptr = aligned_alloc(MEM_ALIGN_VALUE, size);
        DXRT_ASSERT(ptr != nullptr, "Failed to aligned_alloc");
#endif
        _data.push_back(ptr);
        _pointers.push_back(ptr);
    }
}

FixedSizeBuffer::~FixedSizeBuffer()
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

void* FixedSizeBuffer::getBuffer()
{
    if (_data.empty() || _count <= 0) {
        LOG_DXRT_DBG << "FixedSizeBuffer: Invalid state - empty data or invalid count" << std::endl;
        return nullptr;
    }
    
    std::unique_lock<std::mutex> lock(_lock);

    // Add a 3600 second timeout to prevent deadlocks
    bool success = _cv.wait_for(lock, std::chrono::seconds(3600), [this] { return !_pointers.empty(); });
    
    if (!success) {
        LOG_DXRT_ERR("FixedSizeBuffer: Timeout waiting for buffer. Available: " << _pointers.size() << ", Total: " << _count);
        throw std::runtime_error("Buffer allocation timeout - possible deadlock detected");
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

    // TODO : should delete this line in STD type
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

}  // namespace dxrt
