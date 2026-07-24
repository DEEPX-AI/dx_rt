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
#include "dxrt/configuration.h"

#include <string>
#include <chrono>
#include <vector>
#include <array>
#include <cmath>
#include <limits>
#include <map>
#include <mutex>


#ifdef USE_PROFILER
static constexpr int PROFILER_DEFAULT_SAMPLES = 2000;
#else
static constexpr int PROFILER_DEFAULT_SAMPLES = 0;
#endif

// Compile-time buffer size: always >= 1 to avoid zero-size std::array UB.
// When USE_PROFILER is not defined, PROFILER_DEFAULT_SAMPLES == 0 and
// _enabled == false, so buffer slots are never accessed in practice.
static constexpr int PROFILER_BUFFER_SIZE = (PROFILER_DEFAULT_SAMPLES > 0) ? PROFILER_DEFAULT_SAMPLES : 1;

namespace dxrt {

    /** \brief Clock used by the profiler. */
    using ProfilerClock = std::chrono::steady_clock;

    /** \brief Start/end timestamps for a measured profiler event. */
    struct DXRT_API TimePoint
    {
        ProfilerClock::time_point start;
        ProfilerClock::time_point end;
    };
    using TimePointPtr = std::shared_ptr<TimePoint>;

    /** \brief A named user profiling event with start/end timestamps.
     *  Internal type used by Profiler::Save() to merge user events into the output.
     *  Not part of the public SDK API. */
    struct UserEvent
    {
        std::string name;
        TimePoint   tp;
    };

    /** \brief Timing breakdown for one NPU device within a task. */
    struct DXRT_API NpuDeviceMetrics
    {
        double input_format_us   = 0.0;   ///< NPU input format handler time in microseconds
        double h2d_us            = 0.0;   ///< Host-to-Device transfer time in microseconds
        double inference_core_all_us = 0.0;   ///< NPU inference time aggregated across all cores (us)
        double inference_core_0_us   = 0.0;   ///< NPU inference time on core 0 (us)
        double inference_core_1_us   = 0.0;   ///< NPU inference time on core 1 (us)
        double inference_core_2_us   = 0.0;   ///< NPU inference time on core 2 (us)
        double d2h_us            = 0.0;   ///< Device-to-Host transfer time in microseconds
        double output_format_us  = 0.0;   ///< NPU output format handler time in microseconds
        double total_us          = 0.0;   ///< End-to-end NPU task time in microseconds (H2D + inference + D2H + overhead)
        bool   valid             = false;  ///< true if at least one of H2D/NPU/D2H was measured
        bool   task_measured  = false; ///< true if NPU_TASK_TOTAL event was recorded (distinguishes
                                       ///<  "not measured" from a genuine total_us == 0)

        /** \\brief Returns true if total_us >= sum of components (within 10% tolerance).
         *  Returns true unconditionally if NPU_TASK_TOTAL was not measured (task_measured == false). */
        bool IsConsistent() const
        {
            if (!task_measured) return true;  // @no_else: NPU_TASK_TOTAL not recorded, nothing to compare
            if (total_us <= 0.0) return true;  // @no_else: zero duration, nothing to compare
            return total_us >= (h2d_us + inference_core_all_us + d2h_us) * 0.9;
        }
    };

    /** \brief Metrics for one registered task (NPU task or CPU task).
     *
     * \li NPU task: `devices` is populated; `cpu_task_us` is always 0
     *              (per-device NPU_TASK_TOTAL duration is in `devices[devId].total_us`).
     * \li CPU task: `cpu_task_us` is the CPU execution duration; `devices` is empty.
     */
    struct DXRT_API TaskMetrics
    {
        std::string                    task_name;
        std::map<int, NpuDeviceMetrics> devices;   ///< deviceId → per-device NPU metrics (NPU tasks only)
        double                         cpu_task_us = 0.0; ///< CPU task execution duration in microseconds.
                                                          ///< Non-zero only for CPU tasks; always 0 for NPU tasks.
        bool                           valid  = false;
    };

    /** \brief All task metrics for one job, returned by Profiler::GetJobMetrics().
     *
     * One entry per registered task that participated in the job.
     * Example: a YoloV7 job produces tasks = {"npu_0", "cpu_0", "cpu_1", ..., "cpu_8"}.
     */
    struct DXRT_API JobMetrics
    {
        std::vector<TaskMetrics> tasks; ///< One TaskMetrics per task (sorted by task name)
        bool valid = false;

        /** \brief Safe lookup by task name; returns valid=false if not found. */
        TaskMetrics GetTask(const std::string& name) const
        {
            for (const auto& t : tasks)
                if (t.task_name == name) return t;
            return TaskMetrics{};
        }
    };

    /** \brief Runtime profiler for per-job inference timing.
     *
     * Typical usage:
     * \code
     * auto& profiler = dxrt::Profiler::GetInstance();
     * int jobId = engine.RunAsync(input);
     * auto outputs = engine.Wait(jobId);
     * auto metrics = profiler.GetJobMetrics(jobId);
     * \endcode
     *
     * All public duration values are reported in microseconds.
     * \headerfile "dxrt/dxrt_api.h"
    */
    class DXRT_API Profiler
    {
     public:
        /** \brief Profiler event identifiers.
         *
         * Most applications should use GetJobMetrics() instead of recording
         * individual events directly.
         *
         * \note When adding a new EventType, update the following:
         *   1. EventTypeToString() in profiler.cpp - Add case for string conversion
         *   2. PrintDeviceSection()/PrintCpuSection() in profiler.cpp - Add to display list if needed
         *   3. Determine placement: before _PUBLIC_END (public API) or after (SDK internal)
         *   4. Update GetPerformanceDataByDevice() if the sentinel logic changes
         *   5. Consider if NpuDeviceMetrics or TaskMetrics struct needs new fields
         *   6. Update InferenceCoreEventType() if core count changes
         *
         * \note Value ranges (following common SDK patterns like Vulkan, Windows API):
         *   - Public events:   0-999   (allows up to 1000 public events)
         *   - _PUBLIC_END:     999     (sentinel for filtering)
         *   - Internal events: 1000+   (SDK implementation details)
         *   - UNKNOWN:         9999    (error/fallback value)
         *
         * \note _PUBLIC_END sentinel: Events with value < _PUBLIC_END are exposed via GetPerformanceData().
         *       Events with value >= _PUBLIC_END are SDK-internal and filtered out from public API.
         */
        enum class EventType : int
        {
            // ── Public Events (0-999 range: exposed via GetPerformanceData()) ─
            NPU_INPUT_FORMAT     = 0,    ///< Input preprocessing (nfh_layer.cpp)
            H2D                  = 1,    ///< Host-to-Device DMA      → NpuDeviceMetrics::h2d_us
            INFERENCE_CORE_ALL   = 2,    ///< NPU inference on all cores → NpuDeviceMetrics::inference_core_all_us
            INFERENCE_CORE_0     = 3,    ///< NPU inference on core 0 → NpuDeviceMetrics::inference_core_0_us
            INFERENCE_CORE_1     = 4,    ///< NPU inference on core 1 → NpuDeviceMetrics::inference_core_1_us
            INFERENCE_CORE_2     = 5,    ///< NPU inference on core 2 → NpuDeviceMetrics::inference_core_2_us
            D2H                  = 6,    ///< Device-to-Host DMA      → NpuDeviceMetrics::d2h_us
            NPU_OUTPUT_FORMAT    = 7,    ///< Output postprocessing (nfh_layer.cpp)
            NPU_TASK_TOTAL       = 8,    ///< Total NPU task time      → NpuDeviceMetrics::total_us
            CPU_TASK_TOTAL       = 9,    ///< CPU task execution time (ORT session.Run()) → TaskMetrics::cpu_task_us
            // Add new public events here (values 10-998)

            _PUBLIC_END          = 999,  ///< Sentinel: values < 999 are public; >= 999 are SDK-internal

            // ── SDK Internal Events (1000+ range: not exposed via GetPerformanceData()) ─
            BUFFER_POOL_WAIT     = 1000, ///< Waiting to acquire a slot from the internal buffer pool (AcquireAllBuffers)
            FRAMEWORK_OVERHEAD   = 1001, ///< SDK response handling delay
            CPU_DISPATCH_WAIT    = 1002, ///< CPU task scheduler queue delay (not execution)
            SERVICE_PROCESS_WAIT = 1003, ///< Service thread wait for NPU response
            // Add new internal events here (values 1004-9998)

            UNKNOWN              = 9999  ///< Error/fallback value
        };

     private:

        Profiler();
        ~Profiler();

        // Delete copy constructor and assignment operator
        Profiler(const Profiler&) = delete;
        Profiler& operator=(const Profiler&) = delete;
        Profiler(Profiler&&) = delete;
        Profiler& operator=(Profiler&&) = delete;

        friend class ObjectsPool;
        static Profiler* _staticInstance;
        static void deleteInstance();

     public:
        /** \brief Get pre-created instance. (Don't create your own instance.)
         * \code
         * auto& profiler = dxrt::Profiler::GetInstance();
         * \endcode
         * \return Singleton instance of dxrt::Profiler
        */
        static Profiler& GetInstance();


        /** \internal
         * \brief Add measured timing data.
         * For internal SDK use only. Called by AccDeviceTaskLayer and RequestResponse
         * to record pre-measured TimePoint spans. External callers should use GetJobMetrics().
         * \param[in] type     event type
         * \param[in] taskName task name
         * \param[in] deviceId device ID
         * \param[in] jobId    job ID
         * \param[in] tp       timing data
        */
        void AddTimePoint(EventType type, const std::string& taskName, int deviceId, int jobId, TimePointPtr tp);
        /** \internal
         * \brief Record the start point of an event.
         * For internal SDK use only. Called by AccDeviceTaskLayer and CpuHandle
         * to mark the beginning of a timed region. External callers should use GetJobMetrics().
         * \param[in] type      event type
         * \param[in] taskName  task name (e.g. req->task()->name())
         * \param[in] deviceId  device ID (NPU device ID; use CPU_DEVICE_ID for CPU tasks)
         * \param[in] jobId     job ID
        */
        void Start(EventType type, const std::string& taskName, int deviceId, int jobId);
        /** \internal
         * \brief Convenience overload for CPU tasks (deviceId = CPU_DEVICE_ID = -1).
         * For internal SDK use only. Called by CpuHandle.
         * \param[in] type      event type
         * \param[in] taskName  CPU task name
         * \param[in] jobId     job ID
        */
        void Start(EventType type, const std::string& taskName, int jobId);
        /** \internal
         * \brief Record the end point of an event.
         * For internal SDK use only. Called by AccDeviceTaskLayer and CpuHandle
         * to mark the end of a timed region started with Start(). External callers should use GetJobMetrics().
         * \param[in] type      event type
         * \param[in] taskName  task name
         * \param[in] deviceId  device ID (NPU device ID; use CPU_DEVICE_ID for CPU tasks)
         * \param[in] jobId     job ID
        */
        void End(EventType type, const std::string& taskName, int deviceId, int jobId);
        /** \internal
         * \brief Convenience overload for CPU tasks (deviceId = CPU_DEVICE_ID = -1).
         * For internal SDK use only. Called by CpuHandle.
         * \param[in] type      event type
         * \param[in] taskName  CPU task name
         * \param[in] jobId     job ID
        */
        void End(EventType type, const std::string& taskName, int jobId);

        /** \internal
         * \brief Clear all timing data.
         * For internal SDK use only. Use Flush() to reset profiler state between runs.
        */
        void Clear() const;
        /** \brief Print profiler statistics collected so far.
        */
        void Show();
        /** \brief Save profiler timing data to a file.
         * \param[in] file file to save
        */
        void Save(const std::string& file);
        /** \brief Save profiler timing data merged with user events.
         * \param[in] file       output file path
         * \param[in] userEvents user-recorded events to merge into the output
         */
        void Save(const std::string& file,
                  const std::vector<UserEvent>& userEvents);
        void Flush();

        /** \brief Get performance data for all devices
         * \return map of event name to vector of durations in milliseconds
         * \deprecated Use GetJobMetrics() instead.
        */
        [[deprecated("Use GetJobMetrics() instead.")]]
        std::map<std::string, std::vector<int64_t>> GetPerformanceData();

        /** \brief Get performance data filtered by device ID
         * \param[in] deviceId device ID to filter by (-1 for all devices, returns device-separated keys)
         * \return map of event name to vector of durations in milliseconds
         * \deprecated Use GetJobMetrics() instead.
        */
        [[deprecated("Use GetJobMetrics() instead.")]]
        std::map<std::string, std::vector<int64_t>> GetPerformanceDataByDevice(int deviceId);

        /** \brief Get timing metrics for a specific job across all tasks and devices.
         *
         * Intended to be called right after eng.Wait(jobId) returns.
         * Returns one TaskMetrics per registered task that participated in the job.
         *
         * \code
         * int jid = eng.RunAsync(...);
         * eng.Wait(jid);
         * auto jm = profiler.GetJobMetrics(jid);
         * if (jm.valid) {
         *     auto npu = jm.GetTask("npu_0");
         *     auto h2d = npu.devices[0].h2d_us;
         *     auto inference = npu.devices[0].inference_core_all_us;
         *     auto d2h = npu.devices[0].d2h_us;
         *
         *     auto cpu = jm.GetTask("cpu_0");
         *     auto cpuExecution = cpu.cpu_task_us;
         * }
         * \endcode
         *
         * \param[in] jobId  job ID returned by RunAsync()
         * \return JobMetrics containing one TaskMetrics per task; valid=false if jobId not found.
        */
        JobMetrics GetJobMetrics(int jobId);

        /** \brief Map a core ID (0, 1, 2) to the corresponding INFERENCE_CORE_X EventType.
         *  Returns INFERENCE_CORE_0 for unknown core IDs as a safe fallback.
         *  \param[in] coreId  NPU core ID (typically response.dma_ch)
         *  \return corresponding EventType
         */
        static EventType InferenceCoreEventType(int coreId);

     private:
        struct DeviceProfilerBuffer
        {
            /** \brief Online statistics accumulator (Welford's algorithm) — O(1) memory. */
            struct StatsAccumulator
            {
                uint64_t count  = 0;
                double   min_us = (std::numeric_limits<double>::max)();  // extra parens prevent Windows max() macro expansion
                double   max_us = 0.0;
                double   mean   = 0.0;  ///< Welford running mean
                double   M2     = 0.0;  ///< Welford sum of squared deviations

                bool Valid() const { return count > 0; }

                void Update(double us)
                {
                    if (us <= 0.0) return;
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
                    M2   += delta * (us - mean);  // uses updated mean (Welford)
                }

                double Variance() const
                {
                    return (count > 1) ? M2 / static_cast<double>(count - 1) : 0.0;
                }
                double StdDev() const { return std::sqrt(Variance()); }
                /// Coefficient of Variation (%) = stddev / mean * 100.
                /// Dimensionless: allows direct comparison across events with different magnitudes.
                double CoV() const { return (mean > 0.0) ? (StdDev() / mean * 100.0) : 0.0; }
            };

            std::map<EventType,
                     std::array<TimePoint, PROFILER_BUFFER_SIZE>> events;
            std::map<EventType, StatsAccumulator> stats;  ///< running stats — O(1) memory
            std::array<int, PROFILER_BUFFER_SIZE> job_ids;
            int current_idx = 0;
            int job_count   = 0;

            DeviceProfilerBuffer() { job_ids.fill(-1); }

            int FindJobIndex(int jobId) const
            {
                for (int i = 0; i < PROFILER_DEFAULT_SAMPLES; ++i)
                {  // PROFILER_DEFAULT_SAMPLES == 0 → loop body unreachable; returns -1 safely
                    if (job_ids[i] == jobId) return i;
                } // @no_else: linear search, returns -1 on miss
                return -1;
            }

        };

        std::map<std::string, std::map<int, DeviceProfilerBuffer>> _deviceBuffers;

        std::mutex _lock;
        bool _save_exit;
        bool _show_exit;
        bool _enabled;

        void SetSettings(Configuration::ATTRIBUTE attrib, bool enabled);
        void SetEnabled(bool enabled) { _enabled = enabled;}

        std::string EventTypeToString(EventType type) const;
        static constexpr int CPU_DEVICE_ID = -1;

        bool PrintStatsRow(const DeviceProfilerBuffer::StatsAccumulator& acc,
                           const std::string& displayName,
                           int depth = 0) const;
        bool PrintStatsRow(const std::map<EventType, DeviceProfilerBuffer::StatsAccumulator>& stats,
                           EventType type, const std::string& displayName,
                           int depth = 0) const;
        void PrintDeviceSection(const DeviceProfilerBuffer& buf, int devId) const;
        void PrintCpuSection(const std::map<EventType, DeviceProfilerBuffer::StatsAccumulator>& stats) const;

        friend class Configuration;
    };

    extern uint8_t DEBUG_DATA;              // NOSONAR: Modified at runtime via DXRT_DEBUG_DATA env var
    extern uint8_t DXRT_API SHOW_PROFILE;   // NOSONAR: Modified at runtime via DXRT_SHOW_PROFILE env var
    extern uint8_t DXRT_API SKIP_INFERENCE_IO; // NOSONAR: Modified at runtime via CLI option

} // namespace dxrt
