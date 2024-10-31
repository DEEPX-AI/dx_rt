// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/worker.h"
#include "dxrt/device.h"

namespace dxrt {


DeviceErrorWorker::DeviceErrorWorker(string name_, Device *device_)
: Worker(name_, Type::DEVICE_ERROR, 1, device_, nullptr)
{
    InitializeThread();
}
DeviceErrorWorker::~DeviceErrorWorker()
{
}
shared_ptr<DeviceErrorWorker> DeviceErrorWorker::Create(string name_, Device *device_)
{
    shared_ptr<DeviceErrorWorker> ret = make_shared<DeviceErrorWorker>(name_, device_);
    return ret;
}
void DeviceErrorWorker::ThreadWork(int id)
{
    std::ignore = id;
    string threadName = getName();

    int loopCnt = 0;  // ret;
    LOG_DXRT_DBG << getName() << " : Entry" << endl;
    // int devId = _device->id();
#ifdef WORKER_USE_PROFILER
    auto& profiler = dxrt::Profiler::GetInstance();
#endif
    dxrt_cmd_t cmd = dxrt::dxrt_cmd_t::DXRT_CMD_ERROR;
    while (true)
    {
        LOG_DXRT_DBG << threadName << " : wait" << endl;
        if (_stop)
        {
            LOG_DXRT_DBG << threadName << " : requested to stop thread." << endl;
            break;
        }
        dxrt::dx_pcie_dev_err_t errInfo;
        _device->Process(cmd, &errInfo);
        // LOG_VALUE((int)errInfo.err_code);
        if (static_cast<dxrt::dxrt_error_t>(errInfo.err_code)!=dxrt::dxrt_error_t::ERR_NONE)
            kill(getpid(), SIGINT);
        DXRT_ASSERT(static_cast<dxrt::dxrt_error_t>(errInfo.err_code)==dxrt::dxrt_error_t::ERR_NONE, errInfo);
        loopCnt++;
    }
    LOG_DXRT_DBG << threadName << " : End" << endl;
}

}  // namespace dxrt
