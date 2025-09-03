/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <memory>
#include <mutex>
#include <vector>
#include "dxrt/common.h"
#include <iostream>


namespace dxrt {
    template <typename T>
    class DXRT_API CircularDataPool
    {
        std::vector<std::shared_ptr<T>> _dataPool;
        size_t _headIndex = 0;
        std::mutex _mutex;

    public:
        CircularDataPool(int count)
        {
            std::lock_guard<std::mutex> lock(_mutex);

            for(int i = 0; i < count; ++i)
            {
                _dataPool.emplace_back(std::make_shared<T>(i));
            }
        }

        
        ~CircularDataPool()
        {
            std::lock_guard<std::mutex> lock(_mutex);

            for(auto& ptr : _dataPool)
            {
                ptr.reset();
            }

            _dataPool.clear();
        }

        size_t GetSize()
        {
            std::lock_guard<std::mutex> lock(_mutex);
            return _dataPool.size();
        }

        // reuse buffer pointer
        std::shared_ptr<T> pick()
        {
            std::lock_guard<std::mutex> lock(_mutex);

            if ( _dataPool.empty() ) return nullptr;
            for (size_t i = 0; i < _dataPool.size(); ++i)
            {
                std::shared_ptr<T> data = _dataPool[_headIndex];
                _headIndex++;
                if ( _headIndex == _dataPool.size() )
                {
                    _headIndex = 0;
                }
                bool expected = false;
                bool desired = true;
                if (data->_use_flag.compare_exchange_strong(expected, desired))
                {
                    return data;
                }
            }
            LOG_DXRT_ERR ("CircularDataPool::pick(): not selected");
            return nullptr;

        }
        std::shared_ptr<T> GetById(int id)
        {
            std::lock_guard<std::mutex> lock(_mutex);
            if ( id >= 0 && id < static_cast<int>(_dataPool.size()) )
            {
                return _dataPool[id];
            }
            else
            {
                LOG_DXRT_ERR("The id is out of the _dataPool range. pool-size=" << _dataPool.size() << " id=" << id);
            }

            return nullptr;
        }
    };
}  // namespace dxrt
