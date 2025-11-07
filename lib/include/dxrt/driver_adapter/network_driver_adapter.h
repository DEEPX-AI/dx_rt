/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <map>

#include "driver_adapter.h"


namespace dxrt {

typedef enum _tcpMsgType {
    TCP_MESSAGE     = 0,
    TCP_QUEUE       = 1,
    TCP_DATAS       = 2,
    TCP_DATAS_GET   = 3,
    TCP_TYPES_MAX   = 4,
} tcpMsgType;
#ifdef __linux__
class NetworkDriverAdapter : public DriverAdapter {
 public:
    explicit NetworkDriverAdapter();
    int32_t NetControl(dxrt_cmd_t request, void* data, uint32_t size = 0, uint32_t sub_cmd = 0, uint64_t address = 0, bool ctrlCmd = true) override;
    int32_t Write(const void* buffer, uint32_t size) override;
    int32_t Read(void* buffer, uint32_t size) override;


    void* MemoryMap(void *__addr, size_t __len, off_t __offset = 0) override {
      std::ignore = __addr;
      std::ignore = __len;
      std::ignore = __offset;
      return nullptr;
    }
    int32_t Poll() override {
      return 0;
    }

    int GetFd() const override {
      return 0;
    }

    ~NetworkDriverAdapter() override;

    std::string GetName() const override { return "NetworkDriverAdapter"; }

  private:
    /* Type, socket, port */
    std::map<tcpMsgType, std::pair<int, int>> sockMap;
};
#endif
}  // namespace dxrt

