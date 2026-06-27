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

#include "dxrt/dxrt_cxx_api.h"
#include "dxrt/extern/cxxopts.hpp"

// Local constants replacing internal macros.
// TODO: expose via public API (dxrt_get_bound_inf_max / dxrt_get_task_max_load_limit)
// when these values change in driver.h. Current values verified against internal defs.
static constexpr int kBoundInfMax = 7;       // == dxrt::N_BOUND_INF_MAX (driver.h)
static constexpr int kTaskMaxLoadLimit = 100; // == DXRT_TASK_MAX_LOAD_LIMIT (common.h)
static int getTaskMaxLoadValue() { return dxrt::get_task_max_load(); }
#define LOG_VALUE(val) std::cout << #val << ": " << val << std::endl

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
    BENCHMARK_MODE  = 0,
    SINGLE_MODE     = 1,
    TARGET_FPS_MODE = 2,
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
        case BENCHMARK_MODE:  os << "Benchmark Mode"; break;
        case SINGLE_MODE:     os << "Single Mode"; break;
        case TARGET_FPS_MODE: os << "Target FPS Mode"; break;
        default:              os << "Unknown Mode"; break;
    }
    return os;
}

std::string float_to_string_fixed(float value, int precision) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << value;
    return oss.str();
}

void PrintInfResult(const std::string& inputFile, const std::string& outputFile, const std::string& modelFile,
                    float latencyMs, float infTimeMs, float fps_val, int64_t loops, RunModelMode current_mode, bool verbose) {
    std::vector<std::string> lines;
    (void)modelFile;

    if (dxrt::Configuration::GetInstance().GetEnable(dxrt::Configuration::ITEM::SHOW_PROFILE))
        verbose = true;

    const std::string desc_npu_time = "Actual NPU core computation time for a single request";
    const std::string desc_latency = "End-to-end time per request including data transfer and overheads";
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
        int current_length = core_content.length();
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
    std::string latencyStr = float_to_string_fixed(latencyMs, 3);
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
            lines.push_back(build_formatted_line("  - Latency              : ", latencyStr, "ms", desc_latency));
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
            lines.push_back(build_formatted_line("  - Latency Average             : ", latencyStr, "ms", desc_latency));
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


void SetRunModelMode(bool single, int targetFps)
{
    if (single) {
        mode = SINGLE_MODE;
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

static float runBenchmarkByTime(int64_t& outLoops, dxrt::InferenceEngine& ie, void* inputBuffer, int64_t targetDurationSec)
{
    float fps = 0;

    int64_t target_duration_ms = targetDurationSec * 1000;
    int64_t run_count = 0; // run async count
    int64_t cb_count = 0; // callback count
    bool run_complete = false;
    std::mutex cb_mutex;
    std::condition_variable cb_cv;

    ie.RegisterCallback([&run_count, &cb_count, &run_complete, &cb_mutex, &cb_cv]
        (dxrt::TensorPtrs &outputs, void *userArg) {

        std::ignore = outputs;
        std::ignore = userArg;

        // check competion of run & callback
        std::unique_lock<std::mutex> lock(cb_mutex);
        cb_count++;
        if ( run_complete && cb_count == run_count)
            cb_cv.notify_one();

        return 0;
    });

    // Pre-allocate multi-input buffers before the loop
    auto mi = prepareMultiInputBuffers(ie, inputBuffer);

    auto start = std::chrono::steady_clock::now();
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
            std::unique_lock<std::mutex> lock(cb_mutex);
            run_count++;
        }

        auto current = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> duration = current - start;

        if ( duration.count() > target_duration_ms )
        {
            break;
        }

    } // for i

    // wait...
    std::unique_lock<std::mutex> lock(cb_mutex);
    run_complete = true;
    cb_cv.wait(lock, [&cb_count, &run_count](){return cb_count == run_count; } );

    auto current = std::chrono::steady_clock::now();
    uint64_t infTime = std::chrono::duration_cast<std::chrono::microseconds>(current - start).count();
    fps = static_cast<float>(1000000.0 * run_count/infTime);

    ie.RegisterCallback(nullptr);

    outLoops = run_count;
    std::cout << "Inference by time: total-inference-time=" << infTime / 1000000.0 << "(s)" << " total-loops=" << outLoops << std::endl;

    return fps;
}

static float runAsyncTargetFPS(int64_t& outLoops, dxrt::InferenceEngine& ie, int targetFps, void* inputBuffer, int64_t targetDurationSec)
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
    fps = 1000000.0 * outLoops/infTime;

#ifdef TARGET_FPS_DEBUG
    for (const auto& result : results) {
        cout << result << endl;
    }
#endif

    return fps;

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
    int buffer_count = getTaskMaxLoadValue();
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
        ("b, benchmark", "Perform a benchmark test (Maximum throughput)\n(This is the default mode,\n if --single or --fps > 0 are not specified)", cxxopts::value<bool>(benchmark)->default_value("false"))
        ("s, single", "Perform a single run test\n(Sequential single-input inference on a single-core)", cxxopts::value<bool>(single)->default_value("false"))
        ("v, verbose", "Shows NPU Processing Time and Latency", cxxopts::value<bool>(verbose)->default_value("false"))
        ("n, npu",
            "NPU bounding (default:0)\n"
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
        ("f, fps", "Target FPS for TARGET_FPS_MODE (default: 0)\n(enables this mode if > 0 and --single is not set)", cxxopts::value<int>(targetFps)->default_value("0"))

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
        ("buffer-count", "Number of input/output buffers, count's range is 1~" + std::to_string(kTaskMaxLoadLimit), cxxopts::value<int>(buffer_count)->default_value(std::to_string(getTaskMaxLoadValue())))
        ("h, help", "Print usage" );

    options.add_options("internal")
        ("i, input", "Input data file", cxxopts::value<string>(inputFile)->default_value(""))
        ("o, output", "Output data file", cxxopts::value<string>(outputFile)->default_value("output.bin"))
        ("skip-io", "Attempt to skip Inference I/O (Benchmark mode only)", cxxopts::value<bool>(skip_inference_io)->default_value("false"))
        ("throttling-info", "Enable display throttling info", cxxopts::value<bool>(throttling_info)->default_value("false"));

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
            if ( buffer_count <= 0 || buffer_count > kTaskMaxLoadLimit )
            {
                std::cout << "Please check --buffer-count option value. Must be between 1 and " << kTaskMaxLoadLimit << endl;
                exit(1);
            }
            else
            {
                std::cout << "Using I/O Buffer Count=" << buffer_count << std::endl;
                std::cout << std::endl;
            }
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        cout << options.help({""}) << endl;
        exit(1);
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
        cout << options.help() << endl;
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

        SetRunModelMode(single, targetFps);

        // duration
        if ( duration > 0 && mode != SINGLE_MODE)
        {
            std::cout << "Inference by time: duration=" << duration << "(s)" << std::endl;
        }
        else
        {
            std::cout << "Inference by loops: count=" << loops << std::endl;
        }

        dxrt::InferenceEngine ie(modelFile, op);
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
                    fps = 1000000.0/infTime;
                    if (!inputFile.empty())
                        localDataDumpBin(outputFile, outputs);
                    PrintInfResult(inputFile, outputFile, modelFile, ie.GetLatency()/1000., ie.GetNpuInferenceTime()/1000., fps, 1, mode, verbose);
                }
                break;
            }
            case TARGET_FPS_MODE: {

                float fps = runAsyncTargetFPS(loops, ie, targetFps, inputBuf.data(), duration);
                PrintInfResult(inputFile, outputFile, modelFile, ie.GetLatencyMean()/1000.0, ie.GetNpuInferenceTimeMean()/1000.0, fps, loops, mode, verbose);
                break;
            }
            case BENCHMARK_MODE: {
                float fps = 0;
                if ( duration > 0 )
                {
                    fps = runBenchmarkByTime(loops, ie, inputBuf.data(), duration);
                }
                else
                {
                    fps = ie.RunBenchmark(loops, inputBuf.data());
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
                PrintInfResult(inputFile, outputFile, modelFile, ie.GetLatencyMean()/1000., ie.GetNpuInferenceTimeMean()/1000., fps, loops, mode, verbose);

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
