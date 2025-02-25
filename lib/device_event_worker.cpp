// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/worker.h"
#include "dxrt/device.h"
#include "dxrt/profiler.h"

namespace dxrt {


DeviceEventWorker::DeviceEventWorker(string name_, Device *device_)
: Worker(name_, Type::DEVICE_EVENT, 1, device_, nullptr)
{
    InitializeThread();
}
DeviceEventWorker::~DeviceEventWorker()
{
    LOG_DXRT_DBG<<endl;
}
shared_ptr<DeviceEventWorker> DeviceEventWorker::Create(string name_, Device *device_)
{
    shared_ptr<DeviceEventWorker> ret = make_shared<DeviceEventWorker>(name_, device_);
    return ret;
}
void DeviceEventWorker::ThreadWork(int id)
{
    std::ignore = id;
    string threadName = getName();

    int loopCnt = 0;  // ret;
    LOG_DXRT_DBG << getName() << " : Entry" << endl;
    // int devId = _device->id();
    dxrt_cmd_t cmd = dxrt::dxrt_cmd_t::DXRT_CMD_EVENT;
    while (true)
    {
        LOG_DXRT_DBG << threadName << " : wait" << endl;
        if (_stop)
        {
            LOG_DXRT_DBG << threadName << " : requested to stop thread." << endl;
            break;
        }
        dxrt::dx_pcie_dev_event_t eventInfo;
        _device->Process(cmd, &eventInfo); //Waiting in kernel. (device::terminate())
        // LOG_VALUE((int)errInfo.err_code);
        if (static_cast<dxrt::dxrt_event_t>(eventInfo.event_type)==dxrt::dxrt_event_t::DXRT_EVENT_ERROR) {
            DXRT_ASSERT(static_cast<dxrt::dxrt_error_t>(eventInfo.dx_rt_err.err_code)==dxrt::dxrt_error_t::ERR_NONE, 
                eventInfo.dx_rt_err);
        }
        else if (static_cast<dxrt::dxrt_event_t>(eventInfo.event_type)==dxrt::dxrt_event_t::DXRT_EVENT_NOTIFY_THROT) {
            cout << eventInfo.dx_rt_ntfy_throt << endl;
        }
        else {
            LOG_DXRT_DBG << "!! unknown event occured from device\n";
        }
        loopCnt++;
    }
    LOG_DXRT_DBG << threadName << " : End" << endl;
}

}  // namespace dxrt
