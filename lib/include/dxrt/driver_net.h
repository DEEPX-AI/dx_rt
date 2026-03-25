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

namespace dxrt {

/**********************/
/* RT/driver sync     */

#pragma pack(push, 1)
struct net_control_info {
    uint32_t type;
    uint32_t cmd;
    uint32_t sub_cmd;
    uint32_t size;
    uint64_t address;
    uint32_t data[100]; // NOSONAR: Fixed-size array required for packed struct used in driver ioctl binary protocol
};
#pragma pack(pop)


} // namespace dxrt