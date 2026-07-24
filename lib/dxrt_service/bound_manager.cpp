/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "bound_manager.h"

#include <algorithm>
#include <errno.h>

#include "dxrt/common.h"

using std::to_string;

namespace dxrt {

BoundManager::BoundManager(std::shared_ptr<DeviceCore> core)
    : _core(std::move(core)), _deviceId(_core->id())
{
    _bound_count.fill(0);
}

int BoundManager::Process(dxrt_cmd_t cmd, void* data, uint32_t size, uint32_t sub_cmd) const
{
    return _core->Process(cmd, data, size, sub_cmd);
}

int BoundManager::BoundOption(dxrt_sche_sub_cmd_t subCmd, npu_bound_op boundOp) const
{
    LOG_DXRT_S_DBG << "Device " << _deviceId << " " << subCmd << " bound " << boundOp << std::endl;
    return Process(dxrt::dxrt_cmd_t::DXRT_CMD_SCHEDULE,
                   static_cast<void*>(&boundOp),
                   sizeof(dxrt_sche_sub_cmd_t),
                   subCmd);
}

int BoundManager::GetBoundTypeCountInternal() const
{
    return static_cast<int>(std::count_if(_bound_count.begin(), _bound_count.end(),
                                          [](int c) { return c > 0; }));
}

int BoundManager::AddBound(npu_bound_op boundOp)
{
    UniqueLock lk(_lock);
    LOG_DXRT_S << "Device " << _deviceId << " ADD bound " << boundOp << std::endl;

    if (_bound_count[static_cast<int>(boundOp)] > 0)
    {
        _bound_count[static_cast<int>(boundOp)]++;
        return 0;
    }
    int ret = BoundOption(dxrt_sche_sub_cmd_t::DX_SCHED_ADD, boundOp);
    if (ret == 0)
        _bound_count[static_cast<int>(boundOp)]++;
    else
        LOG_DXRT_S_ERR("Failed to add bound option: " << ret);
    return ret;
}

int BoundManager::DeleteBound(npu_bound_op boundOp)
{
    UniqueLock lk(_lock);
    LOG_DXRT_S_DBG << "Device " << _deviceId << " DELETE bound " << boundOp << std::endl;

    if (_bound_count[static_cast<int>(boundOp)] > 1)
    {
        _bound_count[static_cast<int>(boundOp)]--;
        return 0;
    }
    int ret = BoundOption(dxrt_sche_sub_cmd_t::DX_SCHED_DELETE, boundOp);
    if (ret == 0)
        _bound_count[static_cast<int>(boundOp)]--;
    else
        LOG_DXRT_S_ERR("Failed to delete bound option: " << ret);
    return ret;
}

int BoundManager::GetBoundCount(npu_bound_op boundOp)
{
    SharedLock lk(_lock);
    return _bound_count[static_cast<int>(boundOp)];
}

int BoundManager::GetBoundTypeCount()
{
    SharedLock lk(_lock);
    return GetBoundTypeCountInternal();
}

bool BoundManager::CanAcceptBound(npu_bound_op boundOp)
{
    SharedLock lk(_lock);
    if (_bound_count[static_cast<int>(boundOp)] > 0)
        return true;

    if (GetBoundTypeCountInternal() >= 3)
    {
        std::string msg = "Current Bound: ";
        for (size_t i = 0; i < _bound_count.size(); i++)
        {
            if (_bound_count[i] > 0)
                msg += to_string(i) + "," + to_string(_bound_count[i]) + " ";
        }
        msg += "cannot accept new bound " + to_string(static_cast<int>(boundOp));
        LOG_DXRT_DBG << msg << std::endl;
    }
    return GetBoundTypeCountInternal() < 3;
}

}  // namespace dxrt
