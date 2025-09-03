/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/common.h"
#include <iostream>
#include <string>
#include "dxrt/task.h"
#include "dxrt/device.h"
#include "dxrt/request.h"
#include "dxrt/cpu_handle.h"
#include "dxrt/profiler.h"
#include "dxrt/util.h"
#include "dxrt/objects_pool.h"
#include <memory>

using std::to_string;

namespace dxrt {


int InferenceRequest(RequestPtr req)
{
    LOG_DXRT_DBG << "[" << req->id() << "] " << "- - - - - - - Req " << req->id() << ": "
        << req->requestor_name() << " -> " << req->task()->name() << std::endl;
    TASK_FLOW_START("["+to_string(req->job_id())+"]"+req->task()->name()+" Inference Reqeust ");
    if (req->task()->processor() == Processor::NPU)
    {
        LOG_DXRT_DBG << "[" << req->id() << "] " << "N) Req " << req->id() << ": "
            << req->requestor_name() << " -> " << req->task()->name() << std::endl;
        // if(req->id()%DBG_LOG_REQ_MOD_NUM > DBG_LOG_REQ_MOD_NUM-DBG_LOG_REQ_WINDOW_NUM || req->id()%DBG_LOG_REQ_MOD_NUM < DBG_LOG_REQ_WINDOW_NUM)
        //    cout<<"[PROC         ][Job_"<<req->getData()->jobId<<"][Req_"<<req->id()<<"] wait device..."<<endl;
        auto device = ObjectsPool::GetInstance().PickOneDevice(req->task()->getDeviceIds(), req->DSP_GetDspEnable());
        // if(req->id()%DBG_LOG_REQ_MOD_NUM > DBG_LOG_REQ_MOD_NUM-DBG_LOG_REQ_WINDOW_NUM || req->id()%DBG_LOG_REQ_MOD_NUM < DBG_LOG_REQ_WINDOW_NUM)
        //    cout<<"[PROC         ][Job_"<<req->getData()->jobId<<"][Req_"<<req->id()<<"][Dev_"<<device->id()<<"] DEV GET"<<endl;
        // cout<<"[PROC         ][Job_"<<req->getData()->jobId<<"][Req_"<<req->id()<<"][Dev_"<<device->id()<<"]"<<endl;
        TASK_FLOW("[" + to_string(req->job_id())+"]"+req->task()->name()+" device pick");

        req->model_type() = req->taskData()->_npuModel.type;

        if (req->getData()->output_buffer_base == nullptr)
        {
            // if(req->id()%DBG_LOG_REQ_MOD_NUM > DBG_LOG_REQ_MOD_NUM-DBG_LOG_REQ_WINDOW_NUM || req->id()%DBG_LOG_REQ_MOD_NUM < DBG_LOG_REQ_WINDOW_NUM)
            //    cout<<"[PROC         ][Job_"<<req->getData()->jobId<<"][Req_"<<req->id()<<"][Dev_"<<device->id()<<"] wait buffer..."<<endl;

            try {
                BufferSet buffers = req->task()->AcquireAllBuffers();
#ifdef USE_PROFILER
                req->CheckTimePoint(0);
                // Start profiling for overall NPU task (input preprocess + PCIe + NPU execution + output postprocess)
                auto& profiler = dxrt::Profiler::GetInstance();
                std::string profile_name = "NPU Task[Job_" + to_string(req->job_id()) + "][" + req->task()->name() + "][Req_" + to_string(req->id()) + "]";
                profiler.Start(profile_name);     
#endif
                req->getData()->output_buffer_base = buffers.output;
                req->getData()->encoded_inputs_ptr = buffers.encoded_input;
                req->getData()->encoded_outputs_ptr = buffers.encoded_output;
                // Store the BufferSet in the Request so it can be released automatically
                req->setBufferSet(std::unique_ptr<BufferSet>(new BufferSet(buffers)));
            }
            catch (const std::exception& e) {
                LOG_DXRT_ERR("Buffer allocation failed for request " << req->id() << ": " << e.what());
                // CRITICAL: Reduce device load and wake up other waiting jobs to avoid deadlocks
                device->CallBack();
                LOG_DXRT_DBG << "Device " << device->id() <<
                  " load decreased due to buffer allocation failure for request " << req->id() << std::endl;
                throw;
            }

            // if(req->id()%DBG_LOG_REQ_MOD_NUM > DBG_LOG_REQ_MOD_NUM-DBG_LOG_REQ_WINDOW_NUM || req->id()%DBG_LOG_REQ_MOD_NUM < DBG_LOG_REQ_WINDOW_NUM)
            //    cout<<"[PROC         ][Job_"<<req->getData()->jobId<<"][Req_"<<req->id()<<"][Dev_"<<device->id()<<"][BUFFER] BUFFER GET"<<endl;
            // cout<<"[PROC         ][Job_"<<req->getData()->jobId<<"][Req_"<<req->id()<<"][Dev_"<<device->id()<<"][GETBUFFER]"<<endl;
        }
        else
        {
            // If output buffers already exist, allocate only the remaining buffers
            req->getData()->encoded_inputs_ptr = req->task()->GetEncodedInputBuffer();
            req->getData()->encoded_outputs_ptr = req->task()->GetEncodedOutputBuffer();
        }



        req->getData()->BuildEncodedInputPtrs(req->taskData()->_encodedInputOffsets);
        req->getData()->BuildEncodedOutputPtrs(req->taskData()->_encodedOutputOffsets);
        TASK_FLOW("["+to_string(req->job_id())+"]"+req->task()->name()+" buffers get");
        device->InferenceRequest(req->getData(), static_cast<npu_bound_op>(req->task()->getNpuBoundOp()));
    }
    else
    {
        LOG_DXRT_DBG << "[" << req->id() << "] " << "C) Req " << req->id() << ": "
            << req->requestor_name() << " -> " << req->task()->name() << std::endl;
        if (req->getData()->output_buffer_base == nullptr)
        {
            // Allocate an atomic buffer to avoid deadlocks
            try {
                BufferSet buffers = req->task()->AcquireAllBuffers();
#ifdef USE_PROFILER
                req->CheckTimePoint(0);
#endif
                TASK_FLOW("["+to_string(req->job_id())+"]"+req->task()->name()+" buffers get");
                req->getData()->output_buffer_base = buffers.output;
                // CPU tasks do not use encoded buffers, so can be nullptr
                req->getData()->encoded_inputs_ptr = nullptr;
                req->getData()->encoded_outputs_ptr = nullptr;

                // Store the BufferSet in the Request so it can be released automatically
                req->setBufferSet(std::unique_ptr<BufferSet>(new BufferSet(buffers)));
            }
            catch (const std::exception& e) {
                LOG_DXRT_ERR("CPU Buffer allocation failed for request " << req->id() << ": " << e.what());
                throw;
            }
        }
        TASK_FLOW("["+to_string(req->job_id())+"]"+req->task()->name()+" buffers get");
        req->task()->getCpuHandle()->InferenceRequest(req);
    }
    return req->id();
}

int ProcessResponse(RequestPtr req, dxrt_response_t *response, int deviceType)
{
#ifdef USE_PROFILER
    req->CheckTimePoint(1);
    // End profiling for overall NPU task
    auto& profiler = dxrt::Profiler::GetInstance();
    std::string profile_name = "NPU Task[Job_" + to_string(req->job_id()) + "][" + req->task()->name() + "][Req_" + to_string(req->id()) + "]";
    profiler.End(profile_name);
        
#endif
    LOG_DXRT_DBG << "[" << req->id() << "] " << "    Response : " << req->id() << ", "
      << req->task()->name() << ", " << req->latency() << std::endl;
    if (deviceType != 1)
    {
        req->task()->setLastOutput(req->outputs());  // TODO: STD issue can be possible
    }

    if (req->task()->processor() == Processor::NPU)
    {
        req->inference_time() = response->inf_time;
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

// DSP code //////////////////////////////////////////////////////////////////////////////////////////////////////////

int DSP_ProcRequest(RequestPtr req, dxrt_dspcvmat_t *dspCvMatInPtr, dxrt_dspcvmat_t *dspCvMatOutPtr)
{
    LOG_DXRT_DBG << "[" << req->id() << "] " << "N) Req " << req->id() << ": "
        << req->requestor_name() << " -> " << req->task()->name() << std::endl;
    auto device = ObjectsPool::GetInstance().PickOneDevice(req->task()->getDeviceIds(), req->DSP_GetDspEnable());
    TASK_FLOW("["+std::to_string(req->job_id())+"]"+req->task()->name()+" picks device");

    device->DSP_ProcessRequest(req->getData(), dspCvMatInPtr, dspCvMatOutPtr);

    return req->id();
}

int DSP_ProcessResponse(RequestPtr req)
{
    req->DSP_reqOnRequestComplete(req);
    return 0;
}

// ~DSP code //////////////////////////////////////////////////////////////////////////////////////////////////////////
}  // namespace dxrt
