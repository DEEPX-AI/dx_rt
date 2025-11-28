#pragma once

// project common
#include "dxrt/common.h"

// self header (none)

// C headers (none)

// C++ headers
#include <atomic>
#include <memory>
#include <string>
#include <vector>

// project headers
#include "dxrt/device_core.h"
#include "dxrt/device_task_layer.h"
#include "dxrt/filesys_support.h"
#include "dxrt/nfh_layer.h"

namespace dxrt {

class DXRT_API DevicePool {
 public:


    void InitCores();
    void InitTaskLayers();
    void InitNFHLayers();
    std::shared_ptr<DeviceTaskLayer> PickOneDevice(const std::vector<int> &device_ids_);
    std::shared_ptr<DeviceTaskLayer> GetDeviceTaskLayer(int deviceId);
    std::shared_ptr<DeviceCore> GetDeviceCores(int deviceId) {return _deviceCores[deviceId];}
    std::shared_ptr<NFHLayer> GetNFHLayer(int deviceId) {
        InitNFHLayers();
        if (USE_ONE_NFH_LAYERS)
        {
            return _nfhLayers[0];
        }
        return _nfhLayers[deviceId];
    }
    std::shared_ptr<NFHLayer> PickOneNFHDevice(const std::vector<int> &device_ids_);

    std::shared_ptr<ServiceLayerInterface> GetServiceLayer() { return _serviceLayer;}
    void AwakeDevice(int devIndex);

    // member functions
    static DevicePool &GetInstance();

    size_t GetDeviceCountNoInit() const;
    size_t GetDeviceCount();

    std::vector<std::shared_ptr<DeviceCore>> GetAllDeviceCores() { return _deviceCores; }
    std::vector<std::shared_ptr<DeviceTaskLayer>> GetAllDeviceTaskLayers() { return _taskLayers; }

 protected:

    std::vector<std::shared_ptr<DeviceTaskLayer>> _taskLayers;
    std::vector<std::shared_ptr<DeviceCore>> _deviceCores;
    std::shared_ptr<ServiceLayerInterface> _serviceLayer;
    std::vector<std::shared_ptr<NFHLayer>> _nfhLayers;

    DevicePool() = default;
    ~DevicePool() = default;

    // Delete copy constructor and assignment operator
    DevicePool(const DevicePool&) = delete;
    DevicePool& operator=(const DevicePool&) = delete;
    DevicePool(DevicePool&&) = delete;
    DevicePool& operator=(DevicePool&&) = delete;


    size_t _curDevIdx = 0;

    std::once_flag _coresFlag;
    std::once_flag _taskLayersFlag;
    std::once_flag _nfhLayersFlag;
    void InitCores_once();
    void InitTaskLayers_once();
    void InitNFHLayers_once();





    std::condition_variable _deviceCV;
    std::mutex _deviceMutex;
    std::mutex _methodMutex;
    int _currentPickDevice = 0;

    int pickDeviceIndex(const std::vector<int> &device_ids);
    std::shared_ptr<DeviceTaskLayer> WaitDevice(const std::vector<int> &device_ids);

    static constexpr bool USE_ONE_NFH_LAYERS = true;
};

}  // namespace dxrt
