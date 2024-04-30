// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/buffer.h"

using namespace std;

namespace dxrt {

Buffer::Buffer(uint32_t size) : _size(size)
{
    _mem = vector<uint8_t>(_size, 0);
    _start = reinterpret_cast<uint64_t>(_mem.data());
    _cur = _start;
    _end = _start + _size;
}
Buffer::~Buffer()
{

}
void *Buffer::Get()
{
    return _mem.data();
}
void *Buffer::Get(uint32_t size)
{
    uint64_t addr;
    if(size>_size)
    {
        return nullptr;
    }
    if(_cur + size > _end)
    {
        _cur = _start;
    }
    addr = _cur;
    _cur += size;
    return reinterpret_cast<void*>(addr);
}

} // namespace dxrt
