/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "memfd_handle_transfer.h"
#include "dxrt/common.h"

#include <string>

// ============================================================
// Windows implementation
// ============================================================
#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

namespace dxrt {
namespace shm {

intptr_t MemFDHandleTransfer::PrepareForPacket(MemFDHandle handle, uint32_t targetPid)
{
    if (handle == kInvalidMemFDHandle || targetPid == 0)
    {
        return kInvalidMemFDHandle;
    }

    // Open the receiver process with handle-duplication permission.
    HANDLE hTarget = ::OpenProcess(PROCESS_DUP_HANDLE, FALSE, static_cast<DWORD>(targetPid));
    if (hTarget == nullptr)
    {
        LOG_DXRT_ERR("MemFDHandleTransfer::PrepareForPacket: OpenProcess failed: pid=" +
            std::to_string(targetPid) + ", err=" + std::to_string(::GetLastError()));
        return kInvalidMemFDHandle;
    }

    HANDLE hSrc = reinterpret_cast<HANDLE>(static_cast<LONG_PTR>(handle));
    HANDLE hDup = nullptr;

    const BOOL ok = ::DuplicateHandle(
        ::GetCurrentProcess(),  // source process (server)
        hSrc,                   // handle to duplicate
        hTarget,                // target process (client)
        &hDup,                  // receives duplicated handle value in target process
        0,                      // dwDesiredAccess: ignored due to DUPLICATE_SAME_ACCESS
        FALSE,                  // disable handle inheritance
        DUPLICATE_SAME_ACCESS); // same access rights as source

    ::CloseHandle(hTarget);

    if (!ok)
    {
        LOG_DXRT_ERR("MemFDHandleTransfer::PrepareForPacket: DuplicateHandle failed: pid=" +
            std::to_string(targetPid) + ", err=" + std::to_string(::GetLastError()));
        return kInvalidMemFDHandle;
    }

    return static_cast<intptr_t>(reinterpret_cast<LONG_PTR>(hDup));
}

void MemFDHandleTransfer::RevokeOnFailure(intptr_t dupHandleValue, uint32_t targetPid)
{
    if (dupHandleValue == kInvalidMemFDHandle || targetPid == 0)
    {
        return;
    }

    // Close the handle that was duplicated into the target process.
    // DUPLICATE_CLOSE_SOURCE: closes only hSourceHandle (handle in target), without creating a new handle.
    HANDLE hTarget = ::OpenProcess(PROCESS_DUP_HANDLE, FALSE, static_cast<DWORD>(targetPid));
    if (hTarget == nullptr)
    {
        LOG_DXRT_ERR("MemFDHandleTransfer::RevokeOnFailure: OpenProcess failed: pid=" +
            std::to_string(targetPid) + ", err=" + std::to_string(::GetLastError()));
        return;
    }

    HANDLE hDup = reinterpret_cast<HANDLE>(static_cast<LONG_PTR>(dupHandleValue));

    // source process = hTarget (because the handle lives in the target process)
    // hSourceHandle = hDup (handle to close, in target process)
    // hTargetHandle = nullptr (no new handle creation)
    ::DuplicateHandle(
        hTarget,                // process that owns the handle
        hDup,                   // handle to close (target process view)
        nullptr,                // no duplication target
        nullptr,                // no output handle
        0,
        FALSE,
        DUPLICATE_CLOSE_SOURCE); // close only the source (=handle in target)

    ::CloseHandle(hTarget);
}

}  // namespace shm
}  // namespace dxrt

// ============================================================
// Linux / POSIX implementation (no-op: SCM_RIGHTS handles fd transfer)
// ============================================================
#else

namespace dxrt {
namespace shm {

intptr_t MemFDHandleTransfer::PrepareForPacket(MemFDHandle /*handle*/, uint32_t /*targetPid*/)
{
    // On Linux, SCM_RIGHTS copies the fd into the receiver process at kernel level.
    // The packet inline field (IPCPacketResponseAllocateBuffer::fd) is not used,
    // so return kInvalidMemFDHandle(-1).
    return kInvalidMemFDHandle;
}

void MemFDHandleTransfer::RevokeOnFailure(intptr_t /*dupHandleValue*/, uint32_t /*targetPid*/)
{
    // On Linux, PrepareForPacket always returns kInvalidMemFDHandle,
    // so there is no handle to revoke. no-op.
}

}  // namespace shm
}  // namespace dxrt

#endif  // _WIN32
