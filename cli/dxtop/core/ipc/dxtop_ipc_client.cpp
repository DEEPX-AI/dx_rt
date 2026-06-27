/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxtop_ipc_client.h"

#include "dxrt/dynamic_ipc_endpoint.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <stdexcept>

namespace dxrt {

namespace {
constexpr int kEndpointRetryRounds = 3;
constexpr int kEndpointRetryIntervalMs = 100;
// Max initial wait in constructor path = 3 rounds * 100 ms = 300 ms.
}

DXTopIPCClient::DXTopIPCClient()
{
    // Initialize Dynamic IPC packet client for dxtop service queries
    // Client will establish connection to service v2 IPC endpoint
    // Default IPC endpoint path used by DxrtServiceV2
    if (!ensureConnected())
    {
        std::cerr << "DXTopIPCClient: initial IPC connection failed; will retry on first query"
                  << std::endl;
    }
}

bool DXTopIPCClient::ensureConnected()
{
    if (_connected)
    {
        return true;
    }

    const auto endpoints = GetDynamicIpcEndpointCandidates();
    for (int round = 0; round < kEndpointRetryRounds && !_connected; ++round)
    {
        for (const auto &endpoint : endpoints)
        {
            const int rc = _client.connectToServer(endpoint, 0, 0);
            if (rc == 0)
            {
                _connected = true;
                break;
            }
        }
        if ((round == kEndpointRetryRounds - 1) && (_connected.load() == false))
        {
            std::cerr << "DXTopIPCClient: failed to connect to any IPC endpoint after "
                      << kEndpointRetryRounds << " rounds: ";
            for (const auto &endpoint : endpoints)
            {
                std::cerr << endpoint << " ";
            }
            std::cerr << std::endl;
        }
        else if (_connected.load() == false)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(kEndpointRetryIntervalMs));
        }
    }
    return _connected;
}

double DXTopIPCClient::GetCoreUtilizationLegacy(int deviceId, int coreId)
{
    try {
        if (!ensureConnected())
        {
            return 0.0;
        }

        double usage = 0.0;
        const int rc = _client.GetUsageLegacy(deviceId, coreId, &usage);
        if (rc != 0) {
            return 0.0;
        }
        return usage;
    } catch (const std::exception&) {
        return 0.0;
    }
}

uint64_t DXTopIPCClient::GetDramUsageLegacy(int deviceId)
{
    try {
        if (!ensureConnected())
        {
            return 0;
        }

        uint64_t bytes = 0;
        const int rc = _client.ViewUsedMemoryLegacy(deviceId, &bytes);
        if (rc != 0) {
            return 0;
        }
        return bytes;
    } catch (const std::exception&) {
        return 0;
    }
}

bool DXTopIPCClient::GetDeviceTelemetry(int deviceId, DeviceTelemetrySnapshot *snapshot)
{
    if (snapshot == nullptr)
    {
        return false;
    }

    try {
        if (!ensureConnected())
        {
            return false;
        }

        IPCPacketClient::DeviceTelemetryResult response{};
        const int rc = _client.GetDeviceTelemetry(deviceId, &response);
        if (rc != 0 || response.result != 0) {
            return false;
        }

        snapshot->coreCount = response.coreCount;
        snapshot->usedMemoryBytes = response.usedMemoryBytes;
        snapshot->freeMemoryBytes = response.freeMemoryBytes;

        const size_t utilizationCopyCount = (std::min)(response.utilizationPermille.size(), snapshot->utilizationPermille.size());
        std::copy_n(response.utilizationPermille.begin(), utilizationCopyCount, snapshot->utilizationPermille.begin());

        const size_t temperatureCopyCount = (std::min)(response.temperature.size(), snapshot->temperature.size());
        std::copy_n(response.temperature.begin(), temperatureCopyCount, snapshot->temperature.begin());

        const size_t clockCopyCount = (std::min)(response.clock.size(), snapshot->clock.size());
        std::copy_n(response.clock.begin(), clockCopyCount, snapshot->clock.begin());

        const size_t voltageCopyCount = (std::min)(response.voltage.size(), snapshot->voltage.size());
        std::copy_n(response.voltage.begin(), voltageCopyCount, snapshot->voltage.begin());
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

}  // namespace dxrt
