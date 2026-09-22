/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/common.h"
#include "dxrt/inference_timer.h"

namespace dxrt {

InferenceTimer::InferenceTimer()
: _latency(30), _queueWaitTime(30), _infTime(30)  // NOSONAR:S3230
{

}

void InferenceTimer::PushLatency(int latency)
{
    _latency.Push(latency);
}
void InferenceTimer::PushQueueWaitTime(int64_t queue_wait_time)
{
    _queueWaitTime.Push((std::max)(static_cast<int64_t>(0), queue_wait_time));
}

void InferenceTimer::PushInferenceTime(uint32_t inference_time)
{
    _infTime.Push(inference_time);
}

int InferenceTimer::latency()
{
    std::unique_lock<std::mutex> lk(_lock);
    if (_latency.IsEmpty())
        return 0;
    return _latency.Get();
}

int64_t InferenceTimer::queue_wait_time()
{
    std::unique_lock<std::mutex> lk(_lock);
    if (_queueWaitTime.IsEmpty())
        return 0;
    return _queueWaitTime.Get();
}

uint32_t InferenceTimer::inference_time()
{
    std::unique_lock<std::mutex> lk(_lock);
    if (_infTime.IsEmpty())
        return 0;
    return _infTime.Get();
}

std::vector<int> InferenceTimer::GetLatencyVector()
{
    std::unique_lock<std::mutex> lk(_lock);
    if (_latency.IsEmpty())
        return {};
    return _latency.ToVector();
}

std::vector<int64_t> InferenceTimer::GetQueueWaitTimeVector()
{
    std::unique_lock<std::mutex> lk(_lock);
    if (_queueWaitTime.IsEmpty())
        return {};
    return _queueWaitTime.ToVector();
}

std::vector<uint32_t> InferenceTimer::GetNpuInferenceTimeVector()
{
    std::unique_lock<std::mutex> lk(_lock);
    if (_infTime.IsEmpty())
        return {};
    return _infTime.ToVector();
}

void InferenceTimer::UpdateLatencyStatistics(int latency) {
    std::unique_lock<std::mutex> lk(_lock);
    _latencyN++;
    double delta = latency - _latencyMean;
    _latencyMean += delta / _latencyN;
    double delta2 = latency - _latencyMean;
    _latencyM2 += delta * delta2;
}

void InferenceTimer::UpdateQueueWaitTimeStatistics(int64_t queueWaitTime) {
    std::unique_lock<std::mutex> lk(_lock);
    queueWaitTime = (std::max)(static_cast<int64_t>(0), queueWaitTime);
    _queueWaitTimeN++;
    double delta = static_cast<double>(queueWaitTime) - _queueWaitTimeMean;
    _queueWaitTimeMean += delta / _queueWaitTimeN;
    double delta2 = static_cast<double>(queueWaitTime) - _queueWaitTimeMean;
    _queueWaitTimeM2 += delta * delta2;
}

void InferenceTimer::UpdateInferenceTimeStatistics(uint32_t inferenceTime) {
    std::unique_lock<std::mutex> lk(_lock);
    _inferenceTimeN++;
    double delta = inferenceTime - _inferenceTimeMean;
    _inferenceTimeMean += delta / _inferenceTimeN;
    double delta2 = inferenceTime - _inferenceTimeMean;
    _inferenceTimeM2 += delta * delta2;
}

double InferenceTimer::GetLatencyMean() const{
    std::unique_lock<std::mutex> lk(_lock);
    return (_latencyN > 0) ? _latencyMean : 0.0;
}

double InferenceTimer::GetLatencyStdDev() const{
    std::unique_lock<std::mutex> lk(_lock);
    return (_latencyN > 1) ? std::sqrt(_latencyM2 / (_latencyN - 1)) : 0.0;
}

int InferenceTimer::GetLatencyCnt() const{
    std::unique_lock<std::mutex> lk(_lock);
    return _latencyN;
}

double InferenceTimer::GetQueueWaitTimeMean() const{
    std::unique_lock<std::mutex> lk(_lock);
    return (_queueWaitTimeN > 0) ? _queueWaitTimeMean : 0.0;
}

double InferenceTimer::GetQueueWaitTimeStdDev() const{
    std::unique_lock<std::mutex> lk(_lock);
    return (_queueWaitTimeN > 1) ? std::sqrt(_queueWaitTimeM2 / (_queueWaitTimeN - 1)) : 0.0;
}

int InferenceTimer::GetQueueWaitTimeCnt() const{
    std::unique_lock<std::mutex> lk(_lock);
    return _queueWaitTimeN;
}

double InferenceTimer::GetNpuInferenceTimeMean() const{
    std::unique_lock<std::mutex> lk(_lock);
    return (_inferenceTimeN > 0) ? _inferenceTimeMean : 0.0;
}

double InferenceTimer::GetNpuInferenceTimeStdDev() const{
    std::unique_lock<std::mutex> lk(_lock);
    return (_inferenceTimeN > 1) ? std::sqrt(_inferenceTimeM2 / (_inferenceTimeN - 1)) : 0.0;
}

int InferenceTimer::GetNpuInferenceTimeCnt() const{
    std::unique_lock<std::mutex> lk(_lock);
    return _inferenceTimeN;
}

}  // namespace dxrt
