// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once


template <typename T>
class SimpleCircularDataPool
{
    std::vector<T*>  _dataPool;
    int _headIndex = 0;
    std::mutex _mutex;;

public:
    SimpleCircularDataPool(int count, size_t bufferSize)
    {
        for(int i = 0; i < count; ++i)
        {
            _dataPool.emplace_back(new T(bufferSize));
        }
       
    }

    SimpleCircularDataPool(int count)
    {
        for(int i = 0; i < count; ++i)
        {
            _dataPool.emplace_back(new T());
        }
    }

    ~SimpleCircularDataPool()
    {
        for(auto& ptr : _dataPool)
        {
            delete ptr;
        }

        _dataPool.clear();
    }

    // reuse buffer pointer
    T* pointer()
    {
        std::lock_guard<std::mutex> lock(_mutex);

        if ( _dataPool.empty() ) return nullptr;
        
        T* data = _dataPool[_headIndex];
        _headIndex++;
        if ( _headIndex == static_cast<int>(_dataPool.size()) )
        {
            _headIndex = 0;
        }

        return data;
    }

    //void release(T* data)
    //{
    //    std::lock_guard<std::mutex> lock(_mutex);
    //    _dataPool.emplace_back(data);
    //}
};