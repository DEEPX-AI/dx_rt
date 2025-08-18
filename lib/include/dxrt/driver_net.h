// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "dxrt/common.h"
#ifdef __linux__
    #include <linux/ioctl.h>
#elif _WIN32
    #include <windows.h>
#endif

namespace dxrt {

/**********************/
/* RT/driver sync     */

#pragma pack(push, 1)
typedef struct _net_control_info{
    uint32_t type;
    uint32_t cmd;
    uint32_t sub_cmd;
    uint32_t size;
    uint64_t address;
    uint32_t data[100];
} net_control_info;
#pragma pack(pop)


} // namespace dxrt