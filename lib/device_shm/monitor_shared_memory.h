/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <cstdint>
#include <cstring>
#include <atomic>
#include <array>

namespace dxrt {

constexpr uint32_t MONITOR_SHM_MAGIC = 0x44585254;  // "DXRT"
constexpr uint32_t MONITOR_SHM_VERSION = 1;
constexpr int MAX_MONITOR_DEVICES = 32;
#ifdef _WIN32
constexpr const char* MONITOR_SHM_NAME = "Local\\dxrt_monitor";
#else
constexpr const char* MONITOR_SHM_NAME = "/dxrt_monitor";
#endif

struct MonitorDeviceData {
    uint32_t device_id = 0;
    
    // NPU utilization per core (0.0 ~ 1.0)
    std::array<double, 3> utilization = {};
    
    // Core status per core
    std::array<uint32_t, 3> voltage = {};      // mV
    std::array<uint32_t, 3> clock = {};        // MHz
    std::array<uint32_t, 3> temperature = {};  // Celsius
    
    // Memory information (bytes)
    uint64_t memory_total = 0;
    uint64_t memory_used = 0;
    uint64_t memory_free = 0;
    
    // Device status
    bool is_active = false;
    uint32_t inference_count = 0;
    
    MonitorDeviceData() = default;
};

/*
 * Sequence Lock for Reader-Writer Synchronization
 * 
 * This implements a lightweight lock-free synchronization mechanism optimized for
 * read-heavy workloads (many readers, single writer).
 * 
 * How it works:
 * 1. Writer increments sequence to ODD number (signals "update in progress")
 * 2. Writer updates data
 * 3. Writer increments sequence to EVEN number (signals "update complete")
 * 4. Reader reads sequence (if ODD, retry)
 * 5. Reader copies data
 * 6. Reader reads sequence again (if changed, data was inconsistent, retry)
 * 
 * Benefits:
 * - No mutex locks needed
 * - Readers never block writers
 * - Writers never block readers
 * - Low overhead for monitoring use case
 * 
 * Sequence number interpretation:
 * - EVEN: Data is stable and can be read safely
 * - ODD:  Writer is currently updating data, reader should retry
 */
struct MonitorSharedMemory {
    // Sequence lock for synchronization (see comment above)
    std::atomic<uint64_t> sequence{0};
    
    // Header
    uint32_t magic = MONITOR_SHM_MAGIC;
    uint32_t version = MONITOR_SHM_VERSION;
    uint32_t writer_pid = 0;
    uint64_t update_count = 0;
    uint64_t last_update_timestamp = 0;  // nanoseconds since epoch
    
    // Device data
    uint32_t device_count = 0;
    std::array<MonitorDeviceData, MAX_MONITOR_DEVICES> devices = {};
    
    MonitorSharedMemory() = default;
};

} // namespace dxrt
