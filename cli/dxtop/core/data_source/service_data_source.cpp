/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "service_data_source.h"
#include "dxrt/service_util.h"

namespace dxrt {

ServiceDataSource::ServiceDataSource()
{
}

double ServiceDataSource::GetCoreUtilization(int deviceId, int coreId)
{
    try {
        // Request utilization via IPC (returns percentage)
        uint64_t utilization = _ipcClient.SendRequest(
            dxrt::REQUEST_CODE::GET_USAGE,
            deviceId,
            coreId
        );
        return static_cast<double>(utilization);
    } catch (const std::exception& e) {
        // Return 0 on error
        return 0.0;
    }
}

uint64_t ServiceDataSource::GetDramUsage(int deviceId)
{
    try {
        return _ipcClient.SendRequest(
            dxrt::REQUEST_CODE::VIEW_USED_MEMORY,
            deviceId,
            10
        );
    } catch (const std::exception& e) {
        return 0;
    }
}

bool ServiceDataSource::IsAvailable()
{
    return isDxrtServiceRunning();
}

} // namespace dxrt
