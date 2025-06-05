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
      std::shared_ptr<CircularDataPool<Request>> _requestPool;
      std::vector<std::shared_ptr<Device>> _devices;
      std::shared_ptr<MultiprocessMemory> _multiProcessMemory;
      std::once_flag _initDevicesOnceFlag;

      bool _device_identified = false;
      size_t _curDevIdx = 0;
      std::mutex _methodMutex;           // Mutex for synchoronizing method access

   public:

      // member functions
      static ObjectsPool& GetInstance();

      RequestPtr PickRequest(); // new one
      RequestPtr GetRequestById(int id); // find one by id

      DevicePtr GetDevice(int id);

      MultiprocessMemoryPtr GetMultiProcessMemory();
      
      int DeviceCount();

      void InitDevices(SkipMode skip, uint32_t subCmd);

      std::shared_ptr<Device> PickOneDevice(const vector<int> &device_ids_);

      std::vector<shared_ptr<Device>>& CheckDevices();

      // wait and awake
      std::shared_ptr<Device> WaitDevice(const vector<int> &device_ids);
      void AwakeDevice(size_t devIndex);
      

   private:
      std::condition_variable _deviceCV;
      std::mutex _deviceMutex;
      int _currentPickDevice;
      int pickDeviceIndex(const vector<int> &device_ids);

   };

}