// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "usage_timer.h"
#include<chrono>
#include<queue>
#include<iostream>


UsageTimer::UsageTimer()
{
    _stop = false;
}

UsageTimer::~UsageTimer()
{

}

void UsageTimer::onTick()
{
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

    _usageDuration = value + _usageDuration;
    
    _usageCount++;
}

double UsageTimer::getUsage()
{
    return _usage;
}