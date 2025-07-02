// Copyright (c) 2024 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/driver_adapter/mock_driver_adapter.h"

using namespace dxrt;

// input & output control
int32_t Mock_DriverAdapter::IOControl(dxrt_cmd_t request, void* data, uint32_t size, uint32_t sub_cmd)
{
    (void)request;
    (void)data;
    (void)size;
    (void)sub_cmd;

    return 0;
}

// Write Data via DMA
int32_t Mock_DriverAdapter::Write(const void* buffer, uint32_t size)
{
    (void)buffer;
    (void)size;

    return 0;
}

// Read Datea via DMA
int32_t Mock_DriverAdapter::Read(void* buffer, uint32_t size)
{
    (void)buffer;
    (void)size;

    return 0;
}

// standalone only

// Memory Map
//note : int __prot = PROT_READ|PROT_WRITE,int __flags =  MAP_SHARED,
void* Mock_DriverAdapter::MemoryMap(void *__addr, size_t __len, off_t __offset)
{
    (void)__addr;
    (void)__len;
    (void)__offset;

    return 0;
}

// Poll
//note : nfds_t __nfds = 1, int __timeout = DEVICE_POLL_LIMIT_MS
int32_t Mock_DriverAdapter::Poll()
{

    return 0;
}