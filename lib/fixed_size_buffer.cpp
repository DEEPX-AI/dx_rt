// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/fixed_size_buffer.h"


namespace dxrt {

FixedSizeBuffer::FixedSizeBuffer(int64_t size, int buffer_count)
:  _count(buffer_count), _size(size)
{
    std::unique_lock<std::mutex> lock(_lock);
    
    _pointers.reserve(_count);
    for (int i = 0; i < _count; i++)
    {
        _data.emplace_back(size);
        _pointers.push_back(_data.back().data());
    }
}

void* FixedSizeBuffer::getBuffer()
{
    std::unique_lock<std::mutex> lock(_lock);

    if (_pointers.empty())
    {
        return nullptr;
    }
    void* retval = _pointers.back();
    _pointers.pop_back();
    return retval;
}
void FixedSizeBuffer::releaseBuffer(void* ptr)
{
    _pointers.push_back(ptr);
}
bool FixedSizeBuffer::hasBuffer()
{
    return _pointers.empty() == false;
}

}