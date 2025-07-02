// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "dxrt/common.h"
#include "dxrt/circular_buffer.h"

#include <mutex>
#include <cmath>
#include <iostream>

namespace dxrt {

class InferenceTimer
{
 public:
    InferenceTimer();
    void PushLatency(int latency);
    void PushInferenceTime(uint32_t inference_time);
    int latency();
    uint32_t inference_time();
    std::vector<int> GetLatencyVector();
    std::vector<uint32_t> GetNpuInferenceTimeVector();
    double GetLatencyMean() const;
    double GetLatencyStdDev() const;
    int GetLatencyCnt() const;
    double GetNpuInferenceTimeMean() const;
    double GetNpuInferenceTimeStdDev() const;
    int GetNpuInferenceTimeCnt() const;
    void UpdateLatencyStatistics(int latency);
    void UpdateInferenceTimeStatistics(uint32_t inferenceTime);

 private:
    CircularBuffer<int> _latency;
    CircularBuffer<uint32_t> _infTime;
    double _latencyMean=0.0;
    double _latencyM2=0.0;
    int _latencyN=0;
    double _inferenceTimeMean=0.0;
    double _inferenceTimeM2=0.0;
    int _inferenceTimeN=0;
    mutable std::mutex _lock;

};

}  // namespace dxrt
