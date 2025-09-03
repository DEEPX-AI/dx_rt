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

#define PROFILER_FORCE_SHOW_DURATIONS 1

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
    : _save_exit(ENABLE_SAVE_PROFILER_DATA), _show_exit(ENABLE_SHOW_PROFILER_DATA), _enabled(USE_PROFILER)
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
            int ret = std::chrono::duration_cast<std::chrono::microseconds>(
                timePoints.at(x)[idx_].end - timePoints.at(x)[idx_].start).count();
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
        double avgValue = 0, sum = 0;
        if (!timePoints.empty())
        {
            vector<uint64_t> durations;
            auto tps = timePoints.at(x);
            for (auto &tp : tps)
            {
                if (tp.start.time_since_epoch().count() == 0 || tp.end.time_since_epoch().count() == 0 )
                    continue;
                int duration = std::chrono::duration_cast<std::chrono::microseconds>(tp.end-tp.start).count();
                if (duration > 0)
                {
                    durations.push_back(duration);
                    sum += duration;
                }
            }
            avgValue = sum/durations.size();
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

    void Profiler::Clear(void)
    {
    }

    void Profiler::Show(bool showDurations)
    {
        if (_enabled == false)
            return;
        std::unique_lock<std::mutex> lk(_lock);
        LOG_DXRT_DBG << "profiler" << endl;
        if (!timePoints.empty())
        {
            cout << "  -------------------------------------------------------------------------------" << endl;
            cout << "  |           Name                 |  min (us)    |  max (us)    | average (us) |" << endl;
            cout << "  -------------------------------------------------------------------------------" << endl;
            
            // Group by base name (before first bracket)
            std::map<string, std::vector<TimePoint>> groupedTimePoints;
            std::map<string, vector<TimePoint>>::iterator iter;
            for (iter = timePoints.begin(); iter != timePoints.end(); ++iter)
            {
                string fullName = iter->first;
                string baseName = fullName;
                
                // Extract base name (before first bracket)
                size_t bracketPos = fullName.find('[');
                if (bracketPos != string::npos)
                {
                    baseName = fullName.substr(0, bracketPos);
                }
                
                // Add all time points for this base name
                auto& tps = iter->second;
                for (auto& tp : tps)
                {
                    groupedTimePoints[baseName].push_back(tp);
                }
            }
            
            // Process grouped time points
            for (auto& group : groupedTimePoints)
            {
                string name = group.first;
                uint64_t minValue, maxValue;
                double avgValue = 0, sum = 0;
                vector<uint64_t> durations;
                auto tps = group.second;
                for (auto &tp : tps)
                {
                    if ( tp.start.time_since_epoch().count() == 0 || tp.end.time_since_epoch().count() == 0 )
                        continue;
                    int duration = std::chrono::duration_cast<std::chrono::microseconds>(tp.end-tp.start).count();
                    if (duration > 0)
                    {
                        durations.emplace_back(duration);
                        sum += duration;
                    }
                }
                if (durations.empty())
                    continue;
                minValue = *std::min_element(durations.begin(), durations.end() );
                maxValue = *std::max_element(durations.begin(), durations.end() );
                avgValue = sum/durations.size();
                cout << "  | " << dec << setw(30) << name.substr(0, 28) << " | " << setw(12) << minValue \
                        << " | " << setw(12) << maxValue << " | " << setw(12) << avgValue << " | ";
                if (showDurations || PROFILER_FORCE_SHOW_DURATIONS)
                {
                    int count = 0;
                    for (auto& duration : durations)
                    {
                        if (count >= 30) {
                            cout << "...";
                            break;
                        }
                        cout << duration << ", ";
                        count++;
                    }
                }
                cout << endl;
            }
            cout << "  -------------------------------------------------------------------------------" << endl;
        }
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
            const std::string& name = entry.first;
            const std::vector<TimePoint>& tps = entry.second;
            // cout << name << endl;
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
            if (document.HasMember(name.c_str())) {
                document[name.c_str()].SetArray().Swap(timePointsArray);
            } else {
                document.AddMember(Value(name.c_str(), allocator).Move(), timePointsArray, allocator);
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
    uint8_t DEBUG_DATA = 0;
    uint8_t SHOW_PROFILE = 0;
    uint8_t SKIP_INFERENCE_IO = 0;

}  // namespace dxrt
