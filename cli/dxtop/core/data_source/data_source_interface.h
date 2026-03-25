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

namespace dxrt {

/**
 * Abstract interface for NPU data source
 * 
 * This interface abstracts the data source for NPU monitoring,
 * allowing dxtop to work with both Service mode (IPC) and NoService mode (shared memory).
 */
class IDataSource {
public:
    virtual ~IDataSource() = default;
    
    /**
     * Get core utilization percentage
     * @param deviceId Device ID
     * @param coreId Core ID (0-2)
     * @return Utilization percentage (0.0 - 100.0)
     */
    virtual double GetCoreUtilization(int deviceId, int coreId) = 0;
    
    /**
     * Get device DRAM usage in bytes
     * @param deviceId Device ID
     * @return DRAM usage in bytes
     */
    virtual uint64_t GetDramUsage(int deviceId) = 0;
    
    /**
     * Check if data source is available
     * @return true if available, false otherwise
     */
    virtual bool IsAvailable() = 0;
};

} // namespace dxrt
