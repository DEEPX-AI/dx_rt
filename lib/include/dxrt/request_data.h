// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "dxrt/common.h"
#include "dxrt/tensor.h"
#include "dxrt/task_data.h"
#include <memory>


namespace dxrt {



class RequestData
{
public:
    int requestId;
    int jobId;
    Tensors inputs;
    Tensors outputs;
    TaskData* taskData;
    void* output_ptr;

    std::string _processedPU;
    int _processedDevId;
    int _processedId;
};

}  // namespace dxrt
