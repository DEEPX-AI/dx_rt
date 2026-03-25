/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/common.h"
#include "dxrt/usage_timer.h"
#include<chrono>
#include<queue>
#include<iostream>


UsageTimer::UsageTimer()
{
    _prevTickTime = std::chrono::system_clock::now();
}

UsageTimer::~UsageTimer() = default;

void UsageTimer::onTick()
{
    std::lock_guard<std::mutex> lock(_mutex);
    auto nowTime = std::chrono::system_clock::now();
    auto interval = nowTime - _prevTickTime;
    _prevTickTime = nowTime;

    _usage = _usageDuration / static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(interval).count());

    _usageDuration = 0;
    _prevUsage = _usageCount;
    _usageCount = 0;
}

void UsageTimer::add(double value)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _usageDuration = value + _usageDuration;
    _usageCount++;
}

double UsageTimer::getUsage()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _usage;
}
