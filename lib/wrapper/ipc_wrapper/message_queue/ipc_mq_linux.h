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
#include <future>
#include <array>


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
            std::array<uint8_t, 1024> data;
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
        int32_t Send(const Message& message, size_t size) const;

        // receive message
        int32_t Receive(Message& message, size_t size, long msgType) const;

        // delete message queue
        int32_t Delete();

        bool IsAvailable() const
        {
            return _msgId >= 0 ? true : false;
        }
        void threadFunc();

    };

}  // namespace dxrt
