/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "noservice_data_source.h"
#include <iostream>

namespace dxrt {

NoServiceDataSource::NoServiceDataSource()
{
    TryReconnect();
}

NoServiceDataSource::~NoServiceDataSource()
{
    _reader.Close();
}

bool NoServiceDataSource::TryReconnect()
{
    // Close existing connection if any
    _reader.Close();
    
    // Try to open shared memory
    if (!_reader.Open()) 
    {
        // SharedMemory doesn't exist yet, Writer not started
        return false;
    }
    
    // Get current writer PID
    _lastWriterPid = _reader.GetWriterPid();
    return true;
}

bool NoServiceDataSource::UpdateDeviceCache(int deviceId)
{
    // If reader not opened, try to connect
    if (!_reader.IsOpened()) 
    {
        if (!TryReconnect()) 
        {
            return false;  // Writer not started yet
        }
    }
    
    // Check if writer is alive
    if (!_reader.IsWriterAlive()) 
    {
        // std::cerr << "[NoServiceDataSource] Writer process not alive. Attempting to reconnect..." << std::endl;
        if (!TryReconnect()) 
        {
            return false;
        }
    }
    
    // Check if writer changed (new process)
    uint32_t currentWriterPid = _reader.GetWriterPid();
    if (currentWriterPid != _lastWriterPid && currentWriterPid != 0)
    {
        // Writer changed, clear cache and update PID
        _deviceDataCache.clear();
        _lastWriterPid = currentWriterPid;
    }
    
    // Try to read device data
    MonitorDeviceData deviceData;
    if (_reader.ReadDeviceData(deviceId, deviceData)) 
    {
        _deviceDataCache[deviceId] = deviceData;
        return true;
    }
    
    return false;
}

double NoServiceDataSource::GetCoreUtilization(int deviceId, int coreId)
{
    if (coreId < 0 || coreId >= 3) 
    {
        return 0.0;
    }
    
    if (!UpdateDeviceCache(deviceId)) 
    {
        return 0.0;
    }
    
    auto it = _deviceDataCache.find(deviceId);
    if (it != _deviceDataCache.end()) 
    {
        return it->second.utilization[coreId] * 1000.0;
    }
    
    return 0.0;
}

uint64_t NoServiceDataSource::GetDramUsage(int deviceId)
{
    if (!UpdateDeviceCache(deviceId)) 
    {
        return 0;
    }
    
    auto it = _deviceDataCache.find(deviceId);
    if (it != _deviceDataCache.end()) 
    {
        return it->second.memory_used;
    }
    
    return 0;
}

bool NoServiceDataSource::IsAvailable()
{
    return _reader.IsWriterAlive();
}

} // namespace dxrt
