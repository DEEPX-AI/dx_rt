// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "dxrt/common.h"
#include "dxrt/device.h"

namespace dxrt {
typedef enum
{
    FW_LOG_TEMP = 0x10000000,
    FW_LOG_DXRT_DEQUEUE_REQUEST,
    FW_LOG_INFERENCE_REQUEST,
    FW_LOG_INFERENCE_RESPONSE,
    FW_LOG_GENERATE_MSI,
    FW_LOG_NPU_HANG,
    FW_LOG_MAX,
} dxrt_fwlog_cmd_t;
class FwLog
{
public:
    FwLog(std::vector<dxrt_device_log_t>);
    ~FwLog();
    std::string str();
    void toFile(std::string file);
private:
    std::vector<dxrt_device_log_t> _logs;
    std::string _str;
};

} // namespace dxrt