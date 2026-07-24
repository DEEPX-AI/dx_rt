/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt_service_v2_main_logic.hpp"
#include <vector>
#include <string>
#include "dxrt/dynamic_ipc_endpoint.h"

namespace dxrt {

std::vector<std::string> BuildDxrtServiceV2Endpoints(bool endpointOptionProvided,
    const std::string &cliEndpoint, const std::string &envEndpoint, const std::string &defaultEndpoint)
{
    // Priority: CLI option > Environment variable > Default
    if (endpointOptionProvided || !envEndpoint.empty())
    {
        std::vector<std::string> finalEndpoints;
        const std::string explicitEndpoint = endpointOptionProvided ? cliEndpoint : envEndpoint;
        const std::string normalizedEndpoint = dxrt::NormalizeDynamicIpcEndpoint(explicitEndpoint);
        if (!normalizedEndpoint.empty())
        {
            finalEndpoints.push_back(normalizedEndpoint);
        }
        return finalEndpoints;
    }

    return dxrt::GetDynamicIpcEndpointCandidates(defaultEndpoint);
}

}  // namespace dxrt
