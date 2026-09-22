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
#include "dxrt/circular_buffer.h"

#include <mutex>
#include <cmath>
#include <iostream>

namespace dxrt {

class DXRT_API InferenceTimer
{
 public:
    InferenceTimer();
    void PushLatency(int latency);
    void PushQueueWaitTime(int64_t queue_wait_time);
    void PushInferenceTime(uint32_t inference_time);
    int latency();
    int64_t queue_wait_time();
    uint32_t inference_time();
    std::vector<int> GetLatencyVector();
    std::vector<int64_t> GetQueueWaitTimeVector();
    std::vector<uint32_t> GetNpuInferenceTimeVector();
    double GetLatencyMean() const;
    double GetLatencyStdDev() const;
    int GetLatencyCnt() const;
    double GetQueueWaitTimeMean() const;
    double GetQueueWaitTimeStdDev() const;
    int GetQueueWaitTimeCnt() const;
    double GetNpuInferenceTimeMean() const;
    double GetNpuInferenceTimeStdDev() const;
    int GetNpuInferenceTimeCnt() const;
    void UpdateLatencyStatistics(int latency);
    void UpdateQueueWaitTimeStatistics(int64_t queueWaitTime);
    void UpdateInferenceTimeStatistics(uint32_t inferenceTime);

 private:
    CircularBuffer<int> _latency;
    CircularBuffer<int64_t> _queueWaitTime;
    CircularBuffer<uint32_t> _infTime;
    double _latencyMean=0.0;
    double _latencyM2=0.0;
    int _latencyN=0;
    double _queueWaitTimeMean=0.0;
    double _queueWaitTimeM2=0.0;
    int _queueWaitTimeN=0;
    double _inferenceTimeMean=0.0;
    double _inferenceTimeM2=0.0;
    int _inferenceTimeN=0;
    mutable std::mutex _lock;

};

}  // namespace dxrt
