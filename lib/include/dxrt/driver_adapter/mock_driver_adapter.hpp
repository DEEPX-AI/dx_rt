// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "driver_adapter.hpp"

namespace dxrt {

class Mock_DriverAdapter : public DriverAdapter {

public:

    // input & output control
    int32_t IOControl(dxrt_cmd_t request, void* data, uint32_t size = 0, uint32_t sub_cmd = 0) override;

    // Write Data via DMA
    int32_t Write(const void* buffer, uint32_t size) override;

    // Read Datea via DMA
    int32_t Read(void* buffer, uint32_t size) override;

    // standalone only

    // Memory Map
    // note : int __prot = PROT_READ|PROT_WRITE,int __flags =  MAP_SHARED,
    void* MemoryMap(void *__addr, size_t __len, off_t __offset = 0) override;

    // Poll
    // note : nfds_t __nfds = 1, int __timeout = DEVICE_POLL_LIMIT_MS
    int32_t Poll() override;
};

}  // namespace dxrt
