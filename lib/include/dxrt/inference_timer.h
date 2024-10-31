// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "dxrt/common.h"
#include "dxrt/circular_buffer.h"

namespace dxrt {

class InferenceTimer
{
 public:
    InferenceTimer();
    void PushLatency(int latency);
    void PushInferenceTime(uint32_t inference_time);
    int latency();
    uint32_t inference_time();

 private:
    CircularBuffer<int> _latency;
    CircularBuffer<uint32_t> _infTime;
};

}  // namespace dxrt
