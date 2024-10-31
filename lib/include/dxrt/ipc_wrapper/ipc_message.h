#pragma once

#include <stdint.h>
#include <cstdint>

#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"

namespace dxrt 
{
    enum class IPC_TYPE : int {
        SOCKET_SYNC = 1,     // socket sync read/write
        SOCKET_CB = 2,       // socket read callback & write sync
        MESSAE_QUEUE = 3,   // message queue (FIFO)
        MSG_QUEUE = 4       // message queue (POSIX)
    };

    enum class MEMORY_REQUEST_CODE : int {
        REGISTESR_PROCESS = 0,      //set msg to pid
        GET_MEMORY = 1,             //set msg to size
        FREE_MEMORY = 2,            //set msg to value returned by GET_MEMORY

    };

    enum class MEMORY_ERROR_CODE : int {
        MEMORY_OK = 0,              //msg is allocated memory if GET_MEMORY, for REGISTER_PROCESS, it is start
        NOT_ENOUGH_MEMORY = 1,
        NOT_ALLOCATED = 2,
    };

    enum class REQUEST_CODE : uint32_t {
        REGISTESR_PROCESS = 0,      //set msg to pid
        GET_MEMORY = 1,             //set msg to size
        FREE_MEMORY = 2,            //set msg to value returned by GET_MEMORY
        GET_MEMORY_FOR_MODEL = 3, //memory from backwards;

        MEMORY_ALLOCATION_AND_TRANSFER_MODEL = 100,
        COMPLETE_TRANSFER_MODEL = 101,
        MEMORY_ALLOCATION_INPUT_AND_OUTPUT = 102,
        TRANSFER_INPUT_AND_RUN = 103,
        COMPLETE_TRANSFER_AND_RUN = 104,
        COMPLETE_TRNASFER_OUTPUT = 105,
        REQUEST_SCHEDULE_INFERENCE = 301,
        INFERENCE_COMPLETED = 302,
        INF_STOP_REQUEST = 900,
        INF_STOP_END_REQUEST = 901,
        CLOSE = 1001
    };
    std::ostream& operator<< (std::ostream& os, REQUEST_CODE code);
    enum class RESPONSE_CODE : uint32_t {
        CONFIRM_MEMORY_ALLOCATION_AND_TRANSFER_MODEL = 200,
        CONFIRM_MEMORY_ALLOCATION = 201,
        CONFIRM_TRANSFER_INPUT_AND_RUN = 202,
        CONFIRM_MEMORY_FREE = 203,
        DO_SCHEDULED_INFERENCE_CH0 = 400,
        DO_SCHEDULED_INFERENCE_CH1 = 401,
        DO_SCHEDULED_INFERENCE_CH2 = 402,
        INF_STOP_REQUEST_RESP = 800,
        ERROR_REPORT = 900,
        CLOSE = 1001,
        INVALID_REQUEST_CODE = 1234,
    };
    std::ostream& operator<< (std::ostream& os, RESPONSE_CODE code);
#pragma pack(push, 1)

    struct IPCClientMessage
    {
        REQUEST_CODE code;
        uint32_t deviceId;
        uint64_t data;
        pid_t pid;
        long msgType; // for message queue
        int seqId;
        dxrt::dxrt_request_acc_t npu_acc;
    };

    struct IPCServerMessage
    {
        RESPONSE_CODE code;
        uint32_t deviceId;
        uint32_t result;
        uint64_t data;
        long msgType; // for message queue
        int seqId;
        dxrt::dxrt_response_t npu_resp;
    };

    struct IPCRegisterTask
    {
        RESPONSE_CODE code;
        uint32_t deviceId;
        int taskId;
        pid_t pid;
        int8_t    model_type;
        int8_t    model_format;
        uint32_t  model_cmds;
        uint32_t  cmd_offset;
        uint32_t  weight_offset;
    };

    struct IPCRequestInference
    {
        RESPONSE_CODE code;
        uint32_t deviceId;
        int taskId;
        int requestId;
        long msgType;
        pid_t pid;
        uint64_t input_base = 0;
        uint32_t input_offset = 0;
        uint32_t input_size = 0;
        uint64_t output_base = 0;
        uint32_t output_offset = 0;
        uint32_t output_size = 0;
    };


#pragma pack(pop)

    inline std::ostream& operator<<(std::ostream& os, const IPCClientMessage& clientMessage)
    {
        os << "client-message code=" << clientMessage.code;
        return os;
    }

    inline std::ostream& operator<<(std::ostream& os, const IPCServerMessage& serverMessage)
    {
        os << "server-message code=" << serverMessage.code;
        return os;
    }

}  // namespace dxrt