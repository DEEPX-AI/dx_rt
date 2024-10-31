// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include <stdint.h>
#include <cstdint>
#include <future>
#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"
#include "dxrt/ipc_wrapper/ipc_client.h"
#include "dxrt/ipc_wrapper/ipc_message.h"
#include "dxrt/device.h"
#include "dxrt/task.h"
#include "service_error.h"

namespace dxrt {

int codeToChannel(RESPONSE_CODE code)
{
    switch (code)
    {
        case RESPONSE_CODE::DO_SCHEDULED_INFERENCE_CH0:
            return 0;
        case RESPONSE_CODE::DO_SCHEDULED_INFERENCE_CH1:
            return 1;
        case RESPONSE_CODE::DO_SCHEDULED_INFERENCE_CH2:
            return 2;
        default:
            return -1;
    }
}

int ipc_callBack_inference_message(IPCServerMessage& outResponseServerMessage, void* usrData);



std::ostream& operator<< (std::ostream& os, RESPONSE_CODE code)
{
    switch (code)
    {
        case RESPONSE_CODE::CONFIRM_MEMORY_ALLOCATION_AND_TRANSFER_MODEL:
            os << "CONFIRM_MEMORY_ALLOCATION_AND_TRANSFER_MODEL";
            break;
        case RESPONSE_CODE::CONFIRM_MEMORY_ALLOCATION:
            os << "CONFIRM_MEMORY_ALLOCATION";
            break;
        case RESPONSE_CODE::CONFIRM_TRANSFER_INPUT_AND_RUN:
            os << "CONFIRM_TRANSFER_INPUT_AND_RUN";
            break;
        case RESPONSE_CODE::CONFIRM_MEMORY_FREE:
            os << "CONFIRM_MEMORY_FREE";
            break;
        case RESPONSE_CODE::DO_SCHEDULED_INFERENCE_CH0:
            os << "DO_SCHEDULED_INFERENCE_CH0";
            break;
        case RESPONSE_CODE::DO_SCHEDULED_INFERENCE_CH1:
            os << "DO_SCHEDULED_INFERENCE_CH1";
            break;
        case RESPONSE_CODE::DO_SCHEDULED_INFERENCE_CH2:
            os << "DO_SCHEDULED_INFERENCE_CH2";
            break;
        case RESPONSE_CODE::CLOSE:
            os << "CLOSE";
            break;
        case RESPONSE_CODE::INVALID_REQUEST_CODE:
            os << "INVALID_REQUEST_CODE";
            break;
        default:
            os << "Invalid RESPONSE_CODE value";
            break;
    }
    return os;
}

std::ostream& operator<< (std::ostream& os, REQUEST_CODE code)
{
    switch (code)
    {
        case REQUEST_CODE::REGISTESR_PROCESS:
            os << "REGISTESR_PROCESS";
            break;
        case REQUEST_CODE::GET_MEMORY:
            os << "GET_MEMORY";
            break;
        case REQUEST_CODE::FREE_MEMORY:
            os << "FREE_MEMORY";
            break;
        case REQUEST_CODE::GET_MEMORY_FOR_MODEL:
            os << "GET_MEMORY_FOR_MODEL";
            break;
        case REQUEST_CODE::MEMORY_ALLOCATION_AND_TRANSFER_MODEL:
            os << "MEMORY_ALLOCATION_AND_TRANSFER_MODEL";
            break;
        case REQUEST_CODE::COMPLETE_TRANSFER_MODEL:
            os << "COMPLETE_TRANSFER_MODEL";
            break;
        case REQUEST_CODE::MEMORY_ALLOCATION_INPUT_AND_OUTPUT:
            os << "MEMORY_ALLOCATION_INPUT_AND_OUTPUT";
            break;
        case REQUEST_CODE::TRANSFER_INPUT_AND_RUN :
            os << "TRANSFER_INPUT_AND_RUN";
            break;
        case REQUEST_CODE::COMPLETE_TRANSFER_AND_RUN:
            os << "COMPLETE_TRANSFER_AND_RUN";
            break;
        case REQUEST_CODE::COMPLETE_TRNASFER_OUTPUT:
            os << "COMPLETE_TRNASFER_OUTPUT";
            break;
        case REQUEST_CODE::REQUEST_SCHEDULE_INFERENCE:
            os << "REQUEST_SCHEDULE_INFERENCE";
            break;
        case REQUEST_CODE::INFERENCE_COMPLETED:
            os << "INFERENCE_COMPLETED";
            break;
        case REQUEST_CODE::CLOSE:
            os << "CLOSE";
            break;
        default:
            break;
    }
    return os;
}



int ipc_callBack(IPCServerMessage& outResponseServerMessage, void* usrData)
{
    (void)usrData;

    LOG_DXRT_I_DBG << "callback " << outResponseServerMessage.code << endl;
    //int deviceId = outResponseServerMessage.deviceId;
    switch (outResponseServerMessage.code)
    {
        case RESPONSE_CODE::CONFIRM_MEMORY_ALLOCATION:
        case RESPONSE_CODE::CONFIRM_MEMORY_ALLOCATION_AND_TRANSFER_MODEL:
            return 234;
            break;
        case RESPONSE_CODE::CONFIRM_MEMORY_FREE:
            break;
        case RESPONSE_CODE::DO_SCHEDULED_INFERENCE_CH0:
        case RESPONSE_CODE::DO_SCHEDULED_INFERENCE_CH1:
        case RESPONSE_CODE::DO_SCHEDULED_INFERENCE_CH2:
            std::async(launch::async, [&outResponseServerMessage, usrData]()
            {
                ipc_callBack_inference_message(outResponseServerMessage, usrData);
            }
            );
            
            break;
        case RESPONSE_CODE::ERROR_REPORT: {
            cout << "============================================================" << endl;
            cout << " ** Reason : " <<  static_cast<dxrt::dxrt_server_err_t>(outResponseServerMessage.data) <<
                "(value: " << static_cast<int>(outResponseServerMessage.result) << ")" << endl;
            cout << " ** Take error message from server" << endl;
            cout << " ** Please restart daemon and applications" << endl;
            cout << "============================================================" << endl;
            DXRT_ASSERT(false, "");
            break;
        }
        default:
            break;
    }
    return 0;
}

int ipc_callBack_inference_message(IPCServerMessage& outResponseServerMessage, void* usrData)
{
    (void)usrData;

    //int deviceId = outResponseServerMessage.deviceId;

    dxrt_response_t& resp = outResponseServerMessage.npu_resp;
    int reqId = resp.req_id;
    auto req = Request::GetById(reqId);
    DevicePtr devicePtr = CheckDevices()[outResponseServerMessage.deviceId];
    dxrt_request_acc_t* request_acc = devicePtr->peekInferenceAcc(reqId);
    int debugData = req->debug_env();

    LOG_DXRT_I_DBG << "request arrived:" << reqId << endl;
    if (request_acc != nullptr)
    {
        dxrt_meminfo_t output = request_acc->output;
        DXRT_ASSERT(devicePtr->Read(output) == 0, "Failed to read output");
        if (debugData == 1)
        {
            dxrt::TensorPtrs outputs = devicePtr->Validate(req, true);
            if (outputs.empty() == false){
                DataDumpBin(req->taskData()->name() + "_output.bin", outputs);
            }
        }
        else if (debugData > 1)
        {
            DataDumpBin(req->taskData()->name() + "_output.bin", req->outputs());
        }
        if (req->model_type() == 1)
        {
            //LOG_VALUE(resp.argmax);
            *(static_cast<uint16_t *>(req->outputs().front().data())) = resp.argmax;
            if (debugData > 0)
                DataDumpBin(req->taskData()->name() + "_output.argmax.bin", req->outputs());
        }
        else if (req->model_type() == 2)
        {
            //LOG_VALUE(resp.ppu_filter_num);
            vector<int64_t> shape{resp.ppu_filter_num};
            req->outputs().front().shape() = shape;
            if (debugData > 0)
                DataDumpBin(req->taskData()->name() + "_output.ppu.bin", req->outputs());
        }

        devicePtr->CallBack();

        devicePtr->Deallocate(request_acc->input.offset);
        req->task()->ProcessResponse(req, &resp);


        Profiler::GetInstance().End(devicePtr->name());
        devicePtr->popInferenceStruct(reqId);
    }
    else
    {
        LOG_DXRT_I_ERR(": Error request acc is NULL");
        return -1;
    }

    return 0;
}

}  // namespace dxrt
