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
#include <vector>
#include <stdexcept>
#include <mutex>
#include <condition_variable>

namespace dxrt {
template <typename T>
class DXRT_API CircularBuffer
{    
public:
    CircularBuffer(int size);
    ~CircularBuffer();
    void Push(const T& item);
    T Pop();
    T Get();
    bool IsEmpty();
    bool IsFull();
    int size();
    int count();
    std::vector<T> ToVector();
private:
    std::vector<T> _buf;
    int _size;
    int _head;
    int _tail;
    int _count;
    std::mutex _lock;
};

} // namespace dxrt