
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
#include <memory>
#include <string>
#include "dxrt/driver.h"

namespace dxrt {

class Request;

// NFH Input work structure
struct NfhInputRequest
{
    int deviceId;
    uint32_t requestId;
    std::shared_ptr<Request> req;
    int threadId;
    npu_bound_op boundOp;  // Original boundOp parameter preserved

    NfhInputRequest()
    : deviceId(0), requestId(0), req(nullptr), threadId(0), boundOp(N_BOUND_NORMAL)
    {
    }

    NfhInputRequest(int device_id, uint32_t reqId, std::shared_ptr<Request> request, int tId, npu_bound_op bound = N_BOUND_NORMAL)
    : deviceId(device_id), requestId(reqId), req(request), threadId(tId), boundOp(bound)
    {
    }
};

// NFH Output work structure
struct NfhOutputRequest
{
    int deviceId;
    uint32_t requestId;
    dxrt_response_t response;
    std::shared_ptr<Request> req;
    int threadId;

    NfhOutputRequest()
    : deviceId(0), requestId(0), response{}, req(nullptr), threadId(0)
    {
    }

    NfhOutputRequest(int device_id, uint32_t reqId, const dxrt_response_t& resp, std::shared_ptr<Request> request, int tId)
    :  deviceId(device_id), requestId(reqId), response(resp), req(request), threadId(tId)
    {
    }
};


}  // namespace dxrt
