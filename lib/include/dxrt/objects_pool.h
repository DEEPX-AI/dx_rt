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

#include <memory>
#include <mutex>
#include <vector>

#include "circular_data_pool.h"
#include "request.h"
#include "inference_job.h"
#include "dxrt/device.h"

namespace dxrt {

class InferenceJob;
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


    void makeDeviceList();
    void InitDevices_once(SkipMode skip, uint32_t subCmd);


    // member variable
    std::shared_ptr<CircularDataPool<Request> > _requestPool;
    std::vector<std::shared_ptr<Device> > _devices;
    std::shared_ptr<MultiprocessMemory> _multiProcessMemory;
    std::once_flag _initDevicesOnceFlag;

    bool _device_identified = false;
    size_t _curDevIdx = 0;
    std::mutex _methodMutex;           // Mutex for synchoronizing method access

    static ObjectsPool _staticInstance;

 public:
    // member functions
    static ObjectsPool& GetInstance();

    RequestPtr PickRequest();  // new one
    RequestPtr GetRequestById(int id);  // find one by id

    DevicePtr GetDevice(int id);

    MultiprocessMemoryPtr GetMultiProcessMemory();

    int DeviceCount();

    void InitDevices(SkipMode skip, uint32_t subCmd);
    int DSP_GetBufferPtrFromDevices(uint64_t *inputPtr, uint64_t *outputPtr);

    std::shared_ptr<Device> PickOneDevice(const std::vector<int> &device_ids_, int isDspReq);

    std::vector<std::shared_ptr<Device> >& CheckDevices();

    // wait and awake
    std::shared_ptr<Device> WaitDevice(const std::vector<int> &device_ids, int isDspReq);
    void AwakeDevice(size_t devIndex);


 private:
    std::condition_variable _deviceCV;
    std::mutex _deviceMutex;
    int _currentPickDevice;
    int pickDeviceIndex(const std::vector<int> &device_ids, int isDspReq);

};

}  // namespace dxrt
