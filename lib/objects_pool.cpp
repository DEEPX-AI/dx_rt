// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/objects_pool.h"
#include "dxrt/filesys_support.h"

namespace dxrt {



ObjectsPool::ObjectsPool()
{
    _requestPool = std::make_shared<CircularDataPool<Request>>(ObjectsPool::REQUEST_MAX_COUNT);
    _inferenceJobPool = std::make_shared<CircularDataPool<InferenceJob>>(ObjectsPool::INFERENCE_JOB_MAX_COUNT);

    makeDeviceList();
    
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
                _devices.emplace_back(move(device));
            }
            else
            {
                break;
            }
            cnt++;
        }
        DXRT_ASSERT(cnt > 0, "Device not found.");
    }
}
constexpr int ObjectsPool::REQUEST_MAX_COUNT;
constexpr int ObjectsPool::INFERENCE_JOB_MAX_COUNT;


RequestPtr ObjectsPool::PickRequest()  // new one
{
    return _requestPool->pick();
}

RequestPtr ObjectsPool::GetRequestById(int id)  // find one by id
{
    return _requestPool->GetById(id);
}

InferenceJobPtr ObjectsPool::PickInferenceJob()  // new one
{
    return _inferenceJobPool->pick();
}

InferenceJobPtr ObjectsPool::GetInferenceJobById(int id)  // find one by id
{
    return _inferenceJobPool->GetById(id);
}



void ObjectsPool::InitDevices(SkipMode skip, uint32_t subCmd)
{
    std::call_once(_initDevicesOnceFlag, &ObjectsPool::InitDevices_once, this, skip, subCmd);
}

void ObjectsPool::InitDevices_once(SkipMode skip, uint32_t subCmd)
{
    for (size_t i = 0; i < _devices.size(); i++)
    {
        _devices[i]->Identify(i, skip, subCmd);
    }
}



shared_ptr<Device> ObjectsPool::PickOneDevice(vector<int> &device_ids)
{
#if 1
    LOG_DXRT_DBG << endl;
    unique_lock<mutex> lk(_devicesLock);
    shared_ptr<Device> pick = nullptr;
    int load = numeric_limits<int>::max();
    int curDeviceLoad;
    int device_id_size = device_ids.size();
    _curDevIdx++;
    while(1)
    {
        for(int i = 0; i < device_id_size; i++)
        {
            int idx = (i + _curDevIdx) % device_id_size;
            int device_id = device_ids[idx];
            curDeviceLoad = _devices[device_id]->load();
            if(curDeviceLoad < DXRT_ASYNC_LOAD_THRE && curDeviceLoad < load)
            // if(curDeviceLoad < load)
            {
                load = curDeviceLoad;
                pick = _devices[device_id];
            }
        }
        if(pick!=nullptr)
            break;
    }
    pick->pick();
    //cout << "dev " << pick->id() << ", " << pick->load() << endl;
    LOG_DXRT_DBG << " pick device " << pick->id() << endl;
    return pick;
#else
    unique_lock<mutex> lk(_devicesLock);
    shared_ptr<Device> pick = nullptr;
    pick = devices_[(curDevIdx++)%devices_.size()];
    pick->pick();
    while(pick->load()>=DXRT_ASYNC_LOAD_THRE);
    // cout << "dev " << pick->id() << ", " << pick->load() << endl;
    return pick;
#endif
}

vector<shared_ptr<Device>> ObjectsPool::CheckDevices()
{
    return _devices;
}



}  // namespace dxrt
