// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/profiler.h"
#include "dxrt/device.h"
#include "dxrt/request.h"
#include "dxrt/task.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/pointer.h"
#include "rapidjson/rapidjson.h"

#define PROFILER_FORCE_SHOW_DURATIONS 1

using namespace std;
using namespace rapidjson;

namespace dxrt
{
Profiler::Profiler()
{
    LOG_DXRT_DBG << endl;
}
Profiler::~Profiler()
{
    LOG_DXRT_DBG << endl;
    // LOG_DXRT << endl;
    // cout << "destroy profiler" << endl;
    if(!timePoints.empty())
    {
        Save("profiler.json");
#if SHOW_PROFILER_DATA
        Show();
#endif
    }
    //timePoints.clear();
    //idx.clear();
}

void Profiler::Add(const string &x) {
    LOG_DXRT_DBG << x << endl;
    
    unique_lock<mutex> lk(_lock);
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
    LOG_DXRT_DBG << x << endl;
    Add(x);
    if(timePoints.empty()) return;
    ++(idx.at(x));
    if(idx.at(x)>=numSamples) idx.at(x) = 0;
    timePoints.at(x)[idx.at(x)] = *tp;
}
void Profiler::Start(const string &x) {
    LOG_DXRT_DBG << x << endl;
    Add(x);
    unique_lock<mutex> lk(_lock);
    if (timePoints.empty()) return;
    ++(idx.at(x));
    if (idx.at(x) >= numSamples) idx.at(x) = 0;
    timePoints.at(x)[idx.at(x)].start = ProfilerClock::now();
}

void Profiler::End(const string &x) {
    LOG_DXRT_DBG << x << endl;
    unique_lock<mutex> lk(_lock);
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
    if(timePoints.find(x)!=timePoints.end())
    {
        int idx_ = idx.at(x);
        int ret = chrono::duration_cast<chrono::microseconds>(timePoints.at(x)[idx_].end - timePoints.at(x)[idx_].start).count();
        if(ret<0) ret = 0;
        return ret;
    }
    else
    {
        return 0;
    }
}

double Profiler::GetAverage(const string &x)
{
    double avgValue = 0, sum = 0;
    if(!timePoints.empty())
    {
        vector<uint64_t> durations;
        auto tps = timePoints.at(x);
        for(auto &tp:tps)
        {
            if( tp.start.time_since_epoch().count()==0 || tp.end.time_since_epoch().count()==0 ) continue;
            int duration = chrono::duration_cast<chrono::microseconds>(tp.end-tp.start).count();
            if(duration>0)
            {
                durations.emplace_back( duration );
                sum += duration;
            }
        }
        avgValue = sum/durations.size();
    }
    return avgValue;
}

void Profiler::Erase(const string &x)
{
    if(!timePoints.empty())
    {
        if(timePoints.find(x)!=timePoints.end())
        {
            timePoints.erase(x);
        }
    }
}

void Profiler::Clear(void)
{
    ;
}

void Profiler::Show(bool showDurations)
{
    LOG_DXRT_DBG << "profiler" << endl;
    if(!timePoints.empty())
    {
        std::cout << "  -------------------------------------------------------------------------------" << std::endl;
        std::cout << "  |           Name                 |  min (us)    |  max (us)    | average (us) |" << std::endl;
        std::cout << "  -------------------------------------------------------------------------------" << std::endl;
        map<string, vector<TimePoint>>::iterator iter;
        for(iter=timePoints.begin();iter!=timePoints.end();iter++)
        {
            string name = iter->first;
            uint64_t minValue, maxValue;
            double avgValue = 0, sum = 0;
            vector<uint64_t> durations;
            auto tps = iter->second;
            for(auto &tp:tps)
            {
                if( tp.start.time_since_epoch().count()==0 || tp.end.time_since_epoch().count()==0 ) continue;
                int duration = chrono::duration_cast<chrono::microseconds>(tp.end-tp.start).count();
                if(duration>0)
                {
                    durations.emplace_back( duration );
                    sum += duration;
                }
            }
            if(durations.empty())
                continue;
            minValue = *min_element( durations.begin(), durations.end() );
            maxValue = *max_element( durations.begin(), durations.end() );
            avgValue = sum/durations.size();
            cout << "  | " << dec << setw(30) << name.substr(0, 28) << " | " << setw(12) << minValue \
                    << " | " << setw(12) << maxValue << " | " << setw(12) << avgValue << " | ";
            if(showDurations || PROFILER_FORCE_SHOW_DURATIONS)
            {
                for(auto &duration:durations)
                {
                    cout << duration << ", ";
                }
            }
            cout << endl;
        }
        std::cout << "  -------------------------------------------------------------------------------" << std::endl;
    }
}

void Profiler::Save(const string &filename)
{
    if(timePoints.empty()) return;
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
            if( tp.start.time_since_epoch().count()==0 || tp.end.time_since_epoch().count()==0 ) continue;
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
        std::cout << "Profiler data has been written to " << filename << std::endl;
    } else {
        std::cerr << "Failed to open output file" << std::endl;
    }
}
uint8_t DEBUG_DATA = 0;
uint8_t SKIP_INFERENCE_IO = 0;

} // namespace dxrt