// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/fixed_size_buffer.h"

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
    std::unique_lock<std::mutex> lock(_lock);

    _cv.wait(lock, [this] { return !_pointers.empty(); });

    void* retval = _pointers.back();
    _pointers.pop_back();
    return retval;
}

void FixedSizeBuffer::releaseBuffer(void* ptr)
{
    std::unique_lock<std::mutex> lock(_lock);
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
    _pointers.push_back(ptr);
    _cv.notify_one();
}

bool FixedSizeBuffer::hasBuffer()
{
    std::unique_lock<std::mutex> lock(_lock);
    return _pointers.empty() == false;
}

}  // namespace dxrt
