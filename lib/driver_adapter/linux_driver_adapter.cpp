/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#ifdef __linux__ // all or nothing


#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/poll.h>
#include <sys/fcntl.h>
#include <cstdint>
#include <cstdio>
#include <cstring>


#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"
#include "dxrt/driver_adapter/linux_driver_adapter.h"

namespace dxrt {

LinuxDriverAdapter::LinuxDriverAdapter(const char* fileName)
: _name(fileName)
{
    _fd = open(fileName, O_RDWR|O_SYNC);

}

int32_t LinuxDriverAdapter::IOControl(dxrt_cmd_t request, void* data, uint32_t size , uint32_t sub_cmd)
{
    int ret = 0;
    dxrt_message_t msg = dxrt_message_t{};
    // memset(&msg, 0, sizeof(dxrt_message_t));  // for valgrind
    msg.cmd = static_cast<::int32_t>(request);
    msg.sub_cmd = static_cast<::int32_t>(sub_cmd);
    msg.data = data;
    msg.size = size;

    ret = ioctl(_fd, static_cast<unsigned long>(dxrt::dxrt_ioctl_t::DXRT_IOCTL_MESSAGE), &msg);

    /*
    if (ret < 0) {
            std::string req_info = "";

            if (data != nullptr) {
                if (request == dxrt::dxrt_cmd_t::DXRT_CMD_NPU_RUN_REQ && size >= sizeof(dxrt_request_acc_t)) {
                    dxrt_request_acc_t* req = static_cast<dxrt_request_acc_t*>(data);
                    req_info = ", req_id: " + std::to_string(req->req_id) + ", dma_ch: " + std::to_string(req->dma_ch);
                } else if (request == dxrt::dxrt_cmd_t::DXRT_CMD_NPU_RUN_RESP && size >= sizeof(dxrt_response_t)) {
                    dxrt_response_t* resp = static_cast<dxrt_response_t*>(data);
                    req_info = ", req_id: " + std::to_string(resp->req_id) + ", dma_ch: " + std::to_string(resp->dma_ch);
                }
            }

            else {
                req_info = ", data is nullptr";
            }

            LOG_DXRT_S << "IOControl FAILED - ret: " << ret 
                    << ", errno: " << errno 
                    << " (" << strerror(errno) << ")"
                    << ", fd: " << _fd
                    << ", cmd: " << static_cast<int>(request)
                    << req_info << std::endl; 
            return errno * (-1);
    } else {
        LOG_DXRT_S_DBG << "IOControl SUCCESS - ret: " << ret << std::endl;
    }
    */
    return ret;

}

int32_t LinuxDriverAdapter::Write(const void* buffer, uint32_t size)
{
    int ret = write(_fd, buffer, size);
    if (ret < 0) return ret;
    return 0;
}


int32_t LinuxDriverAdapter::Read(void* buffer, uint32_t size)
{
    int ret = read(_fd, buffer, size);
    if (ret < 0) return ret;
    return 0;
}

void* LinuxDriverAdapter::MemoryMap(void *__addr, size_t __len, off_t __offset)
{
    void* ret = mmap(__addr, __len, PROT_READ|PROT_WRITE, MAP_SHARED, _fd, __offset);
    return ret;
}

int32_t LinuxDriverAdapter::Poll()
{
    LOG_DXRT_DBG << "Polling device..." << std::endl;
    pollfd _devPollFd = {
        .fd = _fd,
        .events = POLLIN,
        // .events = POLLIN|POLLHUP,
        .revents = 0,
    };
    return poll(&_devPollFd, 1, -1); // wait indefinitely
}

LinuxDriverAdapter::~LinuxDriverAdapter()
{
    close(_fd);
}


}  // namespace dxrt

#endif // __linux__
