// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

typedef struct DXRT_API device_status
{
    uint32_t voltage[4];
    uint32_t clock[4];
    uint32_t temperature[4];
    uint32_t ddr_status[4];
    uint32_t dvfs_enable;
    uint32_t dvfs_maxfreq;
    uint32_t count[4];
    uint32_t boot_state;
    uint32_t ddr_sbe_cnt[4];
    uint32_t ddr_dbe_cnt[4];
} dxrt_device_status_t;

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
    uint32_t        rt_drv_ver;
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
