// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "dxrt/common.h"
#include <linux/ioctl.h>

namespace dxrt {

/**********************/
/* RT/driver sync     */

typedef enum
{
    ERR_NPU0_HANG = 1,
    ERR_NPU1_HANG,
    ERR_PCIE_DMA_CH0_FAIL,
    ERR_PCIE_DMA_CH1_FAIL,
} dxrt_error_t;

typedef struct device_info
{
    uint32_t type; /* 0: ACC type, 1: STD type */
    uint32_t variant; /* 100: L1, 101: L2, 102: L3, 103: L4,
                        200: M1, 201: M1A */
    uint64_t mem_addr;
    uint64_t mem_size;
    uint32_t num_dma_ch;
    uint16_t fw_ver;                // firmware version. A.B.C (e.g. 103)
    uint16_t bd_rev;                // board revision. /10 (e.g. 1 = 0.1, 2 = 0.2)
    uint16_t bd_type;               // board type. (1 = SOM, 2 = M.2, 3 = H1)
    uint16_t ddr_freq;              // ddr frequency. (e.g. 4200, 5500)
    uint16_t ddr_type;              // ddr type. (1 = lpddr4, 2= lpddr5)
    uint16_t interface;
    char     fw_info[64];
} dxrt_device_info_t;

typedef struct _dxrt_meminfo_t
{
    uint64_t data;
    uint64_t base;
    uint32_t offset;
    uint32_t size;
} dxrt_meminfo_t;

typedef struct _dxrt_request_t {
    uint32_t  req_id;
    dxrt_meminfo_t input;
    dxrt_meminfo_t output;
    uint32_t  model_type;
    uint32_t  model_cmds;
    uint32_t  cmd_offset;
    uint32_t  weight_offset;
    uint32_t  last_output_offset;
} dxrt_request_t;

typedef struct _dxrt_request_acc_t {
    uint32_t  req_id;
    uint32_t  task_id;
    dxrt_meminfo_t input;
    dxrt_meminfo_t output;
    int16_t   npu_id;
    int16_t   model_type;
    uint32_t  model_cmds;
    uint32_t  cmd_offset;
    uint32_t  weight_offset;
    uint32_t  model_cmds2; // for m1 8k
    uint32_t  cmd_offset2; // for m1 8k
    uint32_t  weight_offset2; // for m1 8k
    int32_t   dma_ch;
    uint32_t  arg0; // additional parameter dependent to hw (for m1 8k)
    uint32_t  status;
} dxrt_request_acc_t;

typedef struct _dxrt_response_t {
    uint32_t  req_id;
    uint32_t  inf_time;
    uint16_t   argmax;
    uint16_t   model_type;
    int32_t   status;
    uint32_t   ppu_filter_num;
} dxrt_response_t;

typedef struct
{
    int32_t cmd;
    void* data;
    uint32_t size;
} dxrt_message_t;
typedef struct
{
    uint32_t cmd;	/* command */
    uint32_t ack;	/* Response from device */
    uint32_t size;	/* Data Size */
    uint32_t data[1000];
} dxrt_device_message_t;
typedef enum {
    DXRT_CMD_IDENTIFY_DEVICE = 0,
    DXRT_CMD_GET_STATUS,
    DXRT_CMD_RESET,
    DXRT_CMD_UPDATE_CONFIG,
    DXRT_CMD_UPDATE_FIRMWARE,
    DXRT_CMD_GET_LOG,
    DXRT_CMD_DUMP,
    DXRT_CMD_WRITE_MEM,
    DXRT_CMD_READ_MEM,
    DXRT_CMD_CPU_CACHE_FLUSH,
    DXRT_CMD_SOC_CUSTOM,
    DXRT_CMD_WRITE_INPUT_DMA_CH0,
    DXRT_CMD_WRITE_INPUT_DMA_CH1,
    DXRT_CMD_READ_OUTPUT_DMA_CH0,
    DXRT_CMD_READ_OUTPUT_DMA_CH1,
    DXRT_CMD_TERMINATE,
    DXRT_CMD_MAX,
} dxrt_cmd_t;

#define DXRT_IOCTL_MAGIC     'D'
typedef enum {
    DXRT_IOCTL_MESSAGE = _IOW(DXRT_IOCTL_MAGIC, 0, dxrt_message_t),
    DXRT_IOCTL_DUMMY = _IOW(DXRT_IOCTL_MAGIC, 1, dxrt_message_t),
    DXRT_IOCTL_MAX
} dxrt_ioctl_t;

/**********************/

typedef struct
{
    int16_t npu_id;
    int16_t type; // 0: normal, 1: argmax, 2: ppu
    int32_t cmds;
    dxrt_meminfo_t cmd;
    dxrt_meminfo_t weight;
    uint32_t output_all_offset;
    uint32_t output_all_size;
    uint32_t last_output_offset;
    uint32_t last_output_size;
} dxrt_model_t;

extern std::vector<std::pair<int, std::string>> ioctlTable;
extern std::string ErrTable(dxrt_error_t error);
std::ostream& operator<<(std::ostream&, const dxrt_error_t&);
std::ostream& operator<<(std::ostream&, const dxrt_meminfo_t&);
std::ostream& operator<<(std::ostream&, const dxrt_request_t&);
std::ostream& operator<<(std::ostream&, const dxrt_request_acc_t&);
std::ostream& operator<<(std::ostream&, const dxrt_response_t&);
std::ostream& operator<<(std::ostream&, const dxrt_model_t&);
std::ostream& operator<<(std::ostream&, const dxrt_device_info_t&);

} // namespace dxrt