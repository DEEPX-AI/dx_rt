/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/buffer.h"
#include <vector>
#include "dxrt/safe_cast.h"



namespace dxrt {

Buffer::Buffer(uint32_t size) : _size(size)
{
    _mem = std::vector<uint8_t>(_size, 0);
    _start = SafeCast::PointerToInteger<uint8_t*>(_mem.data());
    _cur = _start;
    _end = _start + _size;
}
Buffer::~Buffer() = default;
void *Buffer::Get()
{
    return static_cast<void*>(_mem.data());
}
void *Buffer::Get(uint32_t size)
{
    uint64_t addr;
    if (size > _size)
    {
        return nullptr;
    }  // @no_else: input_validation
    if (_cur + size > _end)
    {
        _cur = _start;
    }  // @no_else: conditional_work
    addr = _cur;
    _cur += size;
    return SafeCast::IntegerToPointer<void*>(addr);
}

}  // namespace dxrt
