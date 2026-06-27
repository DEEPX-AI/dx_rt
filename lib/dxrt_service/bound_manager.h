/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <array>
#include <memory>

#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"
#include "dxrt/device_core.h"

namespace dxrt {

// BoundManager
//   Manages NPU scheduler bound options for a single device.
//   Tracks ref-counts per bound type so that the same bound is only
//   registered/deregistered once when multiple clients share it.
class BoundManager
{
 public:
    BoundManager(std::shared_ptr<DeviceCore> core);
    ~BoundManager() = default;

    BoundManager(const BoundManager&)            = delete;
    BoundManager& operator=(const BoundManager&) = delete;
    BoundManager(BoundManager&&)                 = delete;
    BoundManager& operator=(BoundManager&&)      = delete;

    int  AddBound(npu_bound_op boundOp);
    int  DeleteBound(npu_bound_op boundOp);
    int  GetBoundCount(npu_bound_op boundOp);
    int  GetBoundTypeCount();
    bool CanAcceptBound(npu_bound_op boundOp);

 private:
    int  Process(dxrt_cmd_t cmd, void* data, uint32_t size = 0, uint32_t sub_cmd = 0) const;
    int  BoundOption(dxrt_sche_sub_cmd_t subCmd, npu_bound_op boundOp) const;
    int  GetBoundTypeCountInternal() const;

    std::shared_ptr<DeviceCore> _core;
    int                            _deviceId;

    std::array<int, static_cast<int>(npu_bound_op::N_BOUND_INF_MAX)> _bound_count{};
    mutable SharedMutex _lock;
};

}  // namespace dxrt
