// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <memory>
#include <mutex>
#include <vector>
#include "dxrt/common.h"


namespace dxrt {
template <typename T>
class DXRT_API CircularDataPool
{
    std::vector<std::shared_ptr<T>> _dataPool;
    size_t _headIndex = 0;
    std::mutex _mutex;;

public:
    CircularDataPool(int count)
    {
        for(int i = 0; i < count; ++i)
        {
            _dataPool.emplace_back(std::make_shared<T>(i));
        }
    }

    
    ~CircularDataPool()
    {
        for(auto& ptr : _dataPool)
        {
            ptr.reset();
        }

        _dataPool.clear();
    }

    // reuse buffer pointer
    std::shared_ptr<T> pick()
    {
        std::lock_guard<std::mutex> lock(_mutex);

        if ( _dataPool.empty() ) return nullptr;
        
        std::shared_ptr<T> data = _dataPool[_headIndex];
        _headIndex++;
        if ( _headIndex == _dataPool.size() )
        {
            _headIndex = 0;
        }

        return data;
    }
    std::shared_ptr<T> GetById(int id)
    {
        return _dataPool[id];
    }

};
} // namespace dxrt