/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt_service_v2_bridge.hpp"
#include "dxrt_service_v2_main_logic.hpp"

#include "dxrt/common.h"
#include "dxrt/dynamic_ipc_endpoint.h"
#include "dxrt/extern/cxxopts.hpp"

#include <algorithm>
#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

int dxrt_service_v2_main(int argc, char** argv)
{
    cxxopts::Options options("dxrtd", "dxrtd dynamic_ipc service v2");

    std::string schedulerOption;
    std::string endpoint = dxrt::GetDynamicIpcEndpoint();
    int backlog = 16;

    options.add_options()
        ("s,scheduler", "Scheduler Mode(FIFO, RoundRobin, SJF)", cxxopts::value<std::string>(schedulerOption)->default_value("FIFO"))
        ("e,endpoint", "Unix domain socket path / named pipe endpoint", cxxopts::value<std::string>(endpoint)->default_value(endpoint))
        ("b,backlog", "Listen backlog", cxxopts::value<int>(backlog)->default_value("16"));

    cxxopts::ParseResult parsedOptions;
    try
    {
        parsedOptions = options.parse(argc, argv);
    }
    catch (const cxxopts::exceptions::exception& e)
    {
        std::cerr << "Error parsing options: " << e.what() << std::endl;
        std::cerr << options.help() << std::endl;
        return EXIT_FAILURE;
    }

    const bool endpointOptionProvided = parsedOptions.count("endpoint") > 0;

    const char *envEndpointRaw = std::getenv("DXRT_DYNAMIC_IPC_ENDPOINT");
    const std::string envEndpoint = (envEndpointRaw != nullptr) ? envEndpointRaw : "";

    const std::vector<std::string> finalEndpoints = dxrt::BuildDxrtServiceV2Endpoints(
        endpointOptionProvided,
        endpoint,
        envEndpoint,
        dxrt::GetDynamicIpcEndpoint());

    LOG_DXRT_S << "DxrtServiceV2 initializing with scheduler=" << schedulerOption
               << " endpoint_count=" << finalEndpoints.size()
               << " backlog=" << backlog
               << " env_endpoint='" << dxrt::GetDynamicIpcEndpointForLog(envEndpoint) << "'"
               << std::endl;
    for (const auto &candidate : finalEndpoints)
    {
        LOG_DXRT_S << "  endpoint='" << dxrt::GetDynamicIpcEndpointForLog(candidate) << "'" << std::endl;
    }

    auto runtime = std::unique_ptr<dxrtdNamespace::DxrtServiceV2Runtime, void (*)(dxrtdNamespace::DxrtServiceV2Runtime *)>(
        dxrtdNamespace::CreateDxrtServiceV2Runtime(schedulerOption),
        &dxrtdNamespace::DestroyDxrtServiceV2Runtime);
    if (runtime == nullptr)
    {
        LOG_DXRT_S_ERR("Failed to create DxrtServiceV2Runtime");
        return EXIT_FAILURE;
    }

    LOG_DXRT_S << "DxrtServiceV2Runtime created, starting IPC server..." << std::endl;
    if (dxrtdNamespace::DxrtServiceV2StartIpcServers(runtime.get(), finalEndpoints, backlog) != 0)
    {
        auto errnoCopy = errno;  // copy errno before any other calls that might change it
        LOG_DXRT_S_ERR(std::string("failed to start dynamic ipc server endpoints")
                  + " (errno=" + std::to_string(errnoCopy) + ": " + std::string(std::strerror(errnoCopy)) + ")");
        return EXIT_FAILURE;
    }

    LOG_DXRT_S << "IPC server started successfully, entering run loop..." << std::endl;

    const int rc = dxrtdNamespace::DxrtServiceV2RunIpcServer(runtime.get(), 1000);
    dxrtdNamespace::DxrtServiceV2StopIpcServer(runtime.get());
    return rc;
}
