/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include "dxrt/common.h"
#ifndef _WIN32
#include <sys/types.h>
#endif
#include "monitor_shared_memory.h"

namespace dxrt {

class DXRT_API SharedMemoryReader {
public:
    SharedMemoryReader();
    ~SharedMemoryReader();
    
    // Delete copy operations (shared memory fd should not be copied)
    SharedMemoryReader(const SharedMemoryReader&) = delete;
    SharedMemoryReader& operator=(const SharedMemoryReader&) = delete;
    
    // Delete move operations (to prevent accidental misuse)
    SharedMemoryReader(SharedMemoryReader&&) = delete;
    SharedMemoryReader& operator=(SharedMemoryReader&&) = delete;
    
    // Open existing shared memory (read-only)
    bool Open();
    
    // Read device data
    bool ReadDeviceData(int deviceId, MonitorDeviceData& outData) const;
    
    // Get all devices
    bool GetAllDevices(MonitorDeviceData* outDevices, uint32_t& outCount, uint32_t maxCount) const;
    
    // Check if writer is alive
    bool IsWriterAlive() const;
    
    // Get writer PID
    uint32_t GetWriterPid() const;
    
    // Get update count
    uint64_t GetUpdateCount() const;
    
    // Close
    void Close();
    
    bool IsOpened() const { return _opened; } 
    
private:
    bool _opened = false;
#ifdef _WIN32
    void* _shm_handle{nullptr};
#else
    int _shm_fd = -1;
#endif
    void* _shm_ptr = nullptr;
};

} // namespace dxrt
