// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.
#pragma once

#include "circular_data_pool.h"
#include "request.h"
#include "inference_job.h"
#include "dxrt/device.h"

#include <memory>
#include <mutex>

namespace dxrt {

    class InferenceJob;
    using RequestWeakPtr = std::weak_ptr<Request>;
    using InferenceJobPtr = std::shared_ptr<InferenceJob>;
    using InferenceJobWeakPtr = std::weak_ptr<InferenceJob>;

    class ObjectsPool
    {
    public:
        // static
        static constexpr int REQUEST_MAX_COUNT = 50000;
        static constexpr int INFERENCE_JOB_MAX_COUNT = 5000;

    private:
        // member
        std::shared_ptr<CircularDataPool<Request>> _requestPool;
        std::shared_ptr<CircularDataPool<InferenceJob>> _inferenceJobPool;
        std::vector<std::shared_ptr<Device>> _devices;
        std::once_flag _initDevicesOnceFlag;

        bool _device_identified = false;
        std::mutex _devicesLock;
        int _curDevIdx = 0;


        ObjectsPool();
        void makeDeviceList();
        void InitDevices_once(SkipMode skip, uint32_t subCmd);

    public:
        
        static ObjectsPool* GetInstance() 
        {
            static ObjectsPool instance;

            return &instance;
        }

        RequestPtr PickRequest(); // new one
        RequestPtr GetRequestById(int id); // find one by id

        InferenceJobPtr PickInferenceJob(); // new one
        InferenceJobPtr GetInferenceJobById(int id); // find one by id

        DevicePtr GetDevices(int id){ return _devices[id]; }
        int DeviceCount(){ return _devices.size(); }
        void InitDevices(SkipMode skip, uint32_t subCmd);
        shared_ptr<Device> PickOneDevice(const vector<int> &device_ids_);
        vector<shared_ptr<Device>> CheckDevices();



        // devices (workers)
        // cpu handle
        // request pool
        // inference-job pool

    };

}