/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include "dxrt/common.h"

#include <cstdint>
#include <string>
#include <memory>
#ifndef _WIN32
#include <sys/types.h>
#endif

namespace dxrt {
namespace shm {

/// Platform-neutral memory handle type.
/// Linux  : memfd_create() returns int fd -> stored as intptr_t.
/// Windows: CreateFileMapping() returns HANDLE(= void*) -> stored as intptr_t.
///          INVALID_HANDLE_VALUE == (HANDLE)(LONG_PTR)(-1) -> matches kInvalidMemFDHandle(-1).
using MemFDHandle = intptr_t;
static constexpr MemFDHandle kInvalidMemFDHandle = -1;

/**
 * @brief Platform-specific shared memory syscall adapter
 *
 * Linux  : based on memfd_create / mmap
 * Windows: based on CreateFileMapping / MapViewOfFile
 *
 * MemFDHandle holds a native handle value that can be transferred across processes.
 * (Linux: fd transferred via SCM_RIGHTS, Windows: HANDLE transferred via DuplicateHandle)
 */
class SharedMemorySyscallAdapter {
public:
    /**
    * @brief Create shared memory
    * @param size  memory size to allocate (bytes, 0 < size <= 1TB)
    * @param flags additional Linux memfd_create flags / ignored on Windows
    * @return MemFDHandle, or kInvalidMemFDHandle(-1) on failure
     */
    static MemFDHandle CreateMemFD(size_t size, int flags = 0);

    /**
    * @brief Validate a handle (received from another process)
    * @param fd MemFDHandle to validate
    * @return true if valid
     */
    static bool OpenMemFD(MemFDHandle fd);

    /**
    * @brief Map memory
     * @param fd     MemFDHandle
    * @param size   mapping size
    * @param offset mapping offset
    * @return mapped pointer, or nullptr on failure
     */
    static void* MapMemFD(MemFDHandle fd, size_t size, size_t offset = 0);

    /**
    * @brief Unmap memory
    * @param addr mapped address
    * @param size mapping size (required for munmap on Linux / ignored on Windows)
    * @return true on success
     */
    static bool UnmapMemFD(void* addr, size_t size);

    /**
    * @brief Close handle
     * @param fd MemFDHandle
    * @return true on success
     */
    static bool CloseMemFD(MemFDHandle fd);

    /**
    * @brief Resize memory
     * @param fd       MemFDHandle
    * @param new_size new size
    * @return true on success (Windows: unsupported, always false)
     */
    static bool ResizeMemFD(MemFDHandle fd, size_t new_size);

    /**
    * @brief Set seals (Linux: F_ADD_SEALS / Windows: no-op)
     * @param fd    MemFDHandle
    * @return true on success
     */
    static bool SealMemFD(MemFDHandle fd);

    /**
    * @brief Get current mapped size
     * @param fd MemFDHandle
    * @return size in bytes, or 0 on failure
     */
    static size_t GetMemFDSize(MemFDHandle fd);

    /**
    * @brief Sync CPU cache to main memory (call before NPU DMA reads)
    * @param addr mapped address
    * @param size size
    * @return true on success
     */
    static bool SyncMemory(void* addr, size_t size);

    /**
    * @brief Invalidate CPU cache (call before CPU reads after NPU DMA writes)
    * @param addr mapped address
    * @param size size
    * @return true on success
     */
    static bool InvalidateMemory(void* addr, size_t size);

private:
    SharedMemorySyscallAdapter() = default;
    ~SharedMemorySyscallAdapter() = default;
};

} // namespace shm
} // namespace dxrt
