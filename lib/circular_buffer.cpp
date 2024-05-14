// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/circular_buffer.h"

using namespace std;

namespace dxrt {
template <typename T>
CircularBuffer<T>::CircularBuffer(int size)
: _buf(size), _size(size), _head(0), _tail(0), _count(0)
{

}

template <typename T>
void CircularBuffer<T>::Push(const T& item)
{
    unique_lock<mutex> lk(_lock);
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
    unique_lock<mutex> lk(_lock);
    DXRT_ASSERT(_count>0, "circular buffer is empty.");
    T item = _buf[_tail];
    _tail = (_tail +1)%_size;
    --_count;
    return item;
}

template <typename T>
T CircularBuffer<T>::Get()
{
    unique_lock<mutex> lk(_lock);
    DXRT_ASSERT(_count>0, "circular buffer is empty.");
    T item = _buf[(_head - 1 + _size)%_size];
    return item;
}

template <typename T>
bool CircularBuffer<T>::IsEmpty()
{
    unique_lock<mutex> lk(_lock);
    return _count==0;
}

template <typename T>
bool CircularBuffer<T>::IsFull()
{
    unique_lock<mutex> lk(_lock);
    return _count==_size;
}

template <typename T>
int CircularBuffer<T>::size()
{
    unique_lock<mutex> lk(_lock);
    return _size;
}

template <typename T>
int CircularBuffer<T>::count()
{
    unique_lock<mutex> lk(_lock);
    return _count;
}

template <typename T>
vector<T> CircularBuffer<T>::ToVector()
{
    unique_lock<mutex> lk(_lock);
    vector<T> ret;
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
