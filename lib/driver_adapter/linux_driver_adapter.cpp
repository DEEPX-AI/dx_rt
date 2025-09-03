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

    if (ret < 0)
        return errno*(-1);
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

#define DEVICE_POLL_LIMIT_MS 3*1000*1000

int32_t LinuxDriverAdapter::Poll()
{
    pollfd _devPollFd = {
        .fd = _fd,
        .events = POLLIN,
        // .events = POLLIN|POLLHUP,
        .revents = 0,
    };
    return poll(&_devPollFd, 1, DEVICE_POLL_LIMIT_MS);
}

LinuxDriverAdapter::~LinuxDriverAdapter()
{
    close(_fd);
}


}  // namespace dxrt

#endif // __linux__
