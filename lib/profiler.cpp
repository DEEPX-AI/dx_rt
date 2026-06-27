/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/profiler.h"
#include "dxrt/user_event_store.h"
#include "dxrt/request.h"
#include "dxrt/configuration.h"
#include "dxrt/extern/rapidjson/document.h"
#include "dxrt/extern/rapidjson/writer.h"
#include "dxrt/extern/rapidjson/prettywriter.h"
#include "dxrt/extern/rapidjson/stringbuffer.h"
#include "dxrt/extern/rapidjson/filereadstream.h"
#include "dxrt/extern/rapidjson/pointer.h"
#include "dxrt/extern/rapidjson/rapidjson.h"
#include "dxrt/exception/exception.h"
#include "resource/log_messages.h"

#include <iostream>
#include <iomanip>
#include <algorithm>

using std::cout;
using std::endl;
using std::setw;
using std::dec;
using std::vector;
using std::string;
using rapidjson::Document;
using rapidjson::kObjectType;
using rapidjson::kArrayType;
using rapidjson::StringBuffer;
using rapidjson::Value;
using rapidjson::Writer;

namespace dxrt
{
    static bool isInferenceCoreEvent(Profiler::EventType type)
    {
        return type == Profiler::EventType::INFERENCE_CORE_0
            || type == Profiler::EventType::INFERENCE_CORE_1
            || type == Profiler::EventType::INFERENCE_CORE_2;
    }

    Profiler* Profiler::_staticInstance = nullptr;

    Profiler& Profiler::GetInstance()
    {
        if ( _staticInstance == nullptr ) _staticInstance = new Profiler();
        return *_staticInstance;
    }

    void Profiler::deleteInstance()
    {
        if ( _staticInstance != nullptr ) delete _staticInstance;
        _staticInstance = nullptr;
    }


    Profiler::Profiler()
    : _save_exit(SAVE_PROFILER_DATA), _show_exit(SHOW_PROFILER_DATA), _enabled(USE_PROFILER)  // NOSONAR:S3230
    {
        LOG_DXRT_DBG << endl;
    }

    void Profiler::SetSettings(Configuration::ATTRIBUTE attrib, bool enabled)
    {
        if (attrib == Configuration::ATTRIBUTE::PROFILER_SAVE_DATA)
        {
            _save_exit = enabled;
        }

        if (attrib == Configuration::ATTRIBUTE::PROFILER_SHOW_DATA)
        {
            _show_exit = enabled;
        }
    }

    Profiler::~Profiler()
    {
        LOG_DXRT_DBG << endl;
        if (!_deviceBuffers.empty())
        {
            if (_save_exit)
            {
                // UserEventStore must outlive Profiler so GetInstance() is safe here.
                // ObjectsPool guarantees: Profiler::deleteInstance() before UserEventStore::deleteInstance().
                std::vector<UserEvent> user_events;
                if (UserEventStore::HasInstance())
                {
                    user_events = UserEventStore::GetInstance().GetEvents();
                }
                Save("profiler.json", user_events);
            }

            if (_show_exit)
            {
                try
                {
                    Show();
                    if (UserEventStore::HasInstance())
                    {
                        UserEventStore::GetInstance().Show();
                    }
                }
                catch (dxrt::Exception& e)
                {
                    e.printTrace();
                }
                catch (std::exception& e)
                {
                    LOG_DXRT_ERR(e.what());
                }
                catch (...)
                {
                    LOG_DXRT_ERR("UNKNOWN error type");
                }
            }
        }
    }

    void Profiler::AddTimePoint(EventType type, const std::string& taskName, int deviceId, int jobId, TimePointPtr tp)
    {
        if (!_enabled)
        {
            return;  // @no_else: guard clause
        }
        // Defense-in-depth: PROFILER_DEFAULT_SAMPLES==0 implies _enabled==false (above),
        // but guard the modulo explicitly to avoid division-by-zero UB if that invariant breaks.
        if (PROFILER_DEFAULT_SAMPLES == 0)
        {
            return;  // @no_else: profiler disabled at compile time
        }
        if (type == EventType::UNKNOWN)
        {
            return;  // @no_else: guard clause
        }
        if (jobId < 0 || !tp)
        {
            return;  // @no_else: guard clause
        }

        std::unique_lock<std::mutex> lk(_lock);
        auto& buf = _deviceBuffers[taskName][deviceId];

        // ── Determine job slot ───────────────────────────────────────────────
        // In a pipelined environment, current_idx may have been advanced by
        // another job, so always search for an existing slot via FindJobIndex()
        // first.
        // [slot found]     → append to existing slot.
        // [not found = new job] → advance current_idx and clear the new slot.
        int slot = buf.FindJobIndex(jobId);
        if (slot < 0)
        {
            buf.current_idx = (buf.current_idx + 1) % PROFILER_DEFAULT_SAMPLES;
            slot = buf.current_idx;
            buf.job_ids[slot] = jobId;
            buf.job_count++;
            for (auto& pair : buf.events)
            {
                pair.second[slot] = TimePoint{};
            }
        } // @no_else: existing job slot found, just append

        buf.events[type][slot] = *tp;
        // Update running statistics
        if (tp->start.time_since_epoch().count() != 0 && tp->end.time_since_epoch().count() != 0)
        {
            double us = std::chrono::duration<double, std::micro>(tp->end - tp->start).count();
            buf.stats[type].Update(us);
            if (isInferenceCoreEvent(type))
            {
                // Aggregate stats across all cores — O(1) memory, no events duplication.
                // Per-job CORE_ALL data is derived on-demand from per-core events in GetJobMetrics().
                buf.stats[EventType::INFERENCE_CORE_ALL].Update(us);
            }
        }
    }

    void Profiler::Start(EventType type, const std::string& taskName, int deviceId, int jobId)
    {
        if (!_enabled) 
        {
            return;  // @no_else: guard clause
        }
        if (PROFILER_DEFAULT_SAMPLES == 0)
        {
            return;  // @no_else: profiler disabled at compile time
        }
        if (type == EventType::UNKNOWN) 
        {
            return;  // @no_else: guard clause
        }
        if (jobId < 0) 
        {
            return;  // @no_else: guard clause
        }

        std::unique_lock<std::mutex> lk(_lock);
        auto& buf = _deviceBuffers[taskName][deviceId];
        auto now = ProfilerClock::now();

        // ── Determine job slot ───────────────────────────────────────────────
        // In a pipelined environment (e.g. Job N+1's BUFFER_POOL_WAIT advances
        // current_idx while Job N's DMA is still in flight), resolving the
        // slot from current_idx alone can scatter the same job across multiple
        // slots, breaking Start/End pairs.
        // → Always search for an existing slot via FindJobIndex() first;
        //   advance to a new slot only when no existing slot is found.
        int slot = buf.FindJobIndex(jobId);
        if (slot < 0)
        {
            buf.current_idx = (buf.current_idx + 1) % PROFILER_DEFAULT_SAMPLES;
            slot = buf.current_idx;
            buf.job_ids[slot] = jobId;
            buf.job_count++;
            for (auto& pair : buf.events)
            {
                pair.second[slot] = TimePoint{};
            }
        } // @no_else: existing job slot found, just append

        buf.events[type][slot] = TimePoint{now, {}};  // end = epoch (zero); filled by End()
    }

    void Profiler::End(EventType type, const std::string& taskName, int deviceId, int jobId)
    {
        if (!_enabled) 
        {
            return;  // @no_else: guard clause
        }
        if (PROFILER_DEFAULT_SAMPLES == 0)
        {
            return;  // @no_else: profiler disabled at compile time
        }
        if (type == EventType::UNKNOWN) 
        {
            return;  // @no_else: guard clause
        }   
        if (jobId < 0) 
        {
            return;  // @no_else: guard clause
        }

        std::unique_lock<std::mutex> lk(_lock);
        auto& buf = _deviceBuffers[taskName][deviceId];
        int slot = buf.FindJobIndex(jobId);
        if (slot < 0)
        {
            return;  // @no_else: job not in buffer
        }

        auto& tp = buf.events[type][slot];
        if (tp.start.time_since_epoch().count() == 0)
        {
            return;  // @no_else: no matching Start()
        }
        tp.end = ProfilerClock::now();
        // Update running statistics (Welford’s online algorithm)
        double us = std::chrono::duration<double, std::micro>(tp.end - tp.start).count();
        buf.stats[type].Update(us);
        if (isInferenceCoreEvent(type))
        {
            buf.stats[EventType::INFERENCE_CORE_ALL].Update(us);
        }
    }

    void Profiler::Start(EventType type, const std::string& taskName, int jobId)
    {
        Start(type, taskName, CPU_DEVICE_ID, jobId);
    }

    void Profiler::End(EventType type, const std::string& taskName, int jobId)
    {
        End(type, taskName, CPU_DEVICE_ID, jobId);
    }

    // ── Legacy string API ──────────────────────────────────────────────────
    // Start(string), End(string), AddTimePoint(string), Get(string),
    // GetAverage(string), Erase(string) all removed.
    // All callers migrated to type-safe API.

    void Profiler::Clear(void) const
    {
        // now nothing to do since we are using a fixed-size vector for each event type, but can be extended in the future if needed
    }

    bool Profiler::PrintStatsRow(const DeviceProfilerBuffer::StatsAccumulator& acc,
                                 const string& displayName,
                                 int depth) const
    {
        if (!acc.Valid())
        {
            return false;
        }
        string label;
        switch (depth)
        {
            case 1:  label = "  - " + displayName; break;
            case 2:  label = "    - " + displayName; break;
            default: label = displayName; break;
        }
        cout << std::fixed << std::setprecision(2);
        cout << "  | " << std::left << setw(30) << label.substr(0, 30) << std::right
             << " | " << setw(12) << acc.min_us
             << " | " << setw(12) << acc.max_us
             << " | " << setw(12) << acc.mean
             << " | " << setw(12) << acc.CoV() << " |" << endl;
        cout << std::defaultfloat;
        return true;
    }

    bool Profiler::PrintStatsRow(const std::map<EventType, DeviceProfilerBuffer::StatsAccumulator>& stats,
                                 EventType type, const string& displayName,
                                 int depth) const
    {
        auto it = stats.find(type);
        if (it == stats.end() || !it->second.Valid())
        {
            return false;
        }
        return PrintStatsRow(it->second, displayName, depth);
    }

    // !! IMPORTANT: When adding a new NPU-related EventType, consider adding it to the display list below !!
    void Profiler::PrintDeviceSection(const DeviceProfilerBuffer& buf, int devId) const
    {
        const auto& stats = buf.stats;
        // Check if any NPU-related events have data
        bool has_any = false;
        for (auto type : { EventType::BUFFER_POOL_WAIT, EventType::NPU_INPUT_FORMAT,
                           EventType::H2D, EventType::INFERENCE_CORE_0,
                           EventType::INFERENCE_CORE_1, EventType::INFERENCE_CORE_2,
                           EventType::D2H,
                           EventType::NPU_OUTPUT_FORMAT, EventType::NPU_TASK_TOTAL })
        {
            auto it = stats.find(type);
            if (it != stats.end() && it->second.Valid()) { has_any = true; break; }
        }
        if (!has_any) return;

        cout << "  ----------------------------------------------------------------------------------------------" << endl;
        cout << "  | Device " << devId
             << setw(85 - static_cast<int>(std::to_string(devId).length())) << "|" << endl;
        cout << "  | " << std::left << setw(30) << "Name" << std::right
             << " | " << setw(12) << "min (us)"
             << " | " << setw(12) << "max (us)"
             << " | " << setw(12) << "average (us)"
             << " | " << setw(12) << "CoV (%)" << " |" << endl;
        cout << "  ----------------------------------------------------------------------------------------------" << endl;

        // Buffer Pool Wait is outside NPU Task (total) - shown first as independent phase
        PrintStatsRow(stats, EventType::BUFFER_POOL_WAIT,    "Buffer Pool Wait");
        // Show NPU Task total, then sub-components indented below it
        PrintStatsRow(stats, EventType::NPU_TASK_TOTAL,      "NPU Task (total)");
        PrintStatsRow(stats, EventType::NPU_INPUT_FORMAT,    "NPU Input Format Handler",  1);
        PrintStatsRow(stats, EventType::H2D,                 "H2D (Host-to-Device)",      1);
        // Combined inference stats across all cores, then per-core breakdown
        PrintStatsRow(stats, EventType::INFERENCE_CORE_ALL,  "Inference",                 1);
        PrintStatsRow(stats, EventType::INFERENCE_CORE_0,    "Inference Core 0",          2);
        PrintStatsRow(stats, EventType::INFERENCE_CORE_1,    "Inference Core 1",          2);
        PrintStatsRow(stats, EventType::INFERENCE_CORE_2,    "Inference Core 2",          2);
        PrintStatsRow(stats, EventType::D2H,                 "D2H (Device-to-Host)",      1);
        PrintStatsRow(stats, EventType::NPU_OUTPUT_FORMAT,   "NPU Output Format Handler", 1);
        // Add new NPU-related events here if needed

        cout << "  ----------------------------------------------------------------------------------------------" << endl;
    }

    // !! IMPORTANT: When adding a new CPU-related EventType, consider adding it to the display list below !!
    void Profiler::PrintCpuSection(const std::map<EventType, DeviceProfilerBuffer::StatsAccumulator>& stats) const
    {
        // Check if any CPU-related events have data
        bool has_any = false;
        for (auto type : { EventType::BUFFER_POOL_WAIT, EventType::CPU_DISPATCH_WAIT, EventType::CPU_TASK_TOTAL })
        {
            auto it = stats.find(type);
            if (it != stats.end() && it->second.Valid()) { has_any = true; break; }
        }
        if (!has_any)
        {
            return;  // @no_else: no data yet for this CPU task
        }

        cout << "  ----------------------------------------------------------------------------------------------" << endl;
        cout << "  | CPU" << setw(89) << "|" << endl;
        cout << "  | " << std::left << setw(30) << "Name" << std::right
             << " | " << setw(12) << "min (us)"
             << " | " << setw(12) << "max (us)"
             << " | " << setw(12) << "average (us)"
             << " | " << setw(12) << "CoV (%)" << " |" << endl;
        cout << "  ----------------------------------------------------------------------------------------------" << endl;
        // All three are sequential independent phases
        PrintStatsRow(stats, EventType::BUFFER_POOL_WAIT,  "Buffer Pool Wait");
        PrintStatsRow(stats, EventType::CPU_DISPATCH_WAIT, "CPU Dispatch Wait");
        PrintStatsRow(stats, EventType::CPU_TASK_TOTAL,     "CPU Task (total)");
        cout << "  ----------------------------------------------------------------------------------------------" << endl;
    }

    void Profiler::Show()
    {
        if (_enabled == false)
        {
            return;
        }
        std::unique_lock<std::mutex> lk(_lock);
        LOG_DXRT_DBG << "profiler" << endl;
        if (_deviceBuffers.empty())
        {
            return;
        }

        for (const auto& taskPair : _deviceBuffers)
        {
            const string& taskName = taskPair.first;

            // Skip tasks that have no accumulated stats at all
            bool has_data = false;
            for (const auto& devPair : taskPair.second)
            {
                if (!devPair.second.stats.empty()) { has_data = true; break; }
            }
            if (!has_data) continue;

            // ── Task header ──────────────────────────────────────────────────
            cout << "  ==============================================================================================" << endl;
            cout << "  | Task: " << taskName
                 << setw(86 - static_cast<int>(taskName.length())) << "|" << endl;
            cout << "  ==============================================================================================" << endl;

            // Iterate devices in sorted order (map sorted by devId; CPU_DEVICE_ID=-1 comes first)
            for (const auto& devPair : taskPair.second)
            {
                int devId = devPair.first;
                const auto& buf = devPair.second;
                if (devId == CPU_DEVICE_ID)
                {
                    PrintCpuSection(buf.stats);
                }
                else
                {
                    PrintDeviceSection(buf, devId);
                }
            }
        }
    }

    void Profiler::Save(const string &filename)
    {
        Save(filename, {});
    }

    void Profiler::Save(const string &filename,
                        const std::vector<UserEvent>& userEvents)
    {
        if (_enabled == false && userEvents.empty())
            return;

        std::unique_lock<std::mutex> lk(_lock);

        if (_deviceBuffers.empty() && userEvents.empty())
            return;

        Document document;
        document.SetObject();
        Document::AllocatorType& allocator = document.GetAllocator();

        // ── Internal events ──
        for (const auto& taskPair : _deviceBuffers)
        {
            const string& taskName = taskPair.first;
            for (const auto& devPair : taskPair.second)
            {
                int devId = devPair.first;
                const auto& buf = devPair.second;

                for (const auto& evPair : buf.events)
                {
                    EventType type = evPair.first;
                    if (type == EventType::INFERENCE_CORE_ALL)
                    {
                        continue;  // @no_else: aggregate-only, per-core events are the ground truth
                    }

                    for (int slot = 0; slot < PROFILER_DEFAULT_SAMPLES; ++slot)
                    {
                        const auto& tp = evPair.second[slot];
                        if (tp.start.time_since_epoch().count() == 0
                            || tp.end.time_since_epoch().count() == 0)
                        {
                            continue;  // @no_else: incomplete TimePoint
                        }
                        int job_id = buf.job_ids[slot];
                        if (job_id < 0)
                        {
                            continue;  // @no_else: slot not assigned to a job
                        }

                        string entryName = EventTypeToString(type)
                            + "[" + taskName + "]"
                            + "[Device_" + std::to_string(devId) + "]"
                            + "[Job_" + std::to_string(job_id) + "]";

                        Value timePointsArray(kArrayType);
                        Value timePointObject(kObjectType);
                        timePointObject.AddMember("start", tp.start.time_since_epoch().count(), allocator);
                        timePointObject.AddMember("end",   tp.end.time_since_epoch().count(),   allocator);
                        timePointsArray.PushBack(timePointObject, allocator);

                        if (document.HasMember(entryName.c_str()))
                        {
                            document[entryName.c_str()].SetArray().Swap(timePointsArray);
                        }
                        else
                        {
                            document.AddMember(Value(entryName.c_str(), allocator).Move(), timePointsArray, allocator);
                        }
                    } // for slot
                } // for evPair
            } // for devPair
        } // for taskPair

        // ── User events ──
        std::map<std::string, int> name_index_map;
        for (const auto& ue : userEvents)
        {
            if (ue.tp.start.time_since_epoch().count() == 0
                || ue.tp.end.time_since_epoch().count() == 0)
            {
                continue;
            }

            int idx = name_index_map[ue.name]++;
            string entryName = "User:" + ue.name + "[" + std::to_string(idx) + "]";

            Value timePointsArray(kArrayType);
            Value timePointObject(kObjectType);
            timePointObject.AddMember("start", ue.tp.start.time_since_epoch().count(), allocator);
            timePointObject.AddMember("end",   ue.tp.end.time_since_epoch().count(),   allocator);
            timePointsArray.PushBack(timePointObject, allocator);

            document.AddMember(Value(entryName.c_str(), allocator).Move(), timePointsArray, allocator);
        }

        // ── Write to file ──
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        document.Accept(writer);

        std::ofstream outFile(filename);
        if (outFile.is_open())
        {
            outFile << buffer.GetString();
            outFile.close();
            cout << "Profiler data has been written to " << filename << endl;
        }
        else
        {
            LOG_DXRT_ERR("Failed to open output file");
        }
    }

    void Profiler::Flush()
    {
        if (!_enabled)
        {
            return;  // @no_else: guard clause
        }

        std::unique_lock<std::mutex> lk(_lock);
        _deviceBuffers.clear();
    }

    std::map<string, std::vector<int64_t>> Profiler::GetPerformanceData()
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
        return GetPerformanceDataByDevice(-1);  // -1 means all devices
#pragma GCC diagnostic pop
    }

    // !! IMPORTANT: When adding a new EventType, add its string mapping here !!
    std::string Profiler::EventTypeToString(EventType type) const
    {
        switch (type)
        {
            case EventType::H2D:                    return "H2D";
            case EventType::INFERENCE_CORE_ALL:     return "Inference";
            case EventType::INFERENCE_CORE_0:       return "Inference Core 0";
            case EventType::INFERENCE_CORE_1:       return "Inference Core 1";
            case EventType::INFERENCE_CORE_2:       return "Inference Core 2";
            case EventType::D2H:                    return "D2H";
            case EventType::NPU_TASK_TOTAL:         return "NPU Task";
            case EventType::CPU_TASK_TOTAL:         return "CPU Task";
            case EventType::BUFFER_POOL_WAIT:       return "Buffer Pool Wait";
            case EventType::FRAMEWORK_OVERHEAD:     return "Framework Overhead";
            case EventType::NPU_INPUT_FORMAT:       return "NPU Input Format Handler";
            case EventType::NPU_OUTPUT_FORMAT:      return "NPU Output Format Handler";
            case EventType::CPU_DISPATCH_WAIT:      return "CPU Dispatch Wait";
            case EventType::SERVICE_PROCESS_WAIT:   return "Service Process Wait";
            // Add new EventType cases above this line
            default:                                return "Unknown";
        }
    }

    Profiler::EventType Profiler::InferenceCoreEventType(int coreId)
    {
        switch (coreId)
        {
            case 0: return EventType::INFERENCE_CORE_0;
            case 1: return EventType::INFERENCE_CORE_1;
            case 2: return EventType::INFERENCE_CORE_2;
            default: return EventType::INFERENCE_CORE_0;
        }
    }

    std::map<string, std::vector<int64_t>> Profiler::GetPerformanceDataByDevice(int deviceId)
    {
        std::unique_lock<std::mutex> lk(_lock);
        if (_deviceBuffers.empty())
        {
            return {};
        }

        std::map<string, std::vector<int64_t>> data;

        for (const auto& taskPair : _deviceBuffers)
        {
            for (const auto& devPair : taskPair.second)
            {
                int devId = devPair.first;
                if (deviceId >= 0 && devId != deviceId)
                {
                    continue;  // @no_else: filter by requested device
                }

                const auto& buf = devPair.second;

                for (const auto& evIt : buf.events)
                {
                    EventType type = evIt.first;
                    // Only expose public event types (value < _PUBLIC_END = 999)
                    // Public events: 0-998, Internal events: 1000+
                    if (static_cast<int>(type) >= static_cast<int>(EventType::_PUBLIC_END))
                    {
                        continue;  // @no_else: internal event, skip
                    }

                    string base_name = EventTypeToString(type);
                    string data_key = (deviceId < 0)
                        ? taskPair.first + "/" + base_name + "[Device_" + std::to_string(devId) + "]"
                        : base_name;

                    for (int slot = 0; slot < PROFILER_DEFAULT_SAMPLES; ++slot)
                    {
                        const auto& tp = evIt.second[slot];
                        if (tp.start.time_since_epoch().count() == 0
                            || tp.end.time_since_epoch().count() == 0)
                        {
                            continue;  // @no_else: incomplete TimePoint
                        }
                        int64_t elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                            tp.end - tp.start).count();
                        data[data_key].push_back(elapsed_ms);
                    }
                }
            }
        }

        return data;
    }

    JobMetrics Profiler::GetJobMetrics(int jobId)
    {
        std::unique_lock<std::mutex> lk(_lock);

        auto getDuration = [](const DeviceProfilerBuffer& buf, int slot, EventType type) -> double
        {
            auto evIt = buf.events.find(type);
            if (evIt == buf.events.end()) return 0.0;
            const auto& tp = evIt->second[slot];
            if (tp.start.time_since_epoch().count() == 0
                || tp.end.time_since_epoch().count() == 0) return 0.0;
            return std::chrono::duration<double, std::micro>(tp.end - tp.start).count();
        };

        // One TaskMetrics per registered task that contains this jobId.
        // Tasks are stored in _deviceBuffers keyed by task name (e.g. "npu_0", "cpu_0", "cpu_1").
        JobMetrics result;

        for (const auto& taskPair : _deviceBuffers)
        {
            // Collect all devices for this task that have the jobId
            TaskMetrics tm;
            tm.task_name = taskPair.first;

            for (const auto& devPair : taskPair.second)
            {
                int devId = devPair.first;
                const auto& buf = devPair.second;
                int slot = buf.FindJobIndex(jobId);
                if (slot < 0) continue;

                if (devId == CPU_DEVICE_ID)
                {
                    tm.cpu_task_us += getDuration(buf, slot, EventType::CPU_TASK_TOTAL);
                }
                else
                {
                    NpuDeviceMetrics m;
                    m.h2d_us  = getDuration(buf, slot, EventType::H2D);
                    m.inference_core_0_us = getDuration(buf, slot, EventType::INFERENCE_CORE_0);
                    m.inference_core_1_us = getDuration(buf, slot, EventType::INFERENCE_CORE_1);
                    m.inference_core_2_us = getDuration(buf, slot, EventType::INFERENCE_CORE_2);
                    // NPU driver assigns exactly one core per (task, device, job).
                    // Derive aggregate from the active core; 0.0 if none recorded.
                    m.inference_core_all_us = m.inference_core_0_us > 0.0 ? m.inference_core_0_us
                                           : m.inference_core_1_us > 0.0 ? m.inference_core_1_us
                                           :                                m.inference_core_2_us;
                    m.d2h_us  = getDuration(buf, slot, EventType::D2H);
                    m.total_us = getDuration(buf, slot, EventType::NPU_TASK_TOTAL);
                    m.input_format_us  = getDuration(buf, slot, EventType::NPU_INPUT_FORMAT);
                    m.output_format_us = getDuration(buf, slot, EventType::NPU_OUTPUT_FORMAT);
                    m.task_measured = (buf.events.count(EventType::NPU_TASK_TOTAL) > 0
                                    && buf.events.at(EventType::NPU_TASK_TOTAL)[slot].start.time_since_epoch().count() != 0);
                    m.valid   = (m.h2d_us > 0 || m.inference_core_all_us > 0 || m.d2h_us > 0);
                    if (m.valid)
                    {
                        tm.devices[devId] = m;
                    }
                }
            }

            // Include this task only if it had data for the jobId
            bool task_has_data = (!tm.devices.empty() && tm.devices.begin()->second.valid)
                              || tm.cpu_task_us > 0;
            if (!task_has_data) continue;

            tm.valid = true;
            result.tasks.push_back(std::move(tm));
        }

        result.valid = !result.tasks.empty();
        return result;  // @no_else: empty JobMetrics if jobId not found
    }

    // NOSONAR - Runtime configuration flags modified by environment variables and CLI options
    uint8_t DEBUG_DATA = 0;          // Modified by DXRT_DEBUG_DATA env var in InferenceEngine::initializeEnvironmentVariables()  // NOSONAR
    DXRT_API uint8_t SHOW_PROFILE = 0;        // Modified by DXRT_SHOW_PROFILE env var in InferenceEngine::initializeEnvironmentVariables()  // NOSONAR
    DXRT_API uint8_t SKIP_INFERENCE_IO = 0;   // Modified by CLI option in run_model.cpp // NOSONAR

}  // namespace dxrt
