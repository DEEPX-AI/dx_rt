/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

// Layer 3: Detail-value and memory-translation layer
//
// Defines the minimum data structures needed to describe a task (TaskStaticConfig)
// and a single inference request (InferenceSlimRequest), together with the shared
// BuildDriverRequest() function that assembles a full dxrt_request_acc_t from them.
//
// Key property: this unit is intentionally side-effect-free and carries no
// IPC or driver knowledge.  It is compiled into both:
//   - libdxrt (service-OFF path: DeviceTaskLayer calls BuildDriverRequest directly)
//   - dxrtd   (service-ON path:  service daemon calls BuildDriverRequest after
//               receiving an InferenceSlimRequest from the client)

#include "dxrt/common.h"
#include "dxrt/driver.h"

#include <cstdint>

namespace dxrt {

// ---------------------------------------------------------------------------
// TaskStaticConfig
//
// All fields that are determined once at RegisterTask and never change
// during the lifetime of the task.  Stored per-taskId in DeviceTaskLayer;
// sent to the service daemon on service-ON paths.
// ---------------------------------------------------------------------------
struct TaskStaticConfig {
    int8_t   model_type         = 0;
    int8_t   model_format       = 0;
    uint32_t model_cmds         = 0;
    uint32_t op_mode            = 0;

    uint32_t cmd_offset         = 0;  ///< rmap device offset (post-allocation)
    uint32_t weight_offset      = 0;  ///< weight device offset (post-allocation)
    uint32_t custom_offset      = 0;  ///< PPU binary device offset (0 for non-PPU)

    uint32_t input_size         = 0;  ///< encoded_input_size (bytes, fixed per model)
    uint32_t output_size        = 0;  ///< last_output_size (bytes)
    uint32_t last_output_offset = 0;  ///< offset of last output tensor within output region
    uint32_t output_all_offset  = 0;  ///< non-zero only for legacy models with non-contiguous I/O

    uint32_t datas[MAX_CHECKPOINT_COUNT] = {};

    /// Device-side base address used in dxrt_meminfo_t::base.
    ///   service-ON  → 0         (driver receives absolute offsets)
    ///   service-OFF → mem_addr  (driver receives offsets relative to base)
    uint64_t memory_base        = 0;
};

// ---------------------------------------------------------------------------
// InferenceSlimRequest
//
// Per-inference runtime state — the minimum needed to fill in the dynamic
// fields of a dxrt_request_acc_t.  Combined with a TaskStaticConfig by
// BuildDriverRequest().
// ---------------------------------------------------------------------------
struct InferenceSlimRequest {
    uint32_t req_id              = 0;
    uint32_t task_id             = 0;

    /// Host-side source/destination pointers.
    ///   non-VNPU: virtual address   (process_vm_readv/writev on service-ON)
    ///   VNPU:     physical address  (used directly by driver)
    uint64_t input_host_addr     = 0;
    uint64_t output_host_addr    = 0;

    /// Absolute device-side offset where input staging begins.
    /// Equals cacheSlice.deviceAddress() = phys_addr_offset + slice_start.
    uint64_t input_device_offset = 0;

    // Scheduler options
    uint32_t bound               = 0;
    uint32_t prior               = 0;
    uint32_t prior_level         = 0;
    uint32_t bandwidth           = 0;
    uint32_t queue               = 0;
};

// ---------------------------------------------------------------------------
// InferenceContext
//
// Per-task device-side inference state.  Populated at TaskInit from the data
// carried by IPCPacketTaskInitRequest, and optionally extended after the
// per-task I/O staging buffer is allocated on the device (e.g. via
// HandleAllocate).
//
// Stored in ProcessTaskInfoStore.  Provides everything needed to assemble the
// static fields of a dxrt_request_acc_t via BuildDriverRequest(), and to
// fill in the input/output meminfo once the I/O buffer address is known.
//
// Note: InferenceSlimRequest (per-inference runtime data) is separate and
// unrelated to this struct.
// ---------------------------------------------------------------------------
struct InferenceContext {
    TaskStaticConfig config{};           ///< Model metadata — set at TaskInit.

    // Device I/O staging buffer — populated after the per-task buffer is
    // allocated (io_buffer_size == 0 means not yet assigned).
    uint64_t io_phys_addr_base   = 0;    ///< Physical base of the device memory region (mem_addr).
    uint64_t io_phys_addr_offset = 0;    ///< Buffer offset within that region.
    uint32_t io_buffer_size      = 0;    ///< Allocated size in bytes (input + output, aligned).
};

// ---------------------------------------------------------------------------
// BuildDriverRequest  (Layer 3 entry point)
//
// Assembles a complete dxrt_request_acc_t that can be handed directly to
// DeviceCore (service-OFF) or forwarded to the service daemon (service-ON).
//
// Output-offset calculation:
//   output.offset = input_device_offset
//                 + (output_all_offset != 0 ? output_all_offset : align64(input_size))
//                 + last_output_offset
// ---------------------------------------------------------------------------
dxrt_request_acc_t BuildDriverRequest(
    const TaskStaticConfig&     config,
    const InferenceSlimRequest& slim);

uint32_t current_pid();

}  // namespace dxrt
