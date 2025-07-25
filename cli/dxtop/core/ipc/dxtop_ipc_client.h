// Copyright (c) 2025 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "dxrt/ipc_wrapper/ipc_client_wrapper.h"
#include "dxrt/ipc_wrapper/ipc_message.h"

namespace dxrt {
    
    class DXTopIPCClient
    {
    public:
        explicit DXTopIPCClient();
        virtual ~DXTopIPCClient() = default;

        template <typename T = uint64_t>
        T SendRequest(dxrt::REQUEST_CODE code, uint32_t deviceId, uint64_t data = 0) 
        {
            dxrt::IPCClientMessage clientMessage;
            dxrt::IPCServerMessage serverMessage;

            clientMessage.code = code;
            clientMessage.deviceId = deviceId;
            clientMessage.data = data;
            clientMessage.pid = getpid();

            _wrapper.SendToServer(serverMessage, clientMessage);

            if (serverMessage.result == 0 )
            {
                return static_cast<T>(serverMessage.data);
            }
            else
            {
                throw std::runtime_error("IPC Request failed with code: " + std::to_string(static_cast<uint32_t>(code)));
            }

        }

    private:
        dxrt::IPCClientWrapper _wrapper;
    };

}