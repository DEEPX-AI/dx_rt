// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once


template <typename T>
class SimpleCircularBufferPool
{
    std::vector<T*>  _bufferPool;
    int _headIndex;
    size_t _bufferSize;
    std::mutex _mutex;;

public:
    SimpleCircularBufferPool(int count, size_t bufferSize)
    : _headIndex(0), _bufferSize(bufferSize)
    {
        for(int i = 0; i < count; ++i)
        {
            T* buffer = new T[bufferSize];
            _bufferPool.emplace_back(buffer);
        }
      
    }


    ~SimpleCircularBufferPool()
    {
        for(auto& ptr : _bufferPool)
        {
            delete[] ptr;
        }

        _bufferPool.clear();
    }

    // single buffer size
    size_t bufferSize()
    {
        return _bufferSize;
    }

    // reuse buffer pointer
    T* pointer()
    {
        std::lock_guard<std::mutex> lock(_mutex);

        if ( _bufferPool.empty() ) return nullptr;

        T* buffer = _bufferPool[_headIndex];
        _headIndex++;
        if ( _headIndex == static_cast<int>(_bufferPool.size()) )
        {
            _headIndex = 0;
        }

        return buffer;
    }


};