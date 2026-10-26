/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 *
 * This file uses cxxopts (MIT License) - Copyright (c) 2014 Jarryd Beck.
 */
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <memory>
#include <chrono>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <stdexcept>
#include <iomanip>

#include "dxrt/dxrt_cxx_api.h"
#include "dxrt/extern/cxxopts.hpp"

// Local constants replacing internal macros.
// TODO: expose via public API (dxrt_get_bound_inf_max / dxrt_get_task_max_load_limit)
// when these values change in driver.h. Current values verified against internal defs.
static constexpr int kBoundInfMax = 7;       // == dxrt::N_BOUND_INF_MAX (driver.h)
static constexpr int kTaskMaxLoadLimit = 100; // == DXRT_TASK_MAX_LOAD_LIMIT (common.h)
static int getTaskMaxLoadValue() { return dxrt::get_task_max_load(); }

// --max-throughput sweep defaults
// kMtDefaultStart=3: below 3 in-flight requests at least one NPU core sits idle on a
// 3-core device, so 1-2 can never be optimal; measured optima go as low as 3-4 regardless
// of device core count, but the sweep only moves upward, so starting at 6 could never reach them.
static constexpr int    kMtDefaultStart     = 3;                  // starting buffer count
static constexpr int    kMtDefaultStep      = 1;                  // buffer count increment per round
static constexpr double kMtDefaultThreshold = 3.0;                // stop once FPS falls this far below the peak (%)
static constexpr double kMtDefaultTolerance = 1.0;                // FPS within this % of another is treated as equal
static constexpr int    kMtDefaultCap       = kTaskMaxLoadLimit;  // maximum buffer count
static constexpr double kMtDefaultRoundSec  = 2.0;                // measurement seconds per round
static constexpr int    kMtDefaultRepeatCount = 3;                // measurements averaged per buffer count
static constexpr int    kMtDefaultPatience  = 2;                  // consecutive below-peak rounds before stop
static constexpr int    kMtDefaultStall     = 3;                  // consecutive rounds without a new peak before stop
static constexpr int    kMtDefaultCoolDownTemp = 45;              // wait until NPU temperature is below this ('C) before each measurement
static constexpr int    kMtMaxCoolDownTemp  = 125;                // upper bound accepted for --cool-down-temp ('C)
static constexpr int    kMtCoolDownTimeoutSec = 300;              // give up (and fail the sweep) if not cooled within this

// Local file-I/O helpers (replace internal filesys_support)
static int64_t localGetFileSize(const std::string& filename) {
    std::ifstream f(filename, std::ios::binary | std::ios::ate);
    if (!f) return -1;
    return static_cast<int64_t>(f.tellg());
}

static bool localDataFromFile(const std::string& filename, void* dst, size_t size) {
    std::ifstream f(filename, std::ios::binary);
    if (!f) return false;
    f.read(static_cast<char*>(dst), static_cast<std::streamsize>(size));
    return static_cast<size_t>(f.gcount()) == size;
}

static void localDataDumpBin(const std::string& filename, const dxrt::TensorPtrs& outputs) {
    std::ofstream f(filename, std::ios::binary);
    if (!f) {
        std::cerr << "[ERR] Cannot open output file: " << filename << std::endl;
        return;
    }
    for (const auto& t : outputs) {
        if (t && t->data() && t->size_in_bytes() > 0)
            f.write(static_cast<const char*>(t->data()), static_cast<std::streamsize>(t->size_in_bytes()));
    }
}


#define APP_NAME "DXRT " DXRT_VERSION
// #define TARGET_FPS_DEBUG

using std::cout;
using std::endl;
using std::vector;
using std::shared_ptr;
using std::string;

enum RunModelMode {
    BENCHMARK_MODE      = 0,
    SINGLE_MODE         = 1,
    TARGET_FPS_MODE     = 2,
    MAX_THROUGHPUT_MODE = 3,
};
static RunModelMode mode;
static int bounding = 0;

// linux host info
#ifdef __linux__
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <cstdio>
#include <array>

void printCpuInfo() {
    std::cout << "--- CPU Information ---" << std::endl;

    // Use popen to execute lscpu command
    std::array<char, 128> buffer;
    std::string result;
    auto pipeCloser = [](FILE* fp) {
        if (fp) {
            pclose(fp);
        }
    };
    std::unique_ptr<FILE, decltype(pipeCloser)> pipe(popen("lscpu 2>/dev/null", "r"), pipeCloser);

    if (!pipe) {
        std::cerr << "... No CPU Info." << std::endl;
        return;
    }

    // Read the output of lscpu
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    if (result.empty()) {
        std::cerr << "... No CPU Info." << std::endl;
        return;
    }

    // Parse and display relevant information
    std::istringstream stream(result);
    std::string line;
    bool architectureFound = false;
    bool modelNameFound = false;
    bool cpuCoresFound = false;
    bool vendorIdFound = false;

    while (std::getline(stream, line)) {
        // Architecture
        if (!architectureFound && line.find("Architecture:") != std::string::npos) {
            std::cout << "  " << line << std::endl;
            architectureFound = true;
        }
        // Model name
        else if (!modelNameFound && line.find("Model name:") != std::string::npos) {
            std::cout << "  " << line << std::endl;
            modelNameFound = true;
        }
        // CPU(s) - total number of logical CPUs
        else if (!cpuCoresFound && line.find("CPU(s):") != std::string::npos && line.find("NUMA") == std::string::npos && line.find("On-line") == std::string::npos) {
            std::cout << "  " << line << std::endl;
            cpuCoresFound = true;
        }
        // Vendor ID
        else if (!vendorIdFound && line.find("Vendor ID:") != std::string::npos) {
            std::cout << "  " << line << std::endl;
            vendorIdFound = true;
        }
    }

    if (!modelNameFound && !architectureFound && !cpuCoresFound) {
        std::cerr << "... No CPU Info." << std::endl;
    }
}

// cpu architecture info
void printArchitectureInfo() {
    std::cout << "\n--- Architecture Information ---" << std::endl;
    struct utsname buffer;

    if (uname(&buffer) == 0) {
        std::cout << "  System Name: " << buffer.sysname << std::endl;
        std::cout << "  Node Name:   " << buffer.nodename << std::endl;
        std::cout << "  Release:     " << buffer.release << std::endl;
        std::cout << "  Version:     " << buffer.version << std::endl;
        std::cout << "  Machine:     " << buffer.machine << std::endl;  // architecture information
    }
}

void printMemoryInfo() {
    std::cout << "\n--- Memory Information ---" << std::endl;
    struct sysinfo memInfo;

    if (sysinfo(&memInfo) == 0) {
        // total physical memory (bytes)
        long long totalPhysMem = static_cast<long long>(memInfo.totalram) * memInfo.mem_unit;
        // availabe physical memory (bytes)
        long long availPhysMem = static_cast<long long>(memInfo.freeram) * memInfo.mem_unit;
        // total swap space (bytes)
        long long totalSwap = static_cast<long long>(memInfo.totalswap) * memInfo.mem_unit;
        // available swap space (bytes)
        long long freeSwap = static_cast<long long>(memInfo.freeswap) * memInfo.mem_unit;

        // byte --> GB
        std::cout << std::fixed << std::setprecision(2);

        std::cout << "  Total Physical Memory: " << static_cast<double>(totalPhysMem) / (1024 * 1024 * 1024) << " GB" << std::endl;
        std::cout << "  Available Physical Memory: " << static_cast<double>(availPhysMem) / (1024 * 1024 * 1024) << " GB" << std::endl;
        std::cout << "  Total Swap Space: " << static_cast<double>(totalSwap) / (1024 * 1024 * 1024) << " GB" << std::endl;
        std::cout << "  Free Swap Space: " << static_cast<double>(freeSwap) / (1024 * 1024 * 1024) << " GB" << std::endl;
        std::cout << std::endl;

    } else {
        std::cerr << "No System Memory Info." << std::endl;
    }
}

#endif  //  __linux__

std::ostream& operator<<(std::ostream& os, RunModelMode mode) {
    switch (mode) {
        case BENCHMARK_MODE:      os << "Benchmark Mode"; break;
        case SINGLE_MODE:         os << "Single Mode"; break;
        case TARGET_FPS_MODE:     os << "Target FPS Mode"; break;
        case MAX_THROUGHPUT_MODE: os << "Max-Throughput Mode"; break;
        default:                  os << "Unknown Mode"; break;
    }
    return os;
}

struct ReportConfig
{
    int64_t interval_sec = 0;
    std::string input_file;
    std::string output_file;
    std::string model_file;
    bool verbose = false;
};

std::string float_to_string_fixed(float value, int precision) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << value;
    return oss.str();
}

void PrintInfResult(const std::string& inputFile, const std::string& outputFile, const std::string& modelFile,
                    float e2eMs, float coreLatencyMs, float infTimeMs, float queueWaitMs, float fps_val, int64_t loops, RunModelMode current_mode, bool verbose) {
    std::vector<std::string> lines;
    (void)modelFile;

    if (dxrt::Configuration::GetInstance().GetEnable(dxrt::Configuration::ITEM::SHOW_PROFILE))
        verbose = true;

    const std::string desc_npu_time = "Actual NPU core computation time for a single request";
    const std::string desc_e2e = "End-to-end time per request including data transfer and overheads";
    const std::string desc_core_latency = "Core latency per request excluding buffer pool and queue wait times";
    const std::string desc_queue_wait = "Time spent waiting in NPU/CPU task queues; included in E2E Response Time";
    const std::string desc_fps = "Overall user-observed inference throughput (inputs/second), reflecting perceived speed";

    const int description_parenthesis_start_column = 45;

    auto build_formatted_line =
        [&](const std::string& label,
            const std::string& value_str,
            const std::string& unit_str,
            const std::string& description) -> std::string {
        std::string value_with_unit = value_str + (unit_str.empty() ? "" : " " + unit_str);
        std::string core_content = label + value_with_unit;

        std::string line = core_content;
        int current_length = static_cast<int>(core_content.length());
        int spaces_to_add = description_parenthesis_start_column - current_length;

        if (spaces_to_add <= 0) {
            spaces_to_add = 1;
        }

        line += std::string(spaces_to_add, ' ');
        if (verbose)
            line += "(" + description + ")";
        return line;
    };

    std::string infTimeStr = float_to_string_fixed(infTimeMs, 3);
    std::string e2eStr = float_to_string_fixed(e2eMs, 3);
    std::string latencyStr = float_to_string_fixed(coreLatencyMs, 3);
    std::string queueWaitStr = float_to_string_fixed(queueWaitMs, 3);
    std::string fpsStr     = float_to_string_fixed(fps_val, 2);

    if (!inputFile.empty()) {
        lines.push_back("* Processing File : " + inputFile);
        lines.push_back("* Output Saved As : " + outputFile);
    }

    if (current_mode == SINGLE_MODE)
    {
        lines.push_back("* Benchmark Result (single input)");
        if (verbose) {
            lines.push_back(build_formatted_line("  - NPU Processing Time  : ", infTimeStr, "ms", desc_npu_time));
            lines.push_back(build_formatted_line("  - Queue Wait Time      : ", queueWaitStr, "ms", desc_queue_wait));
            lines.push_back(build_formatted_line("  - E2E Response Time    : ", e2eStr, "ms", desc_e2e));
            lines.push_back(build_formatted_line("  - Latency              : ", latencyStr, "ms", desc_core_latency));
            lines.push_back(build_formatted_line("  - FPS                  : ", fpsStr, "", desc_fps));
        }
        else
        {
            lines.push_back(build_formatted_line("  - FPS : ", fpsStr, "", desc_fps));
        }
    } else {
        lines.push_back("* Benchmark Result (" + std::to_string(loops) + " inputs)");
        if (verbose) {
            lines.push_back(build_formatted_line("  - NPU Processing Time Average : ", infTimeStr, "ms", desc_npu_time));
            lines.push_back(build_formatted_line("  - Queue Wait Time Average     : ", queueWaitStr, "ms", desc_queue_wait));
            lines.push_back(build_formatted_line("  - E2E Response Time Average   : ", e2eStr, "ms", desc_e2e));
            lines.push_back(build_formatted_line("  - Latency Average             : ", latencyStr, "ms", desc_core_latency));
            lines.push_back(build_formatted_line("  - FPS                         : ", fpsStr, "", desc_fps));
        }
        else
        {
            lines.push_back(build_formatted_line("  - FPS : ", fpsStr, "", desc_fps));
        }
    }

    size_t maxLength = 0;
    for (const auto& line : lines) {
        maxLength = (std::max)(maxLength, line.length());
    }

    std::cout << std::string(maxLength, '=') << std::endl;
    for (const auto& line : lines) {
        std::cout << line << std::endl;
    }
    std::cout << std::string(maxLength, '=') << std::endl;
}

class PeriodicReporter
{
public:
    PeriodicReporter(dxrt::InferenceEngine& ie,
                     const ReportConfig& cfg,
                     std::function<int64_t()> getCount,
                     std::chrono::steady_clock::time_point startTime,
                     RunModelMode runMode)
        : _ie(ie)
        , _cfg(cfg)
        , _get_count(std::move(getCount))
        , _start(startTime)
        , _mode(runMode)
        , _stop(false)
    {
        if (_cfg.interval_sec <= 0) return;
        _thread = std::thread([this]() { loop(); });
    }

    ~PeriodicReporter()
    {
        stopAndJoin();
    }

    PeriodicReporter(const PeriodicReporter&) = delete;
    PeriodicReporter& operator=(const PeriodicReporter&) = delete;
    PeriodicReporter(PeriodicReporter&&) = delete;
    PeriodicReporter& operator=(PeriodicReporter&&) = delete;

    void stopAndJoin()
    {
        std::call_once(_stop_once, [this]()
        {
            {
                std::lock_guard<std::mutex> lock(_m);
                _stop.store(true);
            }
            _cv.notify_all();
            if (_thread.joinable()) _thread.join();
        });
    }

private:
    void loop()
    {
        int seq = 0;
        while (true)
        {
            {
                std::unique_lock<std::mutex> lock(_m);
                _cv.wait_for(lock, std::chrono::seconds(_cfg.interval_sec),
                             [this] { return _stop.load(); });
                if (_stop.load()) break;
            }
            ++seq;

            auto now = std::chrono::steady_clock::now();
            double elapsed_us = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(now - _start).count());
            int64_t count = _get_count();
            float fps = (elapsed_us > 0 && count > 0) ? static_cast<float>(1e6 * count / elapsed_us) : 0.0f;
            float e2e_ms = static_cast<float>(_ie.GetE2EResponseTimeMean() / 1000.0);
            float core_latency_ms = static_cast<float>(_ie.GetCoreLatencyMean() / 1000.0);
            float npu_ms = static_cast<float>(_ie.GetNpuInferenceTimeMean() / 1000.0);
            float queue_wait_ms = static_cast<float>(_ie.GetQueueWaitTimeMean() / 1000.0);

            std::cout << "\n[Periodic Report #" << seq << "] cumulative elapsed="
                      << float_to_string_fixed(static_cast<float>(elapsed_us / 1e6), 3)
                      << "s, completions=" << count << std::endl;
            PrintInfResult(_cfg.input_file, _cfg.output_file, _cfg.model_file,
                           e2e_ms, core_latency_ms, npu_ms, queue_wait_ms, fps, count, _mode, _cfg.verbose);
            std::cout << std::endl;
        }
    }

    dxrt::InferenceEngine& _ie;
    const ReportConfig& _cfg;
    std::function<int64_t()> _get_count;
    std::chrono::steady_clock::time_point _start;
    RunModelMode _mode;
    std::atomic<bool> _stop;
    std::once_flag _stop_once;
    std::mutex _m;
    std::condition_variable _cv;
    std::thread _thread;
};


void SetRunModelMode(bool single, int targetFps, bool maxThroughput)
{
    if (single) {
        mode = SINGLE_MODE;
    } else if (maxThroughput) {
        mode = MAX_THROUGHPUT_MODE;
    } else if (targetFps) {
        mode = TARGET_FPS_MODE;
    } else {
        mode = BENCHMARK_MODE;
    }
    cout << "Run model target mode : " << mode << endl;
}

struct MultiInputBuffers
{
    bool is_multi_input = false;
    std::vector<std::vector<uint8_t>> buffers;
    std::vector<void*> ptrs;
};

static MultiInputBuffers prepareMultiInputBuffers(dxrt::InferenceEngine& ie, void* inputBuffer)
{
    MultiInputBuffers result;
    result.is_multi_input = ie.IsMultiInputModel();
    if (!result.is_multi_input)
    {
        return result;
    }

    auto tensor_sizes = ie.GetInputTensorSizes();
    result.buffers.resize(tensor_sizes.size());
    uint64_t offset = 0;
    for (size_t idx = 0; idx < tensor_sizes.size(); ++idx)
    {
        result.buffers[idx].resize(tensor_sizes[idx], 0);
        if (inputBuffer != nullptr)
        {
            std::memcpy(result.buffers[idx].data(),
                        static_cast<uint8_t*>(inputBuffer) + offset,
                        tensor_sizes[idx]);
            offset += tensor_sizes[idx];
        }
        result.ptrs.push_back(result.buffers[idx].data());
    }
    return result;
}

static float runBenchmarkByTime(int64_t& outLoops, dxrt::InferenceEngine& ie, void* inputBuffer,
                                double targetDurationSec, const ReportConfig& reportCfg = {})
{
    float fps = 0;

    int64_t target_duration_ms = static_cast<int64_t>(targetDurationSec * 1000);

    // Shared with the callback: if RunAsync throws, in-flight callbacks outlive this frame.
    struct CallbackState
    {
        int64_t run_count = 0;
        int64_t cb_count = 0;
        bool run_complete = false;
        std::mutex mutex;
        std::condition_variable cv;
    };
    auto state = std::make_shared<CallbackState>();

    ie.RegisterCallback([state](dxrt::TensorPtrs &outputs, void *userArg) {

        std::ignore = outputs;
        std::ignore = userArg;

        // check competion of run & callback
        std::unique_lock<std::mutex> lock(state->mutex);
        state->cb_count++;
        if ( state->run_complete && state->cb_count == state->run_count)
            state->cv.notify_one();

        return 0;
    });

    // Pre-allocate multi-input buffers before the loop
    auto mi = prepareMultiInputBuffers(ie, inputBuffer);

    auto start = std::chrono::steady_clock::now();

    PeriodicReporter reporter(ie, reportCfg,
        [state]() -> int64_t {
            std::lock_guard<std::mutex> lock(state->mutex);
            return state->cb_count;
        },
        start, BENCHMARK_MODE);

    for(;;) // no limit
    {
        if (mi.is_multi_input)
        {
            ie.RunAsyncMultiInput(mi.ptrs);
        }
        else
        {
            ie.RunAsync(inputBuffer);
        }

        {
            std::unique_lock<std::mutex> lock(state->mutex);
            state->run_count++;
        }

        auto current = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> duration = current - start;

        if ( duration.count() > target_duration_ms )
        {
            break;
        }

    } // for i

    // Release the mutex before stopping the reporter, whose count getter takes it.
    int64_t run_count = 0;
    {
        std::unique_lock<std::mutex> lock(state->mutex);
        state->run_complete = true;
        state->cv.wait(lock, [&state](){ return state->cb_count == state->run_count; } );
        run_count = state->run_count;
    }

    auto current = std::chrono::steady_clock::now();
    uint64_t infTime = std::chrono::duration_cast<std::chrono::microseconds>(current - start).count();
    fps = static_cast<float>(1000000.0 * run_count/infTime);

    ie.RegisterCallback(nullptr);

    reporter.stopAndJoin();

    outLoops = run_count;
    std::cout << "Inference by time: total-inference-time=" << infTime / 1000000.0 << "(s)" << " total-loops=" << outLoops << std::endl;

    return fps;
}

static float runAsyncTargetFPS(int64_t& outLoops, dxrt::InferenceEngine& ie, int targetFps, void* inputBuffer,
                               int64_t targetDurationSec)
{

#ifdef TARGET_FPS_DEBUG
    vector<string> results;  // Company and time storage
#endif
    uint64_t infTime = 0;
    int64_t target_duration_ms = targetDurationSec * 1000;
    int64_t run_count = 0; // run async count
    int64_t cb_count = 0; // callback count
    float fps = 0;

    std::mutex cv_mutex;
    std::condition_variable cv;

    std::function<int(vector<shared_ptr<dxrt::Tensor>>, void*)> postProcCallBack = \
        [&cv_mutex, &cv, &cb_count, &run_count, outLoops](vector<shared_ptr<dxrt::Tensor>> outputs, void *arg)
        {
            (void)arg;
            std::ignore = outputs;

            std::lock_guard<std::mutex> lock(cv_mutex);
            cb_count++;
            if (cb_count == run_count) {
                cv.notify_one();
            }
            return 0;
        };

    ie.RegisterCallback(postProcCallBack);

    // Pre-allocate multi-input buffers before the loop
    auto mi = prepareMultiInputBuffers(ie, inputBuffer);

    auto start_clock = std::chrono::steady_clock::now();

    for(int64_t i = 0; ; ++i)
    {
#ifdef TARGET_FPS_DEBUG
        auto loopStartTime = std::chrono::steady_clock::now();  // Start time for this loop
#endif

        if (mi.is_multi_input)
        {
            (void)ie.RunAsyncMultiInput(mi.ptrs);
        }
        else
        {
            (void)ie.RunAsync(inputBuffer, 0);
        }
        run_count ++;

#ifdef TARGET_FPS_DEBUG
        auto elapsed = std::chrono::steady_clock::now() - loopStartTime;
        auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
        results.push_back("Iteration " + to_string(i + 1) + ": " + inputFile + " -> " + outputFile +
                        ", Time: " + to_string(elapsedMs) + " ms");
#endif
        // Calculate the expected time per frame
        if (targetFps > 0)
        {
            auto targetTime = 1000 / targetFps;  // in milliseconds
            auto totalElapsed = std::chrono::steady_clock::now() - start_clock;
            auto totalElapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(totalElapsed).count();

            if (totalElapsedMs < (i + 1) * targetTime)
            {
                auto sleepDuration = (i + 1) * targetTime - totalElapsedMs;
                std::this_thread::sleep_for(std::chrono::milliseconds(sleepDuration));
            }
        }

        // inference by time
        if ( targetDurationSec > 0 )
        {
            auto current = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::milli> duration = current - start_clock;

            if ( duration.count() > target_duration_ms )
            {
                break;
            }
        } //
        else if ( i == (outLoops - 1) ) // inference by loops
        {
            break;
        }
    }
    auto end_clock = std::chrono::steady_clock::now();
    std::unique_lock<std::mutex> lock(cv_mutex);
    cv.wait(lock, [&cb_count, &run_count]{
        return cb_count == run_count;
    });

    infTime = std::chrono::duration_cast<std::chrono::microseconds>(end_clock - start_clock).count();
    if ( targetDurationSec > 0 )
    {
        outLoops = run_count;
        std::cout << "Inference by time: total-inference-time=" << infTime / 1000000.0 << "(s)" << " total-loops=" << outLoops << std::endl;
    }
    fps = static_cast<float>(1000000.0 * outLoops / infTime);

#ifdef TARGET_FPS_DEBUG
    for (const auto& result : results) {
        cout << result << endl;
    }
#endif

    return fps;

}

// Guards the "nothing ran" case: a measurement that exited without a single
// successful inference must not be reported as a result.
static bool ensureInferenceRan(float fps, int64_t completed)
{
    if (completed > 0 && fps > 0.0f) return true;
    std::cerr << "[ERR] No inference completed successfully (completions=" << completed
              << ", fps=" << float_to_string_fixed(fps, 2) << ")." << std::endl;
    return false;
}

struct MaxThroughputOption
{
    int start_count = kMtDefaultStart;
    int step = kMtDefaultStep;
    double threshold_pct = kMtDefaultThreshold;
    int cap = kMtDefaultCap;
    double round_sec = kMtDefaultRoundSec;
    int repeat_count = kMtDefaultRepeatCount;
    int patience = kMtDefaultPatience;
    int stall = kMtDefaultStall;
    double tolerance_pct = kMtDefaultTolerance;
    int cool_down_temp = kMtDefaultCoolDownTemp;
    bool stop_on_peak = true;
};

struct MaxThroughputRound
{
    int buffer_count;
    float fps;
};

// Stop and recommendation policy, kept apart from measurement so it is testable without an NPU.
struct MaxThroughputTracker
{
    explicit MaxThroughputTracker(const MaxThroughputOption& option) : opt(option) {}

    const MaxThroughputOption& opt;
    std::vector<MaxThroughputRound> history;
    float best_fps = 0.0f;
    int peak_buffer = 0;
    int64_t best_loops = 0;
    // Rises within the tolerance of the anchor are noise, so they do not reset the stall count.
    float anchor_fps = 0.0f;
    int drop_streak = 0;
    int stall_streak = 0;

    double dropFromPeak(float fps) const
    {
        return (best_fps > 0.0f) ? (static_cast<double>(best_fps) - fps) / best_fps * 100.0 : 0.0;
    }

    // Records a completed round; returns true and sets stopReason when the sweep should stop.
    bool observe(int bufferCount, float fps, int64_t loops, std::string& stopReason)
    {
        history.push_back(MaxThroughputRound{bufferCount, fps});
        if (fps > best_fps)
        {
            best_fps = fps;
            peak_buffer = bufferCount;
            best_loops = loops;
        }

        if (static_cast<double>(fps) > anchor_fps * (1.0 + opt.tolerance_pct / 100.0))
        {
            anchor_fps = fps;
            stall_streak = 0;
        }
        else
        {
            ++stall_streak;
        }

        // The first round establishes the peak, so it can never be a drop.
        if (history.size() > 1 && dropFromPeak(fps) >= opt.threshold_pct)
            ++drop_streak;
        else
            drop_streak = 0;

        if (!opt.stop_on_peak) return false;
        if (drop_streak >= opt.patience)
        {
            stopReason = "throughput fell "
                + float_to_string_fixed(static_cast<float>(opt.threshold_pct), 2)
                + "% or more below the peak (" + float_to_string_fixed(best_fps, 2)
                + " fps at buffer-count " + std::to_string(peak_buffer) + ") for "
                + std::to_string(opt.patience) + " consecutive rounds";
            return true;
        }
        if (stall_streak >= opt.stall)
        {
            stopReason = "no gain above " + float_to_string_fixed(static_cast<float>(opt.tolerance_pct), 2)
                + "% for " + std::to_string(opt.stall) + " consecutive rounds (peak "
                + float_to_string_fixed(best_fps, 2) + " fps at buffer-count "
                + std::to_string(peak_buffer) + ")";
            return true;
        }
        return false;
    }

    // Smallest measured buffer count whose FPS is within the tolerance of the actual peak.
    int recommend(float& recommendedFps) const
    {
        int best_buffer = peak_buffer;
        recommendedFps = best_fps;
        const double minimum_fps = static_cast<double>(best_fps) * (1.0 - opt.tolerance_pct / 100.0);
        for (const auto& round : history)
        {
            if (round.fps >= minimum_fps && round.buffer_count < best_buffer)
            {
                best_buffer = round.buffer_count;
                recommendedFps = round.fps;
            }
        }
        return best_buffer;
    }
};

// Highest temperature across all devices and NPU channels; a failed read counts as 0.
static int GetMaxNpuTemperature()
{
    static constexpr int kNpuChannelCount = 3;  // dxrt_device_get_temperature supports channels 0-2

    int max_temp = 0;
    const int device_count = dxrt::DeviceStatus::GetDeviceCount();
    for (int dev = 0; dev < device_count; ++dev)
    {
        auto status = dxrt::DeviceStatus::GetCurrentStatus(dev);
        for (int ch = 0; ch < kNpuChannelCount; ++ch)
            max_temp = (std::max)(max_temp, status.Temperature(ch));
    }
    return max_temp;
}

// Returns false if the NPU is still at or above the target after kMtCoolDownTimeoutSec seconds.
static bool WaitForTemperatureDown(int targetTemperature)
{
    int current = GetMaxNpuTemperature();
    if (current < targetTemperature)
    {
        std::cout << "[INFO] NPU temperature " << current << "'C < " << targetTemperature
                  << "'C, no cool-down needed." << std::endl;
        return true;
    }

    for (int waited = 1; waited <= kMtCoolDownTimeoutSec; ++waited)
    {
        std::cout << "[INFO] NPU temperature " << current << "'C >= " << targetTemperature
                  << "'C, waiting to cool down... (" << waited << "/" << kMtCoolDownTimeoutSec
                  << "s)" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        current = GetMaxNpuTemperature();
        if (current < targetTemperature)
        {
            std::cout << "[INFO] Cool-down complete: NPU temperature " << current << "'C < "
                      << targetTemperature << "'C." << std::endl;
            return true;
        }
    }
    std::cerr << "[ERR] NPU temperature " << current << "'C did not drop below " << targetTemperature
              << "'C within " << kMtCoolDownTimeoutSec << "s." << std::endl;
    return false;
}

// Sweep buffer counts to find the count that maximizes throughput.
// Each repeat builds and releases a dedicated InferenceEngine for the buffer count
// under test. Each round records the arithmetic mean FPS across all repeats.
// With stop_on_peak enabled, stop on a sustained drop or stall, or at the cap.
// Explicit buffer-count ranges disable peak-based stopping and run through the cap.
// Returns the smallest measured buffer count within tolerance_pct of peak throughput, or -1.
// A failed measurement or any CRITICAL runtime event invalidates the whole sweep;
// an engine-creation failure only bounds the sweep to the counts already measured.
static int runMaxThroughput(const std::string& modelFile, const dxrt::InferenceOption& baseOption,
                            void* inputBuffer, const MaxThroughputOption& mtOption,
                            const std::atomic<bool>& criticalError)
{
    if (mtOption.repeat_count < 1)
    {
        std::cerr << "[ERR] Max-throughput repeat_count must be >= 1." << std::endl;
        return -1;
    }
    if (criticalError.load())
    {
        std::cerr << "\n[ERR] A CRITICAL runtime event fired before the max-throughput sweep; "
                     "not sweeping on a device in an unknown state." << std::endl;
        return -1;
    }
    MaxThroughputTracker tracker(mtOption);

    int current = mtOption.start_count;
    std::string stop_reason;
    bool sweep_invalid = false;

    for (;;)
    {
        std::cout << "\n[max-throughput] Measuring buffer-count=" << current
                  << " for " << mtOption.round_sec << "s x " << mtOption.repeat_count
                  << " repeats ..." << std::endl;

        int64_t loops = 0;
        bool round_failed = false;
        double fps_sum = 0.0;
        for (int repeat = 0; repeat < mtOption.repeat_count; ++repeat)
        {
            dxrt::InferenceOption round_option = baseOption;
            round_option.bufferCount = current;
            round_option.showModelInfo = false;

            std::unique_ptr<dxrt::InferenceEngine> round_ie;
            try
            {
                round_ie.reset(new dxrt::InferenceEngine(modelFile, round_option));
            }
            catch (const std::exception& e)
            {
                stop_reason = "could not create an engine at buffer-count " + std::to_string(current)
                    + " (" + e.what() + "); searched up to " + std::to_string(current - 1);
                round_failed = true;
                break;
            }

            int64_t repeat_loops = 0;
            float repeat_fps = 0.0f;
            if (!WaitForTemperatureDown(mtOption.cool_down_temp))
            {
                stop_reason = "NPU did not cool below " + std::to_string(mtOption.cool_down_temp)
                    + "'C within " + std::to_string(kMtCoolDownTimeoutSec) + "s before buffer-count "
                    + std::to_string(current) + " repeat " + std::to_string(repeat + 1);
                sweep_invalid = true;
                round_failed = true;
                break;
            }
            try
            {
                repeat_fps = runBenchmarkByTime(repeat_loops, *round_ie, inputBuffer, mtOption.round_sec);
            }
            catch (const std::exception& e)
            {
                stop_reason = "measurement failed at buffer-count " + std::to_string(current)
                    + " (" + e.what() + ")";
                sweep_invalid = true;
            }
            // Destroy the engine first so events from this round's in-flight work are dispatched.
            round_ie.reset();
            if (!sweep_invalid && criticalError.load())
            {
                stop_reason = "a CRITICAL runtime event fired during buffer-count " + std::to_string(current)
                    + " repeat " + std::to_string(repeat + 1);
                sweep_invalid = true;
            }
            if (sweep_invalid)
            {
                round_failed = true;
                break;
            }

            fps_sum += repeat_fps;
            loops += repeat_loops;
            std::cout << "[max-throughput] buffer-count=" << current
                      << " repeat=" << repeat + 1 << "/" << mtOption.repeat_count
                      << " fps=" << float_to_string_fixed(repeat_fps, 2)
                      << " loops=" << repeat_loops << std::endl;
        }
        if (round_failed) break;
        float fps = static_cast<float>(fps_sum / mtOption.repeat_count);

        bool has_prev = !tracker.history.empty();
        float prev_fps = has_prev ? tracker.history.back().fps : 0.0f;
        bool stop = tracker.observe(current, fps, loops, stop_reason);

        std::cout << "[max-throughput] buffer-count=" << current
                  << " avg-fps=" << float_to_string_fixed(fps, 2)
                  << " total-loops=" << loops;
        if (has_prev)
        {
            double improvement = (prev_fps > 0.0f)
                ? (static_cast<double>(fps) - prev_fps) / prev_fps * 100.0 : 0.0;
            std::cout << " improvement=" << float_to_string_fixed(static_cast<float>(improvement), 2) << "%"
                      << " peak-drop=" << float_to_string_fixed(static_cast<float>(tracker.dropFromPeak(fps)), 2) << "%"
                      << " stall=" << tracker.stall_streak;
        }
        std::cout << std::endl;

        if (stop) break;

        int next = (std::min)(current + mtOption.step, mtOption.cap);
        if (next <= current)
        {
            stop_reason = "reached buffer-count cap (" + std::to_string(mtOption.cap) + ")";
            break;
        }
        current = next;
    }

    if (!sweep_invalid && criticalError.load())
    {
        stop_reason += (stop_reason.empty() ? "" : "; ") + std::string("a CRITICAL runtime event fired");
        sweep_invalid = true;
    }
    if (sweep_invalid)
    {
        std::cerr << "\n[ERR] Max-throughput sweep invalidated: " << stop_reason
                  << ". No buffer count is recommended from this sweep." << std::endl;
        return -1;
    }

    // No round completed, so there is no peak to report and peak_buffer is still the
    // untested start value. Signal failure instead of recommending it.
    if (tracker.history.empty())
    {
        std::cerr << "\n[ERR] Max-throughput sweep produced no successful round";
        if (!stop_reason.empty()) std::cerr << ": " << stop_reason;
        std::cerr << std::endl;
        return -1;
    }
    if (tracker.best_fps <= 0.0f)
    {
        std::cerr << "\n[ERR] Max-throughput sweep completed but every round measured 0 fps; "
                      "no buffer count can be recommended." << std::endl;
        return -1;
    }

    float recommended_fps = 0.0f;
    const int best_buffer = tracker.recommend(recommended_fps);
    const float best_fps = tracker.best_fps;
    const int peak_buffer = tracker.peak_buffer;
    const std::vector<MaxThroughputRound>& history = tracker.history;

    // Result summary
    std::vector<std::string> lines;
    lines.push_back("* Max-Throughput Sweep Result");
    lines.push_back("  Stop reason : " + stop_reason);
    lines.push_back("  FPS averaged over " + std::to_string(mtOption.repeat_count) + " measurements per buffer-count");
    lines.push_back("");
    lines.push_back("   buffer-count |    Avg FPS |  improvement |   peak-drop");
    lines.push_back("  --------------+------------+--------------+------------");
    float table_prev = 0.0f;
    for (size_t i = 0; i < history.size(); ++i)
    {
        const auto& r = history[i];
        std::ostringstream row;
        row << "  " << std::setw(13) << r.buffer_count << " | "
            << std::setw(10) << float_to_string_fixed(r.fps, 2) << " | ";
        if (i == 0 || table_prev <= 0.0f)
        {
            row << std::setw(11) << "--";
        }
        else
        {
            double imp = (static_cast<double>(r.fps) - table_prev) / table_prev * 100.0;
            std::string s = float_to_string_fixed(static_cast<float>(imp), 2) + "%";
            if (imp >= 0.0) s = "+" + s;
            row << std::setw(11) << s;
        }
        row << "  | " << std::setw(10)
            << (float_to_string_fixed(static_cast<float>(tracker.dropFromPeak(r.fps)), 2) + "%");
        std::string marker = (r.buffer_count == best_buffer) ? "  <== recommended" : "";
        if (r.buffer_count == peak_buffer) marker += "  <== peak";
        lines.push_back(row.str() + marker);
        table_prev = r.fps;
    }
    lines.push_back("");
    lines.push_back("  => Recommended buffer-count : " + std::to_string(best_buffer));
    lines.push_back("     Recommended FPS          : " + float_to_string_fixed(recommended_fps, 2));
    lines.push_back("     FPS tolerance            : "
                    + float_to_string_fixed(static_cast<float>(mtOption.tolerance_pct), 2) + "%");
    lines.push_back("     Max FPS                  : " + float_to_string_fixed(best_fps, 2)
                    + "  (buffer-count=" + std::to_string(peak_buffer)
                    + ", loops=" + std::to_string(tracker.best_loops) + ")");
    lines.push_back("     Drop from max FPS        : "
                    + float_to_string_fixed(static_cast<float>(tracker.dropFromPeak(recommended_fps)), 2) + "%");

    size_t maxLength = 0;
    for (const auto& l : lines) maxLength = (std::max)(maxLength, l.length());
    std::cout << "\n" << std::string(maxLength, '=') << std::endl;
    for (const auto& l : lines) std::cout << l << std::endl;
    std::cout << std::string(maxLength, '=') << std::endl;

    return best_buffer;
}

int main(int argc, char *argv[])
{
    string modelFile = "";
    string inputFile = "";
    string outputFile = "";
    bool benchmark = false;
    bool single = false;
    int64_t loops = 1;
    string devices_spec = "";
    int targetFps = 0;
    bool skip_inference_io = false;
    bool use_ort = false;
    bool verbose = false;
    int64_t duration = 0;
    int num_devices = 0;
    int64_t warmup_runs = 0;  // Added warmup runs
    int64_t report_interval = 0;  // Hidden: periodic cumulative report interval (sec)
    std::string buffer_count_str = std::to_string(getTaskMaxLoadValue());
    int buffer_count = getTaskMaxLoadValue();
    bool buffer_count_is_range = false;
    bool buffer_count_range_explicit = false;
    int buffer_count_range_lo = kMtDefaultStart;
    int buffer_count_range_hi = kMtDefaultCap;
    bool loops_explicit = false;
    bool duration_explicit = false;
    bool max_throughput = false;
    double probe_time = kMtDefaultRoundSec;
    double sweep_tolerance = kMtDefaultTolerance;
    int cool_down_temp = kMtDefaultCoolDownTemp;
    bool profiler_enable = false;
    bool throttling_info = false;
#ifdef DXRT_NFH_ACCELERATION_AVAILABLE
    bool accel_nfh = false;
#endif
#ifdef DXRT_CPU_OP_ACCELERATION_AVAILABLE
    bool accel_cpu = false;
#endif

    mode = RunModelMode::BENCHMARK_MODE;  // default mode: benchmark
    bounding = 0;  // default bounding: NPU_ALL

    std::string program_name(argv[0]);
    { auto s = program_name.rfind('/');  if (s != std::string::npos) program_name = program_name.substr(s + 1); }
    { auto s = program_name.rfind('\\'); if (s != std::string::npos) program_name = program_name.substr(s + 1); }
    const std::string help_text = std::string(APP_NAME) + " " + program_name;
    cxxopts::Options options(program_name, help_text);
    options.add_options()
        ("m, model", "Model file (.dxnn)" , cxxopts::value<string>(modelFile)->default_value(""))
        // Disable until dx_sim support is ready
        //("i, input", "Input data file", cxxopts::value<string>(inputFile))
        //("o, output", "Output data file", cxxopts::value<string>(outputFile)->default_value("output.bin"))
        ("b, benchmark", "Perform a benchmark test at a fixed --buffer-count (default mode if --single, --fps > 0, or --max-throughput is not specified)", cxxopts::value<bool>(benchmark)->default_value("false"))
        ("s, single", "Perform a single run test (sequential single-input inference on a single core)", cxxopts::value<bool>(single)->default_value("false"))
        ("v, verbose", "Shows NPU Processing Time, Queue Wait Time, E2E Response Time, and Latency", cxxopts::value<bool>(verbose)->default_value("false"))
        ("n, npu",
            "NPU bounding\n"
            "  0: NPU_ALL\n  1: NPU_0\n  2: NPU_1\n  3: NPU_2\n"
            "  4: NPU_0/1\n  5: NPU_1/2\n  6: NPU_0/2", cxxopts::value<int>(bounding)->default_value("0") )
        ("l, loops", "Number of inference loops to perform", cxxopts::value<int64_t>(loops)->default_value("30") )
        ("t, time", "Duration of inference in seconds (benchmark and target-fps mode, overrides --loops)", cxxopts::value<int64_t>(duration)->default_value("0") )
        ("w, warmup-runs", "Number of warmup runs before actual measurement", cxxopts::value<int64_t>(warmup_runs)->default_value("0") )
        ("d, devices",
            "Specify target NPU devices.\nExamples:\n"
            "  'all' (default): Use all available/bound NPUs\n"
            "  '0': Use NPU0 only\n"
            "  '0,1,2': Use NPU0, NPU1, and NPU2\n"
            "  'count:N': Use the first N NPUs\n  (e.g., 'count:2' for NPU0, NPU1)",
            cxxopts::value<std::string>(devices_spec)->default_value("all"))
        ("f, fps", "Target FPS for TARGET_FPS_MODE (enabled if > 0 and --single is not set)", cxxopts::value<int>(targetFps)->default_value("0"))

#ifdef USE_ORT
        ("use-ort", "Enable ONNX Runtime for CPU tasks in the model graph\nIf disabled, only NPU tasks operate", cxxopts::value<bool>(use_ort)->default_value("false"))
#endif
        ("profiler", "Enable profiler", cxxopts::value<bool>(profiler_enable)->default_value("false"))
#ifdef DXRT_NFH_ACCELERATION_AVAILABLE
        ("accel-nfh", "Enable NFH (transpose) acceleration", cxxopts::value<bool>(accel_nfh)->default_value("false"))
#endif
#ifdef DXRT_CPU_OP_ACCELERATION_AVAILABLE
        ("accel-cpu", "Enable CPU op acceleration (OpenVINO/XNNPACK)", cxxopts::value<bool>(accel_cpu)->default_value("false"))
#endif
        ("buffer-count",
            "I/O buffer count: a single number, or a range such as 3-16 "
            "when used with --max-throughput (range defaults to "
            + std::to_string(kMtDefaultStart) + "-" + std::to_string(kMtDefaultCap) + ")", cxxopts::value<std::string>(buffer_count_str)->default_value(std::to_string(getTaskMaxLoadValue())))
        ("max-throughput",
            "Search buffer counts for peak throughput and recommend one. "
            "Give --buffer-count a range (e.g. 3-16) to measure the full range; "
            "without a range, detect the peak within "
            + std::to_string(kMtDefaultStart) + "-" + std::to_string(kMtDefaultCap) + ". "
            "Add -l or -t to benchmark at the recommended count afterward. "
            "Mutually exclusive with --single and --fps.", cxxopts::value<bool>(max_throughput)->default_value("false"))
        ("probe-time", "Seconds to measure each candidate buffer count", cxxopts::value<double>(probe_time)->default_value(float_to_string_fixed(static_cast<float>(kMtDefaultRoundSec), 1)))
        ("sweep-tolerance",
            "FPS difference (%) treated as noise: smaller gains do not extend "
            "the sweep, and the smallest buffer count within this of the peak "
            "is recommended",
            cxxopts::value<double>(sweep_tolerance)->default_value(float_to_string_fixed(static_cast<float>(kMtDefaultTolerance), 1)))
        ("cool-down-temp",
            "Before each max-throughput measurement, wait until the NPU "
            "temperature ('C) drops below this value; the sweep fails if it "
            "does not within " + std::to_string(kMtCoolDownTimeoutSec) + " seconds",
            cxxopts::value<int>(cool_down_temp)->default_value(std::to_string(kMtDefaultCoolDownTemp)))
        ("h, help", "Print usage" );

    options.add_options("internal")
        ("i, input", "Input data file", cxxopts::value<string>(inputFile)->default_value(""))
        ("o, output", "Output data file", cxxopts::value<string>(outputFile)->default_value("output.bin"))
        ("skip-io", "Attempt to skip Inference I/O (Benchmark mode only)", cxxopts::value<bool>(skip_inference_io)->default_value("false"))
        ("throttling-info", "Enable display throttling info", cxxopts::value<bool>(throttling_info)->default_value("false"))
        ("report-interval", "Periodically print cumulative latency/fps every N seconds", cxxopts::value<int64_t>(report_interval)->default_value("0"));

    try
    {
        if (argc == 1)
        {
            cout << options.help({""}) << endl;
            exit(1);
        }
        auto cmd = options.parse(argc, argv);
        if (cmd.count("help"))
        {
            cout << options.help({""}) << endl;
            exit(0);
        }

        if ( cmd.count("buffer-count") )
        {
            auto dash = buffer_count_str.find('-', 1);  // start at 1: a leading '-' is a sign, not a range separator
            if ( dash != std::string::npos )
            {
                buffer_count_is_range = true;
                buffer_count_range_explicit = true;
                std::string lo_str = buffer_count_str.substr(0, dash);
                std::string hi_str = buffer_count_str.substr(dash + 1);
                try
                {
                    size_t lo_pos = 0, hi_pos = 0;
                    buffer_count_range_lo = std::stoi(lo_str, &lo_pos);
                    buffer_count_range_hi = std::stoi(hi_str, &hi_pos);
                    if ( lo_pos != lo_str.size() || hi_pos != hi_str.size() )
                        throw std::invalid_argument("trailing characters");
                }
                catch (const std::exception&)
                {
                    std::cerr << "[ERR] Invalid --buffer-count range: " << buffer_count_str << std::endl;
                    exit(1);
                }
                if ( buffer_count_range_lo < 1 || buffer_count_range_hi > kTaskMaxLoadLimit
                     || buffer_count_range_lo > buffer_count_range_hi )
                {
                    std::cerr << "[ERR] --buffer-count range must satisfy 1 <= low <= high <= "
                               << kTaskMaxLoadLimit << " (got " << buffer_count_str << ")." << std::endl;
                    exit(1);
                }
            }
            else
            {
                try
                {
                    size_t pos = 0;
                    buffer_count = std::stoi(buffer_count_str, &pos);
                    if ( pos != buffer_count_str.size() ) throw std::invalid_argument("trailing characters");
                }
                catch (const std::exception&)
                {
                    std::cerr << "[ERR] Invalid --buffer-count value: " << buffer_count_str << std::endl;
                    exit(1);
                }
                if ( buffer_count <= 0 || buffer_count > kTaskMaxLoadLimit )
                {
                    std::cerr << "Please check --buffer-count option value. Must be between 1 and " << kTaskMaxLoadLimit << std::endl;
                    exit(1);
                }
            }
            if ( buffer_count_is_range && !max_throughput )
            {
                std::cerr << "[ERR] --buffer-count range (e.g. 3-16) is only valid together with --max-throughput." << std::endl;
                exit(1);
            }
            if ( !buffer_count_is_range && max_throughput )
            {
                std::cerr << "[ERR] --max-throughput requires a buffer-count RANGE (e.g. 3-16), not a single value.\n"
                              "      Use \"--buffer-count 6\" alone to benchmark at a fixed count." << std::endl;
                exit(1);
            }
        }
        else if ( max_throughput )
        {
            // --max-throughput searches a range, so its default is a range too; the
            // single-count default used by the other modes would be meaningless here.
            buffer_count_is_range = true;
            buffer_count_range_lo = kMtDefaultStart;
            buffer_count_range_hi = kMtDefaultCap;
            buffer_count_str = std::to_string(buffer_count_range_lo) + "-" + std::to_string(buffer_count_range_hi);
        }
        // Always logged (default, env-derived, or explicit) so the measured count is reproducible.
        if ( buffer_count_is_range )
            std::cout << "Searching I/O Buffer Count range=" << buffer_count_str << std::endl << std::endl;
        else
            std::cout << "Using I/O Buffer Count=" << buffer_count << std::endl << std::endl;

        loops_explicit = cmd.count("loops") > 0;
        duration_explicit = cmd.count("time") > 0;

        if ( cmd.count("cool-down-temp") && !max_throughput )
        {
            std::cerr << "[ERR] --cool-down-temp is only valid together with --max-throughput." << std::endl;
            exit(1);
        }

        if ( loops < 1 )
        {
            std::cerr << "[ERR] --loops must be >= 1." << std::endl;
            exit(1);
        }
        if ( duration_explicit && duration < 0 )
        {
            std::cerr << "[ERR] --time must be >= 0 (seconds)." << std::endl;
            exit(1);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        cout << options.help({""}) << endl;
        exit(1);
    }

    MaxThroughputOption mt_option;
    if ( max_throughput )
    {
        if ( single || targetFps > 0 )
        {
            std::cerr << "[ERR] --max-throughput cannot be combined with --single or --fps." << std::endl;
            exit(1);
        }
        if ( probe_time < 0.1 )
        {
            std::cerr << "[ERR] --probe-time must be >= 0.1 (seconds)." << std::endl;
            exit(1);
        }
        if ( !(sweep_tolerance >= 0.0 && sweep_tolerance < 100.0) )
        {
            std::cerr << "[ERR] --sweep-tolerance must be >= 0 and < 100 (%)." << std::endl;
            exit(1);
        }
        if ( cool_down_temp <= 0 || cool_down_temp > kMtMaxCoolDownTemp )
        {
            std::cerr << "[ERR] --cool-down-temp must be > 0 and <= " << kMtMaxCoolDownTemp << " ('C)." << std::endl;
            exit(1);
        }
        // Guaranteed a range here: parsing either validated an explicit one or filled in the default.
        int sweep_start = buffer_count_range_lo;
        int sweep_cap   = buffer_count_range_hi;
        mt_option.start_count = sweep_start;
        mt_option.step = kMtDefaultStep;
        mt_option.threshold_pct = kMtDefaultThreshold;
        mt_option.cap = sweep_cap;
        mt_option.round_sec = probe_time;
        mt_option.patience = kMtDefaultPatience;
        mt_option.stall = kMtDefaultStall;
        mt_option.tolerance_pct = sweep_tolerance;
        mt_option.cool_down_temp = cool_down_temp;
        mt_option.stop_on_peak = !buffer_count_range_explicit;
        buffer_count = sweep_start;  // probe engine starts at the sweep start count
    }

    std::atomic<bool> critical_error{false};

    // Runtime Event dispatching
    dxrt::RuntimeEventDispatcher::GetInstance().RegisterEventHandler(
        [&critical_error](dxrt::RuntimeEventDispatcher::LEVEL level,
            dxrt::RuntimeEventDispatcher::TYPE type,
            dxrt::RuntimeEventDispatcher::CODE code, const std::string& message, const std::string& timestamp) {
            std::cout << "[run-model] level=" << std::to_string(static_cast<int>(level))
                        << ", type=" << std::to_string(static_cast<int>(type))
                        << ", code=" << std::to_string(static_cast<int>(code))
                        << ", message: " << message
                        << ", timestamp: " << timestamp << std::endl;

            if (level == dxrt::RuntimeEventDispatcher::LEVEL::CRITICAL )
            {
                if (type == dxrt::RuntimeEventDispatcher::TYPE::DEVICE_MEMORY &&
                code == dxrt::RuntimeEventDispatcher::CODE::MEMORY_OVERFLOW) {
                    std::cerr << "Terminating program due to critical NPU memory error." << std::endl;
                    exit(-1);
                }
                critical_error.store(true);
            }
        }
    );
    dxrt::RuntimeEventDispatcher::GetInstance().SetCurrentLevel(
        dxrt::RuntimeEventDispatcher::LEVEL::WARNING);

    // always showing the model information
    dxrt::Configuration::GetInstance().SetEnable(dxrt::Configuration::ITEM::SHOW_MODEL_INFO, true);


    try
    {
        // print version
        std::cout << "Runtime Framework Version: v" << dxrt::Configuration::GetInstance().GetVersion() << std::endl;

        // print device driver version
        std::cout << "Device Driver Version: v" << dxrt::Configuration::GetInstance().GetDriverVersion() << std::endl;

        // print pcie driver version
        std::cout << "PCIe Driver Version: v" << dxrt::Configuration::GetInstance().GetPCIeDriverVersion() << std::endl;

        std::cout << std::endl;
    }
    catch (const dxrt::Exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    if ( throttling_info )
    {
        dxrt::Configuration::GetInstance().SetEnable(dxrt::Configuration::ITEM::SHOW_THROTTLING, true);
        std::cout << "<Throttling info enabled>" << std::endl;
    }

    if ( modelFile.length() == 0)
    {
        cout << options.help({""}) << endl;
        exit(0);
    }

    // print host info
    if ( verbose )
    {
#ifdef __linux__
        // print cpu info
        printCpuInfo();

        // print architecture info
        printArchitectureInfo();

        // print memory info
        printMemoryInfo();
#endif  //  __linux__
    }

    LOG_VALUE(modelFile);
    LOG_VALUE(inputFile);
    LOG_VALUE(outputFile);
    LOG_VALUE(benchmark);
    LOG_VALUE(loops);
    dxrt::InferenceOption op;

    try
    {
        num_devices = dxrt::DeviceStatus::GetDeviceCount();
    }
    catch (const dxrt::Exception& e)
    {
        std::cerr << "[ERR] Failed to get device count: " << e.what() << std::endl;
        return -1;
    }
    catch (const std::exception& e)
    {
        std::cerr << "[ERR] Failed to get device count: " << e.what() << std::endl;
        return -1;
    }

    if (devices_spec.empty() || devices_spec == "all")
    {
        cout << "Device specification: 'all' (default)" << endl;
    }
    else if (devices_spec.rfind("count:", 0) == 0)
    {
        try
        {
            string count_str = devices_spec.substr(6);
            int count = std::stoi(count_str);

            if (count > num_devices)
            {
                std::cerr << "[ERR] Invalid device count: " << count << ". Available device(s): " << num_devices << std::endl;
                return -1;
            }

            if (count > 0)
            {
                for (int i = 0; i < count; ++i)
                {
                    op.devices.push_back(i);
                }
                cout << "Device specification: First " << count << " NPU(s) {";
                for (size_t i = 0; i < op.devices.size(); ++i)
                {
                    cout << op.devices[i] << (i == op.devices.size() - 1 ? "" : ", ");
                }
                cout << "}" << endl;
            }
            else
            {
                std::cerr << "[ERR] Device count in '" << devices_spec << "' must be positive." << std::endl;
                return -1;
            }
        }
        catch (const std::invalid_argument& ia)
        {
            std::cerr << "[ERR] Invalid number in '" << devices_spec << "' for 'count:N' format." << std::endl;
            std::cerr << ia.what() << std::endl;
            return -1;
        }
        catch (const std::out_of_range& oor)
        {
            std::cerr << "[ERR] Number out of range in '" << devices_spec << "' for 'count:N' format." << std::endl;
            std::cerr << oor.what() << std::endl;
            return -1;
        }
    }
    else
    {
        std::stringstream ss(devices_spec);
        std::string segment;
        bool first_device = true;
        std::set<int> dupID;
        while (std::getline(ss, segment, ','))
        {
            try
            {
                segment.erase(std::remove_if(segment.begin(), segment.end(), ::isspace), segment.end());
                if (segment.empty()) continue;
                int device_id = std::stoi(segment);

                if (device_id+1 > num_devices)
                {
                    cout << endl;
                    if (num_devices == 1) {
                        std::cerr << "[ERR] Invalid device number " << device_id << ". Only device 0 is available" << std::endl;
                    } else {
                        std::cerr << "[ERR] Invalid device number " << device_id << ". Only devices 0-" << (num_devices-1) << " are available" << std::endl;
                    }
                    return -1;
                }

                if (!dupID.count(device_id)) op.devices.push_back(device_id);
                dupID.insert(device_id);

                cout << "Device specification: Specific NPU(s) {";
                if (!first_device) cout << ", ";
                cout << device_id;
                first_device = false;
            }
            catch (const std::invalid_argument& ia)
            {
                std::cerr << "[ERR] Invalid device ID '" << segment << "' in --devices list." << std::endl;
                std::cerr << ia.what() << std::endl;
                return -1;
            }
            catch (const std::out_of_range& oor)
            {
                std::cerr << "[ERR] Device ID '" << segment << "' out of range in --devices list." << std::endl;
                std::cerr << oor.what() << std::endl;
                return -1;
            }
        }
        cout << "}" << endl;
        if (op.devices.empty() && !devices_spec.empty() && devices_spec != "all")
        {
            std::cerr << "[WARN] No valid device IDs parsed from --devices string: '" << devices_spec << "'. Defaulting to 'all'." << std::endl;
        }
    }

    if (bounding >= 0 && bounding < kBoundInfMax)
    {
        op.boundOption = static_cast<dxrt::InferenceOption::BOUND_OPTION>(bounding);
    }
    else
    {
        std::cout << "[ERR] Please check bounding option value. Must be between 0 and " << (kBoundInfMax -1) << endl;
        return -1;
    }
    op.useORT = use_ort;
    op.bufferCount = buffer_count;

    try{

        if ( profiler_enable )
        {
            dxrt::Configuration::GetInstance().SetEnable(dxrt::Configuration::ITEM::PROFILER, true);
            dxrt::Configuration::GetInstance().SetAttribute(dxrt::Configuration::ITEM::PROFILER, dxrt::Configuration::ATTRIBUTE::PROFILER_SAVE_DATA, "on");
            dxrt::Configuration::GetInstance().SetAttribute(dxrt::Configuration::ITEM::PROFILER, dxrt::Configuration::ATTRIBUTE::PROFILER_SHOW_DATA, "on");
            std::cout << "[INFO] Profiler is enabled." << std::endl;
        }
        else
        {
            dxrt::Configuration::GetInstance().SetEnable(dxrt::Configuration::ITEM::PROFILER, false);
        }

        // Acceleration options
#ifdef DXRT_NFH_ACCELERATION_AVAILABLE
        if (accel_nfh)
        {
            dxrt::Configuration::GetInstance().SetEnable(dxrt::Configuration::ITEM::NFH_ACCELERATION, true);
            std::cout << "[INFO] NFH acceleration is enabled." << std::endl;
        }
#endif
#ifdef DXRT_CPU_OP_ACCELERATION_AVAILABLE
        if (accel_cpu)
        {
            dxrt::Configuration::GetInstance().SetEnable(dxrt::Configuration::ITEM::CPU_OP_ACCELERATION, true);
            std::cout << "[INFO] CPU op acceleration is enabled." << std::endl;
        }
#endif

        //dxrt::Configuration::GetInstance().SetEnable(dxrt::Configuration::ITEM::PROFILER, false);

        SetRunModelMode(single, targetFps, max_throughput);

        // duration
        if ( mode == MAX_THROUGHPUT_MODE )
        {
            std::cout << "Max-throughput sweep: start=" << mt_option.start_count << " step=" << mt_option.step
                      << " cap=" << mt_option.cap << " round-time=" << mt_option.round_sec << "s"
                      << " peak-drop-threshold=" << mt_option.threshold_pct << "% (patience "
                      << mt_option.patience << ", stall " << mt_option.stall << ")"
                      << " tolerance=" << mt_option.tolerance_pct << "%"
                      << " cool-down-temp=" << mt_option.cool_down_temp << "'C" << std::endl;
        }
        else if ( duration > 0 && mode != SINGLE_MODE)
        {
            std::cout << "Inference by time: duration=" << duration << "(s)" << std::endl;
        }
        else
        {
            std::cout << "Inference by loops: count=" << loops << std::endl;
        }

        auto ie_owner = std::unique_ptr<dxrt::InferenceEngine>(new dxrt::InferenceEngine(modelFile, op));
        dxrt::InferenceEngine& ie = *ie_owner;
        vector<uint8_t> inputBuf(ie.GetInputSize(), 0);
        if (!inputFile.empty())
        {
            int64_t fileSize = localGetFileSize(inputFile);
            if (fileSize < 0)
                throw std::runtime_error("cannot open input file: " + inputFile);
            if (ie.GetInputSize() != static_cast<uint64_t>(fileSize))
                throw std::runtime_error("input file size mismatch");
            if (!localDataFromFile(inputFile, inputBuf.data(), inputBuf.size()))
                throw std::runtime_error("failed to read input file: " + inputFile);
        }

        // Perform warmup runs if specified
        if (warmup_runs > 0) {
            std::cout << "Performing " << warmup_runs << " warmup run(s)..." << std::endl;
            auto warmup_mi = prepareMultiInputBuffers(ie, inputBuf.data());
            for (int64_t i = 0; i < warmup_runs; ++i) {
                if (warmup_mi.is_multi_input)
                {
                    ie.RunMultiInput(warmup_mi.ptrs);
                }
                else
                {
                    ie.Run(inputBuf.data());
                }
            }
            std::cout << "Warmup completed." << std::endl;
        }


        if (skip_inference_io)
        {
            dxrt::set_skip_inference_io(true);
        }

        ReportConfig report_cfg;
        report_cfg.interval_sec = report_interval;
        report_cfg.input_file = inputFile;
        report_cfg.output_file = outputFile;
        report_cfg.model_file = modelFile;
        report_cfg.verbose = verbose;

        switch (mode)
        {
            case SINGLE_MODE: {
                uint64_t infTime = 0;
                float fps = 0.0;
                auto single_mi = prepareMultiInputBuffers(ie, inputBuf.data());
                for (int i = 0; i < loops; i++)
                {
                    auto start_clock = std::chrono::steady_clock::now();
                    dxrt::TensorPtrs outputs;
                    if (single_mi.is_multi_input)
                    {
                        outputs = ie.RunMultiInput(single_mi.ptrs);
                    }
                    else
                    {
                        outputs = ie.Run(inputBuf.data());
                    }
                    auto end_clock = std::chrono::steady_clock::now();
                    infTime = std::chrono::duration_cast<std::chrono::microseconds>(end_clock - start_clock).count();
                    fps = static_cast<float>(1000000.0 / infTime);
                    if (!inputFile.empty())
                        localDataDumpBin(outputFile, outputs);
                    PrintInfResult(inputFile, outputFile, modelFile,
                                   static_cast<float>(ie.GetE2EResponseTime()/1000.0),
                                   static_cast<float>(ie.GetCoreLatency()/1000.0),
                                   static_cast<float>(ie.GetNpuInferenceTime()/1000.0),
                                   static_cast<float>(ie.GetQueueWaitTime()/1000.0),
                                   fps, 1, mode, verbose);
                }
                break;
            }
            case TARGET_FPS_MODE: {

                float fps = runAsyncTargetFPS(loops, ie, targetFps, inputBuf.data(), duration);
                if (!ensureInferenceRan(fps, loops)) return -1;
                PrintInfResult(inputFile, outputFile, modelFile,
                               static_cast<float>(ie.GetE2EResponseTimeMean()/1000.0),
                               static_cast<float>(ie.GetCoreLatencyMean()/1000.0),
                               static_cast<float>(ie.GetNpuInferenceTimeMean()/1000.0),
                               static_cast<float>(ie.GetQueueWaitTimeMean()/1000.0),
                               fps, loops, mode, verbose);
                break;
            }
            case MAX_THROUGHPUT_MODE: {
                // Destroy the probe engine first: each sweep round must own the only
                // live engine, and `ie` must not be touched again in this branch.
                ie_owner.reset();

                int best_buffer = runMaxThroughput(modelFile, op, inputBuf.data(), mt_option, critical_error);
                if (best_buffer <= 0) return -1; // runMaxThroughput already reported the specific reason

                // Final measurement run only when the user asked for one via -l or -t;
                // in that case behave exactly like the normal benchmark mode.
                if ( loops_explicit || duration_explicit )
                {
                    dxrt::InferenceOption final_op = op;
                    final_op.bufferCount = best_buffer;
                    final_op.showModelInfo = false;
                    dxrt::InferenceEngine final_ie(modelFile, final_op);

                    std::cout << "\n[max-throughput] Final run with buffer-count=" << best_buffer << " ..." << std::endl;
                    float fps = 0;
                    if ( duration > 0 )
                    {
                        fps = runBenchmarkByTime(loops, final_ie, inputBuf.data(), duration, report_cfg);
                    }
                    else
                    {
                        fps = final_ie.RunBenchmark(static_cast<int>(loops), inputBuf.data());
                        if (!inputFile.empty())
                        {
                            auto bench_mi = prepareMultiInputBuffers(final_ie, inputBuf.data());
                            dxrt::TensorPtrs outputs;
                            if (bench_mi.is_multi_input)
                            {
                                outputs = final_ie.RunMultiInput(bench_mi.ptrs);
                            }
                            else
                            {
                                outputs = final_ie.Run(inputBuf.data());
                            }

                            localDataDumpBin(outputFile, outputs);  /* TODO: sparse tensor */
                        }
                    }
                    if (!ensureInferenceRan(fps, loops)) return -1;
                    PrintInfResult(inputFile, outputFile, modelFile,
                                   static_cast<float>(final_ie.GetE2EResponseTimeMean()/1000.0),
                                   static_cast<float>(final_ie.GetCoreLatencyMean()/1000.0),
                                   static_cast<float>(final_ie.GetNpuInferenceTimeMean()/1000.0),
                                   static_cast<float>(final_ie.GetQueueWaitTimeMean()/1000.0),
                                   fps, loops, mode, verbose);
                }
                break;
            }
            case BENCHMARK_MODE: {
                float fps = 0;
                if ( duration > 0 )
                {
                    fps = runBenchmarkByTime(loops, ie, inputBuf.data(), duration, report_cfg);
                }
                else
                {
                    fps = ie.RunBenchmark(static_cast<int>(loops), inputBuf.data());
                    if (!inputFile.empty())
                    {
                        auto bench_mi = prepareMultiInputBuffers(ie, inputBuf.data());
                        dxrt::TensorPtrs outputs;
                        if (bench_mi.is_multi_input)
                        {
                            outputs = ie.RunMultiInput(bench_mi.ptrs);
                        }
                        else
                        {
                            outputs = ie.Run(inputBuf.data());
                        }

                        localDataDumpBin(outputFile, outputs);  /* TODO: sparse tensor */
                    }
                }
                if (!ensureInferenceRan(fps, loops)) return -1;
                PrintInfResult(inputFile, outputFile, modelFile,
                               static_cast<float>(ie.GetE2EResponseTimeMean()/1000.),
                               static_cast<float>(ie.GetCoreLatencyMean()/1000.),
                               static_cast<float>(ie.GetNpuInferenceTimeMean()/1000.0),
                               static_cast<float>(ie.GetQueueWaitTimeMean()/1000.0),
                               fps, loops, mode, verbose);

                break;
            }
            default:
                cout << "Unknown run model mode:" << mode << endl;
                return -1;
        }
    }
    catch (const dxrt::Exception& e)
    {
        std::cerr << e.what() << " error-code=" << static_cast<int>(e.code()) << std::endl;
        return -1;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    catch(...)
    {
        std::cerr << "Exception" << std::endl;
        return -1;
    }

    return critical_error ? -1 : 0;
}
