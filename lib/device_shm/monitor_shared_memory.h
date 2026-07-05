/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <atomic>
#include <array>

#include "dxrt/driver.h"
#include "dxrt/device_struct.h"

namespace dxrt {

constexpr uint32_t MONITOR_SHM_MAGIC = 0x44585254;  // "DXRT"
constexpr uint32_t MONITOR_SHM_VERSION = 2;
constexpr int MAX_MONITOR_DEVICES = 32;
#ifdef _WIN32
constexpr const char* MONITOR_SHM_NAME = "Local\\dxrt_monitor_v2";
#else
// POSIX SHM names use a namespace-style leading '/'.
// This is not a filesystem root path like '/var' or '/tmp'.
constexpr const char* MONITOR_SHM_NAME = "/dxrt_monitor_v2";
// Allow all users to attach (rw-rw-rw-). Single-writer safety is enforced
// via fcntl(F_WRLCK) in SharedMemoryWriter, not via file permissions, so
// widening the mode lets non-root readers (e.g. dxcli/dxrun) attach to an
// SHM created by a root-owned monitor service without hitting EACCES.
constexpr int MONITOR_SHM_PERMS = 0666;
#endif

inline const char* GetMonitorShmName()
{
    const char* override_name = std::getenv("DXRT_MONITOR_SHM_NAME");
    if (override_name != nullptr && override_name[0] != '\0')
    {
        return override_name;
    }

    return MONITOR_SHM_NAME;
}

struct MonitorDeviceData {
    uint32_t device_id = 0;

    dxrt_device_info_t spec = {};
    dxrt_dev_info_t dev_info = {};
    dxrt_device_status_t status = {};

    // NPU utilization per core (0.0 ~ 1.0)
    std::array<double, 3> utilization = {};

    // Memory information (bytes)
    uint64_t memory_total = 0;
    uint64_t memory_used = 0;
    uint64_t memory_free = 0;

    // Device status
    bool is_active = false;
    uint32_t inference_count = 0;

    MonitorDeviceData() = default;
};

// Layout verification for cross-process SHM compatibility.
// If any included C struct changes size, this will catch it at compile time.
static_assert(sizeof(MonitorDeviceData) == 400, "MonitorDeviceData layout changed — SHM ABI break");

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

static_assert(sizeof(MonitorSharedMemory) == 12848, "MonitorSharedMemory layout changed — SHM ABI break");

} // namespace dxrt
