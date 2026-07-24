/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

// Layer 3: inference_context.cpp
// Implements BuildDriverRequest() — the single point where a dxrt_request_acc_t
// is assembled from a TaskStaticConfig and an InferenceSlimRequest.

#include "inference_context.h"
#include "dxrt/util.h"

#include <cstring>

#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif

namespace dxrt {

#ifdef _WIN32
uint32_t current_pid() { return static_cast<uint32_t>(_getpid()); }
#else
uint32_t current_pid() { return static_cast<uint32_t>(getpid()); }
#endif


dxrt_request_acc_t BuildDriverRequest(
    const TaskStaticConfig&     config,
    const InferenceSlimRequest& slim)
{
    dxrt_request_acc_t acc{};

    // -----------------------------------------------------------------------
    // Identity
    // -----------------------------------------------------------------------
    acc.task_id = slim.task_id;
    acc.req_id  = slim.req_id;

    // -----------------------------------------------------------------------
    // Input memory
    // -----------------------------------------------------------------------
    acc.input.data   = slim.input_host_addr;
    acc.input.base   = config.memory_base;
    acc.input.offset = static_cast<uint32_t>(slim.input_device_offset);
    acc.input.size   = config.input_size;

    // -----------------------------------------------------------------------
    // Output memory
    //
    // The output staging area follows the (aligned) input staging area inside
    // the same cache slice, then the last-output tensor is at a further offset.
    //
    //   [  input_size (aligned to 64)  |  ...  | last_output_offset | last_output ]
    //   ^                                                             ^
    //   input_device_offset                                          output.offset
    //
    // Legacy models (output_all_offset != 0) use a pre-computed delta instead
    // of the aligned input size.
    // -----------------------------------------------------------------------
    const uint32_t aligned_input = static_cast<uint32_t>(data_align(config.input_size, 64));
    const uint32_t output_delta  = (config.output_all_offset != 0)
                                       ? config.output_all_offset
                                       : aligned_input;

    acc.output.data   = slim.output_host_addr;
    acc.output.base   = config.memory_base;
    acc.output.offset = static_cast<uint32_t>(slim.input_device_offset)
                      + output_delta
                      + config.last_output_offset;
    acc.output.size   = config.output_size;

    // -----------------------------------------------------------------------
    // Model metadata (all static)
    // -----------------------------------------------------------------------
    acc.model_type    = config.model_type;
    acc.model_format  = config.model_format;
    acc.model_cmds    = config.model_cmds;
    acc.cmd_offset    = config.cmd_offset;
    acc.weight_offset = config.weight_offset;
    acc.custom_offset = config.custom_offset;
    acc.op_mode       = config.op_mode;

    for (int i = 0; i < MAX_CHECKPOINT_COUNT; ++i) {
        acc.datas[i] = config.datas[i];
    }

    // -----------------------------------------------------------------------
    // Runtime fields
    //   proc_id: service-ON will overwrite with the client pid received over IPC.
    //            service-OFF uses current process.
    //   bound / scheduler options: come from InferenceSlimRequest.
    // -----------------------------------------------------------------------
    acc.proc_id     = current_pid();
    acc.bound       = slim.bound;
    acc.prior       = slim.prior;
    acc.prior_level = slim.prior_level;
    acc.bandwidth   = slim.bandwidth;
    acc.queue       = slim.queue;

    return acc;
}

}  // namespace dxrt
