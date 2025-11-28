
/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/nfh_layer.h"

#include <functional>
#include <iostream>
#include <string>

#include "dxrt/common.h"
#include "dxrt/device_task_layer.h"
#include "dxrt/nfh_request.h"
#include "dxrt/npu_format_handler.h"
#include "dxrt/request_response_class.h"
#include "dxrt/device_pool.h"

namespace dxrt
{

static constexpr int COMMON_NFH_LAYER_DEVICE_ID = -1;

NFHLayer::NFHLayer(std::shared_ptr<DeviceTaskLayer> devicePtr, bool isDynamic)
    : _deviceId((devicePtr == nullptr) ? COMMON_NFH_LAYER_DEVICE_ID : devicePtr->id()),
        _device(devicePtr),
        _inputHandler("NFHLayer::handleInput", GetNfhInputWorkerThreads(), std::bind(&NFHLayer::handleInput, this, std::placeholders::_1, std::placeholders::_2)),
        _outputHandler("NFHLayer::handleOutput", GetNfhOutputWorkerThreads(), std::bind(&NFHLayer::handleOutput, this, std::placeholders::_1, std::placeholders::_2)),
        _isDynamic(isDynamic),
        _responseCallback(RequestResponse::ProcessByData) // Default callback
{
    if (isDynamic)
    {
        _inputHandler.Start();
        _outputHandler.Start();
    }
}

void NFHLayer::SetResponseCallback(std::function<void(int, const dxrt_response_t&, int)> cb)
{
    if (cb) {
        _responseCallback = std::move(cb);
    }
}

int NFHLayer::InferenceRequest(int deviceId, std::shared_ptr<Request> req, npu_bound_op boundOp)
{
    if ((_deviceId != COMMON_NFH_LAYER_DEVICE_ID) && (deviceId != _deviceId))
    {
        LOG_DXRT_ERR("NFHLayer::InferenceRequest invalid deviceId " << deviceId << "!=" << _deviceId);
        return -1;
    }
    int reqId = req->id();
    NfhInputRequest inputReq(deviceId, reqId, req, 0, boundOp);
    if (_isDynamic)
    {
        _inputHandler.PushWork(inputReq);
    }
    else
    {
        // Synchronous path: directly handle input and inference
        return handleInput(inputReq, 0);
    }
    return 0;
}


static int processInputNfh(const NfhInputRequest& work, int threadId)
{
    if (!work.req)
    {
        LOG_DXRT_ERR("Invalid work in processInputNfh");
        return -1;
    }

    auto reqData = work.req->getData();
    if (!reqData || !reqData->taskData)
    {
        LOG_DXRT_ERR("Invalid request data in processInputNfh");
        return -1;
    }

    // common encoding utility
    int enc = npu_format_handler::NpuFormatHandler::EncodeInputs(reqData, threadId);
    if (enc != 0) return enc;

    return 0;
}

int NFHLayer::handleInput(const NfhInputRequest &inputReq, int threadId)
{
    try
    {
        int result = processInputNfh(inputReq, threadId);
        if (result != 0)
        {
            LOG_DXRT_ERR("Failed to process input NFH for request " << inputReq.requestId);
        }
        // InferenceRequest_ACC trigger
        if (inputReq.req)
        {
            auto reqData = inputReq.req->getData();
            if (reqData)
            {
                if (_deviceId == COMMON_NFH_LAYER_DEVICE_ID)
                {
                    auto device = DevicePool::GetInstance().GetDeviceTaskLayer(inputReq.deviceId);
                    if (!device)
                    {
                        LOG_DXRT_ERR("Device not found for InferenceRequest_ACC after NFH for request " << inputReq.requestId);
                        return -1;
                    }
                    int inferenceResult = device->InferenceRequest(reqData, inputReq.boundOp);
                    if (inferenceResult != 0)
                    {
                        LOG_DXRT_ERR("Failed to process InferenceRequest_ACC after NFH for request " << inputReq.requestId);
                    }
                }
                else
                {
                    int inferenceResult = _device->InferenceRequest(inputReq.req->getData(), inputReq.boundOp);
                    if (inferenceResult != 0)
                    {
                        LOG_DXRT_ERR("Failed to process InferenceRequest_ACC after NFH for request " << inputReq.requestId);
                    }
                }
            }
            else
            {
                LOG_DXRT_ERR("Request data or device not available for request " << inputReq.requestId);
            }
        }
    }
    catch (const std::exception &e)
    {
        LOG_DXRT_ERR("Exception in NFH input processing: " << e.what());
    }
    return 0;
}

int processOutputNfh(const NfhOutputRequest& work, int threadId)
{
    if (!work.req)
    {
        LOG_DXRT_ERR("Invalid work in processOutputNfh");
        return -1;
    }

    // common decoding utility
    int dec = npu_format_handler::NpuFormatHandler::DecodeOutputs(&work.req, &work.response, threadId);
    if (dec != 0) return dec;

    return 0;
}
int NFHLayer::handleOutput(const NfhOutputRequest &outputReq, int threadId)
{
#ifdef DXRT_USE_DEVICE_VALIDATION
    if (outputReq.req->is_validate_request())
    {
        outputReq.req->onRequestComplete(outputReq.req);
        return 0;
    }
#endif
    try
    {
        int result = 0;
        result = processOutputNfh(outputReq, threadId);
        if (result != 0)
        {
            LOG_DXRT_ERR("Failed to process output NFH for request " << outputReq.requestId);
        }
        else
        {
            // NFH processing completed, proceed to direct subsequent processing (prevent circular calls)
            if (outputReq.req)
            {
                try
                {
                    //auto request_acc = _device->peekInferenceAcc(outputReq.requestId);

                    // direct subsequent processing
                    TASK_FLOW("[" + std::to_string(outputReq.req->job_id()) + "]" +
                                outputReq.req->taskData()->name() + " NFH output completed, load :" +
                                //std::to_string(_device->load()
                                std::to_string(0)
                                );

                    //_device->Deallocate_npuBuf(request_acc.input.offset, outputReq.req->taskData()->id());
                    //ProcessResponse(outputReq.req, const_cast<dxrt_response_t*>(&outputReq.response), 0);
                    //_device->popInferenceStruct(outputReq.requestId);
                    _responseCallback(outputReq.req->id(), outputReq.response, outputReq.deviceId);

                    LOG_DXRT_DBG << "NFH Output processing completed for request " << outputReq.requestId << std::endl;
                }
                catch (const std::exception& e)
                {
                    LOG_DXRT_ERR("Exception in NFH output completion: " << e.what());
                }
            }
        }
    }
    catch (const std::exception& e)
    {
        LOG_DXRT_ERR("Exception in NFH output processing: " << e.what());
    }
    return 0;
}

int NFHLayer::ProcessResponse(int deviceId, int reqId, dxrt_response_t *response)
{
    if ((_deviceId !=COMMON_NFH_LAYER_DEVICE_ID) && (deviceId != _deviceId))
    {
        LOG_DXRT_ERR( "NFHLayer::ProcessResponse invalid deviceId " << deviceId << "!=" << _deviceId);
        return -1;
    }
    if (response == nullptr)
    {
        LOG_DXRT_ERR( "NFHLayer::ProcessResponse null response for reqId " << reqId);
        return -1;
    }

    // Find the associated Request object
    auto req = Request::GetById(reqId);
    NfhOutputRequest outputReq(deviceId, reqId, *response, req, 0);
    if (_isDynamic)
    {
        _outputHandler.PushWork(outputReq);
    }
    else
    {
        // Synchronous path: directly handle output and response
        return handleOutput(outputReq, 0);
    }
    return 0;
}

}  // namespace dxrt
