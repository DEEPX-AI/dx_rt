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
#include "shared_memory_syscall_adapter.h"
#include <cstdint>
#ifdef __linux__
#include <unistd.h>  // for pid_t
#include <sys/types.h>
#elif _WIN32
// Windows: pid_t is not a standard type, so define it as int.
#include <process.h>
typedef int pid_t;
#endif

namespace dxrt {

struct SharedMemoryInfo {
    int64_t block_id;
    uint64_t size;
    void *ptr;
    uint64_t phys_addr_base;
    uint64_t phys_addr_offset;
    int deviceid;
    int taskId;
    pid_t pid;
    dxrt::shm::MemFDHandle fd;

    uint64_t phys_addr() const
    {
        return phys_addr_base + phys_addr_offset;
    }

    void set_phys_addr(uint64_t addr)
    {
        phys_addr_base = 0;
        phys_addr_offset = addr;
    }
};

// A bounded, offset view into a SharedMemoryInfo block.
// Carries (info, offset, size) so callers never have to pass the triplet separately.
struct SharedMemoryView {
    SharedMemoryInfo info;
    uint64_t offset = 0;  // byte offset into info
    uint64_t size   = 0;  // byte count for this view

    // Absolute device-side physical address for this view.
    uint64_t deviceAddress() const { return info.phys_addr() + offset; }

    // Host-side virtual pointer for this view (nullptr when info.ptr is null).
    void* hostPtr() const
    {
        if (info.ptr == nullptr) return nullptr;
        return static_cast<uint8_t *>(info.ptr) + offset;
    }

    bool isValid() const
    {
        return size > 0 && offset <= info.size && size <= (info.size - offset);
    }

    // Create a view that covers the entire SharedMemoryInfo block.
    static SharedMemoryView ofWhole(const SharedMemoryInfo& i)
    {
        SharedMemoryView v;
        v.info    = i;
        v.offset  = 0;
        v.size    = i.size;
        return v;
    }
};

}  // namespace dxrt
