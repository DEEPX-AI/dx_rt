/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <map>

#include "dxrt/common.h"
#include "dxrt/exception/server_err.h"

namespace dxrt {

class DxrtServiceErr
{
 private:
    // Legacy IPC wrapper removed - use Dynamic IPC v2 instead

 public:
    // Constructor accepts null wrapper for compatibility only
    explicit DxrtServiceErr(void *ipcServerWrapperUnused = nullptr) { (void)ipcServerWrapperUnused; }

    // Legacy IPC send disabled - use Dynamic IPC v2 instead
    void ErrorReportToClient(dxrt_server_err_t err, long procId, uint32_t errCode, int deviceId);
};

} //namespace dxrt
