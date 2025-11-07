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

#include "dxrt/common.h"
#include "dxrt/cli.h"

#include <string>
#include <vector>

#include "dxrt/device.h"
#include "dxrt/fw.h"
#include "dxrt/util.h"
#include "dxrt/device_info_status.h"
#include "dxrt/filesys_support.h"
#include "dxrt/driver.h"
#include "dxrt/extern/rapidjson/document.h"
#include "dxrt/extern/rapidjson/istreamwrapper.h"
#include "dxrt/objects_pool.h"
#include "../lib/resource/log_messages.h"
#include "dxrt/device_version.h"
#include "dxrt/device_struct.h"
#include "dxrt/device_struct_operators.h"
#include "dxrt/device_pool.h"
#include "dxrt/cli_support.h"


namespace dxrt {

int UpdateFw(std::shared_ptr<DeviceCore> devicePtr, std::string fwFile, int subCmd)
{
    DXRT_ASSERT(fileExists(fwFile), fwFile + " doesn't exist.");
    std::vector<uint8_t> buf(getFileSize(fwFile));
    DataFromFile(fwFile, buf.data());
    return devicePtr->Process(dxrt::dxrt_cmd_t::DXRT_CMD_UPDATE_FIRMWARE, buf.data(), buf.size(), subCmd);
}

int UploadFw(std::shared_ptr<DeviceCore> devicePtr, std::string fwFile, int subCmd)
{
    DXRT_ASSERT(fileExists(fwFile), fwFile + " doesn't exist.");
    std::vector<uint8_t> buf(getFileSize(fwFile));
    DataFromFile(fwFile, buf.data());
    return devicePtr->Process(dxrt::dxrt_cmd_t::DXRT_CMD_UPLOAD_FIRMWARE, buf.data(), buf.size(), subCmd);
}

int UpdateFwConfig(std::shared_ptr<DeviceCore> devicePtr, std::string jsonFile)
{
    DXRT_ASSERT(fileExists(jsonFile), jsonFile + " doesn't exist.");
    std::vector<uint8_t> buf(getFileSize(jsonFile));
    DataFromFile(jsonFile, buf.data());
    devicePtr->Process(dxrt::dxrt_cmd_t::DXRT_CMD_UPDATE_CONFIG_JSON, buf.data(), buf.size());
    return buf[0];
}

std::vector<uint32_t> Dump(std::shared_ptr<DeviceCore> devicePtr)
{
    std::vector<uint32_t> dump(1000, 0);
    devicePtr->Process(dxrt::dxrt_cmd_t::DXRT_CMD_DUMP, dump.data());
    return dump;
}
void UpdateFwConfig(std::shared_ptr<DeviceCore> devicePtr, std::vector<uint32_t> cfg)
{
    int size = sizeof(uint32_t)*cfg.size();
    devicePtr->Process(dxrt::dxrt_cmd_t::DXRT_CMD_UPDATE_CONFIG, cfg.data(), size);
}
std::shared_ptr<FwLog> GetFwLog(std::shared_ptr<DeviceCore> devicePtr)
{
    std::vector<dxrt_device_log_t> logBuf(static_cast<int>(16 * 1024 / sizeof(dxrt_device_log_t)), {0, 0, {0, }});
    devicePtr ->Process(dxrt::dxrt_cmd_t::DXRT_CMD_GET_LOG, logBuf.data());
    auto fwlog = std::make_shared<FwLog>(logBuf);
    return fwlog;
}


}  // namespace dxrt
