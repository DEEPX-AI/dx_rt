/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include "../../../../lib/dynamic_ipc/protocol/ipc_packet_client.hpp"
#include <atomic>

namespace dxrt {

    class DXTopIPCClient
    {
    public:
        struct DeviceTelemetrySnapshot
        {
            uint32_t coreCount{0};
            uint64_t usedMemoryBytes{0};
            uint64_t freeMemoryBytes{0};
            std::array<uint32_t, 4> utilizationPermille{{0, 0, 0, 0}};
            std::array<int32_t, 4> temperature{{0, 0, 0, 0}};
            std::array<uint32_t, 4> clock{{0, 0, 0, 0}};
            std::array<uint32_t, 4> voltage{{0, 0, 0, 0}};
        };

        explicit DXTopIPCClient();
        virtual ~DXTopIPCClient() = default;

        /**
         * Get core/channel utilization via Dynamic IPC legacy path
         * @param deviceId Device ID
         * @param coreId Channel/core ID
         * @return Utilization percentage
         */
        double GetCoreUtilizationLegacy(int deviceId, int coreId);

        /**
         * Get DRAM usage via Dynamic IPC legacy path
         * @param deviceId Device ID
         * @return Used memory in bytes
         */
        uint64_t GetDramUsageLegacy(int deviceId);
        [[nodiscard]] bool GetDeviceTelemetry(int deviceId, DeviceTelemetrySnapshot *snapshot);

    private:
        IPCPacketClient _client;
        std::atomic<bool> _connected = {false};

        bool ensureConnected();

};

}
