// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <stdint.h>
#include <cstdint>
#include <future>


namespace dxrt 
{

    
    enum class IPCMessageQueueDirection
    {
        TO_SERVER,
        TO_CLIENT
    };

    class IPCMessageQueueLinux
    {
    public:
        static const int QUEUE_KEY;
        static const long SERVER_MSG_TYPE;


        struct Message
        {
            long msgType;
            uint8_t data[1024];
        };

    private:

        int _msgId;
        std::atomic<bool> _stop{false};
    public:

        IPCMessageQueueLinux();
        virtual ~IPCMessageQueueLinux();

        // Intitialize IPC (Message Queue)
        int32_t Initialize(long msgType, IPCMessageQueueDirection direction);

        // send message
        int32_t Send(const Message& message, size_t size);

        // receive message
        int32_t Receive(Message& message, size_t size, long msgType);

        // delete message queue
        int32_t Delete();

        bool IsAvailable()
        {
            return _msgId >= 0 ? true : false;
        }
        void threadFunc();

    };

}  // namespace dxrt