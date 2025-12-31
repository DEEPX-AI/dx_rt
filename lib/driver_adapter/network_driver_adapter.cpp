/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#ifdef __linux__


#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fstream>
#include <algorithm>

#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"
#include "dxrt/driver_adapter/network_driver_adapter.h"
#include "dxrt/driver_net.h"
#include "dxrt/exception/exception.h"

namespace dxrt {

#define SERVER_IP   "192.168.1.105"
#define SERVER_PORT_MSG     (5201)
#define SERVER_PORT_QUEUE   (5202)
#define SERVER_PORT_DATA    (5203)

NetworkDriverAdapter::NetworkDriverAdapter()
{
    struct sockaddr_in server_addr;

    // for (int type = 0; type < TCP_TYPES_MAX; type++)
    int type = TCP_MESSAGE;
    {
        tcpMsgType tcpType = static_cast<tcpMsgType>(type);
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            perror("Socket creation failed");
        } //@no_else: guard_clause

        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(SERVER_PORT_MSG + type);
        if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) != 1) {
            perror("Invalid address format");
        }
        //@no_else: guard_clause

        if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("Connection failed");
        }//@no_else: guard_clause
        sockMap.emplace(tcpType, std::make_pair(sock, SERVER_PORT_MSG + type));

        LOG_DXRT_INFO("Connected to server " << SERVER_IP << ":" << sockMap[tcpType].second);
    }

}

int32_t NetworkDriverAdapter::NetControl(dxrt_cmd_t request, void* data, uint32_t size , uint32_t sub_cmd, uint64_t address, bool ctrlCmd)
{
    net_control_info info{};
    int ret = 0;

    info.cmd = static_cast<uint32_t>(request);
    info.sub_cmd = sub_cmd;
    info.type = 0;
    info.size = size;
    info.address = address;
    {
        if (data != nullptr)
        {
            memcpy(reinterpret_cast<void *>(info.data), data, sizeof(info.data));
        }
        else
        {
            throw dxrt::InvalidArgumentException("data pointer is null in NetworkDriverAdapter::NetControl");
        }


        switch (request)
        {
            case DXRT_CMD_IDENTIFY_DEVICE:
            case DXRT_CMD_TERMINATE:
                info.type = TCP_MESSAGE;
                Write(&info, sizeof(net_control_info));
                ret = Read(data, size);
                break;
            case DXRT_CMD_NPU_RUN_REQ:
                info.type = TCP_QUEUE;
                Write(&info, sizeof(net_control_info));
                break;
            case DXRT_CMD_NPU_RUN_RESP:
                ret = Read(data, size);
                break;
            case DXRT_CMD_WRITE_MEM:
                info.type = TCP_DATAS;
                Write(&info, sizeof(net_control_info));
                Write(data, info.size);
                break;
            case DXRT_CMD_READ_MEM:
                info.type = TCP_DATAS_GET;
                if (ctrlCmd)
                    Write(&info, sizeof(net_control_info));
                    //@no_else: conditional_work
                ret = Read(data, size);
                break;
            default:
                LOG_DXRT_ERR("Undefined request (" << request << ")");
                exit(-1);
                // break not requrired due to exit function call
        }
    }
    return ret;
}

int32_t NetworkDriverAdapter::Write(const void* buffer, uint32_t size)
{
    if (buffer == nullptr || size == 0)
    {
        LOG_DXRT_ERR("Invalid buffer or size in NetworkDriverAdapter::Write, buffer: " << buffer << ", size: " << size);
        return -1;
    }
    //@no_else: input_validation
    int ret = 0;
    // printf("Write burst buffer : %p, size:%d\n", buffer, size);
    {
        ssize_t bytes_sent = send(sockMap[TCP_MESSAGE].first, buffer, size, 0);
        if (bytes_sent < 0)
        {
            perror("Send failed");
            ret = -1;
        }
        //@no_else: guard_clause
    }
    return ret;
}

#define CHUNK_SIZE  (1460)
int32_t NetworkDriverAdapter::Read(void* buffer, uint32_t size)
{
    size_t totalBytesReceived = 0;
    ssize_t bytesReceived;
    size_t totalSize = size;
    size_t remainingSize = totalSize;
    char* writePointer = reinterpret_cast<char *>(buffer);
    int ret = 0;

    while (remainingSize > 0) {
        size_t bytesToReceive = std::min(static_cast<size_t>(CHUNK_SIZE), remainingSize);
        bytesReceived = recv(sockMap[TCP_MESSAGE].first, writePointer, bytesToReceive, 0);
        if (bytesReceived <= 0) {
            if (bytesReceived == 0) {
                LOG_DXRT_INFO("Connection closed by peer.");
            } else {
                LOG_DXRT_ERR("Error while receiving data: " << strerror(errno));
            }
            ret = -1;
            break;
        }
        //@no_else: guard_clause
        writePointer += bytesReceived;
        totalBytesReceived += bytesReceived;
        remainingSize -= bytesReceived;
        // std::cout << "Received " << bytesReceived << " bytes." << std::endl;
    }

    return ret;
}

NetworkDriverAdapter::~NetworkDriverAdapter()
{
    // for (int type = 0; type < TCP_TYPES_MAX; type++)
    {
        tcpMsgType tcpType = static_cast<tcpMsgType>(TCP_MESSAGE);
        close(sockMap[tcpType].first);
    }
}


}  // namespace dxrt

#endif

