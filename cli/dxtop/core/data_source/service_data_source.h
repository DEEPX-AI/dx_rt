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
#include "../ipc/dxtop_ipc_client.h"
#include <memory>

namespace dxrt {

/**
 * Service mode data source implementation using IPC
 * 
 * This implementation communicates with dxrtd service via IPC
 * to retrieve NPU monitoring data.
 */
class ServiceDataSource : public IDataSource {
public:
    explicit ServiceDataSource();
    ~ServiceDataSource() override = default;
    
    double GetCoreUtilization(int deviceId, int coreId) override;
    uint64_t GetDramUsage(int deviceId) override;
    
    bool IsAvailable() override;
    
private:
    DXTopIPCClient _ipcClient;
};

} // namespace dxrt
