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
        //if(req->id()%DBG_LOG_REQ_MOD_NUM > DBG_LOG_REQ_MOD_NUM-DBG_LOG_REQ_WINDOW_NUM || req->id()%DBG_LOG_REQ_MOD_NUM < DBG_LOG_REQ_WINDOW_NUM)
        //    cout<<"[PROC         ][Job_"<<req->getData()->jobId<<"][Req_"<<req->id()<<"] wait device..."<<endl;
        auto device = ObjectsPool::GetInstance().PickOneDevice(req->task()->getDeviceIds());
        //if(req->id()%DBG_LOG_REQ_MOD_NUM > DBG_LOG_REQ_MOD_NUM-DBG_LOG_REQ_WINDOW_NUM || req->id()%DBG_LOG_REQ_MOD_NUM < DBG_LOG_REQ_WINDOW_NUM)
        //    cout<<"[PROC         ][Job_"<<req->getData()->jobId<<"][Req_"<<req->id()<<"][Dev_"<<device->id()<<"] DEV GET"<<endl;
        //cout<<"[PROC         ][Job_"<<req->getData()->jobId<<"][Req_"<<req->id()<<"][Dev_"<<device->id()<<"]"<<endl;
        TASK_FLOW("["+to_string(req->job_id())+"]"+req->task()->name()+" picks device");
#ifdef USE_PROFILER
        req->CheckTimePoint(0);
#endif
        req->model_type() = req->taskData()->_npuModel.front().type;
        if(req->getData()->output_ptr == nullptr)
        {
            //if(req->id()%DBG_LOG_REQ_MOD_NUM > DBG_LOG_REQ_MOD_NUM-DBG_LOG_REQ_WINDOW_NUM || req->id()%DBG_LOG_REQ_MOD_NUM < DBG_LOG_REQ_WINDOW_NUM)
            //    cout<<"[PROC         ][Job_"<<req->getData()->jobId<<"][Req_"<<req->id()<<"][Dev_"<<device->id()<<"] wait buffer..."<<endl;
            req->getData()->output_ptr = req->task()->GetOutputBuffer();
            //if(req->id()%DBG_LOG_REQ_MOD_NUM > DBG_LOG_REQ_MOD_NUM-DBG_LOG_REQ_WINDOW_NUM || req->id()%DBG_LOG_REQ_MOD_NUM < DBG_LOG_REQ_WINDOW_NUM)
            //    cout<<"[PROC         ][Job_"<<req->getData()->jobId<<"][Req_"<<req->id()<<"][Dev_"<<device->id()<<"][BUFFER] BUFFER GET"<<endl;
            //cout<<"[PROC         ][Job_"<<req->getData()->jobId<<"][Req_"<<req->id()<<"][Dev_"<<device->id()<<"][GETBUFFER]"<<endl;
        }
        device->InferenceRequest(req->getData(), static_cast<npu_bound_op>(req->task()->getNpuBoundOp()));
    }
    else
    {
        LOG_DXRT_DBG << "[" << req->id() << "] " << "C) Req " << req->id() << ": "
            << req->requestor_name() << " -> " << req->task()->name() << std::endl;
#ifdef USE_PROFILER
        req->CheckTimePoint(0);
#endif
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
#ifdef USE_PROFILER
    req->CheckTimePoint(1);
#endif
    LOG_DXRT_DBG << "[" << req->id() << "] " << "    Response : " << req->id() << ", " << req->task()->name() << ", " << req->latency() << std::endl;
    req->task()->setLastOutput(req->outputs());  //TODO: STD issue can be possible
    if (req->task()->processor() == Processor::NPU) 
    {
        req->inference_time() = response->inf_time;
    }
    else
    {
        req->inference_time() = req->latency();
    }
#ifdef USE_PROFILER
    req->task()->PushLatency(req->latency());
    req->task()->PushInferenceTime(req->inference_time());
#endif
    req->onRequestComplete(req);
    return 0;
}

}  // namespace dxrt
