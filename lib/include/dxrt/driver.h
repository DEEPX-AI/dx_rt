// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "dxrt/common.h"
#include <linux/ioctl.h>

namespace dxrt {

/**********************/
/* RT/driver sync     */

typedef enum {
    ERR_NONE      = 0,
    ERR_NPU0_HANG = 1,
    ERR_NPU1_HANG,
    ERR_NPU2_HANG,
    ERR_PCIE_DMA_CH0_FAIL = 100,
    ERR_PCIE_DMA_CH1_FAIL,
    ERR_PCIE_DMA_CH2_FAIL,
} dxrt_error_t;

typedef enum _npu_priority_op {
    N_PRIORITY_NORMAL = 0,
    N_PRIORITY_HIGH,
} npu_priority_op;

typedef enum _npu_bandwidth_op {
    N_BANDWIDTH_NORMAL = 0,
    N_BANDWIDTH_NPU0,
    N_BANDWIDTH_NPU1,
    N_BANDWIDTH_NPU2,
    N_BANDWIDTH_PCIE,
    N_BANDWIDTH_MAX,
} npu_bandwidth_op;

typedef enum _npu_bound_op {
    N_BOUND_NORMAL = 0,     /*inference with 3-npu */
    N_BOUND_INF_ONLY_NPU0,
    N_BOUND_INF_ONLY_NPU1,
    N_BOUND_INF_ONLY_NPU2,
    N_BOUND_INF_2_NPU_01,   /* Infrence with 2-npu */
    N_BOUND_INF_2_NPU_12,   /* Infrence with 2-npu */
    N_BOUND_INF_2_NPU_02,   /* Infrence with 2-npu */
    N_BOUND_INF_MAX,
} npu_bound_op;

typedef struct device_info {
    uint32_t type = 0; /* 0: ACC type, 1: STD type */
    uint32_t variant = 0; /* 100: L1, 101: L2, 102: L3, 103: L4,
                        200: M1, 201: M1A */
    uint64_t mem_addr = 0;
    uint64_t mem_size = 0;
    uint32_t num_dma_ch = 0;
    uint16_t fw_ver = 0;                // firmware version. A.B.C (e.g. 103)
    uint16_t bd_rev = 0;                // board revision. /10 (e.g. 1 = 0.1, 2 = 0.2)
    uint16_t bd_type = 0;               // board type. (1 = SOM, 2 = M.2, 3 = H1)
    uint16_t ddr_freq = 0;              // ddr frequency. (e.g. 4200, 5500)
    uint16_t ddr_type = 0;              // ddr type. (1 = lpddr4, 2= lpddr5)
    uint16_t interface = 0;
    char     fw_info[64] = "";
} dxrt_device_info_t;

typedef struct _dxrt_meminfo_t {
    uint64_t data = 0;
    uint64_t base = 0;
    uint32_t offset = 0;
    uint32_t size = 0;
} dxrt_meminfo_t;

typedef struct _dxrt_request_t {
    uint32_t  req_id = 0;
    dxrt_meminfo_t input;
    dxrt_meminfo_t output;
    uint32_t  model_type = 0;
    uint32_t  model_format = 0;
    uint32_t  model_cmds = 0;
    uint32_t  cmd_offset = 0;
    uint32_t  weight_offset = 0;
    uint32_t  last_output_offset = 0;
} dxrt_request_t;

typedef struct _dxrt_request_acc_t {
    uint32_t  req_id = 0;
    uint32_t  task_id = 0;
    dxrt_meminfo_t input;
    dxrt_meminfo_t output;
    int16_t   npu_id = 0;
    int8_t    model_type   = 0;
    int8_t    model_format = 0;
    uint32_t  model_cmds = 0;
    uint32_t  cmd_offset = 0;
    uint32_t  weight_offset = 0;
    uint32_t  model_cmds2 = 0; // for m1 8k
    uint32_t  cmd_offset2 = 0; // for m1 8k
    uint32_t  weight_offset2 = 0; // for m1 8k
    int32_t   dma_ch = 0;
    uint32_t  arg0 = 0; // additional parameter dependent to hw (for m1 8k)
    uint32_t  status = 0;
    uint32_t  prior;        /* scheduler option - priority(npu_priority_op) */
    uint32_t  prior_level;  /* scheduler option - priority level */
    uint32_t  bandwidth;    /* scheduler option - bandwith(npu_bandwidth_op) */
    uint32_t  bound;        /* scheduler option - bound   (npu_bound_op) */
    uint32_t  queue;
} dxrt_request_acc_t;

typedef struct _dxrt_response_t {
    uint32_t  req_id            = 0;
    uint32_t  inf_time          = 0;
    uint16_t  argmax            = 0;
    uint16_t  model_type        = 0;
    int32_t   status            = 0;
    uint32_t  ppu_filter_num    = 0;
    uint32_t  proc_id           = 0;
    uint32_t  queue             = 0;
} dxrt_response_t;

typedef struct
{
    int32_t cmd = 0;
    int32_t sub_cmd = 0;
    void* data = NULL;
    uint32_t size = 0;
} dxrt_message_t;
typedef struct {
    uint32_t cmd = 0;	/* command */
    uint32_t ack = 0;	/* Response from device */
    uint32_t size = 0;	/* Data Size */
    uint32_t data[1000] = {0,};
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
    DXRT_CMD_WRITE_INPUT_DMA_CH2,
    DXRT_CMD_READ_OUTPUT_DMA_CH0,
    DXRT_CMD_READ_OUTPUT_DMA_CH1,
    DXRT_CMD_READ_OUTPUT_DMA_CH2,
    DXRT_CMD_TERMINATE,
    DXRT_CMD_ERROR,
    DXRT_CMD_DRV_INFO, /* Sub-command */
    DXRT_CMD_SCHEDULE, /* Sub-command */
    DXRT_CMD_MAX,
} dxrt_cmd_t;

typedef enum {
    DX_SCHED_ADD    = 1,
    DX_SCHED_DELETE = 2
} dxrt_sche_sub_cmd_t;

typedef enum {
    DRVINFO_CMD_GET_RT_INFO   = 0,
    DRVINFO_CMD_GET_PCIE_INFO = 1,
} dxrt_drvinfo_sub_cmd_t;

typedef enum device_type
{
    DEVICE_TYPE_ACCELERATOR = 0,
    DEVICE_TYPE_STANDALONE = 1,
} dxrt_device_type_t;

typedef enum device_interface
{
    DEVICE_INTERFACE_ASIC = 0,
    DEVICE_INTERFACE_FPGA = 1,
} dxrt_device_interface_t;

/* CMD : DXRT_CMD_UPDATE_FIRMWARE */
typedef enum {
    FWUPDATE_ONLY      = 0,
    FWUPDATE_DEV_RESET = 1 << 1,
    FWUPDATE_FORCE     = 1 << 2,
} dxrt_fwupdate_sub_cmd_t;

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
    int8_t  type; // 0: normal, 1: argmax, 2: ppu
    int8_t  format;
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