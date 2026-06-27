/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <cstdint>

#include "shared_memory_syscall_adapter.h"

namespace dxrt {
namespace shm {

/**
 * @brief Cross-process MemFDHandle duplication utility for IPC transport
 *
 * Linux  : fd is copied directly by the kernel via SCM_RIGHTS, so no conversion is needed (no-op).
 * Windows: DuplicateHandle duplicates HANDLE into the target process,
 *          then the duplicated HANDLE value (intptr_t) is sent in an inline IPC packet field.
 *
 * Typical flow (server sender side):
 *   intptr_t inPacketFd = MemFDHandleTransfer::PrepareForPacket(fd, clientPid);
 *   // Put it into packet.fd = inPacketFd; and call sendToClient(..., -1)
 *   if (sendFailed && inPacketFd != kInvalidMemFDHandle)
 *       MemFDHandleTransfer::RevokeOnFailure(inPacketFd, clientPid);
 */
class MemFDHandleTransfer
{
public:
    /**
    * @brief Duplicate the handle into the target process before sending (Windows only).
     *
    * - Windows: Opens targetPid with PROCESS_DUP_HANDLE and then calls DuplicateHandle.
    *            The return value is an intptr_t representation of a HANDLE immediately valid
    *            in the target process.
    * - Linux  : SCM_RIGHTS transfers fd at kernel level, so conversion is unnecessary.
    *            Always returns kInvalidMemFDHandle(-1).
     *
    * @param handle    source MemFDHandle (valid in the current process).
    * @param targetPid PID of the receiving process.
    * @return Windows: intptr_t value of a HANDLE valid in the target process.
     *         Linux  : kInvalidMemFDHandle.
     */
    static intptr_t PrepareForPacket(MemFDHandle handle, uint32_t targetPid);

    /**
    * @brief Revoke duplicated handles in the target process when send fails (Windows only).
     *
    * If packet transmission fails after PrepareForPacket, call this function
    * to avoid leaked open handles in the target process.
     *
    * - Windows: Closes the target process handle using DUPLICATE_CLOSE_SOURCE.
     * - Linux  : no-op.
     *
    * @param dupHandleValue value returned by PrepareForPacket.
    * @param targetPid      PID passed to PrepareForPacket.
     */
    static void RevokeOnFailure(intptr_t dupHandleValue, uint32_t targetPid);

private:
    MemFDHandleTransfer() = default;
};

}  // namespace shm
}  // namespace dxrt
