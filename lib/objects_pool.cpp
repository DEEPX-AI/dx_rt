/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/objects_pool.h"
#include "dxrt/filesys_support.h"
#include "dxrt/configuration.h"
#include "dxrt/multiprocess_memory.h"
#include "dxrt/profiler.h"
#include "dxrt/exception/exception.h"
#include "resource/log_messages.h"
#ifdef __linux__
#include "dxrt/driver_adapter/linux_driver_adapter.h"
#elif _WIN32
#include "dxrt/driver_adapter/windows_driver_adapter.h"
#endif



#include <chrono>
#include <stdexcept>
#include <string>

using std::string;

namespace dxrt {

constexpr int ObjectsPool::REQUEST_MAX_COUNT;


ObjectsPool& ObjectsPool::GetInstance()
{
    // Thread-safe static local variable Singleton pattern
    static ObjectsPool instance;
    return instance;

}


ObjectsPool::ObjectsPool()
{

    // create configuration
    Configuration::GetInstance();

    // create profiler
    Profiler::GetInstance();

    // create multiprocess_memory
    #ifdef USE_SERVICE
        if (Configuration::GetInstance().GetEnable(Configuration::ITEM::SERVICE))
        {
            if (_multiProcessMemory == nullptr)
            {
                 _multiProcessMemory = std::make_shared<MultiprocessMemory>();
            }
        }
    #endif

    _requestPool = std::make_shared<CircularDataPool<Request>>(ObjectsPool::REQUEST_MAX_COUNT);

    //makeDeviceList();

}

ObjectsPool::~ObjectsPool()
{
    LOG_DXRT_DBG << "~ObjectsPool start" << std::endl;
    //_devices.clear();
    _requestPool = nullptr;

    // delete multiprocess_memory
    _multiProcessMemory = nullptr;

    // delete profiler
    Profiler::deleteInstance();

    // delete configuration
    Configuration::deleteInstance();

    LOG_DXRT_DBG << "~ObjectsPool end" << std::endl;
}

RequestPtr ObjectsPool::PickRequest()  // new one
{
    return _requestPool->pick();
}

RequestPtr ObjectsPool::GetRequestById(int id)  // find one by id
{
    return _requestPool->GetById(id);
}

std::shared_ptr<MultiprocessMemory> ObjectsPool::GetMultiProcessMemory()
{
    return _multiProcessMemory;
}

}  // namespace dxrt
