// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#ifdef __linux__ // all or nothing

#pragma once

#include "driver_adapter.h"

namespace dxrt {

class LinuxDriverAdapter : public DriverAdapter {
 public:
    explicit LinuxDriverAdapter(const char* fileName);
    int32_t IOControl(dxrt_cmd_t request, void* data, uint32_t size = 0, uint32_t sub_cmd = 0) override;
    int32_t Write(const void* buffer, uint32_t size) override;
    int32_t Read(void* buffer, uint32_t size) override;
    void* MemoryMap(void *__addr, size_t __len, off_t __offset = 0) override;
    int32_t Poll() override;
    int GetFd() override { return _fd; }

    ~LinuxDriverAdapter() override;
 private:
    int _fd;
};

}  // namespace dxrt

#endif // __linux__
