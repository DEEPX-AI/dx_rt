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
#include "monitor_shared_memory.h"
#include <array>

namespace dxrt {

class DXRT_API SharedMemoryWriter {
public:
    SharedMemoryWriter() = default;
    ~SharedMemoryWriter();
    
    // Prevent copying and moving (RAII resource management)
    SharedMemoryWriter(const SharedMemoryWriter&) = delete;
    SharedMemoryWriter& operator=(const SharedMemoryWriter&) = delete;
    SharedMemoryWriter(SharedMemoryWriter&&) = delete;
    SharedMemoryWriter& operator=(SharedMemoryWriter&&) = delete;
    
    // Initialize shared memory
    bool Initialize();
    
    // Update device utilization data
    void UpdateDeviceUtilization(int deviceId, const std::array<double, 3>& utilization);
    
    // Update device memory data
    void UpdateDeviceMemory(int deviceId, uint64_t total, uint64_t used, uint64_t free);
    
    // Update device core stats (voltage, clock, temperature)
    void UpdateDeviceCoreStats(int deviceId, const std::array<uint32_t, 3>& voltage, const std::array<uint32_t, 3>& clock, const std::array<uint32_t, 3>& temperature);
    
    // Set device active status
    void SetDeviceActive(int deviceId, bool active);
    
    // Increment inference count
    void IncrementInferenceCount(int deviceId);
    
    // Cleanup
    void Cleanup();
    
    bool IsInitialized() const { return _initialized; }
    
private:
    bool _initialized{false};
#ifdef _WIN32
    void* _shm_handle{nullptr};
#else
    int _shm_fd{-1};
#endif
    MonitorSharedMemory* _shm_ptr{nullptr};
    
    void UpdateTimestamp();
    MonitorDeviceData* GetDeviceData(int deviceId);
    
    // Sequence lock helpers
    void BeginWrite();  // Increment sequence to odd (signals update in progress)
    void EndWrite();    // Increment sequence to even (signals update complete)
};

} // namespace dxrt
