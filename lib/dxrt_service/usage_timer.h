// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

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
