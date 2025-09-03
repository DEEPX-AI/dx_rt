/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include "dxrt/common.h"
#include "dxrt/device.h"

namespace dxrt {

typedef enum DXRT_API
{
    FW_LOG_TEMP = 0x10000000,
    FW_LOG_DXRT_DEQUEUE_IRQ,
    FW_LOG_DXRT_DEQUEUE_POLLING,
    FW_LOG_DXRT_DEQUEUE_POPED,
    FW_LOG_INFERENCE_REQUEST,
    FW_LOG_INFERENCE_RESPONSE,
    FW_LOG_GENERATE_MSI,
    FW_LOG_NPU_HANG,
    FW_LOG_NORMAL_LOCK_IRQ,
    FW_LOG_NORMAL_UNLOCK_IRQ,
    FW_LOG_HIGH_LOCK_IRQ,
    FW_LOG_HIGH_UNLOCK_IRQ,
    FW_LOG_TASK_LOCK,
    FW_LOG_VOLT_UNDER_IRQ,
    FW_LOG_MAX,
} dxrt_fwlog_cmd_t;

typedef struct DXRT_API
{
    uint32_t data_offset;
    uint32_t data_size;
    uint32_t flash_offset;
    uint32_t flash_size;
    uint32_t type;
    uint32_t crc32;
} dx_fw_image_info_t;

typedef struct DXRT_API
{
    char                signature[16];
    dx_fw_image_info_t  images[8];
    uint32_t            length;
    uint32_t            board_type;
    uint32_t            ddr_type;
    char                fw_ver[16];
} dx_fw_header_t;

enum DXRT_API fw_update_err_code_t : uint32_t  {
    FW_UPDATE_SUCCESS       = 0,
    ERR_HEADER_MISMATCH     = 1 << 1,
    ERR_BOARD_TYPE          = 1 << 2,
    ERR_DDR_TYPE            = 1 << 3,
    ERR_CRC_MISMATCH        = 1 << 4,
    ERR_SF_ERASE            = 1 << 5,
    ERR_SF_FLASH            = 1 << 6,
    ERR_LOW_FW_VER          = 1 << 7,
    ERR_NOT_SUPPORT         = 1 << 8
};

class DXRT_API FwLog
{
public:
    FwLog(std::vector<dxrt_device_log_t>);
    ~FwLog();
    std::string str();
    void ToFileAppend(std::string file);
    void SetDeviceInfoString(std::string str)
    { _deviceInfoString = str; }
private:
    std::vector<dxrt_device_log_t> _logs;
    std::string _str;
    std::string _deviceInfoString;
};

class DXRT_API Fw
{
public:
    Fw(std::string file);
    ~Fw();
    void Show();
    std::string GetFwBinVersion();
    bool IsMatchSignature();
    std::string GetFwUpdateResult(uint32_t);
    uint32_t GetBoardType();
    std::string GetBoardTypeString();
    std::string GetDdrTypeString();
private:
    dx_fw_header_t fwHeader;
};

} // namespace dxrt