/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include "data_source_interface.h"
#include "shared_memory_reader.h"
#include "monitor_shared_memory.h"
#include <memory>
#include <map>

namespace dxrt {

/**
 * NoService mode data source implementation using shared memory
 * 
 * This implementation reads monitoring data from shared memory
 * written by the runtime process in NoService mode.
 */
class NoServiceDataSource : public IDataSource {
public:
    explicit NoServiceDataSource();
    ~NoServiceDataSource() override;
    
    double GetCoreUtilization(int deviceId, int coreId) override;
    uint64_t GetDramUsage(int deviceId) override;
  
    bool IsAvailable() override;
    
private:
    SharedMemoryReader _reader;
    std::map<int, MonitorDeviceData> _deviceDataCache;
    uint32_t _lastWriterPid = 0;  // Track writer PID to detect changes
    
    bool UpdateDeviceCache(int deviceId);
    bool TryReconnect();  // Try to reconnect to shared memory
};

} // namespace dxrt
