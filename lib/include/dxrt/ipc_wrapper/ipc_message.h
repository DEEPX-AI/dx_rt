/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <stdint.h>
#include <cstdint>
#include <cstring>
#include <map>
#include <string>
#include <chrono>


#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"

namespace dxrt
{
    enum class IPC_TYPE : int {
        //SOCKET_SYNC = 1,        // socket sync read/write
        //SOCKET_CB = 2,          // socket read callback & write sync
        MESSAGE_QUEUE = 3,       // message queue (FIFO)
        //MSG_QUEUE = 4,          // message queue (POSIX)
        WIN_PIPE = 5            // windows named pipe
    };
    IPC_TYPE inline IPCDefaultType()
    {
#ifdef __linux__
        return IPC_TYPE::MESSAGE_QUEUE;
#elif _WIN32
        return IPC_TYPE::WIN_PIPE ;
#endif
    }

    enum class MEMORY_REQUEST_CODE : int {
        REGISTER_PROCESS = 0,      // set msg to pid
        GET_MEMORY = 1,             //set msg to size
        FREE_MEMORY = 2,            //set msg to value returned by GET_MEMORY

    };

    enum class MEMORY_ERROR_CODE : int {
        MEMORY_OK = 0,              //msg is allocated memory if GET_MEMORY, for REGISTER_PROCESS, it is start
        NOT_ENOUGH_MEMORY = 1,
        NOT_ALLOCATED = 2,
    };

    enum class REQUEST_CODE : uint32_t {
        REGISTER_PROCESS = 0,       // set msg to pid
        GET_MEMORY = 1,             //set msg to size
        FREE_MEMORY = 2,            //set msg to value returned by GET_MEMORY
        GET_MEMORY_FOR_MODEL = 3, //memory from backwards;
        DEVICE_INIT = 4,
        DEVICE_RESET = 5,
        DEVICE_DEINIT = 6,
        TASK_INIT = 7,
        TASK_DEINIT = 8,
        DEALLOCATE_TASK_MEMORY = 9,
        PROCESS_DEINIT = 10,        //process cleanup
        VIEW_FREE_MEMORY = 11,
        VIEW_USED_MEMORY = 12,
        VIEW_AVAILABLE_DEVICE = 15,
        GET_USAGE = 17,

        MEMORY_ALLOCATION_AND_TRANSFER_MODEL = 100,
        COMPLETE_TRANSFER_MODEL = 101,
        MEMORY_ALLOCATION_INPUT_AND_OUTPUT = 102,
        TRANSFER_INPUT_AND_RUN = 103,
        COMPLETE_TRANSFER_AND_RUN = 104,
        COMPLETE_TRANSFER_OUTPUT = 105,
        REQUEST_SCHEDULE_INFERENCE = 301,
        INFERENCE_COMPLETED = 302,
        CLOSE = 1001
    };
    std::ostream& operator<< (std::ostream& os, REQUEST_CODE code);

    enum class RESPONSE_CODE : uint32_t {
        VIEW_FREE_MEMORY_RESULT = 13,
        VIEW_USED_MEMORY_RESULT = 14,
        VIEW_AVAILABLE_DEVICE_RESULT = 16,
        GET_USAGE_RESULT = 18,
        TASK_INIT_SUCCESS = 19,
        CONFIRM_MEMORY_ALLOCATION_AND_TRANSFER_MODEL = 200,
        CONFIRM_MEMORY_ALLOCATION = 201,
        CONFIRM_TRANSFER_INPUT_AND_RUN = 202,
        CONFIRM_MEMORY_FREE = 203,
        DO_SCHEDULED_INFERENCE_CH0 = 400,
        DO_SCHEDULED_INFERENCE_CH1 = 401,
        DO_SCHEDULED_INFERENCE_CH2 = 402,
        ERROR_REPORT = 900,
        TASK_INIT_FAILED = 901,
        THROTTLE_EVENT = 902,
        CLOSE = 1001,
        INVALID_REQUEST_CODE = 1234,
    };
    std::ostream& operator<< (std::ostream& os, RESPONSE_CODE code);
    std::string to_string(dxrt::REQUEST_CODE code);
#pragma pack(push, 1)

    struct IPCClientMessage
    {
        REQUEST_CODE code = REQUEST_CODE::REGISTER_PROCESS;
        uint32_t deviceId = 0;
        uint64_t data = 0;
        pid_t pid = 0;
        long msgType = 0; // for message queue
        int seqId = 0;
        dxrt::dxrt_request_acc_t npu_acc = dxrt::dxrt_request_acc_t{};

        int taskId = -1;
        uint64_t modelMemorySize = 0;

        IPCClientMessage() = default;
    };

    struct IPCServerMessage
    {
        RESPONSE_CODE code = RESPONSE_CODE::CLOSE;
        uint32_t deviceId = 0;
        uint32_t result = 0;
        uint64_t data = 0;
        long msgType = 0; // for message queue
        int seqId = 0;
        dxrt::dxrt_response_t npu_resp = dxrt::dxrt_response_t{};
        IPCServerMessage() = default;
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

    inline DXRT_API std::ostream& operator<<(std::ostream& os, const IPCClientMessage& clientMessage)
    {
        os << "client-message code=" << clientMessage.code;
        return os;
    }

    inline DXRT_API std::ostream& operator<<(std::ostream& os, const IPCServerMessage& serverMessage)
    {
        os << "server-message code=" << serverMessage.code;
        return os;
    }

    // for tracing
    inline DXRT_API std::string _s(dxrt::REQUEST_CODE c)
    {
        static const std::map<dxrt::REQUEST_CODE, std::string> m = {
            {dxrt::REQUEST_CODE::REGISTER_PROCESS, "REGISTER_PROCESS"},
            {dxrt::REQUEST_CODE::GET_MEMORY, "GET_MEMORY"},
            {dxrt::REQUEST_CODE::FREE_MEMORY, "FREE_MEMORY"},
            {dxrt::REQUEST_CODE::GET_MEMORY_FOR_MODEL, "GET_MEMORY_FOR_MODEL"},
            {dxrt::REQUEST_CODE::DEVICE_INIT, "DEVICE_INIT"},
            {dxrt::REQUEST_CODE::DEVICE_RESET, "DEVICE_RESET"},
            {dxrt::REQUEST_CODE::DEVICE_DEINIT, "DEVICE_DEINIT"},
            {dxrt::REQUEST_CODE::TASK_INIT, "TASK_INIT"},
            {dxrt::REQUEST_CODE::TASK_DEINIT, "TASK_DEINIT"},
            {dxrt::REQUEST_CODE::DEALLOCATE_TASK_MEMORY, "DEALLOCATE_TASK_MEMORY"},
            {dxrt::REQUEST_CODE::PROCESS_DEINIT, "PROCESS_DEINIT"},
            {dxrt::REQUEST_CODE::VIEW_FREE_MEMORY, "VIEW_FREE_MEMORY"},
            {dxrt::REQUEST_CODE::VIEW_USED_MEMORY, "VIEW_USED_MEMORY"},
            {dxrt::REQUEST_CODE::VIEW_AVAILABLE_DEVICE, "VIEW_AVAILABLE_DEVICE"},
            {dxrt::REQUEST_CODE::GET_USAGE, "GET_USAGE"},
            {
                dxrt::REQUEST_CODE::MEMORY_ALLOCATION_AND_TRANSFER_MODEL,
                "MEMORY_ALLOCATION_AND_TRANSFER_MODEL"
            },
            {dxrt::REQUEST_CODE::COMPLETE_TRANSFER_MODEL, "COMPLETE_TRANSFER_MODEL"},
            {dxrt::REQUEST_CODE::MEMORY_ALLOCATION_INPUT_AND_OUTPUT, "MEMORY_ALLOCATION_INPUT_AND_OUTPUT"},
            {dxrt::REQUEST_CODE::TRANSFER_INPUT_AND_RUN, "TRANSFER_INPUT_AND_RUN"},
            {dxrt::REQUEST_CODE::COMPLETE_TRANSFER_AND_RUN, "COMPLETE_TRANSFER_AND_RUN"},
            {dxrt::REQUEST_CODE::COMPLETE_TRANSFER_OUTPUT, "COMPLETE_TRANSFER_OUTPUT"},
            {dxrt::REQUEST_CODE::REQUEST_SCHEDULE_INFERENCE, "REQUEST_SCHEDULE_INFERENCE"},
            {dxrt::REQUEST_CODE::INFERENCE_COMPLETED, "INFERENCE_COMPLETED"},
            {dxrt::REQUEST_CODE::CLOSE, "CLOSE"},
        };
        const auto it = m.find(c);
        return it == m.end() ? "REQUEST_Unknown" : it->second;
    }
    inline DXRT_API std::string _s(dxrt::RESPONSE_CODE c)
    {
        static const std::map<dxrt::RESPONSE_CODE, std::string> m = {
            {dxrt::RESPONSE_CODE::VIEW_FREE_MEMORY_RESULT, "VIEW_FREE_MEMORY_RESULT"},
            {dxrt::RESPONSE_CODE::VIEW_USED_MEMORY_RESULT, "VIEW_USED_MEMORY_RESULT"},
            {
                dxrt::RESPONSE_CODE::CONFIRM_MEMORY_ALLOCATION_AND_TRANSFER_MODEL,
                "CONFIRM_MEMORY_ALLOCATION_AND_TRANSFER_MODEL"
            },
            {dxrt::RESPONSE_CODE::CONFIRM_MEMORY_ALLOCATION, "CONFIRM_MEMORY_ALLOCATION"},
            {dxrt::RESPONSE_CODE::CONFIRM_TRANSFER_INPUT_AND_RUN, "CONFIRM_TRANSFER_INPUT_AND_RUN"},
            {dxrt::RESPONSE_CODE::CONFIRM_MEMORY_FREE, "CONFIRM_MEMORY_FREE"},
            {dxrt::RESPONSE_CODE::DO_SCHEDULED_INFERENCE_CH0, "DO_SCHEDULED_INFERENCE_CH0"},
            {dxrt::RESPONSE_CODE::DO_SCHEDULED_INFERENCE_CH1, "DO_SCHEDULED_INFERENCE_CH1"},
            {dxrt::RESPONSE_CODE::DO_SCHEDULED_INFERENCE_CH2, "DO_SCHEDULED_INFERENCE_CH2"},
            {dxrt::RESPONSE_CODE::ERROR_REPORT, "ERROR_REPORT"},
            {dxrt::RESPONSE_CODE::THROTTLE_EVENT, "THROTTLE_EVENT"},
            {dxrt::RESPONSE_CODE::CLOSE, "CLOSE"},
            {dxrt::RESPONSE_CODE::TASK_INIT_SUCCESS, "TASK_INIT_SUCCESS"},
            {dxrt::RESPONSE_CODE::INVALID_REQUEST_CODE, "INVALID_REQUEST_CODE"},
        };
        const auto it = m.find(c);
        return it == m.end() ? "RESPONSE_Unknown" : it->second;
    }


#ifdef _WIN32
    // usage
	// static auto start = std::chrono::high_resolution_clock::now();
	// ...
	// start = durationPrint(start, "IPCPipeWindows::SendOL :");
    inline DXRT_API std::chrono::steady_clock::time_point durationPrint(std::chrono::steady_clock::time_point start, const char* msg)
    {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        double total_time = duration.count();
        double avg_latency = total_time / 1;
        if (avg_latency > 100)
            LOG_DXRT_I_DBG << msg << avg_latency << " ms" << std::endl;
        return end;
    }

#endif // _WIN32



}  // namespace dxrt
