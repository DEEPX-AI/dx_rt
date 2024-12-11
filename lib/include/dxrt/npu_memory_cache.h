// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "dxrt/common.h"
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <iostream>




namespace dxrt {
class Device;

class NpuMemoryCacheManager
{
public:
    NpuMemoryCacheManager(Device* device_);
    bool registerMemoryCache(int taskId, int64_t size, int count);
    void unRegisterMemorgCache(int taskId);
    bool canGetCache(int taskId);
    int64_t getNpuMemoryCache(int taskId);
    void returnNpuMemoryCache(int taskId, int64_t addr);
private:
    std::unordered_map<int, std::vector<int64_t>> _npuMemoryCaches;
    std::unordered_map<int, int64_t> _npuMemoryCacheOffset;
    std::mutex _npuMemoryCacheLock;
    Device* _device;
};

}  // namespace dxrt
