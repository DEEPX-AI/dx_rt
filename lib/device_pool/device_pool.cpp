/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */


#include "dxrt/device_pool.h"

#include <cstdlib>
#include <iostream>

#include "dxrt/common.h"
#include "dxrt/device_core.h"
#include "dxrt/driver_adapter/driver_adapter.h"
#include "dxrt/driver_adapter/driver_adapter_factory.h"
#include "dxrt/filesys_support.h"
#include "dxrt/service_layer_factory.h"
#include "../resource/log_messages.h"
#include "dxrt/configuration.h"

namespace dxrt {


DevicePool &DevicePool::GetInstance()
{
    // Thread-safe static local variable Singleton pattern
    static DevicePool instance;
    return instance;

}

void DevicePool::InitCores_once()
{
    LOG_DXRT_DBG << std::endl;
    const char* forceNumDevStr = getenv("DXRT_FORCE_NUM_DEV");
    const char* forceDevIdStr = getenv("DXRT_FORCE_DEVICE_ID");
    int forceNumDev = 0;
    if (forceNumDevStr)
        forceNumDev = std::stoi(forceNumDevStr);

    int forceDevId = -1;
    if (forceDevIdStr)
        forceDevId = std::stoi(forceDevIdStr);

    _deviceCores.clear();
    int cnt = 0;

    while (true)
    {
#ifdef __linux__
        std::string devFile("/dev/" + std::string(DEVICE_FILE) + std::to_string(cnt));
#elif _WIN32
        std::string devFile("\\\\.\\" + std::string(DEVICE_FILE) + std::to_string(cnt));
#endif
#if DXRT_USB_NETWORK_DRIVER
        if(fileExists(devFile) || (cnt == 0))
#else
        if(fileExists(devFile))
#endif
        {
            if (forceNumDev > 0 && cnt >= forceNumDev)
                break;
            if (forceDevId != -1 && cnt != forceDevId)
            {
                cnt++;
                continue;
            }

            LOG_DBG("Found " + devFile);
            std::unique_ptr<DriverAdapter> adapter(DriverAdapterFactory::CreateForDeviceFile(devFile));
            std::shared_ptr<DeviceCore> device = std::make_shared<DeviceCore>(cnt, std::move(adapter));
            device->Identify(cnt);
            _deviceCores.emplace_back(std::move(device));
        }
        else
        {
            break;
        }
        cnt++;
    }

    if (cnt == 0 )
    {
        throw DeviceIOException(EXCEPTION_MESSAGE(LogMessages::DeviceNotFound()));
    }
}

void DevicePool::InitTaskLayers_once()
{
    InitCores();
    _serviceLayer = ServiceLayerFactory::CreateDefaultServiceLayer();
    // Implementation to be added
    for (std::shared_ptr<DeviceCore> core : _deviceCores)
    {
        DeviceType type = core->GetDeviceType();
        std::shared_ptr<DeviceTaskLayer> layer;
        //std::cout << (int)type << std::endl;
        if (type == DeviceType::ACC_TYPE)
        {
            layer = std::make_shared<AccDeviceTaskLayer>(core, _serviceLayer);
        }
        else if (type == DeviceType::STD_TYPE)
        {
            layer = std::make_shared<StdDeviceTaskLayer>(core, _serviceLayer);
        }
        else
        {
            DXRT_ASSERT(false, "UNKNOWN device type");
        }

        layer->RegisterCallback(std::bind(&DevicePool::AwakeDevice, this, core->id()));
        _taskLayers.push_back(layer);
    }
    for (auto& it : _taskLayers)
    {
        it->StartThread();
    }
}

void DevicePool::InitCores()
{
    std::call_once(_coresFlag, &DevicePool::InitCores_once, this);
}
void DevicePool::InitTaskLayers()
{
    std::call_once(_taskLayersFlag, &DevicePool::InitTaskLayers_once, this);
}

int DevicePool::pickDeviceIndex(const std::vector<int> &device_ids)
{
    int device_index = -1;
    int load = std::numeric_limits<int>::max();
    int curDeviceLoad;
    int device_id_size = device_ids.size();
    int block_count = 0;
    
    // Round-robin starting index (don't increment here to avoid side effect in predicate)
    int startIdx = _curDevIdx % device_id_size;
    
    for (int i = 0; i < device_id_size; i++)
    {
        int idx = (i + startIdx) % device_id_size;
        int device_id = device_ids[idx];
        
        if (_taskLayers[device_id]->isBlocked())
        {
            block_count++;
            LOG_DXRT_DBG << "Device " << device_id << " is blocked" << std::endl;
            continue;
        }
        
        curDeviceLoad = _taskLayers[device_id]->load();
        int fullLoad = _taskLayers[device_id]->getFullLoad();
        
        LOG_DXRT_DBG << "Device " << device_id 
                     << " load=" << curDeviceLoad 
                     << " fullLoad=" << fullLoad << std::endl;
        
        // Select device if it has capacity and lowest load
        if (curDeviceLoad < fullLoad && curDeviceLoad < load)
        {
            load = curDeviceLoad;
            device_index = device_id;
        }
    }

    if (block_count >= device_id_size)
    {
        throw DeviceIOException(EXCEPTION_MESSAGE(LogMessages::AllDeviceBlocked()));
    }
    
    if (device_index >= 0) {
        LOG_DXRT_DBG << "Selected device: " << device_index << " with load=" << load << std::endl;
    } else {
        LOG_DXRT_DBG << "No available device (all at full load)" << std::endl;
    }

    return device_index;
}


std::shared_ptr<DeviceTaskLayer> DevicePool::GetDeviceTaskLayer(int deviceId)
{
    InitTaskLayers();
    if (deviceId >= static_cast<int>(_taskLayers.size()))
    {
        throw DeviceIOException(EXCEPTION_MESSAGE("invalid device id "+ std::to_string(deviceId)));
    }
    return _taskLayers[deviceId];
}

std::shared_ptr<DeviceTaskLayer> DevicePool::PickOneDevice(const std::vector<int> &device_ids )
{
    InitTaskLayers();
    std::lock_guard<std::mutex> lock(_methodMutex);
    return WaitDevice(device_ids);
}
std::shared_ptr<NFHLayer> DevicePool::PickOneNFHDevice(const std::vector<int> &device_ids_)
{
    InitNFHLayers();
    auto device_pick = PickOneDevice(device_ids_);
    int deviceId = device_pick->id();
    return _nfhLayers[deviceId];
}

// wait and awake
std::shared_ptr<DeviceTaskLayer> DevicePool::WaitDevice(const std::vector<int> &device_ids)
{
    std::unique_lock<std::mutex> lock(_deviceMutex);
    
    LOG_DXRT_DBG << "Waiting for available device from " << device_ids.size() << " devices" << std::endl;

    // 3000 second timeout to prevent deadlock
    bool success = _deviceCV.wait_for(lock, std::chrono::seconds(3000), [this, &device_ids]{
        // Try to pick a device without side effects in predicate
        int picked = pickDeviceIndex(device_ids);
        if (picked >= 0) {
            _currentPickDevice = picked;
            return true;
        }
        // No device available, keep waiting
        LOG_DXRT_DBG << "No available device, waiting for notification..." << std::endl;
        return false;
    });

    if (!success) {
        std::string error_msg = "DevicePool: Timeout waiting for available device. Device IDs: ";
        for (int id : device_ids) {
            error_msg += std::to_string(id) + ",";
        }
        error_msg.pop_back();
        
        // Log current state of all devices
        error_msg += "\\n  Current device states: ";
        for (int id : device_ids) {
            if (id < static_cast<int>(_taskLayers.size())) {
                error_msg += "\\n    Device " + std::to_string(id) + 
                             ": load=" + std::to_string(_taskLayers[id]->load()) +
                             ", fullLoad=" + std::to_string(_taskLayers[id]->getFullLoad()) +
                             ", blocked=" + std::string(_taskLayers[id]->isBlocked() ? "true" : "false");
            }
        }
        
        LOG_DXRT_ERR(error_msg);
        throw std::runtime_error("Device allocation timeout - possible deadlock detected");
    }

    auto pick = _taskLayers[_currentPickDevice];
    pick->pick();  // Increment load counter
    
    // Update round-robin index after successful pick
    // Prevent overflow by resetting after a large threshold
    _curDevIdx++;
    if (_curDevIdx > 1000000) {
        _curDevIdx = 0;
    }
    
    LOG_DXRT_DBG << "Successfully picked device " << _currentPickDevice 
                 << " with new load=" << pick->load() << std::endl;

    return pick;
}

void DevicePool::AwakeDevice(int devIndex)
{
    std::unique_lock<std::mutex> lock(_deviceMutex);
    std::ignore = devIndex;
    // Don't reset _curDevIdx to maintain round-robin scheduling
    // _curDevIdx is incremented in WaitDevice() after successful pick
    
    LOG_DXRT_DBG << "Device " << devIndex 
                 << " completed task, notifying waiting threads" << std::endl;
    
    _deviceCV.notify_all();
}

void DevicePool::InitNFHLayers_once()
{
    InitTaskLayers();
    _nfhLayers.clear();
    bool isDynamic = true;
    if (USE_ONE_NFH_LAYERS)
    {
        auto nfhLayer = std::make_shared<NFHLayer>(nullptr, isDynamic);
        _nfhLayers.push_back(nfhLayer);
        for (auto &taskLayer : _taskLayers)
        {
            taskLayer->SetProcessResponseHandler(std::bind(&NFHLayer::ProcessResponse, nfhLayer.get(),
                                                           std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        }
    }
    else
    {
        for (auto &taskLayer : _taskLayers)
        {
            bool isDynamic = true;
            auto nfhLayer = std::make_shared<NFHLayer>(taskLayer, isDynamic);

            _nfhLayers.push_back(nfhLayer);
            taskLayer->SetProcessResponseHandler(std::bind(&NFHLayer::ProcessResponse, nfhLayer.get(),
                                                           std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        }
    }

}

void DevicePool::InitNFHLayers()
{
    std::call_once(_nfhLayersFlag, &DevicePool::InitNFHLayers_once, this);
}

size_t DevicePool::GetDeviceCountNoInit() const
{
    return _deviceCores.size();
}
size_t DevicePool::GetDeviceCount()
{
    InitCores();
    return _deviceCores.size();
}

}  // namespace dxrt
