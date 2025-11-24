
/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#pragma once

#include "dxrt/common.h"

// C++ standard
#include <memory>
#include <string>

// Project
#include "dxrt/driver.h"
#include "dxrt/handler_que_template.h"
#include "dxrt/nfh_request.h"





namespace dxrt {
class DeviceTaskLayer;

class DXRT_API NFHLayer {
 public:
    explicit NFHLayer(std::shared_ptr<DeviceTaskLayer> devicePtr, bool isDynamic);

    int InferenceRequest(int deviceId, std::shared_ptr<Request> req, npu_bound_op boundOp);
    int ProcessResponse(int deviceId, int reqId, dxrt_response_t *response);

    // Test/support hook: override response processing callback (default: RequestResponse::ProcessByData)
    void SetResponseCallback(std::function<void(int, const dxrt_response_t&, int)> cb);

 private:
    int _deviceId;
    std::shared_ptr<DeviceTaskLayer> _device;

    HandlerQueueThread<NfhInputRequest> _inputHandler;
    HandlerQueueThread<NfhOutputRequest> _outputHandler;

    int handleInput(const NfhInputRequest&, int);
    int handleOutput(const NfhOutputRequest &, int);

    bool _isDynamic = false;

    std::function<void(int reqId, const dxrt_response_t& response, int deviceId)> _responseCallback;
};

}  // namespace dxrt
