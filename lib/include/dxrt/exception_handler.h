// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "dxrt/common.h"
#include "../include/dxrt/ipc_wrapper/ipc_client_wrapper.h"

namespace dxrt {
class DXRT_API ExceptionHandler
{
public:
    ExceptionHandler();
    void SetTasks(void *task) { _tasks.emplace_back(task); }
    void SendStopCmdToServer(void);
    void SendStopEndCmdToServer(void);

    std::vector<void *> GetTasks(void) { return _tasks; }
private:
    std::vector<void *> _tasks;
    dxrt::IPCClientWrapper ipcClientWrapper;
};

extern ExceptionHandler exceptionHandler;
} // namespace dxrt
