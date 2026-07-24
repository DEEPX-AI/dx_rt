/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <cstddef>
#include <condition_variable>
#include <mutex>

namespace dxrt
{

class StopGate
{
 public:
    class Pass
    {
     public:
        Pass() = default;
        ~Pass();

        Pass(Pass&& other) noexcept;
        Pass& operator=(Pass&& other) noexcept;

        Pass(const Pass&) = delete;
        Pass& operator=(const Pass&) = delete;

     private:
        friend class StopGate;
        explicit Pass(StopGate* gate, bool active);
        void Release();

        StopGate* gate_ = nullptr;
        bool active_ = false;
    };

    StopGate() = default;
    ~StopGate();

    StopGate(const StopGate&) = delete;
    StopGate& operator=(const StopGate&) = delete;

    // If stop is true, callers will block in WaitIfStopped until resumed.
    void SetStop(bool stop);

    // Pass through immediately when not stopped and return a scoped pass object.
    // While any pass object is alive, SetStop(true) blocks until they are released.
    Pass WaitIfStopped();

    // Unblocks waiters and marks the gate as shutting down.
    void Shutdown();

    bool IsStopped() const;

 private:
    void ReleasePass();

    mutable std::mutex mutex_;
    std::condition_variable condition_;
    bool stop_flag_ = false;
    bool shutting_down_ = false;
    std::size_t active_passes_ = 0;
};

}  // namespace dxrt
