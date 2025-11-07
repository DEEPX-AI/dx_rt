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
#include <cstdint>



typedef struct DXRT_API device_status
{
    uint32_t voltage[4];
    uint32_t clock[4];
    uint32_t temperature[4];
    uint32_t ddr_status[4];
    uint32_t reserved_0[2];
    uint32_t count[4];
    uint8_t reserved_1[4];
    uint32_t ddr_sbe_cnt[4];
    uint32_t ddr_dbe_cnt[4];
} dxrt_device_status_t;

typedef struct {
    uint32_t rx_err_status;
    uint32_t bad_tlp_status;
    uint32_t bad_dllp_status;
    uint32_t replay_no_roleover_status;
    uint32_t rpl_timer_timeout_status;
    uint32_t advisory_non_fatal_err_status;
    uint32_t corrected_int_err_status;
    uint32_t header_log_overflow_status;
} p_corr_err_t;

typedef struct {
    uint32_t dl_protocol_err_status;
    uint32_t surprise_down_err_status;
    uint32_t fc_protocol_err_status;
    uint32_t rec_overflow_err_status;
    uint32_t malf_tlp_err_status;
    uint32_t internal_err_status;
} p_fatal_err_t;

typedef struct {
    uint32_t pois_tlp_err_status;
    uint32_t cmplt_timeout_err_status;
    uint32_t cmplt_abort_err_status;
    uint32_t unexp_cmplt_err_status;
    uint32_t ecrc_err_status;
    uint32_t unsupported_req_err_status;
    uint32_t tlp_prfx_blocked_err_status;
} p_nonfatal_err_t;

typedef struct {
    p_corr_err_t corr;
    p_fatal_err_t fatal;
    p_nonfatal_err_t non_fatal;
} dxrt_pcie_err_stat_t;

typedef struct {
    uint32_t ebuf_ovfl;
    uint32_t ebuf_unfl;
    uint32_t decode_err;
    uint32_t skp_os_parity_err;
    uint32_t disparity_err;
    uint32_t sync_header_err;
} p_evt_by_lane;

typedef struct {
    uint32_t detect_ei; /* PCIE_EVT_GRP_1_NUM */
    uint32_t rx_err;
    uint32_t rx_recovery_req;
    uint32_t n_fts_tout;
    uint32_t framing_err;
    uint32_t deskew_err;
    uint32_t bad_tlp; /* PCIE_EVT_GRP_2_NUM */
    uint32_t lcrc_err;
    uint32_t bad_dllp;
    uint32_t replay_num_rollover;
    uint32_t replay_tout;
    uint32_t rx_nak_dllp;
    uint32_t tx_nak_dllp;
    uint32_t retry_tlp;
    uint32_t fc_tout; /* PCIE_EVT_GRP_3_NUM */
    uint32_t poisoned_tlp;
    uint32_t ecrc_err;
    uint32_t ua;
    uint32_t ca;
    uint32_t c_tout;
} p_evt_common;

typedef struct {
    p_evt_by_lane lane[4];
    p_evt_common common;
} dxrt_pcie_evt_stat_t;

typedef struct {
    uint32_t p_state;
    uint32_t d_state;
    uint32_t l_state;
} dxrt_pcie_power_stat_t;

typedef struct {
    uint32_t id;
    uint32_t cs;
    uint32_t cb;
    uint32_t tcb;
    uint32_t llp;
    uint32_t lie;
    uint32_t rie;
    uint32_t ccs;
    uint32_t lle;
    uint32_t func_num;
    uint32_t tc_tlp_header;
    uint32_t at_tlp_header;
    uint32_t t_size;
    uint32_t sar_msb;
    uint32_t sar_lsb;
    uint32_t dar_msb;
    uint32_t dar_lsb;
    uint32_t llp_msb;
    uint32_t llp_lsb;
} dma_ch;

typedef struct {
    dma_ch r_ch[4];
    dma_ch w_ch[4];
} dxrt_dma_stat_t;

typedef struct {
    uint32_t phy_stat;
    uint32_t dll_stat;
    dxrt_pcie_power_stat_t power_stat;
    dxrt_pcie_err_stat_t err_stat;
    dxrt_dma_stat_t dma_stat;
    dxrt_pcie_evt_stat_t evt_stat;
} dxrt_pcie_info_t;

struct DXRT_API deepx_pcie_info {
    uint32_t driver_version;
    uint8_t  bus;
    uint8_t  dev;
    uint8_t  func;
    int      speed; /* GEN1, GEN2...*/
    int      width; /* 1, 2, 4 */
};

typedef struct DXRT_API
{
    unsigned int driver_version;
    char driver_version_suffix[16];
    unsigned int reserved[16];
} dxrt_rt_drv_version_t;

typedef struct DXRT_API
{
    dxrt_rt_drv_version_t rt_drv_ver;
    deepx_pcie_info pcie;
} dxrt_dev_info_t;


#pragma pack(push, 4)
typedef struct DXRT_API
{
    uint64_t timestamp;
    uint32_t cmd;
    uint32_t args[6];
} dxrt_device_log_t;
#pragma pack(pop)


enum class DeviceType : uint32_t
{
    ACC_TYPE = 0,
    STD_TYPE = 1,
};
