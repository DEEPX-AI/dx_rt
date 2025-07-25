// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "dxrt/common.h"
#include "dxrt/tensor.h"
#include "dxrt/task_data.h"
#include <memory>
#include <vector>
#include <string>

namespace dxrt {

class RequestData
{
public:
    int requestId;
    int jobId;
    TaskData* taskData;

    Tensors inputs;
    Tensors outputs;

    void* outputs_ptr;

    void* encoded_inputs_ptr;
    void* encoded_outputs_ptr;

    std::vector<void*> encoded_input_ptrs;
    std::vector<void*> encoded_output_ptrs;

    std::string _processedPU;
    int _processedDevId;
    int _processedId;

    void BuildEncodedInputPtrs(const std::vector<uint64_t>& offsets) {
        encoded_input_ptrs.clear(); 
        if (encoded_inputs_ptr) {
            for (uint64_t offset : offsets) {
                encoded_input_ptrs.push_back(static_cast<uint8_t*>(encoded_inputs_ptr) + offset);
            }
        }
    }

    void BuildEncodedOutputPtrs(const std::vector<uint64_t>& offsets) {
        encoded_output_ptrs.clear(); 
        if (encoded_outputs_ptr) {
            for (uint64_t offset : offsets) {
                encoded_output_ptrs.push_back(static_cast<uint8_t*>(encoded_outputs_ptr) + offset);
            }
        }
    }
};

}   // namespace dxrt