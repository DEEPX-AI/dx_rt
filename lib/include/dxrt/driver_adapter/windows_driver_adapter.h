// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once
#ifdef _WIN32

#include "driver_adapter.h"

namespace dxrt {

class WindowsDriverAdapter : public DriverAdapter {
 public:
    explicit WindowsDriverAdapter(const char* fileName);
    int32_t IOControl(dxrt_cmd_t request, void* data, uint32_t size = 0, uint32_t sub_cmd = 0) override;
    int32_t Write(const void* buffer, uint32_t size) override;
    int32_t Read(void* buffer, uint32_t size) override;
    void* MemoryMap(void *__addr, size_t __len, off_t __offset = 0) override;
    int32_t Poll() override;
    int GetFd() { return reinterpret_cast<uint64_t>(_fd); }

    ~WindowsDriverAdapter() override;
 private:
    HANDLE _fd = INVALID_HANDLE_VALUE;
};

}  // namespace dxrt

#endif // _WIN32