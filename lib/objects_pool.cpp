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
#include "dxrt/profiler.h"
#include "dxrt/user_event_store.h"
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

    // create user event store
    UserEventStore::GetInstance();

    _requestPool = std::make_shared<CircularDataPool<Request>>(ObjectsPool::REQUEST_MAX_COUNT);
}

ObjectsPool::~ObjectsPool()
{
    LOG_DXRT_DBG << "~ObjectsPool start" << std::endl;

    _requestPool = nullptr;

    // delete multiprocess_memory
    _multiProcessMemory = nullptr;

    // delete profiler (destructor auto-save runs here, while UserEventStore is still alive)
    Profiler::deleteInstance();

    // delete user event store (after profiler is done)
    UserEventStore::deleteInstance();

    // delete configuration
    Configuration::deleteInstance();

    LOG_DXRT_DBG << "~ObjectsPool end" << std::endl;
}

RequestPtr ObjectsPool::PickRequest() const // new one
{
    return _requestPool->pick();
}

RequestPtr ObjectsPool::GetRequestById(int id) const  // find one by id
{
    return _requestPool->GetById(id);
}

std::shared_ptr<MultiprocessMemory> ObjectsPool::GetMultiProcessMemory() const
{
    return _multiProcessMemory;
}

}  // namespace dxrt
