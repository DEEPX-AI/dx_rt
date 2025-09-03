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
#include <string>
#include <chrono>
#include <vector>
#include <map>
#include <mutex>
#include "dxrt/configuration.h"

#ifdef USE_PROFILER
#define PROFILER_DEFAULT_SAMPLES 10 // Modified
#else
#define PROFILER_DEFAULT_SAMPLES 0
#endif
#define DBG_LOG_REQ_MOD_NUM 2500
#define DBG_LOG_REQ_WINDOW_NUM 0


namespace dxrt {
    // using ProfilerClock = std::chrono::high_resolution_clock;
    using ProfilerClock = std::chrono::steady_clock;
    // using ProfilerClock = std::chrono::system_clock;
    struct DXRT_API TimePoint
    {
        ProfilerClock::time_point start;
        ProfilerClock::time_point end;
    };
    using TimePointPtr = std::shared_ptr<TimePoint>;

    /** \brief This class provides time measurement API based on timestamp.
     * \headerfile "dxrt/dxrt_api.h"
    */
    class DXRT_API Profiler
    {
    private:

        Profiler();
        ~Profiler();

        // Delete copy constructor and assignment operator
        Profiler(const Profiler&) = delete;
        Profiler& operator=(const Profiler&) = delete;

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

        
        /** \brief Register an event. (If you use profiler in multi-threads, please call this function first)
         * \param[in] event event name
        */
        void Add(const std::string& event);
        /** \brief Add a timing data
         * \param[in] event event name
         * \param[in] tp timing data
        */
        void AddTimePoint(const std::string& event, TimePointPtr tp);
        /** \brief Record start point of an event
         * \param[in] event event name
        */
        void Start(const std::string& event);
        /** \brief Record end point of an event
         * \param[in] event event name
        */
        void End(const std::string& event);
        /** \brief Get recent elapsed time of an event
         * \code
         * profiler.Start("1sec");
         * sleep(1);
         * profiler.End("1sec");
         * auto measuredTime = profiler.Get("1sec");
         * \endcode
         * \return time in microseconds
        */
        uint64_t Get(const std::string& event);
        /** \brief Get average elapsed time of an event
         * \param[in] event event name
         * \return time in microseconds
        */
        double GetAverage(const std::string& event);
        /** \brief clear timing data of an event
         * \param[in] event event name
        */
        void Erase(const std::string& event);
        /** \brief clear timing data of all events
        */
        void Clear();
        /** \brief show elapsed times for all events
         * \code
         * profiler.Start("1sec");
         * sleep(1);
         * profiler.End("1sec");
         * profiler.Show();
         * \endcode
        */
        void Show(bool showDurations=false);
        /** \brief Save timing data of all events to a file
         * \param[in] file file to save
        */
        void Save(const std::string& file);

        

    protected:
    private:
        
        int numSamples = PROFILER_DEFAULT_SAMPLES;  ///< time points array size per events
        std::string name="";
        std::map<std::string, std::vector<TimePoint>> timePoints; ///< start/end time points per events
        std::map<std::string, int> idx;  ///< next array index to save data
        std::mutex _lock;
        bool _save_exit;
        bool _show_exit;
        bool _enabled;
        
        // Memory usage tracking variables (moved from static in Add function)
        uint64_t call_count = 0;
        uint64_t last_threshold_passed = 0;
        static const uint64_t MEMORY_PER_EVENT = 350;      ///< Hardcoded value, depends on PROFILER_DEFAULT_SAMPLES  
        static const uint64_t THRESHOLD_BASE = 100*1024*1024;  ///< Memory threshold base (100MB)

        void SetSettings(Configuration::ATTRIBUTE attrib, bool enabled);
        void SetEnabled(bool enabled) { _enabled = enabled;}
        
        friend class Configuration;
    };

    extern uint8_t DEBUG_DATA;
    extern uint8_t DXRT_API SHOW_PROFILE;
    extern uint8_t DXRT_API SKIP_INFERENCE_IO;

} // namespace dxrt