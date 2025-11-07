/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#ifdef __linux__ // all or nothing

#pragma once

#include "driver_adapter.h"
#include <string>

namespace dxrt {

class LinuxDriverAdapter : public DriverAdapter {
 public:
    explicit LinuxDriverAdapter(const char* fileName);
    int32_t IOControl(dxrt_cmd_t request, void* data, uint32_t size = 0, uint32_t sub_cmd = 0) override;
    int32_t Write(const void* buffer, uint32_t size) override;
    int32_t Read(void* buffer, uint32_t size) override;
    void* MemoryMap(void *__addr, size_t __len, off_t __offset = 0) override;
    int32_t Poll() override;
    int GetFd() const override { return _fd; }
    std::string GetName() const override { return _name;  }

    ~LinuxDriverAdapter() override;
 private:
    int _fd;
    std::string _name;
};

}  // namespace dxrt

#endif // __linux__
