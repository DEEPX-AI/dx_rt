/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include <iostream>
#include <memory>
#include <string>
#include <cstring>
#include "dxrt/common.h"
#include "dxrt/worker.h"
#include "dxrt/device.h"
#include "dxrt/profiler.h"


using std::cout;
using std::endl;
using std::string;
using std::shared_ptr;

namespace dxrt {


DeviceEventWorker::DeviceEventWorker(string name_, Device *device_)
: Worker(name_, Type::DEVICE_EVENT, 1, device_, nullptr)
{
    InitializeThread();
}
DeviceEventWorker::~DeviceEventWorker()
{
    LOG_DXRT_DBG << endl;
}
shared_ptr<DeviceEventWorker> DeviceEventWorker::Create(string name_, Device *device_)
{
    shared_ptr<DeviceEventWorker> ret = std::make_shared<DeviceEventWorker>(name_, device_);
    return ret;
}

void DeviceEventWorker::ShowPCIEDetails()
{
    _device->ShowPCIEDetails();
}

void DeviceEventWorker::ThreadWork(int id)
{
    std::ignore = id;
    _useSystemCall = true;
    string threadName = getName();

    int loopCnt = 0;
    LOG_DXRT_DBG << getName() << " : Entry" << endl;
    dxrt_cmd_t cmd = dxrt::dxrt_cmd_t::DXRT_CMD_EVENT;
    while (_stop.load(std::memory_order_acquire) == false)
    {
        //LOG_DXRT_DBG << threadName << " : wait" << endl;
        if (_stop.load(std::memory_order_acquire))
        {
            LOG_DXRT_DBG << threadName << " : requested to stop thread." << endl;
            break;
        }
        dxrt::dx_pcie_dev_event_t eventInfo;
        memset(&eventInfo, 0, sizeof(dxrt::dx_pcie_dev_event_t));
        _device->Process(cmd, &eventInfo); //Waiting in kernel. (device::terminate())
        
        // LOG_VALUE((int)errInfo.err_code);
        if (static_cast<dxrt::dxrt_event_t>(eventInfo.event_type)==dxrt::dxrt_event_t::DXRT_EVENT_ERROR)
        {
            if (static_cast<dxrt::dxrt_error_t>(eventInfo.dx_rt_err.err_code)!=dxrt::dxrt_error_t::ERR_NONE)
            {
                _device->block();
                LOG_DXRT_ERR(eventInfo.dx_rt_err);
                ShowPCIEDetails();
                break;
            }
        }
        else if (static_cast<dxrt::dxrt_event_t>(eventInfo.event_type)==dxrt::dxrt_event_t::DXRT_EVENT_NOTIFY_THROT)
        {
            if ( Configuration::GetInstance().GetEnable(Configuration::ITEM::SHOW_THROTTLING) )
                LOG_DXRT << eventInfo.dx_rt_ntfy_throt << endl;
        }
        else if (static_cast<dxrt::dxrt_event_t>(eventInfo.event_type)==dxrt::dxrt_event_t::DXRT_EVENT_RECOVERY)
        {
            if (eventInfo.dx_rt_recv.action==dxrt::dxrt_recov_t::DXRT_RECOV_RMAP)
            {
                auto model = _device->_npuModel.begin()->second;
                DXRT_ASSERT(_device->Write(model.rmap, 3) == 0, "Recovery rmap failed to write model parameters(cmd)");

                LOG_DXRT_ERR("RMAP data has been recovered. This error can cause issues with NPU operation.")
                _device->StartDev(RMAP_RECOVERY_DONE);
            }
            else if (eventInfo.dx_rt_recv.action==dxrt::dxrt_recov_t::DXRT_RECOV_WEIGHT)
            {
                auto model = _device->_npuModel.begin()->second;
                DXRT_ASSERT(_device->Write(model.weight, 3) == 0, "Recovery weight failed to write model parameters(weight)");
              
                LOG_DXRT_ERR("Weight data has been recovered. This error can cause wrong result value.")
                _device->StartDev(WEIGHT_RECOVERY_DONE);
            }
            else if (eventInfo.dx_rt_recv.action==dxrt::dxrt_recov_t::DXRT_RECOV_CPU)
            {
                cout << "Host received a message regarding a CPU abnormal case." << endl;
            }
            else if (eventInfo.dx_rt_recv.action==dxrt::dxrt_recov_t::DXRT_RECOV_DONE)
            {
                cout << "Device recovery is complete" << endl;
            }
            else
            {

                LOG_DXRT_ERR("Unknown data is received from device " << std::hex << eventInfo.dx_rt_recv.action << "\n");
                ShowPCIEDetails();
            }
        }
        else
        {
            LOG_DXRT_DBG << "!! unknown event occured from device "<< eventInfo.event_type << endl;
        }
        loopCnt++;
    }
    LOG_DXRT_DBG << threadName << " : End, LoopCount" << loopCnt << endl;
}

}  // namespace dxrt
