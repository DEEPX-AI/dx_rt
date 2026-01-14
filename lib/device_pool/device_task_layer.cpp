/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */


// Core task layer base implementation (common utilities only)
#include "dxrt/device_task_layer.h"

#include <signal.h>
#include <string>

#include "dxrt/common.h"
#include "dxrt/device_core.h"
#include "dxrt/request_data.h"
#include "dxrt/request_response_class.h"
#include "dxrt/task.h"

#ifdef __linux__
    #include <poll.h>
#elif _WIN32
    #include <windows.h>

#endif

namespace dxrt {

extern uint8_t DEBUG_DATA;
extern uint8_t SKIP_INFERENCE_IO;

DeviceTaskLayer::DeviceTaskLayer(std::shared_ptr<DeviceCore> core, std::shared_ptr<ServiceLayerInterface> service_interface)
: _core(core),  _load(0), _inferenceCnt(0), _serviceLayer(service_interface), _npuMemoryCacheManager(this)
{
    _onCompleteInferenceHandler = [](){};
    _processResponseHandler = [](int deviceId, int reqId, dxrt_response_t *response){
        RequestResponse::ProcessByData(reqId, *response, deviceId);
    };

}

int DeviceTaskLayer::load()
{
    return _load.load();
}

void DeviceTaskLayer::pick()
{
    ++_load;
}

int DeviceTaskLayer::infCnt()
{
    return _inferenceCnt;
}


int64_t DeviceTaskLayer::Allocate(uint64_t size)
{
    return _serviceLayer->Allocate(id(), size);
}

void DeviceTaskLayer::Deallocate(uint64_t addr)
{
    _serviceLayer->DeAllocate(id(), addr);
}



void DeviceTaskLayer::CallBack()
{
    // Decrement load atomically
    _load--;
    _inferenceCnt++;
    
    // Notify device pool that this device is now available
    if (_onCompleteInferenceHandler) {
        _onCompleteInferenceHandler();
    }
}
void DeviceTaskLayer::RegisterCallback(std::function<void()> f)
{
    _onCompleteInferenceHandler = f;
}

static constexpr int TERMINATE_NUM_CHANNEL = 3;

void DeviceTaskLayer::Terminate()
{
    dxrt_response_t data;
    memset(static_cast<void*>(&data), 0x00, sizeof(dxrt_response_t));
    std::ignore = core()->Process(dxrt::dxrt_cmd_t::DXRT_CMD_TERMINATE_EVENT, &data);
    for (int i = 0; i < TERMINATE_NUM_CHANNEL; i++)
    {
        data.req_id = i;
        int ret = core()->Process(dxrt::dxrt_cmd_t::DXRT_CMD_TERMINATE, &data);
        std::ignore = ret;
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
}

int64_t DeviceTaskLayer::AllocateFromCache(int64_t size, int taskId)
{
    LOG_DXRT_DBG << "Device " << id() << " allocate from cache: " << size << " bytes" << std::endl;

    if (_npuMemoryCacheManager.canGetCache(taskId))
    {
        return _npuMemoryCacheManager.getNpuMemoryCache(taskId);
    }
    else
    {
        return Allocate(size);
    }
}


void DeviceTaskLayer::Deallocate_npuBuf(int64_t addr, int taskId)
{
    LOG_DXRT_DBG << "Device " << id() << " deallocate: " << std::showbase << std::hex << addr << std::dec << std::endl;

    if (_npuMemoryCacheManager.canGetCache(taskId))
    {
        _npuMemoryCacheManager.returnNpuMemoryCache(taskId, addr);
    }
    else
    {
        Deallocate(addr);
    }
}

void DeviceTaskLayer::ProcessErrorFromService(dxrt_server_err_t err, int value)
{
    std::cout << "============================================================" << std::endl;
    std::cout << "error occured in device " << id() << std::endl;
    std::cout << " ** Reason : " <<  err <<
        "(value: " << value << ")" << std::endl;
    std::cout << " ** Take error message from server" << std::endl;
    std::cout << " ** Please restart daemon and applications" << std::endl;
    std::cout << "============================================================" << std::endl;

    core()->ShowPCIEDetails();
    block();
}

}  // namespace dxrt
