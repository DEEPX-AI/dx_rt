// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <stdint.h>
#include <cstdint>

#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"

namespace dxrt {

class DriverAdapter {

public:

    // input & output control
    virtual int32_t IOControl(dxrt_cmd_t request, void* data, uint32_t size = 0, uint32_t sub_cmd = 0) {
        std::ignore = request;
        std::ignore = data;
        std::ignore = size;
        std::ignore = sub_cmd;
        return -1;
    }
    virtual int32_t NetControl(dxrt_cmd_t request, void* data, uint32_t size = 0, uint32_t sub_cmd = 0, uint64_t address = 0, bool ctrlCmd = true) {
        std::ignore = request;
        std::ignore = data;
        std::ignore = size;
        std::ignore = sub_cmd;
        std::ignore = address;
        std::ignore = ctrlCmd;
        return -1;
    }

    // Write Data via DMA
    virtual int32_t Write(const void* buffer, uint32_t size) = 0;

    // Read Datea via DMA
    virtual int32_t Read(void* buffer, uint32_t size) = 0;

    dxrt_device_status_t getDeviceStatus();

    // standalone only

    // Memory Map
    //note : int __prot = PROT_READ|PROT_WRITE,int __flags =  MAP_SHARED,
    virtual void* MemoryMap(void *__addr, size_t __len, off_t __offset = 0) = 0;

    // Poll
    //note : nfds_t __nfds = 1, int __timeout = DEVICE_POLL_LIMIT_MS
    virtual int32_t Poll() = 0;

    virtual ~DriverAdapter() = default;

    virtual int GetFd() = 0;
};

}  // namespace dxrt
