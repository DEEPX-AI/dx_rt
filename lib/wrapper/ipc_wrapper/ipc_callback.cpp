/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include <stdint.h>
#include <cstdint>
#include <future>
#include <iostream>
#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"
#include "dxrt/ipc_wrapper/ipc_client.h"
#include "dxrt/ipc_wrapper/ipc_message.h"
#include "dxrt/device.h"
#include "dxrt/task.h"
#include "dxrt/request.h"
#include "service_error.h"
#include "dxrt/device_pool.h"


using std::cout;
using std::endl;

namespace dxrt {

std::shared_ptr<DeviceTaskLayer> getDeviceTaskLayerSafe(uint32_t deviceId)
{
    try {
        auto& pool = DevicePool::GetInstance();
        if (deviceId < pool.GetDeviceCount()) {
            return pool.GetDeviceTaskLayer(static_cast<int>(deviceId));
        }
    }
    catch (const std::exception& e) {
        LOG_DXRT_I_ERR("Failed to get device task layer: " << e.what());
    }
    return nullptr;
}

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
    os << dxrt::to_string(code);
    return os;
}




int ipc_callBack(const IPCServerMessage& outResponseServerMessage, void* usrData)
{
    (void)usrData;

    LOG_DXRT_I_DBG << "callback " << outResponseServerMessage.code << endl;
    // int deviceId = outResponseServerMessage.deviceId;
#ifdef USE_SERVICE
    switch (outResponseServerMessage.code)
    {
    case RESPONSE_CODE::CONFIRM_MEMORY_ALLOCATION:
    case RESPONSE_CODE::CONFIRM_MEMORY_ALLOCATION_AND_TRANSFER_MODEL:
        return 234;

    case RESPONSE_CODE::CONFIRM_MEMORY_FREE:
        break;
    case RESPONSE_CODE::DO_SCHEDULED_INFERENCE_CH0:
    case RESPONSE_CODE::DO_SCHEDULED_INFERENCE_CH1:
    case RESPONSE_CODE::DO_SCHEDULED_INFERENCE_CH2:
    {
        auto taskLayer = getDeviceTaskLayerSafe(outResponseServerMessage.deviceId);
        if (taskLayer)
        {
            taskLayer->ProcessResponseFromService(outResponseServerMessage.npu_resp);
        }
        else
        {
            LOG_DXRT_I_ERR("the device id is out of the devices range. " + std::to_string(outResponseServerMessage.deviceId));
        }
    }
    break;
    case RESPONSE_CODE::ERROR_REPORT: {
        auto taskLayer = getDeviceTaskLayerSafe(outResponseServerMessage.deviceId);
        if (taskLayer)
        {
            taskLayer->ProcessErrorFromService(static_cast<dxrt::dxrt_server_err_t>(outResponseServerMessage.data),
                static_cast<int>(outResponseServerMessage.result));
        }
        else
        {
            cout << "============================================================" << endl;
            cout << " ** Reason : " << static_cast<dxrt::dxrt_server_err_t>(outResponseServerMessage.data) <<
                "(value: " << static_cast<int>(outResponseServerMessage.result) << ")" << endl;
            cout << " ** Take error message from server" << endl;
            cout << " ** Please restart daemon and applications" << endl;
            cout << "============================================================" << endl;
            DXRT_ASSERT(false, "");
        }
        break;
    }
    default:
        break;
    }
#endif
    return 0;
}

}  // namespace dxrt


#define REQUEST_CODE_MACRO(x) case dxrt::REQUEST_CODE::x: return #x;

std::string dxrt::to_string(dxrt::REQUEST_CODE code)
{
switch (code)
    {
        REQUEST_CODE_MACRO(REGISTER_PROCESS)
        REQUEST_CODE_MACRO(GET_MEMORY)
        REQUEST_CODE_MACRO(FREE_MEMORY)
        REQUEST_CODE_MACRO(GET_MEMORY_FOR_MODEL)
        REQUEST_CODE_MACRO(DEVICE_INIT)
        REQUEST_CODE_MACRO(DEVICE_RESET)
        REQUEST_CODE_MACRO(DEVICE_DEINIT)
        REQUEST_CODE_MACRO(TASK_INIT)
        REQUEST_CODE_MACRO(TASK_DEINIT)
        REQUEST_CODE_MACRO(DEALLOCATE_TASK_MEMORY)
        REQUEST_CODE_MACRO(PROCESS_DEINIT)
        REQUEST_CODE_MACRO(VIEW_FREE_MEMORY)
        REQUEST_CODE_MACRO(VIEW_USED_MEMORY)
        REQUEST_CODE_MACRO(VIEW_AVAILABLE_DEVICE)
        REQUEST_CODE_MACRO(GET_USAGE)

        REQUEST_CODE_MACRO(MEMORY_ALLOCATION_AND_TRANSFER_MODEL)
        REQUEST_CODE_MACRO(COMPLETE_TRANSFER_MODEL)
        REQUEST_CODE_MACRO(MEMORY_ALLOCATION_INPUT_AND_OUTPUT)
        REQUEST_CODE_MACRO(TRANSFER_INPUT_AND_RUN)
        REQUEST_CODE_MACRO(COMPLETE_TRANSFER_AND_RUN)
        REQUEST_CODE_MACRO(COMPLETE_TRANSFER_OUTPUT)
        REQUEST_CODE_MACRO(REQUEST_SCHEDULE_INFERENCE)
        REQUEST_CODE_MACRO(INFERENCE_COMPLETED)
        REQUEST_CODE_MACRO(CLOSE)

        default:
            return "--ERROR(" + std::to_string(static_cast<int>(code)) + ")--";
    }
}
