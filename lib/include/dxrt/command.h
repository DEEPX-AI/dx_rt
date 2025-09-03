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

namespace dxrt {
enum DXRT_API Command
{
    GET_STATUS = 0,
    RESET = 1,
    GET_INFO = 2,
    NPU_SET = 3,
    DUMP = 4,
    SET_BD_ID = 5,
    SET_DVFS = 6,
    MAX_CMD,
};
typedef struct DXRT_API {
    uint16_t fw_ver;                // firmware version. A.B.C (e.g. 103)
    uint16_t dummy;
    uint16_t bd_rev;                // board revision. /10 (e.g. 1 = 0.1, 2 = 0.2)
    uint16_t bd_type;               // board type. (1 = SOM, 2 = M.2, 3 = H1)
    uint16_t bd_id;                 // board id. (e.g. 0~65535)
    uint16_t ddr_freq;              // ddr frequency. (e.g. 4200, 5500)
    uint16_t ddr_type;              // ddr type. (1 = lpddr4, 2= lpddr5)
} bdinfo_t;
/** \brief get hardware status (this function is valid only for accelerator devices)
 * \return return 0 if hardware status is read successfully
*/
DXRT_API int CommandLineInterface(Command cmd, std::vector<uint32_t> vec);
} // namespace dxrt
