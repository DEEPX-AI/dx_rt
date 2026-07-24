/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#ifdef __linux__
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#endif

namespace dxrt {

// Can be overridden via DXRT_DYNAMIC_IPC_ENDPOINT environment variable.
//
// Priority policy:
//  1) explicit env endpoint (typically /mnt/... in deployment)
//  2) unix abstract socket
//  3) /tmp filesystem socket fallback
const char* const kAbstractDynamicIpcEndpoint = "@dxrt_dynamic_ipc.sock";
const char* const kTmpDynamicIpcEndpoint = "/tmp/dxrt_dynamic_ipc.sock";
#ifdef _WIN32
const char* const kWindowsDynamicIpcEndpoint = "\\\\.\\pipe\\dxrt_service_ipc_dynamic";
#endif

#ifdef __linux__
inline bool CanUseAbstractDynamicIpcEndpoint()
{
    const int fd = ::socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (fd < 0)
    {
        return false;
    }

    struct sockaddr_un addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;

    constexpr const char kProbeName[] = "dxrt_dynamic_ipc_probe";
    std::memcpy(addr.sun_path + 1, kProbeName, sizeof(kProbeName) - 1);

    const auto len = static_cast<socklen_t>(
        offsetof(struct sockaddr_un, sun_path) + 1 + sizeof(kProbeName) - 1);
    const auto *sockAddr = static_cast<const struct sockaddr *>(static_cast<const void *>(&addr));
    const int rc = ::bind(fd, sockAddr, len);
    (void)::close(fd);
    return (rc == 0);
}
#endif

inline std::string GetDynamicIpcEndpoint()
{
    const char *endpoint = std::getenv("DXRT_DYNAMIC_IPC_ENDPOINT");
    if (endpoint != nullptr && endpoint[0] != '\0')
    {
        return endpoint;
    }

#ifdef _WIN32
    return kWindowsDynamicIpcEndpoint;
#else
    return kAbstractDynamicIpcEndpoint;
#endif
}

inline std::vector<std::string> GetDynamicIpcEndpointCandidates(const std::string &primaryEndpoint);

inline std::vector<std::string> GetDynamicIpcEndpointCandidates()
{
    return GetDynamicIpcEndpointCandidates(GetDynamicIpcEndpoint());
}

inline std::string NormalizeDynamicIpcEndpoint(const std::string &endpoint)
{
    if (endpoint.empty())
    {
        return endpoint;
    }

    if (endpoint[0] == '\0')
    {
        std::string normalized("@");
        normalized.append(endpoint.data() + 1, endpoint.size() - 1);
        return normalized;
    }

    return endpoint;
}

inline std::vector<std::string> GetDynamicIpcEndpointCandidates(const std::string &primaryEndpoint)
{
    std::vector<std::string> endpoints;
    endpoints.reserve(3);

    const std::string primary = NormalizeDynamicIpcEndpoint(primaryEndpoint);
    if (!primary.empty())
    {
        endpoints.push_back(primary);
    }

#ifdef _WIN32
    if (primary != kWindowsDynamicIpcEndpoint)
    {
        endpoints.emplace_back(kWindowsDynamicIpcEndpoint);
    }
    return endpoints;
#else
    if (primary != kAbstractDynamicIpcEndpoint)
    {
        endpoints.emplace_back(kAbstractDynamicIpcEndpoint);
    }

    if (primary != kTmpDynamicIpcEndpoint)
    {
        endpoints.emplace_back(kTmpDynamicIpcEndpoint);
    }

    return endpoints;
#endif
}

// Format endpoint for safe logging (already in @ format for abstract, /path for filesystem)
inline std::string GetDynamicIpcEndpointForLog(const std::string &endpoint)
{
    if (endpoint.empty())
    {
        return endpoint;
    }

    // Some call paths may carry abstract endpoint as a raw NUL-prefixed string.
    // Convert to a printable representation for logs.
    std::string printable;
    printable.reserve(endpoint.size() + 2);
    for (size_t i = 0; i < endpoint.size(); ++i)
    {
        const char ch = endpoint[i];
        if (ch == '\0')
        {
            if (i == 0)
            {
                // Keep Linux abstract-socket notation human-readable in logs.
                printable.push_back('@');
            }
            else
            {
                printable += "\\0";
            }
            continue;
        }
        printable.push_back(ch);
    }

    return printable;
}

}  // namespace dxrt
