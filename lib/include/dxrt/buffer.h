/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include "dxrt/common.h"

namespace dxrt {
class DXRT_API Buffer
{    
public:
    Buffer(uint32_t size);
    ~Buffer();
    void* Get();
    void* Get(uint32_t size);
    friend DXRT_API std::ostream& operator<<(std::ostream&, const Buffer&);
private:
    uint64_t _start = 0;
    uint64_t _end = 0;
    uint64_t _cur = 0;
    uint32_t _size = 0;
    std::vector<uint8_t> _mem;
};

} // namespace dxrt