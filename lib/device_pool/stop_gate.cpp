/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "stop_gate.hpp"


namespace dxrt
{

StopGate::Pass::Pass(StopGate* gate, bool active)
    : gate_(gate), active_(active)
{
}

StopGate::Pass::~Pass()
{
    Release();
}

StopGate::Pass::Pass(Pass&& other) noexcept
    : gate_(other.gate_), active_(other.active_)
{
    other.gate_ = nullptr;
    other.active_ = false;
}

StopGate::Pass& StopGate::Pass::operator=(Pass&& other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    Release();
    gate_ = other.gate_;
    active_ = other.active_;
    other.gate_ = nullptr;
    other.active_ = false;
    return *this;
}

void StopGate::Pass::Release()
{
    if (gate_ == nullptr || !active_)
    {
        return;
    }

    gate_->ReleasePass();
    gate_ = nullptr;
    active_ = false;
}

StopGate::~StopGate()
{
    Shutdown();
}

void StopGate::SetStop(bool stop)
{
    std::unique_lock<std::mutex> lock(mutex_);
    if (shutting_down_)
    {
        return;
    }

    if (!stop)
    {
        stop_flag_ = false;
        lock.unlock();
        condition_.notify_all();
        return;
    }

    // Block new pass acquisition, then wait for in-flight passes to drain.
    stop_flag_ = true;
    condition_.wait(lock, [this]() {
        return shutting_down_ || active_passes_ == 0;
    });
}

StopGate::Pass StopGate::WaitIfStopped()
{
    std::unique_lock<std::mutex> lock(mutex_);
    condition_.wait(lock, [this]() {
        return !stop_flag_ || shutting_down_;
    });

    if (shutting_down_)
    {
        return Pass();
    }

    ++active_passes_;
    return Pass(this, true);
}

void StopGate::Shutdown()
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        shutting_down_ = true;
        stop_flag_ = false;
    }

    condition_.notify_all();
}

void StopGate::ReleasePass()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (active_passes_ > 0)
    {
        --active_passes_;
    }

    if (active_passes_ == 0)
    {
        condition_.notify_all();
    }
}

bool StopGate::IsStopped() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return stop_flag_ && !shutting_down_;
}

}  // namespace dxrt
