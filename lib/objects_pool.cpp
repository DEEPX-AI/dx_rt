// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/objects_pool.h"
#include "dxrt/filesys_support.h"
#include "dxrt/configuration.h"
#include "dxrt/multiprocess_memory.h"
#include "dxrt/profiler.h"
#include "dxrt/exception/exception.h"
#include "resource/log_messages.h"

namespace dxrt {


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

#define DEVICE_FILE "dxrt"

void ObjectsPool::makeDeviceList()
{
    LOG_DXRT_DBG << endl;
    const char* forceNumDevStr = getenv("DXRT_FORCE_NUM_DEV");
    const char* forceDevIdStr = getenv("DXRT_FORCE_DEVICE_ID");
    int forceNumDev = forceNumDevStr?stoi(forceNumDevStr):0;
    int forceDevId = forceDevIdStr?stoi(forceDevIdStr):-1;

    {
        cout << "DXRT " DXRT_VERSION << endl;
        _devices.clear();
        int cnt = 0;
        while (1)
        {
#ifdef __linux__
            string devFile("/dev/" + string(DEVICE_FILE) + to_string(cnt));
#elif _WIN32
            string devFile("\\\\.\\" + string(DEVICE_FILE) + to_string(cnt));
#endif
            if (fileExists(devFile))
            {
                if (forceNumDev > 0 && cnt >= forceNumDev)
                    break;
                if (forceDevId != -1 && cnt != forceDevId)
                {
                    cnt++;
                    continue;
                }

                LOG_DBG("Found " + devFile);
                shared_ptr<Device> device = make_shared<Device>(devFile);
                _devices.emplace_back(std::move(device));
            }
            else
            {
                break;
            }
            cnt++;
        }
        
        if ( cnt == 0 )
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
        _devices[i]->Identify(i, skip, subCmd);
    }
    _device_identified = true;
}



shared_ptr<Device> ObjectsPool::PickOneDevice(const vector<int> &device_ids)
{
    std::lock_guard<std::mutex> lock(_methodMutex);
    return WaitDevice(device_ids);


#if 0
    std::lock_guard<std::mutex> lock(_methodMutex);
    LOG_DXRT_DBG << endl;
    shared_ptr<Device> pick = nullptr;
    int load = numeric_limits<int>::max();
    int curDeviceLoad;
    int device_id_size = device_ids.size();
    _curDevIdx++;
    while(1)
    {
        int block_count = 0;
        for(int i = 0; i < device_id_size; i++)
        {
            int idx = (i + _curDevIdx) % device_id_size;
            int device_id = device_ids[idx];
            if (_devices[device_id]->isBlocked())
            {
                block_count++;
                continue;
            }
            curDeviceLoad = _devices[device_id]->load();
            if(curDeviceLoad < DXRT_TASK_MAX_LOAD && curDeviceLoad < load)
            // if(curDeviceLoad < load)
            {
                load = curDeviceLoad;
                pick = _devices[device_id];
            }
        }
        if(block_count >= device_id_size)
        {
            throw DeviceIOException(LogMessages::AllDeviceBlocked());
        }
        if(pick!=nullptr)
            break;
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
    pick->pick();
    //cout << "dev " << pick->id() << ", " << pick->load() << endl;
    LOG_DXRT_DBG << " pick device " << pick->id() << endl;
    return pick;
#endif // busy waiting.. 

}

vector<shared_ptr<Device>>& ObjectsPool::CheckDevices()
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
    //std::lock_guard<std::mutex> lock(_methodMutex);
    return static_cast<int>(_devices.size());
}

// wait and awake
std::shared_ptr<Device> ObjectsPool::WaitDevice(const vector<int> &device_ids)
{
    std::unique_lock<std::mutex> lock(_deviceMutex);
    _deviceCV.wait(lock, [this, &device_ids]{
        _currentPickDevice = pickDeviceIndex(device_ids);
        return _currentPickDevice >= 0;
    });

    auto pick = _devices[_currentPickDevice];
    pick->pick();

    return pick;
}

void ObjectsPool::AwakeDevice(size_t devIndex)
{
    std::unique_lock<std::mutex> lock(_deviceMutex);
    //_curDevIdx = devIndex;
    std::ignore = devIndex;
    _curDevIdx = 0;
    _deviceCV.notify_all();
}
      

int ObjectsPool::pickDeviceIndex(const vector<int> &device_ids)
{
    int device_index = -1;
    int load = numeric_limits<int>::max();
    int curDeviceLoad;
    int device_id_size = device_ids.size();
    int block_count = 0;
    for(int i = 0; i < device_id_size; i++)
    {
        int idx = (i + _curDevIdx) % device_id_size;
        int device_id = device_ids[idx];
        if (_devices[device_id]->isBlocked())
        {
            block_count++;
            continue;
        }
        curDeviceLoad = _devices[device_id]->load();
        if(curDeviceLoad < DXRT_TASK_MAX_LOAD && curDeviceLoad < load)
        {
            load = curDeviceLoad;
            device_index = device_id;
        }
    }

    _curDevIdx++;
    
    if(block_count >= device_id_size)
    {
        throw DeviceIOException(LogMessages::AllDeviceBlocked());
    }
    //std::cout << "device-index=" << device_index << std::endl;
    
    return device_index;
}

shared_ptr<MultiprocessMemory> ObjectsPool::GetMultiProcessMemory()
{ 
    return _multiProcessMemory;
}

}  // namespace dxrt
