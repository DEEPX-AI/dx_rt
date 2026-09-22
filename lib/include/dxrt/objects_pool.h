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

#include <atomic>
#include <memory>
#include <mutex>
#include <vector>

#include "dxrt/circular_data_pool.h"
#include "dxrt/request.h"
#include "dxrt/inference_job.h"
#include "dxrt/device.h"

namespace dxrt {

class InferenceJob;
class MultiprocessMemory;
using RequestWeakPtr = std::weak_ptr<Request>;
using InferenceJobPtr = std::shared_ptr<InferenceJob>;
using InferenceJobWeakPtr = std::weak_ptr<InferenceJob>;
using MultiprocessMemoryPtr = std::shared_ptr<MultiprocessMemory>;

class ObjectsPool
{
 public:
    // static
    static constexpr int REQUEST_MAX_COUNT = 15000;

 private:
    ObjectsPool();
    ~ObjectsPool();

    // Delete copy constructor and assignment operator
    ObjectsPool(const ObjectsPool&) = delete;
    ObjectsPool& operator=(const ObjectsPool&) = delete;
    ObjectsPool(ObjectsPool&&) = delete;
    ObjectsPool& operator=(ObjectsPool&&) = delete;


    void makeDeviceList();
    void InitDevices_once(SkipMode skip, uint32_t subCmd);


    // member variable
    std::shared_ptr<CircularDataPool<Request> > _requestPool;
    std::shared_ptr<MultiprocessMemory> _multiProcessMemory;
    std::once_flag _initDevicesOnceFlag;

    bool _device_identified = false;
    size_t _curDevIdx = 0;
    std::mutex _methodMutex;           // Mutex for synchoronizing method access

    static ObjectsPool _staticInstance;

    // Set at the very start of ~ObjectsPool(), before the pre-allocated Request
    // pool is torn down. Meyer's singletons (ObjectsPool, DevicePool, ...) are
    // destroyed in reverse order of first construction at process exit; since
    // ObjectsPool is now eagerly constructed at model-load time (before
    // DevicePool), DevicePool is torn down first. Request::releaseBuffers(),
    // invoked from each Request's destructor during this teardown, must not
    // reach into DevicePool::GetInstance() at that point -- doing so touches an
    // already-destroyed singleton (dangling/UB, observed as a SIGSEGV on exit).
    // This flag lets releaseBuffers() detect "we are being destroyed as part of
    // process-wide static teardown" and skip that call; the device itself is
    // going away regardless, so releasing its cache slice is moot at this point.
    static std::atomic<bool> _shuttingDown;

 public:
    // member functions
    static ObjectsPool& GetInstance();
    static bool IsShuttingDown();

    RequestPtr PickRequest() const; // new one
    RequestPtr GetRequestById(int id) const;  // find one by id

    MultiprocessMemoryPtr GetMultiProcessMemory() const;



 private:
    std::mutex _deviceMutex;
    int _currentPickDevice;
    int pickDeviceIndex(const std::vector<int> &device_ids);

};

}  // namespace dxrt
