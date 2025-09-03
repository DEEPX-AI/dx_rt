/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/common.h"


#include <csignal>
#include "memory_service.hpp"
#include "../include/dxrt/ipc_wrapper/ipc_server_wrapper.h"
#include <atomic>
#include <thread>
#include <dxrt/driver.h>


struct TaskItem
{
    int itemId;
    int taskId;
    pid_t pid;
    int8_t    model_type   = 0;
    int8_t    model_format = 0;
    uint32_t  model_cmds = 0;
    uint32_t  cmd_offset = 0;
    uint32_t  weight_offset = 0;

};


struct RequestItem
{
    int itemId;
    int requestId;
    int taskId;
    pid_t pid;
    dxrt::dxrt_request_acc_t npu_inference_acc;
};

