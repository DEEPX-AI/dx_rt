/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 *
 * This file uses cxxopts (MIT License) - Copyright (c) 2014 Jarryd Beck.
 */

#pragma once
#include "dxrt/common.h"
#include <string>
#include "dxrt/device_core.h"
#include "dxrt/fw.h"
#include <vector>
#include <memory>




namespace dxrt {

    int UpdateFw(std::shared_ptr<DeviceCore> devicePtr, std::string fwFile, int subCmd);
    int UploadFw(std::shared_ptr<DeviceCore> devicePtr, std::string fwFile, int subCmd);
    int UpdateFwConfig(std::shared_ptr<DeviceCore> devicePtr, std::string jsonFile);
    std::vector<uint32_t> Dump(std::shared_ptr<DeviceCore> devicePtr);
    void UpdateFwConfig(std::shared_ptr<DeviceCore> devicePtr, std::vector<uint32_t> cfg);
    std::shared_ptr<FwLog> GetFwLog(std::shared_ptr<DeviceCore> devicePtr);
}  // namespace dxrt
