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
#include "dxrt/profiler.h"

#include <array>
#include <atomic>
#include <limits>
#include <map>
#include <mutex>
#include <string>
#include <vector>

namespace dxrt
{

class DXRT_INTERNAL_API UserEventStore
{
public:
    static UserEventStore& GetInstance();
    static bool HasInstance() { return _staticInstance != nullptr; }

    void Start(const std::string& eventName);
    void End(const std::string& eventName);
    void Clear();
    void Show() const;

    std::vector<UserEvent> GetEvents() const;

    void SetEnabled(bool enabled)
    {
        _enabled.store(enabled, std::memory_order_relaxed);
    }

private:
    struct StatsAccumulator
    {
        uint64_t count  = 0;
        double   min_us = (std::numeric_limits<double>::max)();
        double   max_us = 0.0;
        double   mean   = 0.0;
        double   M2     = 0.0;

        bool   Valid() const;
        void   Update(double us);
        double Variance() const;
        double StdDev() const;
        double CoV() const;
    };

    UserEventStore();
    ~UserEventStore() = default;

    UserEventStore(const UserEventStore&) = delete;
    UserEventStore& operator=(const UserEventStore&) = delete;
    UserEventStore(UserEventStore&&) = delete;
    UserEventStore& operator=(UserEventStore&&) = delete;

    friend class ObjectsPool;
    static UserEventStore* _staticInstance;
    static void deleteInstance();

    std::array<UserEvent, PROFILER_BUFFER_SIZE> _events;
    int _currentIdx = 0;
    int _count = 0;
    mutable std::mutex _lock;
    std::atomic<bool> _enabled{false};

    std::map<std::string, ProfilerClock::time_point> _pendingStarts;
    std::map<std::string, StatsAccumulator> _stats;
};

} // namespace dxrt
