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

    makeDeviceList();

}

ObjectsPool::~ObjectsPool()
{
    LOG_DXRT_DBG << "~ObjectPool start" << std::endl;
    _devices.clear();
    _requestPool = nullptr;

    // delete multiprocess_memory
    _multiProcessMemory = nullptr;

    // delete profiler
    Profiler::deleteInstance();

    // delete configuration
    Configuration::deleteInstance();

    LOG_DXRT_DBG << "~ObjectPool end" << std::endl;
}

// #define DEVICE_FILE "dxrt"
// #define DEVICE_FILE_DSP "dxrt_dsp"

void ObjectsPool::makeDeviceList()
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

    {
        // LOG << "DXRT " DXRT_VERSION << std::endl;
        _devices.clear();
        int cnt = 0;
        int cntDsp = 0;
        while (true)
        {
#ifdef __linux__
            string devFile("/dev/" + string(DEVICE_FILE) + std::to_string(cnt));
#elif _WIN32
            string devFile("\\\\.\\" + string(DEVICE_FILE) + std::to_string(cnt));
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
                std::shared_ptr<Device> device = std::make_shared<Device>(devFile);
                _devices.emplace_back(std::move(device));
            }
            else
            {
                break;
            }
            cnt++;
        }
        // find DSP device
        {
#ifdef __linux__
            string devFileDsp("/dev/" + string(DEVICE_FILE_DSP) + std::to_string(cntDsp));
#elif _WIN32
            string devFileDsp("\\\\.\\" + string(DEVICE_FILE_DSP) + std::to_string(cntDsp));
#endif
            if (fileExists(devFileDsp))
            {
                LOG_DBG("Found " + devFileDsp);
                std::shared_ptr<Device> device = std::make_shared<Device>(devFileDsp);
                device->DSP_SetDspEnable(1);
                _devices.emplace_back(std::move(device));
                cntDsp++;
            }
            else
            {
                // No op.
            }
        }
        if ( (cnt+cntDsp) == 0 )
        {
            throw DeviceIOException(EXCEPTION_MESSAGE(LogMessages::DeviceNotFound()));
        }
    }
}

RequestPtr ObjectsPool::PickRequest()  // new one
{
    return _requestPool->pick();
}

RequestPtr ObjectsPool::GetRequestById(int id)  // find one by id
{
    return _requestPool->GetById(id);
}

void ObjectsPool::InitDevices(SkipMode skip, uint32_t subCmd)
{
    std::call_once(_initDevicesOnceFlag, &ObjectsPool::InitDevices_once, this, skip, subCmd);
}

void ObjectsPool::InitDevices_once(SkipMode skip, uint32_t subCmd)
{
    std::lock_guard<std::mutex> lock(_methodMutex);

    for (size_t i = 0; i < _devices.size(); i++)
    {
        if (_devices[i]->DSP_GetDspEnable())
            _devices[i]->DSP_Identify(i, skip, subCmd);
        else
            _devices[i]->Identify(i, skip, subCmd);
    }
    _device_identified = true;
}



shared_ptr<Device> ObjectsPool::PickOneDevice(const std::vector<int> &device_ids, int isDspReq)
{
    std::lock_guard<std::mutex> lock(_methodMutex);
    return WaitDevice(device_ids, isDspReq);


#if 0
    std::lock_guard<std::mutex> lock(_methodMutex);
    LOG_DXRT_DBG << std::endl;
    shared_ptr<Device> pick = nullptr;
    int load = numeric_limits<int>::max();
    int curDeviceLoad;
    int device_id_size = device_ids.size();
    _curDevIdx++;
    while (1)
    {
        int block_count = 0;
        for (int i = 0; i < device_id_size; i++)
        {
            int idx = (i + _curDevIdx) % device_id_size;
            int device_id = device_ids[idx];
            if (_devices[device_id]->isBlocked())
            {
                block_count++;
                continue;
            }
            curDeviceLoad = _devices[device_id]->load();
            if (curDeviceLoad < DXRT_TASK_MAX_LOAD && curDeviceLoad < load)
            // if(curDeviceLoad < load)
            {
                load = curDeviceLoad;
                pick = _devices[device_id];
            }
        }
        if (block_count >= device_id_size)
        {
            throw DeviceIOException(LogMessages::AllDeviceBlocked());
        }
        if (pick != nullptr)
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
    pick->pick();
    // std::cout << "dev " << pick->id() << ", " << pick->load() << std::endl;
    LOG_DXRT_DBG << " pick device " << pick->id() << std::endl;
    return pick;
#endif  // busy waiting..
}

std::vector<std::shared_ptr<Device>>& ObjectsPool::CheckDevices()
{
    return _devices;
}


DevicePtr ObjectsPool::GetDevice(int id)
{
    std::lock_guard<std::mutex> lock(_methodMutex);
    if ( id >= 0 && id < static_cast<int>(_devices.size()))
    {
        return _devices[id];
    }
    else
    {
        LOG_DXRT_ERR("The id is out of the _devices range. device-size=" << _devices.size() << " id=" << id);
    }

    return nullptr;
}

int ObjectsPool::DeviceCount()
{
    // std::lock_guard<std::mutex> lock(_methodMutex);
    return static_cast<int>(_devices.size());
}

// wait and awake
std::shared_ptr<Device> ObjectsPool::WaitDevice(const std::vector<int> &device_ids, int isDspReq)
{
    std::unique_lock<std::mutex> lock(_deviceMutex);

    // 3600 second timeout to prevent deadlock
    bool success = _deviceCV.wait_for(lock, std::chrono::seconds(3600), [this, &device_ids, isDspReq]{
        _currentPickDevice = pickDeviceIndex(device_ids, isDspReq);
        return _currentPickDevice >= 0;
    });

    if (!success) {
        LOG_DXRT_ERR("ObjectsPool: Timeout waiting for available device. Device IDs: ");
        for (int id : device_ids) {
            LOG_DXRT_ERR(id << " ");
        }
        throw std::runtime_error("Device allocation timeout - possible deadlock detected");
    }

    auto pick = _devices[_currentPickDevice];
    pick->pick();

    return pick;
}

void ObjectsPool::AwakeDevice(size_t devIndex)
{
    std::unique_lock<std::mutex> lock(_deviceMutex);
    // _curDevIdx = devIndex;
    std::ignore = devIndex;
    _curDevIdx = 0;
    _deviceCV.notify_all();
}


int ObjectsPool::pickDeviceIndex(const std::vector<int> &device_ids, int isDspReq)
{
    int device_index = -1;
    int load = std::numeric_limits<int>::max();
    int curDeviceLoad;
    int targetDevIsDsp;
    int device_id_size = device_ids.size();
    int block_count = 0;
    for (int i = 0; i < device_id_size; i++)
    {
        int idx = (i + _curDevIdx) % device_id_size;
        int device_id = device_ids[idx];
        if (_devices[device_id]->isBlocked())
        {
            block_count++;
            continue;
        }
        curDeviceLoad = _devices[device_id]->load();
        targetDevIsDsp = _devices[device_id]->DSP_GetDspEnable();
        if (isDspReq == 1)  // DSP
        {
            if (targetDevIsDsp)
            {
                load = curDeviceLoad;
                device_index = device_id;
            }
        }
        else  // NPU
        {
            int maxDeviceLoad;
            DeviceType deviceType = _devices[device_id]->getDeviceType();
            if (deviceType == DeviceType::STD_TYPE)
            {
                maxDeviceLoad = 1;  // DEVICE_NUM_BUF;
            }
            else
            {
                maxDeviceLoad = DXRT_TASK_MAX_LOAD;
            }

            if (curDeviceLoad < maxDeviceLoad && curDeviceLoad < load && !targetDevIsDsp)
            {
                load = curDeviceLoad;
                device_index = device_id;
            }
        }
    }

    _curDevIdx++;

    if (block_count >= device_id_size)
    {
        throw DeviceIOException(EXCEPTION_MESSAGE(LogMessages::AllDeviceBlocked()));
    }
    // std::cout << "device-index=" << device_index << std::endl;

    return device_index;
}

shared_ptr<MultiprocessMemory> ObjectsPool::GetMultiProcessMemory()
{
    return _multiProcessMemory;
}

// DSP code //////////////////////////////////////////////////////////////////////////////////////////////////////////

int ObjectsPool::DSP_GetBufferPtrFromDevices(uint64_t *inputPtr, uint64_t *outputPtr)
{
    int ret = 0;

    *inputPtr  = 0;  // NULL;
    *outputPtr = 0;  // NULL;
    ret = -1;

    for (size_t i = 0; i < _devices.size(); i++)
    {
        LOG_DXRT_DBG << "_devices.size() = " << _devices.size() << " i= " << i << std::endl;
        if (_devices[i]->DSP_GetDspEnable())
        {
            _devices[i]->DSP_GetBufferPtrFromMem(inputPtr, outputPtr);
        }
    }

    return ret;
}

// ~DSP code //////////////////////////////////////////////////////////////////////////////////////////////////////////

}  // namespace dxrt
