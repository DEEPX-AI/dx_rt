// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

typedef struct DXRT_API device_status
{
    uint32_t voltage[4];
    uint32_t clock[4];
    uint32_t temperature[4];
    uint32_t dvfs_enable;
    uint32_t dvfs_maxfreq;
    uint32_t count[4];
    uint32_t boot_state;
} dxrt_device_status_t;
typedef struct DXRT_API
{
    uint64_t timestamp;
    uint32_t cmd;
    uint32_t args[5];
} dxrt_device_log_t;


