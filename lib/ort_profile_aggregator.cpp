/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 */

#include "dxrt/ort_profile_aggregator.h"

#include <algorithm>
#include <atomic>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#ifdef __linux__
    #include <unistd.h>
#elif _WIN32
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #include <windows.h>
#endif

#include "dxrt/extern/rapidjson/document.h"
#include "dxrt/extern/rapidjson/istreamwrapper.h"

namespace dxrt
{

namespace
{

struct NodeStat
{
    uint64_t count = 0;
    uint64_t total_us = 0;
    uint64_t min_us = (std::numeric_limits<uint64_t>::max)();
    uint64_t max_us = 0;
    std::vector<uint64_t> samples;   // for percentile
    std::string op_name;
    std::string node_name;
};

std::string envOr(const char* key, const std::string& fallback)
{
    const char* v = std::getenv(key);
    if (v == nullptr || *v == '\0')
    {
        return fallback;
    } // @no_else: guard clause
    return std::string(v);
}

std::string defaultOutputDir()
{
#ifdef _WIN32
    const char* tempDir = std::getenv("TEMP");
    if (tempDir != nullptr && *tempDir != '\0')
    {
        return std::string(tempDir);
    } // @no_else: guard clause

    const char* tmpDir = std::getenv("TMP");
    if (tmpDir != nullptr && *tmpDir != '\0')
    {
        return std::string(tmpDir);
    } // @no_else: guard clause

    return ".";
#else
    return "/tmp";
#endif
}

unsigned long currentProcessId()
{
#ifdef _WIN32
    return static_cast<unsigned long>(::GetCurrentProcessId());
#else
    return static_cast<unsigned long>(::getpid());
#endif
}

bool envTruthy(const char* key)
{
    const char* v = std::getenv(key);
    if (v == nullptr)
    {
        return false;
    } // @no_else: guard clause
    std::string s(v);
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return (s == "1" || s == "on" || s == "true" || s == "yes");
}

uint64_t percentile(std::vector<uint64_t>& v, double p)
{
    if (v.empty())
    {
        return 0;
    } // @no_else: guard clause
    size_t idx = static_cast<size_t>(std::floor((p / 100.0) * (v.size() - 1)));
    std::nth_element(v.begin(), v.begin() + idx, v.end());
    return v[idx];
}

void parseFile(const std::string& path,
               std::map<std::string, NodeStat>& stats)
{
    std::ifstream ifs(path);
    if (!ifs.is_open())
    {
        std::cerr << "[ORT-PROF] failed to open " << path << std::endl;
        return;
    } // @no_else: guard clause

    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document doc;
    doc.ParseStream(isw);
    if (doc.HasParseError() || !doc.IsArray())
    {
        std::cerr << "[ORT-PROF] invalid JSON: " << path << std::endl;
        return;
    } // @no_else: guard clause

    for (const auto& ev : doc.GetArray())
    {
        if (!ev.IsObject())
        {
            continue;
        } // @no_else: guard clause
        if (!ev.HasMember("cat") || !ev["cat"].IsString())
        {
            continue;
        } // @no_else: guard clause
        if (std::string(ev["cat"].GetString()) != "Node")
        {
            continue;
        } // @no_else: guard clause
        if (!ev.HasMember("args") || !ev["args"].IsObject())
        {
            continue;
        } // @no_else: guard clause
        if (!ev.HasMember("dur") || !ev["dur"].IsNumber())
        {
            continue;
        } // @no_else: guard clause

        const auto& args = ev["args"];
        // Only keep outer Node events (those that carry op_name). Sub-events
        // like "*_kernel_time" share name but may not carry op_name.
        if (!args.HasMember("op_name") || !args["op_name"].IsString())
        {
            continue;
        } // @no_else: guard clause

        std::string op_name = args["op_name"].GetString();
        std::string node_name;
        if (args.HasMember("node_name") && args["node_name"].IsString())
        {
            node_name = args["node_name"].GetString();
        }
        else if (ev.HasMember("name") && ev["name"].IsString())
        {
            node_name = ev["name"].GetString();
        } // @no_else: node_name remains empty if neither condition is met

        // Filter: many ORT builds emit both a "fence_before/after" and a
        // "kernel_time" event per node. To avoid double-counting, keep only
        // the *_kernel_time event when the event name ends with it; otherwise
        // fall back to the plain node event.
        std::string ev_name = ev.HasMember("name") && ev["name"].IsString()
                               ? ev["name"].GetString() : "";
        const std::string kernel_suffix = "_kernel_time";
        bool is_kernel = ev_name.size() >= kernel_suffix.size() &&
                        ev_name.compare(ev_name.size() - kernel_suffix.size(),
                                       kernel_suffix.size(), kernel_suffix) == 0;
        if (!is_kernel)
        {
            continue;
        } // @no_else: guard clause

        std::string key = op_name + "|" + node_name;
        auto& st = stats[key];
        if (st.count == 0)
        {
            st.op_name = op_name;
            st.node_name = node_name;
        } // @no_else: only initialize on first occurrence
        uint64_t dur = static_cast<uint64_t>(ev["dur"].GetInt64());
        st.count += 1;
        st.total_us += dur;
        if (dur < st.min_us)
        {
            st.min_us = dur;
        } // @no_else: conditional update
        if (dur > st.max_us)
        {
            st.max_us = dur;
        } // @no_else: conditional update
        st.samples.push_back(dur);
    }
}

void writeReport(std::ostream& os,
                 std::map<std::string, NodeStat>& stats)
{
    // Sort by total time desc
    std::vector<NodeStat*> sorted;
    sorted.reserve(stats.size());
    uint64_t grand_total = 0;
    for (auto& kv : stats)
    {
        sorted.push_back(&kv.second);
        grand_total += kv.second.total_us;
    }
    std::sort(sorted.begin(), sorted.end(),
              [](const NodeStat* a, const NodeStat* b) { return a->total_us > b->total_us; });

    os << "================ ONNX Runtime Node-Level Profiling Summary ================\n";
    os << "Unit: microseconds (us). Node kernel_time events, aggregated across all sessions.\n";
    os << "Grand total kernel time: " << grand_total << " us\n\n";
    os << std::left
       << std::setw(6)  << "rank"
       << std::setw(14) << "op"
       << std::setw(50) << "node_name"
       << std::right
       << std::setw(8)  << "count"
       << std::setw(12) << "total"
       << std::setw(10) << "avg"
       << std::setw(10) << "min"
       << std::setw(10) << "max"
       << std::setw(10) << "p50"
       << std::setw(10) << "p95"
       << std::setw(8)  << "%tot"
       << "\n";
    os << std::string(148, '-') << "\n";

    int rank = 1;
    for (auto* s : sorted)
    {
        uint64_t avg = s->count > 0 ? (s->total_us / s->count) : 0;
        uint64_t p50 = percentile(s->samples, 50.0);
        uint64_t p95 = percentile(s->samples, 95.0);
        double pct = grand_total > 0
                   ? (100.0 * static_cast<double>(s->total_us) / static_cast<double>(grand_total))
                   : 0.0;

        std::string nn = s->node_name;
        if (nn.size() > 48)
        {
            nn = nn.substr(0, 45) + "...";
        } // @no_else: only truncate long names

        os << std::left
           << std::setw(6)  << rank++
           << std::setw(14) << s->op_name
           << std::setw(50) << nn
           << std::right
           << std::setw(8)  << s->count
           << std::setw(12) << s->total_us
           << std::setw(10) << avg
           << std::setw(10) << (s->min_us == (std::numeric_limits<uint64_t>::max)() ? 0 : s->min_us)
           << std::setw(10) << s->max_us
           << std::setw(10) << p50
           << std::setw(10) << p95
           << std::setw(7)  << std::fixed << std::setprecision(2) << pct << "%"
           << "\n";
    }
    os << std::string(148, '=') << "\n";
}

} // anonymous namespace

OrtProfileAggregator& OrtProfileAggregator::GetInstance()
{
    static OrtProfileAggregator instance;
    return instance;
}

OrtProfileAggregator::OrtProfileAggregator()
{
    _enabled = envTruthy("DXRT_ORT_PROFILING");
    _keepRaw = envTruthy("DXRT_ORT_PROFILE_KEEP_RAW");
    _outputDir = envOr("DXRT_ORT_PROFILE_DIR", defaultOutputDir());
    if (_enabled)
    {
        std::cout << "[ORT-PROF] enabled (output dir=" << _outputDir
                  << ", keep_raw=" << (_keepRaw ? "ON" : "OFF") << ")" << std::endl;
    } // @no_else: logging only when enabled
}

OrtProfileAggregator::~OrtProfileAggregator()
{
    if (!_enabled)
    {
        return;
    } // @no_else: guard clause
    try
    {
        aggregateAndReport();
    }
    catch (const std::exception& e)
    {
        std::cerr << "[ORT-PROF] aggregation error: " << e.what() << std::endl;
    }
}

std::string OrtProfileAggregator::MakeProfilePrefix(const std::string& taskName)
{
    std::lock_guard<std::mutex> lk(_mutex);
    std::ostringstream ss;
    ss << _outputDir << "/dxrt_ort_"
    << currentProcessId() << "_"
       << (_prefixCounter++) << "_";
    // Append task name (sanitized) for readability
    for (char c : taskName)
    {
        ss << (std::isalnum(static_cast<unsigned char>(c)) ? c : '_');
    }
    ss << "_";
    return ss.str();
}

void OrtProfileAggregator::RegisterProfileFile(const std::string& path)
{
    std::lock_guard<std::mutex> lk(_mutex);
    _profileFiles.push_back(path);
}

void OrtProfileAggregator::aggregateAndReport()
{
    std::vector<std::string> files;
    {
        std::lock_guard<std::mutex> lk(_mutex);
        files = _profileFiles;
    }

    if (files.empty())
    {
        std::cout << "[ORT-PROF] no profile files collected." << std::endl;
        return;
    } // @no_else: guard clause

    std::map<std::string, NodeStat> stats;
    for (const auto& f : files)
    {
        parseFile(f, stats);
    }

    // Write to stdout
    writeReport(std::cout, stats);

    // Also write to file
    std::string report_path = _outputDir + "/dxrt_ort_node_stats.txt";
    std::ofstream ofs(report_path);
    if (ofs.is_open())
    {
        writeReport(ofs, stats);
        std::cout << "[ORT-PROF] report saved: " << report_path << std::endl;
    } // @no_else: silently skip if file cannot be opened

    // Optionally remove raw JSON files
    if (!_keepRaw)
    {
        for (const auto& f : files)
        {
            if (std::remove(f.c_str()) != 0)
            {
                std::cerr << "[ORT-PROF] failed to remove " << f << std::endl;
            } // @no_else: only report failure
        }
    }
    else
    {
        std::cout << "[ORT-PROF] raw profile files retained under " << _outputDir << std::endl;
    }
}

} /* namespace dxrt */
