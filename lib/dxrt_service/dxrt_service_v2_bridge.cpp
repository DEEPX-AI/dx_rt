/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt_service_v2_bridge.hpp"

#include "dxrt_service_v2.hpp"
#include "dxrt/exception/exception.h"

namespace dxrtdNamespace
{

dxrt::DXRTScheduleV2 parseSchedulerMode(const std::string &scheduler)
{
    if (scheduler == "RoundRobin")
    {
        return dxrt::DXRTScheduleV2::RoundRobin;
    }
    else if (scheduler == "SJF")
    {
        return dxrt::DXRTScheduleV2::SJF;
    }
    else
    {
        return dxrt::DXRTScheduleV2::FIFO;
    }
}

struct DxrtServiceV2Runtime
{
    explicit DxrtServiceV2Runtime(const std::string &scheduler)
        : service(parseSchedulerMode(scheduler))
    {
    }

    dxrt::DxrtServiceV2 service;
};

DxrtServiceV2Runtime *CreateDxrtServiceV2Runtime(const std::string &scheduler) noexcept
{
    // Keep specific catches for known failures and one fallback at the C API boundary.
    // This function must never allow C++ exceptions to escape to C callers.
    try
    {
        return new DxrtServiceV2Runtime(scheduler);
    }
    /* first catch dxrt::Exception because they are not inherited from std::exception
      so they won't be caught by the catch(std::exception) block below. */
    catch (const dxrt::Exception &e)
    {
        LOG_DXRT_S_ERR(std::string("dxrt General exception: ") + e.what());
        return nullptr;
    }
    catch (const std::bad_alloc &e)
    {
        LOG_DXRT_S_ERR(std::string("Insufficient Memory: ") + e.what());
        return nullptr;
    }
    catch (const std::invalid_argument &e)
    {
        LOG_DXRT_S_ERR(std::string("Invalid argument: ") + e.what());
        return nullptr;
    }
    catch (const std::exception &e)  // NOSONAR: C API boundary fallback for remaining std exceptions.
    {
        LOG_DXRT_S_ERR(std::string("Unhandled exception: ") + e.what());
        return nullptr;
    }

    catch (...)  // NOSONAR: C API boundary fallback for non-std exceptions.
    {
        LOG_DXRT_S_ERR("Unknown non-std exception occurred");
        return nullptr;
    }
}

void DestroyDxrtServiceV2Runtime(DxrtServiceV2Runtime *runtime)
{
    delete runtime;
}

int DxrtServiceV2StartIpcServers(DxrtServiceV2Runtime *runtime, const std::vector<std::string> &endpoints, int backlog)
{
    if (runtime == nullptr)
    {
        return -1;
    }

    return runtime->service.StartIpcServers(endpoints, backlog);
}

int DxrtServiceV2RunIpcServer(DxrtServiceV2Runtime *runtime, int timeoutMs)
{
    if (runtime == nullptr)
    {
        return -1;
    }

    return runtime->service.RunIpcServer(timeoutMs);
}

void DxrtServiceV2StopIpcServer(DxrtServiceV2Runtime *runtime)
{
    if (runtime == nullptr)
    {
        return;
    }

    runtime->service.StopIpcServer();
}
}  // namespace dxrtdNamespace
