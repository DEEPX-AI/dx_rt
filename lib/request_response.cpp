// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/task.h"
#include "dxrt/device.h"
#include "dxrt/request.h"
#include "dxrt/cpu_handle.h"
#include "dxrt/profiler.h"
#include "dxrt/util.h"
#include "dxrt/objects_pool.h"

using namespace std;

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
        auto device = ObjectsPool::GetInstance()->PickOneDevice(req->task()->getDeviceIds());
        TASK_FLOW("["+to_string(req->job_id())+"]"+req->task()->name()+" picks device");
        req->CheckTimePoint(0);
        req->model_type() = req->taskData()->_npuModel.front().type;
        if(req->getData()->output_ptr == nullptr)
        {
            req->getData()->output_ptr = req->task()->GetOutputBuffer();
        }
        device->InferenceRequest(req->getData(), static_cast<npu_bound_op>(req->task()->getNpuBoundOp()));
    }
    else
    {
        LOG_DXRT_DBG << "[" << req->id() << "] " << "C) Req " << req->id() << ": "
            << req->requestor_name() << " -> " << req->task()->name() << std::endl;
        req->CheckTimePoint(0);
        if(req->getData()->output_ptr == nullptr)
        {
            req->getData()->output_ptr = req->task()->GetOutputBuffer();
        }
        req->task()->getCpuHandle()->InferenceRequest(req);
    }
    return req->id();
}

int ProcessResponse(RequestPtr req, dxrt_response_t *response)
{

    req->CheckTimePoint(1);
    LOG_DXRT_DBG << "[" << req->id() << "] " << "    Response : " << req->id() << ", " << req->task()->name() << ", " << req->latency() << std::endl;
    req->taskData()->_outputTensors = req->outputs(); 
    if (req->task()->processor() == Processor::NPU)
    {
        req->inference_time() = response->inf_time;
    }
    else
    {
        req->inference_time() = req->latency();
    }

    req->task()->PushLatency(req->latency());
    req->task()->PushInferenceTime(req->inference_time());
    req->onRequestComplete(req);
    return 0;
}

}  // namespace dxrt
