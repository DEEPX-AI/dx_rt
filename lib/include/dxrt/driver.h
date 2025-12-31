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
#ifdef __linux__
    #include <linux/ioctl.h>
#elif _WIN32
    #include <windows.h>
#endif

#ifndef DEVICE_FILE
#define DEVICE_FILE "dxrt"
#endif

namespace dxrt {

/**********************/
/* RT/driver sync     */

#define MAX_CHECKPOINT_COUNT 3

typedef enum {
    DXRT_EVENT_NONE,
    DXRT_EVENT_ERROR,
    DXRT_EVENT_NOTIFY_THROT,
    DXRT_EVENT_RECOVERY,
    DXRT_EVENT_NUM,
} dxrt_event_t;

typedef enum {
    ERR_NONE      = 0,
    ERR_NPU0_HANG = 1,
    ERR_NPU1_HANG,
    ERR_NPU2_HANG,
    ERR_NPU_BUS,
    ERR_PCIE_DMA_CH0_FAIL = 100,
    ERR_PCIE_DMA_CH1_FAIL,
    ERR_PCIE_DMA_CH2_FAIL,
    ERR_LPDDR_DED_WR     = 200,
    ERR_LPDDR_DED_RD,
    ERR_DEVICE_ERR       = 1000,
} dxrt_error_t;

typedef enum {
    NTFY_NONE       = 0,
    NTFY_THROT_FREQ_DOWN,
    NTFY_THROT_FREQ_UP,
    NTFY_THROT_VOLT_DOWN,
    NTFY_THROT_VOLT_UP,
    NTFY_EMERGENCY_BLOCK,
    NTFY_EMERGENCY_RELEASE,
    NTFY_EMERGENCY_WARN = 300,
    /* TODO */
    /* NPU BOUND FOR THROT? */
} dxrt_notify_throt_t;

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

typedef enum {
    DXRT_RECOV_RMAP     = 1,
    DXRT_RECOV_WEIGHT   = 2,
    DXRT_RECOV_CPU      = 3,
    DXRT_RECOV_DONE     = 4,
} dxrt_recov_t;

typedef struct _dx_pcie_dev_err {
    uint32_t err_code;

    /* Version */
    uint32_t fw_ver;
    uint32_t rt_driver_version;
    uint32_t pcie_driver_version;
    uint32_t reserved_ver[4];

    /* Npu information */
    uint32_t npu_id;
    uint64_t base_axi;
    uint32_t base_rmap;
    uint32_t base_weight;
    uint32_t base_in;
    uint32_t base_out;
    uint32_t cmd_num;
    uint32_t last_cmd;
    uint32_t busy;
    uint32_t abnormal_cnt;
    uint32_t irq_status;
    uint32_t dma_err;
    uint32_t reserved_npu[10];

    /* System infomation power / temperature, etc,,,, */
    uint32_t temperature[20];
    uint32_t npu_voltage[4];
    uint32_t npu_freq[4];
    uint32_t reserved_sys[10];

    /* PCIe information */
    uint8_t  bus;
    uint8_t  dev;
    uint8_t  func;
    uint8_t  reserved;
    int      speed; /* GEN1, GEN2...*/
    int      width; /* 1, 2, 4 */
    uint32_t ltssm;
    uint32_t dma_rd_ch_sts[4];
    uint32_t dma_wr_ch_sts[4];
    uint32_t reserved_pcie[10];

    /* DDR information */
    uint32_t ddr_mr_reg[4];
    uint16_t ddr_freq;
    uint16_t ddr_type;
    uint32_t dbe_cnt[4];
    uint32_t reserved_ddr[5];

    /* Extra Version information */
    char rt_driver_version_suffix[16];
    char fw_version_suffix[16];

} dx_pcie_dev_err_t;

typedef struct _dx_pcie_dev_ntfy_throt {
    uint32_t ntfy_code;
    uint32_t npu_id;
    uint32_t throt_voltage[2];      // [0] current, [1] target
    uint32_t throt_freq[2];         // [0] current, [1] target
    uint32_t throt_temper;
} dx_pcie_dev_ntfy_throt_t;

typedef struct {
    uint32_t action;
} dx_pcie_dev_recovery_t;

/* CMD : DXRT_CMD_CUSTOM, SUBCMD : WEIGHT_INFO */
typedef struct {
    uint32_t address;   /* weight base address of device memory */
    uint32_t size;      /* weight size */
    uint32_t checksum;  /* Bitwiase XOR */
} dxrt_custom_weight_info_t;

#pragma pack(push, 1)
typedef struct otp_info {
    uint32_t    JEP_ID : 8;
    uint32_t    CONTINUATION_CODE : 8;
    char        CHIP_NAME[2];
    char        DEVICE_REV[2];
    uint32_t    RESERVED0 : 16;
    uint32_t    ECID;
    char        FOUNDRY_FAB[4];
    char        PROCESS[4];
    char        LOT_ID[12];
    char        WAFER_ID[4];
    char        X_AXIS[4];
    char        Y_AXIS[4];
    char        TEST_PGM[4];
    char        BARCODE[16];
    uint32_t    BARCODE_IDX;
} otp_info_t;
#pragma pack(pop)

typedef struct fct_result
{
    uint32_t wr_margin[4];
    uint32_t rd_margin[4];
    uint8_t ddr_margin;
    uint8_t ddr_mf;
    uint8_t i2c_fail;
    uint8_t test_done;
    uint32_t reserved;
    uint32_t reserved32[15];
} dxrt_fct_result_t;

typedef struct _dx_pcie_dev_event {
    uint32_t event_type;
    union {
        dx_pcie_dev_err_t           dx_rt_err;
        dx_pcie_dev_ntfy_throt_t    dx_rt_ntfy_throt;
        dx_pcie_dev_recovery_t      dx_rt_recv;
    };
} dx_pcie_dev_event_t;

typedef struct device_info {
    uint32_t type = 0; /* 0: ACC type, 1: STD type */
    uint32_t variant = 0; /* 100: L1, 101: L2, 102: L3, 103: L4, 104: V3,
                        200: M1, 201: M1A */
    uint64_t mem_addr = 0;
    uint64_t mem_size = 0;
    uint32_t num_dma_ch = 0;
    uint16_t fw_ver = 0;                // firmware version. A.B.C (e.g. 103)
    uint16_t bd_rev = 0;                // board revision. /10 (e.g. 1 = 0.1, 2 = 0.2)
    uint16_t bd_type = 0;               // board type. (1 = SOM, 2 = M.2, 3 = H1)
    uint16_t ddr_freq = 0;              // ddr frequency. (e.g. 4200, 5500)
    uint16_t ddr_type = 0;              // ddr type. (1 = lpddr4, 2= lpddr5)
#ifdef __linux__
    uint16_t interface = 0;
#elif _WIN32
    uint16_t interface_value = 0;
#endif
    char     fw_ver_suffix[16];
    uint8_t  reserved[48];
    uint16_t chip_offset = 0;
} dxrt_device_info_t;

typedef struct _dxrt_meminfo_t {
    uint64_t data = 0;
    uint64_t base = 0;
    uint32_t offset = 0;
    uint32_t size = 0;
} dxrt_meminfo_t;

typedef struct _dxrt_req_meminfo_t
{
    uint64_t data;
    uint64_t base;
    uint32_t offset;
    uint32_t size;
    uint32_t ch;
} dxrt_req_meminfo_t;

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
    //uint32_t  custom_offset = 0; // [TODO] necessary for V8 PPCPU on STD
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
    uint32_t  datas[MAX_CHECKPOINT_COUNT] = { 0, };
    int32_t   dma_ch = 0;
    uint32_t  op_mode = 0;   /* operation mode - 1:large model */
    uint32_t  custom_offset = 0;
    uint32_t  proc_id = 0;
    uint32_t  prior = 0;        /* scheduler option - priority(npu_priority_op) */
    uint32_t  prior_level = 0;  /* scheduler option - priority level */
    uint32_t  bandwidth = 0;    /* scheduler option - bandwith(npu_bandwidth_op) */
    uint32_t  bound = 0;        /* scheduler option - bound   (npu_bound_op) */
    uint32_t  queue = 0;
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
    int32_t   dma_ch            = 0;
    uint32_t  ddr_wr_bw         = 0; /* unit : KB/s */
    uint32_t  ddr_rd_bw         = 0; /* unit : KB/s */
    uint64_t  wait_timestamp    = 0; /* duration in microseconds for Process(DXRT_CMD_NPU_RUN_RESP) call (measured by profiler) */
    uint64_t  wait_start_time   = 0; /* start time in nanoseconds when Process(DXRT_CMD_NPU_RUN_RESP) was called */
    uint64_t  wait_end_time     = 0; /* end time in nanoseconds when Process(DXRT_CMD_NPU_RUN_RESP) returned */
} dxrt_response_t;

typedef struct _dxrt_message
{
    int32_t cmd = 0;
    int32_t sub_cmd = 0;
    void* data = NULL;
    uint32_t size = 0;
} dxrt_message_t;
typedef struct _dxrt_device_message {
    uint32_t cmd = 0;    /* command */
    uint32_t ack = 0;    /* Response from device */
    uint32_t size = 0;    /* Data Size */
    uint32_t data[1000] = {0,};
} dxrt_device_message_t;
typedef enum {
    DXRT_CMD_IDENTIFY_DEVICE    = 0, /* Sub-command */
    DXRT_CMD_GET_STATUS         ,
    DXRT_CMD_RESET              ,
    DXRT_CMD_UPDATE_CONFIG      ,
    DXRT_CMD_UPDATE_FIRMWARE    , /* Sub-command */
    DXRT_CMD_GET_LOG            ,
    DXRT_CMD_DUMP               ,
    DXRT_CMD_WRITE_MEM          ,
    DXRT_CMD_READ_MEM           ,
    DXRT_CMD_CPU_CACHE_FLUSH    ,
    DXRT_CMD_SOC_CUSTOM         ,
    DXRT_CMD_WRITE_INPUT_DMA_CH0,
    DXRT_CMD_WRITE_INPUT_DMA_CH1,
    DXRT_CMD_WRITE_INPUT_DMA_CH2,
    DXRT_CMD_READ_OUTPUT_DMA_CH0,
    DXRT_CMD_READ_OUTPUT_DMA_CH1,
    DXRT_CMD_READ_OUTPUT_DMA_CH2,
    DXRT_CMD_TERMINATE_EVENT    ,
    DXRT_CMD_EVENT              ,
    DXRT_CMD_DRV_INFO           , /* Sub-command */
    DXRT_CMD_SCHEDULE           , /* Sub-command */
    DXRT_CMD_UPLOAD_FIRMWARE    ,
    DXRT_CMD_NPU_RUN_REQ        ,
    DXRT_CMD_NPU_RUN_RESP       ,
    DXRT_CMD_UPDATE_CONFIG_JSON ,
    DXRT_CMD_RECOVERY           ,
    DXRT_CMD_CUSTOM             , /* Sub-command */
    DXRT_CMD_START              ,
    DXRT_CMD_TERMINATE          ,
    DXRT_CMD_PCIE               , /* Sub-command */
    DXRT_CMD_MAX                ,
} dxrt_cmd_t;

/* CMD : DXRT_CMD_IDENTIFY_DEVICE*/
typedef enum {
    DX_IDENTIFY_NONE        = 0,
    DX_IDENTIFY_FWUPLOAD    = 1,
} dxrt_ident_sub_cmd_t;

typedef enum {
    DX_SCHED_ADD    = 1,
    DX_SCHED_DELETE = 2
} dxrt_sche_sub_cmd_t;

typedef enum {
    DRVINFO_CMD_GET_RT_INFO   = 0,
    DRVINFO_CMD_GET_PCIE_INFO = 1,
    DRVINFO_CMD_GET_RT_INFO_V2   = 2,
} dxrt_drvinfo_sub_cmd_t;

typedef enum {
    DX_SET_DDR_FREQ         = 1,
    DX_GET_OTP              = 2,
    DX_SET_OTP              = 3,
    DX_SET_LED              = 4,
    DX_ADD_WEIGHT_INFO      = 5,
    DX_DEL_WEIGHT_INFO      = 6,
    DX_INIT_PPCPU           = 10,
    DX_UPLOAD_MODEL         = 100,
    DX_INTERNAL_TESTCASE    = 200,
    DX_GET_FCT_TESTCASE_RESULT   = 201,
    DX_RUN_FCT_TESTCASE     = 202,
    DX_INTERNAL_GET_SOC_ID  = 300,
} dxrt_custom_sub_cmt_t;

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

typedef enum {
    DX_GET_PCIE_INFO = 0,
    DX_CLEAR_ERR_STAT = 1,
} dxrt_pcie_sub_cmd_t;

/* CMD : DXRT_CMD_UPDATE_FIRMWARE */
typedef enum {
    FWUPDATE_ONLY        = 0,
    FWUPDATE_DEV_UNRESET = 1 << 1,
    FWUPDATE_FORCE       = 1 << 2,
} dxrt_fwupdate_sub_cmd_t;

#define DXRT_IOCTL_MAGIC     'D'
#ifdef _WIN32
#define DX_NRM_IOCTL(index)                        CTL_CODE(FILE_DEVICE_VIDEO, index, METHOD_BUFFERED, FILE_ANY_ACCESS)
#endif
typedef enum {
#ifdef __linux__
    DXRT_IOCTL_MESSAGE = _IOW(DXRT_IOCTL_MAGIC, 0, dxrt_message_t),
    DXRT_IOCTL_DUMMY = _IOW(DXRT_IOCTL_MAGIC, 1, dxrt_message_t),
    DXRT_IOCTL_MAX
#elif _WIN32
    DXRT_IOCTL_MESSAGE = DX_NRM_IOCTL(0x001),
    DXRT_IOCTL_DUMMY = _IOW(DXRT_IOCTL_MAGIC, 1, dxrt_message_t),
    //IOCTL_DXM1_MAP_CODE            = DX_NRM_IOCTL(0x101),
    //IOCTL_DXM1_UNMAP_CODE        = DX_NRM_IOCTL(0x102),
    //IOCTL_DXM1_DMA_EVENT_CODE    = DX_NRM_IOCTL(0x201),
    //IOCTL_DXM1_MSG_EVENT_CODE    = DX_NRM_IOCTL(0x202),
    //IOCTL_DXM1_DMA_CH_READ        = DX_NRM_IOCTL(0x301),
    //IOCTL_DXM1_DMA_CH_WRITE        = DX_NRM_IOCTL(0x302),
    //IOCTL_DXM1_DMA_MEM_READ        = DX_NRM_IOCTL(0x401),
    //IOCTL_DXM1_DMA_MEM_WRITE    = DX_NRM_IOCTL(0x402),
    DXRT_IOCTL_MAX
#endif
} dxrt_ioctl_t;

/**********************/



typedef struct _dxrt_model
{
    int16_t npu_id;
    int8_t  type = 0; // 0: normal, 1: argmax, 2: ppu
    int8_t  format = 0; // 0: none, 1: formatted, 2: aligned, 3: pre_formatter, 4: pre_im2col
    int32_t cmds;
    dxrt_meminfo_t rmap;
    dxrt_meminfo_t weight;
    uint32_t input_all_offset;
    uint32_t input_all_size;
    uint32_t output_all_offset;
    uint32_t output_all_size;
    uint32_t last_output_offset;
    uint32_t last_output_size;
    uint32_t  checkpoints[MAX_CHECKPOINT_COUNT] = {0, 0, 0};
    uint32_t  op_mode = 0;   /* operation mode - 1:large model */
} dxrt_model_t;

extern DXRT_API std::vector<std::pair<int, std::string>> ioctlTable;
extern DXRT_API std::string ErrTable(dxrt_error_t error);
DXRT_API std::ostream& operator<<(std::ostream& os, const dx_pcie_dev_err_t& error);
DXRT_API std::ostream& operator<<(std::ostream&, const dxrt_error_t&);
DXRT_API std::ostream& operator<<(std::ostream&, const dxrt_meminfo_t&);
DXRT_API std::ostream& operator<<(std::ostream&, const dxrt_request_t&);
DXRT_API std::ostream& operator<<(std::ostream&, const dxrt_request_acc_t&);
DXRT_API std::ostream& operator<<(std::ostream&, const dxrt_response_t&);
DXRT_API std::ostream& operator<<(std::ostream&, const dxrt_model_t&);
DXRT_API std::ostream& operator<<(std::ostream&, const dxrt_device_info_t&);
DXRT_API std::ostream& operator<<(std::ostream&, const otp_info_t&);
DXRT_API std::ostream& operator<<(std::ostream&, const fct_result&);

DXRT_API std::string dxrt_cmd_t_str(dxrt::dxrt_cmd_t c);

} // namespace dxrt
