/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/user_event_store.h"

#include <cmath>
#include <iomanip>
#include <iostream>

using std::cout;
using std::endl;
using std::setw;
using std::string;

namespace dxrt
{

UserEventStore* UserEventStore::_staticInstance = nullptr;

UserEventStore& UserEventStore::GetInstance()
{
    if (_staticInstance == nullptr)
    {
        _staticInstance = new UserEventStore();
    }

    return *_staticInstance;
}

void UserEventStore::deleteInstance()
{
    if (_staticInstance != nullptr)
    {
        delete _staticInstance;
    }

    _staticInstance = nullptr;
}

UserEventStore::UserEventStore()
    : _enabled(false)
{
}

// ── StatsAccumulator ──

bool UserEventStore::StatsAccumulator::Valid() const
{
    return count > 0;
}

void UserEventStore::StatsAccumulator::Update(double us)
{
    if (us <= 0.0)
    {
        return;
    }

    count++;
    if (us < min_us)
    {
        min_us = us;
    }
    if (us > max_us)
    {
        max_us = us;
    }

    double delta = us - mean;
    mean += delta / static_cast<double>(count);
    M2 += delta * (us - mean);
}

double UserEventStore::StatsAccumulator::Variance() const
{
    return (count > 1) ? M2 / static_cast<double>(count - 1) : 0.0;
}

double UserEventStore::StatsAccumulator::StdDev() const
{
    return std::sqrt(Variance());
}

double UserEventStore::StatsAccumulator::CoV() const
{
    return (mean > 0.0) ? (StdDev() / mean * 100.0) : 0.0;
}

// ── UserEventStore ──

void UserEventStore::Start(const string& eventName)
{
    if (!_enabled)
    {
        return;
    }

    std::lock_guard<std::mutex> lk(_lock);

    auto it = _pendingStarts.find(eventName);
    if (it != _pendingStarts.end())
    {
        LOG_DXRT_WARN("UserEventStore: Start(\"" << eventName
                      << "\") called without End() for previous Start. Overwriting.");
    }

    _pendingStarts[eventName] = ProfilerClock::now();
}

void UserEventStore::End(const string& eventName)
{
    if (!_enabled)
    {
        return;
    }

    std::lock_guard<std::mutex> lk(_lock);

    auto it = _pendingStarts.find(eventName);
    if (it == _pendingStarts.end())
    {
        LOG_DXRT_WARN("UserEventStore: End(\"" << eventName
                      << "\") called without matching Start. Ignoring.");
        return;
    }

    const auto end_time = ProfilerClock::now();
    const auto start_time = it->second;
    _pendingStarts.erase(it);

    _events[_currentIdx].name = eventName;
    _events[_currentIdx].tp.start = start_time;
    _events[_currentIdx].tp.end = end_time;
    _currentIdx = (_currentIdx + 1) % PROFILER_BUFFER_SIZE;
    if (_count < PROFILER_BUFFER_SIZE)
    {
        _count++;
    }

    const double duration_us =
        std::chrono::duration<double, std::micro>(end_time - start_time).count();
    _stats[eventName].Update(duration_us);
}

void UserEventStore::Clear()
{
    std::lock_guard<std::mutex> lk(_lock);

    _events.fill(UserEvent{});
    _currentIdx = 0;
    _count = 0;
    _pendingStarts.clear();
    _stats.clear();
}

std::vector<UserEvent> UserEventStore::GetEvents() const
{
    std::lock_guard<std::mutex> lk(_lock);

    std::vector<UserEvent> result;
    result.reserve(_count);

    if (_count < PROFILER_BUFFER_SIZE)
    {
        for (int i = 0; i < _count; ++i)
        {
            result.push_back(_events[i]);
        }
    }
    else
    {
        for (int i = 0; i < PROFILER_BUFFER_SIZE; ++i)
        {
            const int idx = (_currentIdx + i) % PROFILER_BUFFER_SIZE;
            result.push_back(_events[idx]);
        }
    }

    return result;
}

void UserEventStore::Show() const
{
    if (!_enabled)
    {
        return;
    }

    std::lock_guard<std::mutex> lk(_lock);
    if (_stats.empty())
    {
        return;
    }

    cout << "  ==============================================================================================" << endl;
    cout << "  | User Events" << setw(81) << "|" << endl;
    cout << "  ==============================================================================================" << endl;
    cout << "  ----------------------------------------------------------------------------------------------" << endl;
    cout << "  | " << std::left << setw(30) << "Name" << std::right
         << " | " << setw(12) << "min (us)"
         << " | " << setw(12) << "max (us)"
         << " | " << setw(12) << "average (us)"
         << " | " << setw(12) << "CoV (%)"
         << " |" << endl;
    cout << "  ----------------------------------------------------------------------------------------------" << endl;

    for (const auto& pair : _stats)
    {
        if (!pair.second.Valid())
        {
            continue;
        }

        const auto& acc = pair.second;
        cout << std::fixed << std::setprecision(1);
        cout << "  | " << std::left << setw(30) << pair.first << std::right
             << " | " << setw(12) << acc.min_us
             << " | " << setw(12) << acc.max_us
             << " | " << setw(12) << acc.mean
             << " | " << setw(12) << acc.CoV()
             << " |" << endl;
        cout << std::defaultfloat;
    }

    cout << "  ----------------------------------------------------------------------------------------------" << endl;
}

} // namespace dxrt
