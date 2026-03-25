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

#include<chrono>
#include<queue>
#include<mutex>



class DXRT_API UsageTimer
{
 public:
    void onTick();
    double getUsage();
    void add(double value);
    UsageTimer();
    ~UsageTimer();
 private:
    std::chrono::system_clock::time_point _prevTickTime;
    double _usage = 0.0;
    double _usageDuration = 0.0;

    int _usageCount = 0;
    int _prevUsage = 0;
    std::mutex _mutex;

};
