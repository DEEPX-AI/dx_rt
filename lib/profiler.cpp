/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/profiler.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <set>
#include "dxrt/device.h"
#include "dxrt/request.h"
#include "dxrt/task.h"
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

using std::cout;
using std::endl;
using std::setw;
using std::hex;
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
        if (!timePoints.empty())
        {
            if (_save_exit)
            {
                Save("profiler.json");
            }

            if (_show_exit)
            {
                try
                {
                    Show();
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

    void Profiler::Add(const string &x)
    {
        if (_enabled == false)
            return;
        else
            LOG_DXRT_DBG << x << endl;

        std::unique_lock<std::mutex> lk(_lock);

        call_count++;

        uint64_t current_memory = call_count * MEMORY_PER_EVENT;
        uint64_t current_multiplier = current_memory / THRESHOLD_BASE;

        if (current_multiplier > last_threshold_passed) {
            LOG_DXRT_INFO(LogMessages::Profiler_MemoryUsage(current_memory));
            last_threshold_passed = current_multiplier;
        }

        if (timePoints.find(x) == timePoints.end())
        {
            timePoints.insert(make_pair(x, vector<TimePoint>(numSamples + 1)));
        }

        if (idx.find(x) == idx.end())
        {
            idx.insert(make_pair(x, -1));
        }
    }
    void Profiler::AddTimePoint(const string &x, TimePointPtr tp)
    {
        if (_enabled == false)
            return;
        else
            LOG_DXRT_DBG << x << endl;
        Add(x);

        std::unique_lock<std::mutex> lk(_lock);
        if (timePoints.empty())
            return;
        ++(idx.at(x));
        if (idx.at(x) >= numSamples)
            idx.at(x) = 0;
        timePoints.at(x)[idx.at(x)] = *tp;
    }
    void Profiler::Start(const string &x)
    {
        if (_enabled == false)
            return;
        else
            LOG_DXRT_DBG << x << endl;
        Add(x);

        std::unique_lock<std::mutex> lk(_lock);
        if (timePoints.empty()) return;
        ++(idx.at(x));
        if (idx.at(x) >= numSamples) idx.at(x) = 0;
        timePoints.at(x)[idx.at(x)].start = ProfilerClock::now();
    }

    void Profiler::End(const string &x)
    {
        if (_enabled == false)
            return;
        else
            LOG_DXRT_DBG << x << endl;

        std::unique_lock<std::mutex> lk(_lock);
        if (timePoints.empty()) return;
        if (timePoints.find(x) != timePoints.end())
        {
            if (idx.find(x) == idx.end())
            {
                cout << "error..." << x << endl;
                return;
            }
            timePoints.at(x)[idx.at(x)].end = ProfilerClock::now();
        }
    }

    uint64_t Profiler::Get(const string &x)
    {
        if (_enabled == false) return 0;

        std::unique_lock<std::mutex> lk(_lock);
        if (timePoints.find(x) != timePoints.end())
        {
            int idx_ = idx.at(x);
            auto ret = static_cast<int>(std::chrono::duration_cast<std::chrono::microseconds>(
                timePoints.at(x)[idx_].end - timePoints.at(x)[idx_].start).count());
            if (ret < 0)
                ret = 0;
            return ret;
        }
        else
        {
            return 0;
        }
    }

    double Profiler::GetAverage(const string &x)
    {
        if (_enabled == false) return 0.0;

        std::unique_lock<std::mutex> lk(_lock);
        double avgValue = 0;
        double sum = 0;
        if (!timePoints.empty())
        {
            vector<uint64_t> durations;
            auto tps = timePoints.at(x);
            for (const auto &tp : tps)
            {
                if (tp.start.time_since_epoch().count() == 0 || tp.end.time_since_epoch().count() == 0 )
                    continue;
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(tp.end-tp.start).count();
                if (duration > 0)
                {
                    durations.push_back(duration);
                    sum += static_cast<double>(duration);
                }
            }
            avgValue = sum/static_cast<double>(durations.size());
        }
        return avgValue;
    }

    void Profiler::Erase(const string &x)
    {
        if (_enabled == false) return;

        std::unique_lock<std::mutex> lk(_lock);
        if (!timePoints.empty())
        {
            auto it = timePoints.find(x);
            if (it != timePoints.end())
            {
                timePoints.erase(it);
            }
        }
    }

    void Profiler::Clear(void) const
    {
        // now nothing to do since we are using a fixed-size vector for each event type, but can be extended in the future if needed
    }

    string Profiler::ParseGroupKey(const string& fullName, std::set<int>& deviceIds) const
    {
        static constexpr std::array<const char*, 4> deviceIndependentEvents = {
            "Buffer Wait",
            "NPU Input Format Handler",
            "NPU Output Format Handler",
            "CPU Task Queue Wait"
        };

        size_t device_bracket_pos = fullName.find("[Device_");
        if (device_bracket_pos == string::npos)
        {
            size_t bracket_pos = fullName.find('[');
            return (bracket_pos != string::npos) ? fullName.substr(0, bracket_pos) : fullName;
        }

        size_t device_end_pos = fullName.find(']', device_bracket_pos);
        if (device_end_pos == string::npos)
        {
            return fullName;
        }

        string event_type = fullName.substr(0, device_bracket_pos);

        // Extract device ID number
        size_t device_num_start = device_bracket_pos + 8;
        string device_num_str = fullName.substr(device_num_start, device_end_pos - device_num_start);
        if (!device_num_str.empty()
            && device_num_str.length() <= 9
            && std::all_of(device_num_str.begin(), device_num_str.end(), ::isdigit))
        {
            int dev_id = std::stoi(device_num_str);
            deviceIds.insert(dev_id);
        }

        // Device-independent events: group without device suffix
        return (std::find(std::begin(deviceIndependentEvents), std::end(deviceIndependentEvents), event_type)
            != std::end(deviceIndependentEvents))
            ? event_type
            : fullName.substr(0, device_end_pos + 1);
    }

    bool Profiler::PrintStatsRow(const std::map<string, std::vector<TimePoint>>& grouped,
                                 const string& groupKey, const string& displayName) const
    {
        auto it = grouped.find(groupKey);
        if (it == grouped.end())
        {
            return false;
        }

        vector<uint64_t> durations;
        double sum = 0;

        for (const auto& tp : it->second)
        {
            if (tp.start.time_since_epoch().count() == 0 || tp.end.time_since_epoch().count() == 0)
            {
                continue;
            }
            int64_t duration = std::chrono::duration_cast<std::chrono::microseconds>(tp.end - tp.start).count();
            if (duration > 0)
            {
                durations.emplace_back(duration);
                sum += static_cast<double>(duration);
            }
        }
        if (durations.empty())
        {
            return false;
        }

        uint64_t min_value = *std::min_element(durations.begin(), durations.end());
        uint64_t max_value = *std::max_element(durations.begin(), durations.end());
        double avg_value = sum / static_cast<double>(durations.size());

        cout << "  | " << dec << setw(30) << displayName.substr(0, 30)
             << " | " << setw(12) << min_value
             << " | " << setw(12) << max_value
             << " | " << setw(12) << avg_value << " |" << endl;
        return true;
    }

    void Profiler::PrintDeviceSection(const std::map<string, std::vector<TimePoint>>& grouped,
                                      int devId) const
    {
        string dev_suffix = "[Device_" + std::to_string(devId) + "]";

        cout << "  -------------------------------------------------------------------------------" << endl;
        cout << "  | Device " << devId << setw(70 - static_cast<int>(std::to_string(devId).length())) << " |" << endl;
        cout << "  |            Name                |  min (us)    |  max (us)    | average (us) |" << endl;
        cout << "  -------------------------------------------------------------------------------" << endl;

        PrintStatsRow(grouped, "Buffer Wait", "Buffer Wait");
        PrintStatsRow(grouped, "NPU Input Format Handler", "NPU Input Format Handler");
        PrintStatsRow(grouped, "PCIe Write" + dev_suffix, "PCIe Write");
        PrintStatsRow(grouped, "NPU Core" + dev_suffix, "NPU Core");
        PrintStatsRow(grouped, "PCIe Read" + dev_suffix, "PCIe Read");
        PrintStatsRow(grouped, "NPU Output Format Handler", "NPU Output Format Handler");
        PrintStatsRow(grouped, "NPU Task" + dev_suffix, "NPU Task");

        cout << "  -------------------------------------------------------------------------------" << endl;
    }

    void Profiler::PrintCpuSection(const std::map<string, std::vector<TimePoint>>& grouped) const
    {
        std::vector<string> cpu_keys;
        for (const auto& group : grouped)
        {
            if (group.first.find("cpu_") == 0)
            {
                cpu_keys.push_back(group.first);
            }
        }

        bool has_cpu_section = !cpu_keys.empty()
            || grouped.find("CPU Task Queue Wait") != grouped.end();

        if (!has_cpu_section)
        {
            return;
        }

        std::sort(cpu_keys.begin(), cpu_keys.end());
        cout << "  -------------------------------------------------------------------------------" << endl;
        cout << "  | CPU Tasks" << setw(68) << " |" << endl;
        cout << "  |            Name                |  min (us)    |  max (us)    | average (us) |" << endl;
        cout << "  -------------------------------------------------------------------------------" << endl;
        PrintStatsRow(grouped, "CPU Task Queue Wait", "CPU Task Queue Wait");
        for (const auto& key : cpu_keys)
        {
            PrintStatsRow(grouped, key, key);
        }
        cout << "  -------------------------------------------------------------------------------" << endl;
    }

    void Profiler::Show(bool showDurations)
    {
        (void)showDurations;
        if (_enabled == false)
        {
            return;
        }
        std::unique_lock<std::mutex> lk(_lock);
        LOG_DXRT_DBG << "profiler" << endl;
        if (timePoints.empty())
        {
            return;
        }

        // Group all time points by base event type and device ID
        std::map<string, std::vector<TimePoint>> grouped;
        std::set<int> device_ids;

        for (const auto& entry : timePoints)
        {
            string base_name = ParseGroupKey(entry.first, device_ids);
            for (const auto& tp : entry.second)
            {
                grouped[base_name].push_back(tp);
            }
        }

        for (int dev_id : device_ids)
        {
            PrintDeviceSection(grouped, dev_id);
        }
        PrintCpuSection(grouped);
    }

    void Profiler::Save(const string &filename)
    {
        if (_enabled == false)
            return;

        std::unique_lock<std::mutex> lk(_lock);
        if (timePoints.empty())
            return;
        Document document;
        document.SetObject();
        Document::AllocatorType& allocator = document.GetAllocator();
        // Loop through the collected profiler data
        for (const auto& entry : timePoints) {
            const std::string& entryName = entry.first;
            const std::vector<TimePoint>& tps = entry.second;

            // Create a JSON array for time points
            Value timePointsArray(kArrayType);
            for (const auto& tp : tps) {
                if (tp.start.time_since_epoch().count() == 0 || tp.end.time_since_epoch().count() == 0 )
                    continue;
                Value timePointObject(kObjectType);
                timePointObject.AddMember("start", tp.start.time_since_epoch().count(), allocator);
                timePointObject.AddMember("end", tp.end.time_since_epoch().count(), allocator);
                timePointsArray.PushBack(timePointObject, allocator);
            }
            // Add or update the array in the document
            if (document.HasMember(entryName.c_str())) {
                document[entryName.c_str()].SetArray().Swap(timePointsArray);
            } else {
                document.AddMember(Value(entryName.c_str(), allocator).Move(), timePointsArray, allocator);
            }
        }
        // Serialize the JSON document to a string
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        document.Accept(writer);
        // Write the JSON string to a file
        std::ofstream outFile(filename);
        if (outFile.is_open()) {
            outFile << buffer.GetString();
            outFile.close();
            cout << "Profiler data has been written to " << filename << endl;
        } else {
            LOG_DXRT_ERR("Failed to open output file");
        }
    }

    void Profiler::Flush()
    {
        if (_enabled == false) return;

        std::unique_lock<std::mutex> lk(_lock);
        timePoints.clear();
        idx.clear();
    }

    std::map<string, std::vector<int64_t>> Profiler::GetPerformanceData()
    {
        return GetPerformanceDataByDevice(-1);  // -1 means all devices
    }

    int Profiler::ExtractDeviceId(const string& eventName)
    {
        size_t device_bracket_pos = eventName.find("[Device_");
        if (device_bracket_pos == string::npos)
        {
            return -1;
        }

        size_t device_num_start = device_bracket_pos + 8;
        size_t device_num_end = eventName.find(']', device_num_start);
        if (device_num_end == string::npos)
        {
            return -1;
        }

        string device_num_str = eventName.substr(device_num_start, device_num_end - device_num_start);
        if (!device_num_str.empty()
            && device_num_str.length() <= 9
            && std::all_of(device_num_str.begin(), device_num_str.end(), ::isdigit))
        {
            return std::stoi(device_num_str);
        }
        return -1;
    }

    std::map<string, std::vector<int64_t>> Profiler::GetPerformanceDataByDevice(int deviceId)
    {
        std::unique_lock<std::mutex> lk(_lock);
        if (timePoints.empty())
        {
            return {};
        }

        std::map<string, std::vector<int64_t>> data;

        for (const auto& entry : timePoints)
        {
            const std::string& event_name = entry.first;
            const std::vector<TimePoint>& tps = entry.second;

            // Extract base name and device ID from event name
            int event_device_id = ExtractDeviceId(event_name);
            string base_name = event_name;

            size_t device_bracket_pos = event_name.find("[Device_");
            if (device_bracket_pos != string::npos)
            {
                base_name = event_name.substr(0, device_bracket_pos);
            }
            else
            {
                size_t bracket_pos = event_name.find('[');
                if (bracket_pos != string::npos)
                {
                    base_name = event_name.substr(0, bracket_pos);
                }
            }

            // Filter by device ID if specified
            if (deviceId >= 0 && event_device_id != deviceId)
            {
                continue;
            }

            if (base_name != "NPU Core" && base_name != "NPU Task")
            {
                continue;
            }

            // Create key with device info if filtering by device
            string data_key = base_name;
            if (deviceId < 0 && event_device_id >= 0)
            {
                data_key = base_name + "[Device_" + std::to_string(event_device_id) + "]";
            }

            for (const auto& tp : tps)
            {
                if (tp.start.time_since_epoch().count() == 0 || tp.end.time_since_epoch().count() == 0)
                {
                    continue;
                }

                auto elapsed_time = tp.end - tp.start;
                int64_t elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time).count();
                data[data_key].push_back(elapsed_ms);
            }
        }

        return data;
    }

    // NOSONAR - Runtime configuration flags modified by environment variables and CLI options
    uint8_t DEBUG_DATA = 0;          // Modified by DXRT_DEBUG_DATA env var in InferenceEngine::initializeEnvironmentVariables()  // NOSONAR
    uint8_t SHOW_PROFILE = 0;        // Modified by DXRT_SHOW_PROFILE env var in InferenceEngine::initializeEnvironmentVariables()  // NOSONAR
    uint8_t SKIP_INFERENCE_IO = 0;   // Modified by CLI option in run_model.cpp // NOSONAR

}  // namespace dxrt
