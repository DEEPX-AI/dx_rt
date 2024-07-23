// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

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