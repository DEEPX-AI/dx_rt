// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/inference_timer.h"

namespace dxrt {



InferenceTimer::InferenceTimer()
: _latency(30), _infTime(30)
{

}

void InferenceTimer::PushLatency(int latency)
{
    _latency.Push(latency);
}
void InferenceTimer::PushInferenceTime(uint32_t inference_time)
{
    _infTime.Push(inference_time);
}
int InferenceTimer::latency()
{
    if (_latency.IsEmpty())
        return 0;
    return _latency.Get();
}
uint32_t InferenceTimer::inference_time()
{
    if (_infTime.IsEmpty())
        return 0;
    return _infTime.Get();
}

}  // namespace dxrt
