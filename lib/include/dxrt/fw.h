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

typedef struct
{
    uint32_t data_offset;
    uint32_t data_size;
    uint32_t flash_offset;
    uint32_t flash_size;
    uint32_t type;
    uint32_t crc32;
} dx_fw_image_info_t;

typedef struct
{
    char                signature[16];
    dx_fw_image_info_t  images[8];
    uint32_t            length;
    uint32_t            board_type;
    uint32_t            ddr_type;
    char                fw_ver[16];
} dx_fw_header_t;

enum fw_update_err_code_t : uint32_t  {
	FW_UPDATE_SUCCESS		= 0,
	ERR_HEADER_MISMATCH 	= 1 << 1,
	ERR_BOARD_TYPE			= 1 << 2,
	ERR_DDR_TYPE			= 1 << 3,
	ERR_CRC_MISMATCH		= 1 << 4,
	ERR_SF_ERASE			= 1 << 5,
	ERR_SF_FLASH			= 1 << 6,
	ERR_LOW_FW_VER			= 1 << 7,
};

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

class Fw
{
public:
    Fw(std::string file);
    ~Fw();
    void Show();
    std::string GetFwBinVersion();
    bool IsMatchSignature();
    std::string GetFwUpdateResult(uint32_t);
private:
    dx_fw_header_t fwHeader;
};

} // namespace dxrt