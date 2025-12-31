/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/circular_buffer.h"
#include "dxrt/exception/exception.h"
#include <mutex>
#include <vector>

namespace dxrt {
template <typename T>
CircularBuffer<T>::CircularBuffer(int size)
: _buf(size), _size(size), _head(0), _tail(0), _count(0)
{

}

template <typename T>
void CircularBuffer<T>::Push(const T& item)
{
    std::unique_lock<std::mutex> lk(_lock);
    _buf[_head] = item;
    _head = (_head + 1)%_size;
    if(_count==_size)
    {
        _tail = (_tail + 1)%_size;
    }
    else
    {
        ++_count;
    }
}

template <typename T>
T CircularBuffer<T>::Pop()
{
    std::unique_lock<std::mutex> lk(_lock);
    //DXRT_ASSERT(_count>0, "circular buffer is empty.");
    if ( _count <= 0 ) throw InvalidOperationException(EXCEPTION_MESSAGE("circular buffer is empty"));
    //@no_else: input_validation
    T item = _buf[_tail];
    _tail = (_tail +1)%_size;
    --_count;
    return item;
}

template <typename T>
T CircularBuffer<T>::Get()
{
    std::unique_lock<std::mutex> lk(_lock);
    //DXRT_ASSERT(_count>0, "circular buffer is empty.");
    if ( _count <= 0 ) throw InvalidOperationException(EXCEPTION_MESSAGE("circular buffer is empty"));
    //@no_else: input_validation
    T item = _buf[(_head - 1 + _size)%_size];
    return item;
}

template <typename T>
bool CircularBuffer<T>::IsEmpty()
{
    std::unique_lock<std::mutex> lk(_lock);
    return _count==0;
}

template <typename T>
bool CircularBuffer<T>::IsFull()
{
    std::unique_lock<std::mutex> lk(_lock);
    return _count==_size;
}

template <typename T>
int CircularBuffer<T>::size()
{
    std::unique_lock<std::mutex> lk(_lock);
    return _size;
}

template <typename T>
int CircularBuffer<T>::count()
{
    std::unique_lock<std::mutex> lk(_lock);
    return _count;
}

template <typename T>
std::vector<T> CircularBuffer<T>::ToVector()
{
    std::unique_lock<std::mutex> lk(_lock);
    std::vector<T> ret;
    ret.reserve(_count);
    int cur = _tail;
    for(int i=0; i<_count; i++)
    {
        ret.emplace_back(_buf[cur]);
        cur = (cur+1)%_size;
    }
    return ret;
}

template <typename T>
CircularBuffer<T>::~CircularBuffer()
{
    // LOG_DXRT << endl;
}

template class CircularBuffer<int>;
template class CircularBuffer<uint32_t>;

} // namespace dxrt
