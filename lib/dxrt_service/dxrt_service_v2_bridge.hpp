/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <string>
#include <vector>

namespace dxrtdNamespace
{
    struct DxrtServiceV2Runtime;

    DxrtServiceV2Runtime *CreateDxrtServiceV2Runtime(const std::string &scheduler) noexcept;
    void DestroyDxrtServiceV2Runtime(DxrtServiceV2Runtime *runtime);

    int DxrtServiceV2StartIpcServers(DxrtServiceV2Runtime *runtime,
        const std::vector<std::string> &endpoints, int backlog);
    int DxrtServiceV2RunIpcServer(DxrtServiceV2Runtime *runtime, int timeoutMs);
    void DxrtServiceV2StopIpcServer(DxrtServiceV2Runtime *runtime);
}  // namespace dxrtdNamespace
