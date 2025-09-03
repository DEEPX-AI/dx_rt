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


namespace dxrt {


class FixedSizeBuffer
{
 public:
    explicit FixedSizeBuffer(int64_t size, int buffer_count);
    void* getBuffer();
    void releaseBuffer(void* ptr);
    bool hasBuffer();
    int64_t size() { return _size;}
    ~FixedSizeBuffer();

 private:
    std::vector<void*> _data;
    std::vector<void*> _pointers;

    int _count;
    int64_t _size;
    std::mutex _lock;
    std::condition_variable _cv;
};


}  // namespace dxrt
