/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <signal.h>
#include <mutex>
#include <atomic>
#include <thread>
#include <string>
#include <condition_variable>
#include <ostream>
#include "dxrt/common.h"
#include "dxrt/device_struct.h"

#include "dxrt/device_compatibility_layer.h"

#define DEVICE_NUM_BUF 2
#define DEVICE_OUTPUT_WORKER_NUM 4

#define RMAP_RECOVERY_DONE      (1)
#define WEIGHT_RECOVERY_DONE    (2)

#ifdef __linux__
    #include <poll.h>
#elif _WIN32
    #include <windows.h>

#endif

namespace dxrt {

using DevicePtr = std::shared_ptr<Device>;

enum SkipMode
{
    NONE            = 0,
    VERSION_CHECK,
    COMMON_SKIP,
    STATIC_SAVE_SKIP,
    IDENTIFY_SKIP
};



DXRT_API std::ostream& operator<<(std::ostream&, const dxrt_device_status_t&);
DXRT_API std::ostream& operator<<(std::ostream& os, const dxrt_device_info_t& info);
DXRT_API std::ostream& operator<<(std::ostream& os, const dx_pcie_dev_ntfy_throt_t& notify);
} // namespace dxrt
