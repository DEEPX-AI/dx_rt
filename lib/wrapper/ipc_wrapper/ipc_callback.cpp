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
#include "dxrt/request.h"
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

                auto& devices = CheckDevices();
                if ( outResponseServerMessage.deviceId < static_cast<uint32_t>(devices.size()) )
                {
                    DevicePtr& devicePtr = CheckDevices()[outResponseServerMessage.deviceId];
                    devicePtr->ProcessResponseFromService(outResponseServerMessage.npu_resp);
                }
                else
                {
                    LOG_DXRT_I_ERR("the device id is out of the devices range. "+ std::to_string(outResponseServerMessage.deviceId));
                }

            }

            break;
        case RESPONSE_CODE::ERROR_REPORT: {
            auto& devices = CheckDevices();
            if ( outResponseServerMessage.deviceId < static_cast<uint32_t>(devices.size()) )
            {
                DevicePtr& devicePtr = CheckDevices()[outResponseServerMessage.deviceId];
                devicePtr->ProcessErrorFromService(static_cast<dxrt::dxrt_server_err_t>(outResponseServerMessage.data),
                    static_cast<int>(outResponseServerMessage.result));
            }
            else
            {
                cout << "============================================================" << endl;
                cout << " ** Reason : " <<  static_cast<dxrt::dxrt_server_err_t>(outResponseServerMessage.data) <<
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
