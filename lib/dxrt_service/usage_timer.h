/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/common.h"

#include<chrono>
#include<queue>
#include<atomic>



class UsageTimer
{
 public:
    void onTick();
    double getUsage();
    void add(double value);
    UsageTimer();
    ~UsageTimer();
 private:
    std::chrono::system_clock::time_point _prevTickTime;
    std::atomic<double> _usage = {0};
    std::atomic<double> _usageDuration;
    std::atomic<bool> _stop = {false};

    int _usageCount;
    int _prevUsage;

};
