/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * DXRT Wrapper — Profiler (prebuilt delivery).
 *
 * V2 API: event-based methods (Add/Start/End/Get/GetAverage) removed.
 * Use GetJobMetrics(jobId) after Wait() for per-job timing data.
 */

#pragma once
#include "dxrt/dxrt_c_api.h"
#include <string>

namespace dxrt {

class Profiler
{
public:
    static Profiler& GetInstance()
    {
        static Profiler instance;
        return instance;
    }

    /** Get per-job profiling metrics. Call after engine Wait(jobId) returns.
     *  Caller is responsible for calling dxrt_job_metrics_destroy(handle). */
    dxrt_job_metrics_t GetJobMetrics(int jobId)
    {
        dxrt_job_metrics_t jm = nullptr;
        dxrt_profiler_get_job_metrics(jobId, &jm);
        return jm;
    }

    void Show()
    {
        dxrt_profiler_show();
    }

    void Save(const std::string& file)
    {
        dxrt_profiler_save(file.c_str());
    }

    void Clear() const
    {
        dxrt_profiler_clear();
    }

    void Start(const std::string& name)
    {
        dxrt_profiler_user_start(name.c_str());
    }

    void End(const std::string& name)
    {
        dxrt_profiler_user_end(name.c_str());
    }

    void UserClear()
    {
        dxrt_profiler_user_clear();
    }

private:
    Profiler() = default;
    Profiler(const Profiler&) = delete;
    Profiler& operator=(const Profiler&) = delete;
};

} // namespace dxrt
