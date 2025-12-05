/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */


#include "dxrt/request_response_class.h"

#include <iostream>
#include <memory>
#include <string>

#include "dxrt/common.h"
#include "dxrt/cpu_handle.h"
#include "dxrt/device.h"
#include "dxrt/device_pool.h"
#include "dxrt/profiler.h"
#include "dxrt/request.h"
#include "dxrt/task.h"
#include "dxrt/util.h"
// #include "dxrt/objects_pool.h"
#include "dxrt/npu_format_handler.h"

using std::endl;

namespace dxrt {


int RequestResponse::InferenceRequest(RequestPtr req)
{
    LOG_DXRT_DBG
        << "[" << req->id() << "] - - - - - - - Req " << req->id() << ": "
        << req->requestor_name() << " -> " << req->task()->name()
        << std::endl;
    TASK_FLOW_START(
        "[" + std::to_string(req->job_id()) + "]" + req->task()->name() +
        " Inference Reqeust ");
    if (req->task()->processor() == Processor::NPU)
    {
        LOG_DXRT_DBG
            << "[" << req->id() << "] N) Req " << req->id() << ": "
            << req->requestor_name() << " -> " << req->task()->name()
            << std::endl;

        auto device = DevicePool::GetInstance().PickOneDevice(req->task()->getDeviceIds());

      TASK_FLOW("[" + std::to_string(req->job_id()) + "]" +
            req->task()->name() + " device pick");

        req->model_type() = req->taskData()->_npuModel.type;

        if (req->getData()->output_buffer_base == nullptr)
        {
            // Allocate an atomic buffer to avoid deadlocks
            try {
                BufferSet buffers = req->task()->AcquireAllBuffers();
#ifdef USE_PROFILER
                req->CheckTimePoint(0);
                // Start profiling for overall NPU task (input preprocess + PCIe + NPU execution + output postprocess)
                auto& profiler = dxrt::Profiler::GetInstance();
                std::string profile_name =
                    "NPU Task[Job_" + std::to_string(req->job_id()) + "][" +
                    req->task()->name() + "][Req_" +
                    std::to_string(req->id()) + "]";
                profiler.Start(profile_name);
#endif
                req->getData()->output_buffer_base = buffers.output;
                req->getData()->encoded_inputs_ptr = buffers.encoded_input;
                req->getData()->encoded_outputs_ptr = buffers.encoded_output;
                // Store the BufferSet in the Request so it can be released automatically
                req->setBufferSet(std::unique_ptr<BufferSet>(new BufferSet(buffers)));
            }
            catch (const std::exception& e) {
                LOG_DXRT_ERR(
                    "Buffer allocation failed for request " << req->id() <<
                    ": " << e.what());
                // CRITICAL: Reduce device load and wake up other waiting jobs to avoid deadlocks
                device->CallBack();
                LOG_DXRT_DBG << "Device " << device->id()
                             << " load decreased due to buffer allocation failure for request "
                             << req->id() << std::endl;
                throw;
            }
        }
        else
        {
            // If output buffers already exist, allocate only the remaining buffers
            req->getData()->encoded_inputs_ptr = req->task()->GetEncodedInputBuffer();
            req->getData()->encoded_outputs_ptr = req->task()->GetEncodedOutputBuffer();
        }



        req->getData()->BuildEncodedInputPtrs(req->taskData()->_encodedInputOffsets);
        req->getData()->BuildEncodedOutputPtrs(req->taskData()->_encodedOutputOffsets);
      TASK_FLOW("[" + std::to_string(req->job_id()) + "]" +
            req->task()->name() + " buffers get");
        auto nfhDevice = DevicePool::GetInstance().GetNFHLayer(device->id());
        nfhDevice->InferenceRequest(device->id(), req, static_cast<npu_bound_op>(req->task()->getNpuBoundOp()));
    }
    else
    {
        LOG_DXRT_DBG
            << "[" << req->id() << "] C) Req " << req->id() << ": "
            << req->requestor_name() << " -> " << req->task()->name()
            << std::endl;
        if (req->getData()->output_buffer_base == nullptr)
        {
            // Allocate an atomic buffer to avoid deadlocks
            try {
                BufferSet buffers = req->task()->AcquireAllBuffers();
#ifdef USE_PROFILER
                req->CheckTimePoint(0);
#endif
                TASK_FLOW("[" + std::to_string(req->job_id()) + "]" +
                          req->task()->name() + " buffers get");
                req->getData()->output_buffer_base = buffers.output;
                // CPU tasks do not use encoded buffers, so can be nullptr
                req->getData()->encoded_inputs_ptr = nullptr;
                req->getData()->encoded_outputs_ptr = nullptr;

                // Store the BufferSet in the Request so it can be released automatically
                req->setBufferSet(std::unique_ptr<BufferSet>(new BufferSet(buffers)));
            }
            catch (const std::exception& e) {
                LOG_DXRT_ERR(
                    "CPU Buffer allocation failed for request " << req->id() <<
                    ": " << e.what());
                throw;
            }
        }
      TASK_FLOW("[" + std::to_string(req->job_id()) + "]" +
            req->task()->name() + " buffers get");
        req->task()->getCpuHandle()->InferenceRequest(req);
    }
    return req->id();
}

// moved npu_format_handler include to top


void RequestResponse::ProcessByData(int reqId, const dxrt_response_t& response, int deviceId)
{
    auto req = Request::GetById(reqId);
    if (req == nullptr)
    {
        DXRT_ASSERT(false, "req is nullptr "+std::to_string(reqId));
    }

    if (DEBUG_DATA > 0)
    {
        DataDumpBin(req->taskData()->name() + "_output.bin",
            req->encoded_outputs_ptr(), req->taskData()->encoded_output_size());
    }

    // Temproal Fix for Performance Issue
    if (req->model_type() == 0)
    {
        //ProcessByDataNormal(req, response, deviceId);
    }

    // Argmax
    else if (req->model_type() == 1)
    {
        ProcessByDataArgmax(req, response, deviceId);
    }
    // PPU
    else if (req->model_type() == 2)
    {
        ProcessByDataPPU(req, response, deviceId);
    }
    // PPCPU
    else if (req->model_type() == 3)
    {
        ProcessByDataPPCPU(req, response, deviceId);
    }
    else
    {
        DXRT_ASSERT(false, "Invalid model type (normal, argmax, ppu, ppcpu)");
    }

    RequestResponse::ProcessResponse(req, response, 0);
}



void RequestResponse::ProcessByDataNormal(RequestPtr req, const dxrt_response_t& response, int deviceId)
{
    std::ignore = deviceId;
    std::ignore = response;
    RequestData *req_data = req->getData();
    if (Configuration::_sNpuValidateOpt == false)
    {
        for (size_t i = 0; i < req_data->outputs.size(); i++)
        {
            Tensor& output_tensor = req_data->outputs[i];
            deepx_rmapinfo::TensorInfo tensor_info = req_data->taskData->_npuOutputTensorInfos[i];
            int shape_dims = tensor_info.shape_encoded().size();
            npu_format_handler::Bytes encoded_output = {
                static_cast<uint32_t>(req_data->taskData->_encodedOutputSizes[i]),
                static_cast<uint8_t*>(req_data->encoded_output_ptrs[i])
            };
            npu_format_handler::Bytes decoded_output = {
                static_cast<uint32_t>(output_tensor.size_in_bytes()),
                static_cast<uint8_t*>(output_tensor.data())
            };

            // dummy decoder
            if (tensor_info.layout() == deepx_rmapinfo::Layout::ALIGNED)
            {
                // transpose
                if (tensor_info.transpose() == deepx_rmapinfo::Transpose::TRANSPOSE_NONE)
                {
                    LOG_DXRT_DBG << "Output Transpose (TRANSPOSE_NONE) [" << i << "]" << endl;
                    npu_format_handler::NpuFormatHandler::decode_aligned(
                        encoded_output,
                        decoded_output,
                        tensor_info.shape_encoded()[shape_dims - 1],
                        static_cast<deepx_rmapinfo::DataType>(
                            tensor_info.dtype_encoded()),
                        tensor_info.align_unit());
                    LOG_DXRT_DBG
                        << "Output format is decoded (ALIGNED) [" << i << "] "
                        << "encoded_output size: " << encoded_output.size
                        << ", decoded_output size: " << decoded_output.size
                        << endl;
                }
                else if (tensor_info.transpose() == deepx_rmapinfo::Transpose::CHANNEL_LAST_TO_FIRST)
                {
                    npu_format_handler::NpuFormatHandler::decode_aligned(
                        encoded_output,
                        decoded_output,
                        tensor_info.shape_encoded()[shape_dims - 1],
                        static_cast<deepx_rmapinfo::DataType>(
                            tensor_info.dtype_encoded()),
                        tensor_info.align_unit());
                    LOG_DXRT_DBG
                        << "Output format is decoded (ALIGNED) [" << i << "] "
                        << "encoded_output size: " << encoded_output.size
                        << ", decoded_output size: " << decoded_output.size
                        << endl;
                    npu_format_handler::Bytes transposed_output = {encoded_output.size, decoded_output.data};
                    ///*
                    int col = tensor_info.shape_encoded()[shape_dims - 1];
                    int row = 1;
                    for (int j = 0; j < shape_dims - 1; j++)
                    {
                        row *= tensor_info.shape_encoded()[j];
                    }
                    int elem_size = GetDataSize_rmapinfo_datatype(
                        static_cast<deepx_rmapinfo::DataType>(
                            tensor_info.dtype_encoded()));
                    //*/
                    npu_format_handler::NpuFormatHandler::bidirectional_transpose(
                        transposed_output.data,
                        decoded_output.data,
                        row,
                        col,
                        elem_size);

                    /*
                    npu_format_handler::NpuFormatHandler::decode_aligned_transposed(encoded_output,
                                                                                    decoded_output,
                                                                                    tensor_info.shape_encoded()[shape_dims - 1],
                                                                                    static_cast<deepx_rmapinfo::DataType>(tensor_info.dtype_encoded()),
                                                                                    tensor_info.shape_encoded(),
                                                                                    tensor_info.transpose()
                                                                                    );
                    */
                    LOG_DXRT_DBG
                        << "Output format is decoded (ALIGNED+CHANNEL_LAST_TO_FIRST) ["
                        << i << "] "
                        << "encoded_output size: " << encoded_output.size
                        << ", decoded_output size: " << decoded_output.size
                        << endl;
                }
                else
                {
                    LOG_DXRT_ERR("Invalid transpose type");
                    memcpy(static_cast<void*>(decoded_output.data),
                            static_cast<const void*>(encoded_output.data),
                            encoded_output.size);
                }
            }
            else
            {
                memcpy(static_cast<void*>(decoded_output.data),
                        static_cast<const void*>(encoded_output.data),
                        encoded_output.size);
            }
        }
    }
    else
    {
        for (size_t i = 0; i < req_data->outputs.size(); i++)
            req_data->outputs[i].data() = req_data->encoded_output_ptrs[i];
    }
    if (DEBUG_DATA > 0)
    {
        DataDumpBin(req->taskData()->name() + "_decoder_output.bin", req->outputs());
    }
}

void RequestResponse::ProcessByDataArgmax(RequestPtr req, const dxrt_response_t& response, int deviceId)
{
    std::ignore = deviceId;
    LOG_DXRT_DBG << "response.argmax : " << response.argmax << std::endl;
    *(static_cast<uint16_t *>(req->outputs().front().data())) = response.argmax;
    if (DEBUG_DATA > 0)
        DataDumpBin(req->taskData()->name() + "_output.argmax.bin", req->outputs());


}

void RequestResponse::ProcessByDataPPU(RequestPtr req, const dxrt_response_t& response, int deviceId)
{
    std::ignore = deviceId;
    LOG_DXRT_DBG << "response.ppu_filter_num : " << response.ppu_filter_num << std::endl;
    RequestData* req_data = req->getData();
    if (!req_data->outputs.empty())
    {
        memcpy(static_cast<void*>(req_data->outputs[0].data()),
                static_cast<const void*>(req_data->encoded_output_ptrs[0]),
                128 * 1024);
        req_data->outputs[0].shape() = {1, response.ppu_filter_num};
    }

    DXRT_ASSERT(req_data->outputs.front().shape()[1] == response.ppu_filter_num, "PPU MODEL OUTPUT NOT VALID SET");

    if (DEBUG_DATA > 0)
        DataDumpBin(req->taskData()->name() + "_output.ppu.bin", req->outputs());
}

void RequestResponse::ProcessByDataPPCPU(RequestPtr req, const dxrt_response_t& response, int deviceId)
{
    std::ignore = deviceId;
    LOG_DXRT_DBG << "PPCPU output processing, ppu_filter_num : " << response.ppu_filter_num << std::endl;
    RequestData* req_data = req->getData();

    if (!req_data->outputs.empty() && response.ppu_filter_num > 0)
    {
        DataType dtype = req_data->outputs[0].type();
        size_t unit_size = GetDataSize_Datatype(dtype);
        memcpy(static_cast<void*>(req_data->outputs[0].data()),
               static_cast<const void*>(req_data->encoded_output_ptrs[0]),
               response.ppu_filter_num * unit_size);
        req_data->outputs[0].shape() = {1, response.ppu_filter_num};

        LOG_DXRT_DBG << "PPCPU output shape set to [" << response.ppu_filter_num << "]" << std::endl;
    }
    else
    {
        LOG_DXRT_DBG << "PPCPU output is empty or ppu_filter_num is 0, req id: " << req->id() << std::endl;
        if (!req_data->outputs.empty())
        {
            req_data->outputs[0].shape() = {0, 0};
        }
    }

    if (DEBUG_DATA > 0)
    {
        DataDumpBin(req->taskData()->name() + "_output.ppcpu.bin", req->outputs());
    }
}


int RequestResponse::ProcessResponse(RequestPtr req, const dxrt_response_t& response, int deviceType)
{
#ifdef USE_PROFILER
    req->CheckTimePoint(1);
    // End profiling for overall NPU task
    auto& profiler = dxrt::Profiler::GetInstance();
    std::string profile_name =
        "NPU Task[Job_" + std::to_string(req->job_id()) + "][" +
        req->task()->name() + "][Req_" + std::to_string(req->id()) + "]";
    profiler.End(profile_name);

#endif
        LOG_DXRT_DBG
            << "[" << req->id() << "] Response : " << req->id() << ", "
            << req->task()->name() << ", " << req->latency() << std::endl;
    if (deviceType != 1)
    {
    req->task()->setLastOutput(req->outputs());  // TODO(dxrt): STD issue can be possible
    }

    if (req->task()->processor() == Processor::NPU)
    {
        req->inference_time() = response.inf_time;
        req->task()->PushInferenceTime(req->inference_time());
    }
    else
    {
        req->inference_time() = 0;
    }
#ifdef USE_PROFILER
    req->task()->PushLatency(req->latency());
#endif
    req->onRequestComplete(req);
    return 0;
}

#ifdef DXRT_USE_DEVICE_VALIDATION
int RequestResponse::ValidateRequest(RequestPtr req)
{
    if (!req)
    {
        LOG_DXRT_ERR("Invalid request");
        return -1;
    }

    if (!req->is_validate_request())
    {
        LOG_DXRT_ERR("Request is not a validation request");
        return -1;
    }
    Tensor validateOutputTensor = req->ValidateOutputTensor();
    if (validateOutputTensor.size_in_bytes() != req->taskData()->_npuModel.output_all_size)
    {
        LOG_DXRT_ERR("Validation output tensor size does not match model output size");
        return -1;
    }
    if (validateOutputTensor.data() == nullptr)
    {
        LOG_DXRT_ERR("Validation output tensor data is null");
        return -1;
    }
    if (req->task() == nullptr)
    {
        LOG_DXRT_ERR("Request task is null");
        return -1;
    }
    if (req->task()->processor() != Processor::NPU)
    {
        LOG_DXRT_ERR("Validation request must be for NPU tasks");
        return -1;
    }
    DevicePool::GetInstance().GetDeviceTaskLayer(0)->InferenceRequest(req->getData());
    req->Wait();
    return 0;
}
#endif

}  // namespace dxrt
