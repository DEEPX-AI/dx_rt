// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "dxrt/common.h"
#include <string>
#include <chrono>
#include <vector>
#include <map>
#include <mutex>

#define PROFILER_DEFAULT_SAMPLES 50

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
public:
    /** \brief Get pre-created instance. (Don't create your own instance.)
     * \code
     * auto& profiler = dxrt::Profiler::GetInstance();
     * \endcode
     * \return Singleton instance of dxrt::Profiler
    */
    static Profiler& GetInstance()
    {
        static Profiler profiler;
        return profiler;
    }
    virtual ~Profiler();
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
    Profiler();
    Profiler(const Profiler&) = delete;
    void operator=(const Profiler&) = delete;
    int numSamples = PROFILER_DEFAULT_SAMPLES;  ///< time points array size per events
    std::string name="";
    std::map<std::string, std::vector<TimePoint>> timePoints; ///< start/end time points per events
    std::map<std::string, int> idx;  ///< next array index to save data
    std::mutex _lock;
};

extern uint8_t DEBUG_DATA;

} // namespace dxrt