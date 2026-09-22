/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

 #include <stdlib.h>
 #include <cstdlib>
 #include <stdio.h>
 #include <stdint.h>
 #include <fcntl.h>
 #ifdef __linux__
 #include <cxxabi.h>
 #include <unistd.h>
 #endif
 #ifdef _WIN32
 #include <windows.h>
 #include <io.h>
 #endif
 #include <string>
 #include <iostream>
 #include <fstream>
 #include <vector>
 #include <map>
 #include <set>
 #include <iomanip>
 #include <sstream>
 #include <utility>

 #include "dxrt/common.h"
 #include "dxrt/model.h"
 #include "dxrt/task_data.h"
 #include "dxrt/cpu_handle.h"
 #include "dxrt/filesys_support.h"
 #include "dxrt/exception/exception.h"


 using std::cout;
 using std::endl;
 using std::vector;
 using std::map;
 using std::string;
 using std::set;

 namespace dxrt
 {

#if defined(_WIN32)
#define OS_WINDOWS
#define ISATTY _isatty
#define FILENO _fileno
#else
#define ISATTY isatty
#define FILENO fileno
#endif

 // Forward declarations
 int ParseModelJSONExtract(const std::string& file);
 int ParseModelDetailed(const std::string& file, const ParseOptions& options);

// Shared color output toggles used by ANSI/WinAPI output paths.


enum class ColorName {
    Reset,
    Bold,
    Yellow,
    Green,
    Blue,
    Red,
    Purple,
    Cyan,
    Gray
};

struct ColoredText;

// ANSI escape codes for terminal text colors
class Color {
 public:
    enum class Mode {
        PlainText,
        Ansi,
        WinApi
    };

    static Mode DetectMode()
    {
        // [Check 1] Non-TTY output should not emit color sequences.
        if (!ISATTY(FILENO(stdout)))
        {
            return Mode::PlainText;
        }

        // [Check 2] Respect NO_COLOR standard environment variable.
        if (std::getenv("NO_COLOR") != nullptr)
        {
            return Mode::PlainText;
        }

#if defined(OS_WINDOWS)
        // Windows detection logic.
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE) return Mode::PlainText;

        DWORD dwMode = 0;
        if (!GetConsoleMode(hOut, &dwMode)) return Mode::PlainText;

        // Windows 10+: try enabling ANSI VT mode.
        if (SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING))
        {
            return Mode::Ansi;
        }

        // ANSI enable failed: fallback to WinApi color path.
        return Mode::WinApi;
#else
        // Linux/macOS detection logic.
        const char* term = std::getenv("TERM");
        if (term && std::string(term) == "dumb")
        {
            return Mode::PlainText;
        }
        return Mode::Ansi;
#endif
    }

    static void Configure(bool no_color)
    {
        // Keep existing no_color option as an explicit override.
        if (no_color)
        {
            color_enabled = false;
            use_color_literal = false;
            use_windows_color_api = false;
            return;
        }

        const Mode mode = DetectMode();
        switch (mode)
        {
            case Mode::Ansi:
                color_enabled = true;
                use_color_literal = true;
                use_windows_color_api = false;
                break;
            case Mode::WinApi:
                // Text-only color output path is handled by coutColorText(...).
                // Keep ANSI literals off and enable WinAPI path.
                color_enabled = true;
                use_color_literal = false;
                use_windows_color_api = true;
                break;
            case Mode::PlainText:
            default:
                color_enabled = false;
                use_color_literal = false;
                use_windows_color_api = false;
                break;
        }
    }

    static bool get_color_enabled()
    {
        return color_enabled;
    }
    friend std::ostream &operator<<(std::ostream &os, const ColoredText &value);

private:
    static bool color_enabled;
    static bool use_color_literal;
    static bool use_windows_color_api;
};

bool Color::color_enabled = false;
bool Color::use_color_literal = true;
bool Color::use_windows_color_api = false;

static const char* ansi_code(ColorName color)
{
    switch (color)
    {
        case ColorName::Reset:  return "\033[0m";
        case ColorName::Bold:   return "\033[1m";
        case ColorName::Yellow: return "\033[1;33m";
        case ColorName::Green:  return "\033[1;32m";
        case ColorName::Blue:   return "\033[1;34m";
        case ColorName::Red:    return "\033[1;31m";
        case ColorName::Purple: return "\033[1;35m";
        case ColorName::Cyan:   return "\033[1;36m";
        case ColorName::Gray:   return "\033[90m";
        default:                return "";
    }
}

#ifdef _WIN32
static WORD winapi_color(ColorName color)
{
    switch (color)
    {
        case ColorName::Bold:
            return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        case ColorName::Yellow:
            return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        case ColorName::Green:
            return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        case ColorName::Blue:
            return FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        case ColorName::Red:
            return FOREGROUND_RED | FOREGROUND_INTENSITY;
        case ColorName::Purple:
            return FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        case ColorName::Cyan:
            return FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        case ColorName::Gray:
        case ColorName::Reset:
        default:
            return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    }
}
#endif

struct ColoredText {
    ColorName color;
    std::string text;
};

inline ColoredText coutColorText(ColorName color, std::string text)
{
    return ColoredText{color, std::move(text)};
}

std::ostream& operator<<(std::ostream& os, const ColoredText& value)
{
    if (!Color::get_color_enabled())
    {
        os << value.text;
        return os;
    }

    if (Color::use_color_literal)
    {
        os << ansi_code(value.color) << value.text << ansi_code(ColorName::Reset);
        return os;
    }

#ifdef _WIN32
    if (Color::use_windows_color_api)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hConsole != INVALID_HANDLE_VALUE)
        {
            CONSOLE_SCREEN_BUFFER_INFO current_info;
            const bool has_info = GetConsoleScreenBufferInfo(hConsole, &current_info) != 0;
            SetConsoleTextAttribute(hConsole, winapi_color(value.color));
            os << value.text;
            SetConsoleTextAttribute(
                hConsole,
                has_info ? current_info.wAttributes : winapi_color(ColorName::Reset));
            return os;
        }
    }
#endif

    os << value.text;
    return os;
}

inline std::string colorText(ColorName color, const std::string& text)
{
    std::ostringstream oss;
    oss << coutColorText(color, text);
    return oss.str();
}

 // Helper to add thousand separators to a number string
 static string add_commas(const string& s)
 {
    size_t n = s.length();
    if (n <= 3)
    {
        return s;
    }
    std::string res = "";
    int count = 0;
    for (int i = static_cast<int>(n) - 1; i >= 0; --i)
    {
        res = s[i] + res;
        count++;
        if (count % 3 == 0 && i != 0)
        {
            res = "," + res;
        }
    }
    return res;
}

 // Convert byte size to human-readable string with exact byte count
 static std::string format_bytes(size_t bytes)
 {
     if (bytes == 0) return "0 B";
     if (bytes < 1024)
     {
         return std::to_string(bytes) + " B";
     }

     std::stringstream ss;
     if (bytes < 1024 * 1024)
     {
         // KB with exact bytes
         double kb = static_cast<double>(bytes) / 1024.0;
         ss << std::fixed << std::setprecision(2) << kb << " KB ("
            << add_commas(std::to_string(bytes)) << " bytes)";
     }
     else
     {
         // MB with exact bytes
         double mb = static_cast<double>(bytes) / (1024.0 * 1024.0);
         ss << std::fixed << std::setprecision(2) << mb << " MB ("
            << add_commas(std::to_string(bytes)) << " bytes)";
     }

     return ss.str();
 }

#ifdef USE_ORT
 // Helper function to load CPU model data
 static bool load_cpu_model_data(
     const std::string& order,
     const dxrt::ModelDataBase& modelData,
     std::vector<std::vector<uint8_t>>& data)
 {
     for (size_t j = 0; j < modelData.deepx_binary.cpu_models().size(); j++)
     {
         const auto& cpu_model_name = modelData.deepx_binary.cpu_models(static_cast<int>(j)).name();
         if (order != cpu_model_name) continue;

         const auto& buffer_source = modelData.deepx_binary.cpu_models(static_cast<int>(j)).buffer();
         data.emplace_back(buffer_source.begin(), buffer_source.end());
         return true;
     }
     return false;
 }
#endif

 // Helper function to load binary data for a task
 static bool load_task_binary_data(
     const std::string& order,
     const dxrt::ModelDataBase& modelData,
     size_t rmapCount,
     bool is_v8_format,
     dxrt::rmapinfo& rmap_info,
     std::vector<std::vector<uint8_t>>& data,
     bool& has_ppu_binary)
 {
     for (size_t j = 0; j < rmapCount; j++)
     {
         const auto& rmap_info_name = modelData.deepx_binary.rmap_info(static_cast<int>(j)).name();
         if (order != rmap_info_name) continue;

         rmap_info = modelData.deepx_rmap.rmap_info(static_cast<int>(j));

         // Copy RMAP binary data
         const auto& rmap_buffer = modelData.deepx_binary.rmap(static_cast<int>(j)).buffer();
         data.emplace_back(rmap_buffer.begin(), rmap_buffer.end());
         DXRT_ASSERT(data.back().size() > 0, "invalid model - rmap size is zero");

         // Copy WEIGHT binary data (can be empty for some models)
         const auto& weight_buffer = modelData.deepx_binary.weight(static_cast<int>(j)).buffer();
         data.emplace_back(weight_buffer.begin(), weight_buffer.end());

         // v8: Add PPU binary if exists (for PPCPU type)
         if (!is_v8_format)
         {
             return true;
         }

         const bool has_ppu_data = (j < modelData.deepx_binary.ppu().size() &&
                                  modelData.deepx_binary.ppu(static_cast<int>(j)).size() > 0);
         if (!has_ppu_data)
         {
             return true;
         }

         const auto& ppu_buffer = modelData.deepx_binary.ppu(static_cast<int>(j)).buffer();
         data.emplace_back(ppu_buffer.begin(), ppu_buffer.end());
         has_ppu_binary = true;
         LOG_DXRT_DBG << "Added PPU binary to data vector for task '" << order
                      << "', size: " << data.back().size() << " bytes" << std::endl;

         return true;
     }
     return false;
 }

 // Helper function to format tensor shape
 static std::string format_tensor_shape(const dxrt::Tensor& tensor)
 {
     std::stringstream ss;
     ss << "[";
     const auto& shape = tensor.shape();
     for (size_t i = 0; i < shape.size(); ++i)
     {
         if (i > 0) ss << ", ";
         ss << shape[i];
     }
     ss << "]";
     return ss.str();
 }

 // Helper function to get tensor data type string
 static std::string get_tensor_dtype_string(const dxrt::Tensor& tensor)
 {
     const auto& type = tensor.type();
     switch (type)
     {
         case dxrt::DataType::FLOAT: return "float32";
         case dxrt::DataType::INT32: return "int32";
         case dxrt::DataType::INT16: return "int16";
         case dxrt::DataType::INT8: return "int8";
         case dxrt::DataType::UINT32: return "uint32";
         case dxrt::DataType::UINT16: return "uint16";
         case dxrt::DataType::UINT8: return "uint8";
         case dxrt::DataType::INT64: return "int64";
         case dxrt::DataType::UINT64: return "uint64";
         case dxrt::DataType::BBOX: return "BBOX";
         case dxrt::DataType::FACE: return "FACE";
         case dxrt::DataType::POSE: return "POSE";
         default: return "unknown";
     }
 }

 // Helper function to calculate tensor size in bytes
 static size_t calculate_tensor_bytes(const dxrt::Tensor& tensor)
 {
     // Use the existing size_in_bytes() method from Tensor class
     return tensor.size_in_bytes();
 }

 static void print_detailed_tensors(
     const std::string& title,
     const dxrt::Tensors& tensors,
     const std::vector<deepx_rmapinfo::TensorInfo>* tensorInfos,
     bool is_npu,
     bool verbose)
 {
     bool is_outputs = (title == "Outputs");
     cout << "  +- " << coutColorText(ColorName::Bold, title + ":") << endl;
     if (tensors.empty())
     {
         cout << (is_outputs ? "     " : "  |  ") << "+- (None)" << endl;
         return;
     }
     for (size_t i = 0; i < tensors.size(); ++i)
     {
         const auto& tensor = tensors[i];
         const char* prefix = is_outputs ? "     +- " : "  |  +- ";

         cout << prefix << coutColorText(ColorName::Cyan, tensor.name());

         // Show detailed info in verbose mode
         if (verbose)
         {
                const std::string detail = " {shape: " + format_tensor_shape(tensor)
                                    + ", dtype: " + get_tensor_dtype_string(tensor)
                                    + ", size: " + format_bytes(calculate_tensor_bytes(tensor))
                                    + "}";
                cout << coutColorText(ColorName::Gray, detail);
         }

         // Show layout/transpose info only in verbose mode
         if (verbose && is_npu && tensorInfos && i < tensorInfos->size())
         {
             auto layout = static_cast<deepx_rmapinfo::Layout>((*tensorInfos)[i]._layout);
             std::string layout_str = deepx_rmapinfo::LayoutToString(layout);
             std::string layout_detail = " [layout: " + layout_str;
             if (layout == deepx_rmapinfo::ALIGNED)
             {
                 auto transpose = static_cast<deepx_rmapinfo::Transpose>((*tensorInfos)[i]._transpose);
                 std::string transpose_str = deepx_rmapinfo::TransposeToString(transpose);
                 layout_detail += ", transpose: " + transpose_str;
             }
             layout_detail += "]";
             cout << coutColorText(ColorName::Gray, layout_detail);
         }
         cout << endl;
     }
 }

 int ParseModel(const string& file)
 {
     ParseOptions default_options;
     return ParseModel(file, default_options);
 }

 int ParseModel(const string& file, const ParseOptions& options)
 {
    // Force plain text when writing parse output to a file.
    const bool no_color_effective = options.no_color || !options.output_file.empty();
    Color::Configure(no_color_effective);


     // Redirect output if file is specified
     std::ofstream outputFile;
     std::streambuf* originalCout = nullptr;
     if (!options.output_file.empty())
     {
         outputFile.open(options.output_file);
         if (!outputFile.is_open())
         {
             std::cerr << "Error: Cannot open output file: " << options.output_file << std::endl;
             return -1;
         }
         originalCout = cout.rdbuf();
         cout.rdbuf(outputFile.rdbuf());
     }

     int result = 0;

     try {
         if (options.json_extract)
         {
             result = ParseModelJSONExtract(file);
         }
         else
         {
             result = ParseModelDetailed(file, options);
         }
     } catch (...) {
         // Restore cout before rethrowing
         if (originalCout)
         {
             cout.rdbuf(originalCout);
         }
         throw;
     }

     // Restore cout
     if (originalCout)
     {
         cout.rdbuf(originalCout);
         outputFile.close();
     }

     return result;
 }

 int ParseModelDetailed(const string& file, const ParseOptions& options)
 {
     using std::cout;
     using std::endl;
     using std::vector;
     using std::map;
     using std::string;
     using std::to_string;

// Someone wants to use parse_model without NPU, So, NPU related code is commented out.
#if 0
    DevicePool::GetInstance().InitCores();
    int deviceCount = DevicePool::GetInstance().GetDeviceCount();
    vector<uint64_t> deviceMemSizes;
    for (int i = 0; i < deviceCount; i++)
    {
       auto deviceCore = DevicePool::GetInstance().GetDeviceCores(i);
       deviceMemSizes.push_back(deviceCore->info().mem_size);
    }
#endif

     if (dxrt::fileExists(file) == false)
     {
         throw FileNotFoundException(EXCEPTION_MESSAGE(file));
     }

     std::map<std::string, deepx_graphinfo::SubGraph> graphMap;

     std::vector<TaskData> dataList;

    dxrt::ModelDataBase modelData;
    LoadModelParam(modelData, file);

    cout << "\n" << coutColorText(ColorName::Bold, "===================== Model Information ======================") << endl;
    cout << coutColorText(ColorName::Bold, " Model File Path        : ")
         << coutColorText(ColorName::Cyan, file) << endl;
    cout << coutColorText(ColorName::Bold, " .dxnn Format Version   : ")
         << coutColorText(ColorName::Green, "v" + std::to_string(modelData.deepx_binary._dxnnFileFormatVersion)) << endl;
    cout << coutColorText(ColorName::Bold, " DX-COM Version         : ")
            << coutColorText(ColorName::Green, "v" + modelData.deepx_binary._compilerVersion) << endl;
    cout << endl;
    cout << coutColorText(ColorName::Bold, " Model Input Tensors:") << endl;
    for (const auto& input : modelData.deepx_graph.inputs())
    {
        cout << "  - " << coutColorText(ColorName::Cyan, input) << endl;
    }
    cout << endl;
    cout << coutColorText(ColorName::Bold, " Model Output Tensors:") << endl;
    for (const auto& output : modelData.deepx_graph.outputs())
    {
        cout << "  - " << coutColorText(ColorName::Cyan, output) << endl;
    }

    // Calculate Model Memory Usage for Model Information section
    uint64_t totalModelMemory = 0;
    uint64_t totalBufferMemory = 0;
    int npuTaskCount = 0;

    // First pass: collect task data for memory calculation
    std::vector<TaskData> tempDataList;
    std::vector<std::string> tempTaskOrder = modelData.deepx_graph.topoSort_order();

    if (tempTaskOrder.empty())
    {
        tempTaskOrder.push_back(modelData.deepx_binary.rmap_info(0).name());
    }

    for (auto &order : tempTaskOrder)
    {
        dxrt::rmapinfo rmap_info;
        vector<vector<uint8_t>> data;
#ifdef USE_ORT
        bool is_cpu_model = false;
#endif
        bool found = false;
        bool has_ppu_binary = false;

        const size_t rmap_count = modelData.deepx_binary.rmap_info().size();
        const bool is_v8_format = (modelData.deepx_binary._dxnnFileFormatVersion >= 8);

        found = load_task_binary_data(order, modelData, rmap_count, is_v8_format,
                                       rmap_info, data, has_ppu_binary);
#ifdef USE_ORT
        if (!found && load_cpu_model_data(order, modelData, data))
        {
            found = true;
            is_cpu_model = true;
        }
#endif
        if (found)
        {
            TaskData task_data(0, order, rmap_info);
#ifdef USE_ORT
            if (is_cpu_model)
            {
                const auto* model_buffer = data.front().data();
                const size_t model_size = data.front().size();
                const int device_id = 1;
                task_data.set_from_cpu(
                    std::make_shared<CpuHandle>(model_buffer, model_size, order, device_id, task_data.get_buffer_count())
                );
            }
            else
#endif
            {
                task_data.set_from_npu(data, has_ppu_binary);
            }
            tempDataList.emplace_back(task_data);
        }
    }

    // Calculate memory usage
    int npu_buffer_count = DXRT_TASK_MAX_LOAD_VALUE;

    // Create map for fast lookup
    std::map<std::string, TaskData*, std::less<>> taskDataMap;
    for (auto& td : tempDataList)
    {
        taskDataMap.emplace(td._name, &td);
    }

    for (const auto& taskName : tempTaskOrder)
    {
        auto it = taskDataMap.find(taskName);
        if (it == taskDataMap.end()) continue;
        const TaskData& taskData = *it->second;

        if (taskData._processor == dxrt::Processor::NPU)
        {
            npuTaskCount++;
            totalModelMemory += taskData._memUsage;
            uint64_t buffers_total = (static_cast<uint64_t>(taskData._encodedInputSize) + taskData._outputMemSize) * taskData.get_buffer_count();
            totalBufferMemory += buffers_total;
            npu_buffer_count = taskData.get_buffer_count();
        }
    }

    cout << endl;
        cout << coutColorText(ColorName::Bold, " Model Memory Usage:") << endl;
        cout << "  - " << coutColorText(ColorName::Bold, "Total             : ")
            << coutColorText(ColorName::Purple, format_bytes(totalModelMemory)) << endl;
        cout << "  - " << coutColorText(ColorName::Bold, "Buffers           : ")
            << coutColorText(ColorName::Purple, format_bytes(totalBufferMemory)) << endl;
        cout << "  - " << coutColorText(ColorName::Bold, "NPU Tasks Count   : ")
            << coutColorText(ColorName::Purple, std::to_string(npuTaskCount)) << endl;
        cout << "  - " << coutColorText(ColorName::Bold, "Buffer Pool Size  : ")
            << coutColorText(ColorName::Purple, "x" + std::to_string(npu_buffer_count)) << endl;

// Someone wants to use parse_model without NPU, So, NPU related code is commented out.
#if 0
    for (int i=0; i < deviceCount; i++)
    {
       if (totalModelMemory > deviceMemSizes[i])
       {
           bool canFitWithPoolReduction = false;
           int recommendedPoolSize = 0;

           for (int j=1; j < DXRT_TASK_MAX_LOAD; j++)
           {
               if (deviceMemSizes[i] > totalModelMemory - totalBufferMemory + (totalBufferMemory * (DXRT_TASK_MAX_LOAD - j) / DXRT_TASK_MAX_LOAD))
               {
                   recommendedPoolSize = DXRT_TASK_MAX_LOAD - j;
                   canFitWithPoolReduction = true;
                   break;
               }
           }

           if (canFitWithPoolReduction)
           {
               cout << coutColorText(ColorName::Yellow,
                        " ⚠ Warning: Model size exceeds Device " + std::to_string(i) +
                        " memory (" + format_bytes(deviceMemSizes[i]) +
                        "), but can fit by reducing buffer pool size to x" +
                        std::to_string(recommendedPoolSize) + " or less.") << endl;
           }
           else
           {
               cout << coutColorText(ColorName::Red,
                        " ✗ Error: Model size exceeds Device " + std::to_string(i) +
                        " memory (" + format_bytes(deviceMemSizes[i]) +
                        ") - cannot fit even with minimum buffer pool size.") << endl;
           }
       }
    }
#endif

    cout << "\n" << coutColorText(ColorName::Bold, "================== Task Graph Information ====================") << endl;

     std::vector<std::string> taskOrder = modelData.deepx_graph.topoSort_order();

     if (taskOrder.empty())
     {
         taskOrder.push_back(
             modelData.deepx_binary.rmap_info(0).name());
     }

     for (auto &order : taskOrder )
     {
         dxrt::rmapinfo rmap_info;
         vector<vector<uint8_t>> data;
 #ifdef USE_ORT
         bool is_cpu_model = false;
 #endif
         bool found = false;
         bool has_ppu_binary = false;

         auto graphs = modelData.deepx_graph.subgraphs();
         for (auto &graph : graphs)
         {
             if (order == graph.name())
             {
                 graphMap[graph.name()] = graph;
                 break;
             }
         }

         const size_t rmap_count = modelData.deepx_binary.rmap_info().size();
         const bool is_v8_format = (modelData.deepx_binary._dxnnFileFormatVersion == 8);

         found = load_task_binary_data(order, modelData, rmap_count, is_v8_format,
                                        rmap_info, data, has_ppu_binary);

         // Update input names from graphMap if available
         if (found && graphMap.find(order) != graphMap.end())
         {
             for (size_t k = 0; k < rmap_info.inputs().size(); k++)
             {
                 rmap_info.inputs()[k].memory().name() = graphMap[order].inputs()[k].name();
             }
         }
 #ifdef USE_ORT
         if (!found && load_cpu_model_data(order, modelData, data))
         {
             found = true;
             is_cpu_model = true;
         }
 #endif
         if (found)
         {
             TaskData task_data(0, order, rmap_info);
 #ifdef USE_ORT
             if (is_cpu_model)
             {
                 const auto* model_buffer = data.front().data();
                 const size_t model_size = data.front().size();
                 const int device_id = 1;
                 task_data.set_from_cpu(
                     std::make_shared<CpuHandle>(model_buffer, model_size, order, device_id, task_data.get_buffer_count())
                 );
             }
             else
 #endif
             {
                 task_data.set_from_npu(data, has_ppu_binary);
             }

             dataList.emplace_back(task_data);
         }
     }


    // Analyze entry and output points
    set<string> entryTasks;  // Tasks that process model inputs
    set<string> outputTasks; // Tasks that produce model outputs
    map<string, set<string>> taskPredecessors;
    map<string, set<string>> taskSuccessors;

    // Helper lambda to check if output name is a model output
    auto is_model_output_name = [&modelData](const std::string& outputName) {
        return std::any_of(modelData.deepx_graph.outputs().begin(),
                          modelData.deepx_graph.outputs().end(),
                          [&outputName](const auto& modelOutput) {
                              return outputName == modelOutput;
                          });
    };

    // Helper lambda to collect non-empty successors from users
    auto collect_successors = [](const auto& users, set<string>& successors) {
        for (const auto& user : users)
        {
            if (user.empty()) continue;
            successors.insert(user);
        }
    };

    // Helper lambda to print comma-separated predecessors
    auto print_predecessors = [](const set<string>& predecessors) {
        for (auto it = predecessors.begin(); it != predecessors.end(); ++it)
        {
            if (it != predecessors.begin())
            {
                cout << coutColorText(ColorName::Gray, ", ");
            }
            cout << coutColorText(ColorName::Gray, *it);
        }
    };

    // Helper lambda to print dependency list
    auto print_dependency_list = [](const set<string>& items) {
        for (auto it = items.begin(); it != items.end(); ++it)
        {
            cout << coutColorText(ColorName::Cyan, *it) << (std::next(it) == items.end() ? "" : ", ");
        }
    };

    // Helper lambda to print NPU memory usage details
    auto print_npu_memory_usage = [](const TaskData& taskData) {
        uint64_t model_bytes = taskData._npuModel.rmap.size + taskData._npuModel.weight.size;
        uint64_t buffers_total = (static_cast<uint64_t>(taskData._encodedInputSize) + taskData._outputMemSize) * taskData.get_buffer_count();
        uint64_t input_device_mem = static_cast<uint64_t>(taskData._encodedInputSize) * taskData.get_buffer_count();
        uint64_t output_device_mem = static_cast<uint64_t>(taskData._outputMemSize) * taskData.get_buffer_count();

        cout << "  +- Memory Usage (NPU Device)" << endl;
        cout << "  |  +- Total        : " << coutColorText(ColorName::Bold, format_bytes(taskData._memUsage)) << endl;
        cout << "  |  +- Model        : " << format_bytes(model_bytes) << endl;
        cout << "  |  +- Buffers (x" << taskData.get_buffer_count() << ") : " << format_bytes(buffers_total) << endl;
        cout << "  |     +- Input buffers  : " << format_bytes(input_device_mem)
               << " " << coutColorText(ColorName::Gray, "(" + format_bytes(taskData._encodedInputSize) + " x " + std::to_string(taskData.get_buffer_count()) + ")") << endl;
        cout << "  |     +- Output buffers : " << format_bytes(output_device_mem)
               << " " << coutColorText(ColorName::Gray, "(" + format_bytes(taskData._outputMemSize) + " x " + std::to_string(taskData.get_buffer_count()) + ")") << endl;
    };

    // Helper lambda to print logical vs device memory differences
    auto print_memory_differences = [](const TaskData& taskData) {
        const bool has_difference = (taskData._outputMemSize != taskData._outputSize ||
                                   taskData._encodedInputSize != taskData._inputSize);
        if (!has_difference) return;

        cout << "  |" << endl;
        cout << "  |  " << coutColorText(ColorName::Gray, "Logical tensor size vs Device footprint:") << endl;

        if (taskData._encodedInputSize != taskData._inputSize)
        {
            cout << "  |     +- Input  (logical) : " << format_bytes(taskData._inputSize) << endl;
            cout << "  |     +- Input  (device)  : " << format_bytes(taskData._encodedInputSize)
                  << " " << coutColorText(ColorName::Yellow, "(NPU format conversion)") << endl;
        }
        else
        {
            cout << "  |     +- Input  (logical) : " << format_bytes(taskData._inputSize) << endl;
        }

        if (taskData._outputMemSize != taskData._outputSize)
        {
            cout << "  |     +- Output (logical) : " << format_bytes(taskData._outputSize) << endl;
            cout << "  |     +- Output (device)  : " << format_bytes(taskData._outputMemSize)
                  << " " << coutColorText(ColorName::Yellow, "(includes scratch memory)") << endl;
        }
        else
        {
            cout << "  |     +- Output (logical) : " << format_bytes(taskData._outputSize) << endl;
        }
    };

    for (const auto& taskName : taskOrder)
    {
        auto graph_it = graphMap.find(taskName);
        if (graph_it == graphMap.end()) continue;
        const auto& subgraph = graph_it->second;

        set<string> predecessors;
        set<string> successors;

        // Check if this task processes model inputs
        for (const auto& input : subgraph.inputs())
        {
            if (input.owner().empty())
            {
                entryTasks.insert(taskName);
            }
            else
            {
                predecessors.insert(input.owner());
            }
        }

        // Check if this task produces model outputs
        for (const auto& output : subgraph.outputs())
        {
            const auto& output_name = output.name();
            if (is_model_output_name(output_name))
            {
                outputTasks.insert(taskName);
            }

            collect_successors(output.users(), successors);
        }

        taskPredecessors[taskName] = predecessors;
        taskSuccessors[taskName] = successors;
    }

    cout << "\n" << coutColorText(ColorName::Bold, "-------------------- Task Dependencies -----------------------\n") << endl;

    // Create map for fast lookup
    std::map<std::string, TaskData*, std::less<>> taskDataMapForDeps;
    for (auto& td : dataList)
    {
        taskDataMapForDeps.emplace(td._name, &td);
    }

    for (const auto& taskName : taskOrder)
    {
        auto graph_it = graphMap.find(taskName);
        if (graph_it == graphMap.end()) continue;

        auto task_it = taskDataMapForDeps.find(taskName);
        if (task_it == taskDataMapForDeps.end()) continue;
        const TaskData& taskData = *task_it->second;

        const std::string procType = (taskData._processor == dxrt::Processor::NPU)
                                   ? colorText(ColorName::Green, "[NPU]")
                                   : colorText(ColorName::Blue, "[CPU]");

        std::string tag = "";
        if (entryTasks.count(taskName))
        {
            tag += colorText(ColorName::Yellow, " (model input)");
        }
        if (outputTasks.count(taskName))
        {
            tag += colorText(ColorName::Yellow, " (model output)");
        }

        const auto& predecessors = taskPredecessors[taskName];
        if (predecessors.empty())
        {
            cout << "  " << coutColorText(ColorName::Cyan, taskName) << " " << procType << tag << endl;
        }
        else
        {
            cout << "  ";
            print_predecessors(predecessors);
            cout << coutColorText(ColorName::Gray, " -> ")
                 << coutColorText(ColorName::Cyan, taskName) << " " << procType << tag << endl;
        }
    }

    cout << "\n" << coutColorText(ColorName::Bold, "---------------------- Task Details --------------------------") << endl;

    // Create map for fast lookup
    std::map<std::string, TaskData*, std::less<>> taskDataMapForDetails;
    for (auto& td : dataList)
    {
        taskDataMapForDetails.emplace(td._name, &td);
    }

    int task_idx = 0;
    for (const auto& taskName : taskOrder)
    {
        auto graph_it = graphMap.find(taskName);
        if (graph_it == graphMap.end()) continue;

        auto task_it = taskDataMapForDetails.find(taskName);
        if (task_it == taskDataMapForDetails.end()) continue;
        const TaskData& taskData = *task_it->second;

        // Dependencies are prepared above in taskPredecessors/taskSuccessors

        // Task header with complete dependency info
        const std::string procType = (taskData._processor == dxrt::Processor::NPU)
                                   ? colorText(ColorName::Green, "[NPU]")
                                   : colorText(ColorName::Blue, "[CPU]");
        const ColorName taskColor = (taskData._processor == dxrt::Processor::NPU)
                                  ? ColorName::Green
                                  : ColorName::Blue;

        std::string tag = "";
        if (entryTasks.count(taskName))
        {
            tag += colorText(ColorName::Yellow, " (model input)");
        }
        if (outputTasks.count(taskName))
        {
            tag += colorText(ColorName::Yellow, " (model output)");
        }

        cout << "\n"
             << coutColorText(ColorName::Bold, "Task[" + std::to_string(task_idx) + "]") << ": "
             << coutColorText(taskColor, taskName) << " " << procType << tag << endl;
        task_idx++;

        // Dependencies
        const auto& predecessors = taskPredecessors[taskName];
        const auto& successors = taskSuccessors[taskName];

        // Dependencies one-line (arrow style) - only show if verbose
        if (options.verbose)
        {
            cout << "  +- Dependencies: [";
            print_dependency_list(predecessors);
              cout << "] " << coutColorText(ColorName::Gray, "->") << " "
                  << coutColorText(ColorName::Cyan, taskName) << " "
                  << coutColorText(ColorName::Gray, "->") << " [";
            print_dependency_list(successors);
            cout << "]" << endl;
        }

        // Memory information - tree style for readability - only show if verbose
        if (options.verbose)
        {
            if (taskData._processor == dxrt::Processor::NPU)
            {
                print_npu_memory_usage(taskData);
                print_memory_differences(taskData);
            }
            else
            {
                // Note: buffers_total calculation uses dynamic buffer count instead of fixed DXRT_TASK_MAX_LOAD
                size_t buffers_total = (taskData._inputSize + taskData._outputSize) * taskData.get_buffer_count();
                cout << "  +- Buffer Usage (Host Memory)" << endl;
                cout << "  |  +- Buffers (x" << taskData.get_buffer_count() << ") : " << format_bytes(buffers_total) << endl;
                cout << "  |     +- In: " << format_bytes(taskData._inputSize)
                     << ", Out: " << format_bytes(taskData._outputSize) << endl;
            }
        }

        // Input/Output tensor information with tree connectors
        print_detailed_tensors("Inputs", taskData._inputTensors, &taskData._npuInputTensorInfos,
                               taskData._processor == dxrt::Processor::NPU, options.verbose);
        print_detailed_tensors("Outputs", taskData._outputTensors, &taskData._npuOutputTensorInfos,
                               taskData._processor == dxrt::Processor::NPU, options.verbose);
     }

     return 0;
 }

 // Helper function to get base filename without extension
 static std::string getBaseName(const std::string& filepath)
 {
     size_t lastSlash = filepath.find_last_of("/\\");
     size_t lastDot = filepath.find_last_of(".");

     std::string filename = (lastSlash != std::string::npos) ?
                           filepath.substr(lastSlash + 1) :
                           filepath;

     if (lastDot != std::string::npos && lastDot > lastSlash)
     {
         filename = filename.substr(0, filename.find_last_of("."));
     }

     return filename;
 }

 // JSON binary extraction implementation
 int ParseModelJSONExtract(const string& file)
 {
     using std::cout;
     using std::endl;
     using std::ofstream;

     if (dxrt::fileExists(file) == false)
     {
         throw FileNotFoundException(EXCEPTION_MESSAGE(file));
     }

     dxrt::ModelDataBase modelData;
     LoadModelParam(modelData, file);

     std::string baseName = getBaseName(file);
     int extractedFiles = 0;

    cout << coutColorText(ColorName::Bold, "JSON Binary Data Extraction") << endl;
    cout << coutColorText(ColorName::Cyan, "Model: ") << file << endl;
     cout << endl;

     // Extract graph_info JSON
     const auto& graphInfo = modelData.deepx_binary.graph_info();
     if (!graphInfo.str().empty())
     {
         std::string graphFilename = baseName + "_graph_info.json";
         ofstream graphFile(graphFilename, std::ios::binary);
         if (graphFile.is_open())
         {
             graphFile.write(graphInfo.str().data(), graphInfo.str().size());
             graphFile.close();
               cout << coutColorText(ColorName::Green, "[OK] ")
                   << "Extracted graph info: " << coutColorText(ColorName::Cyan, graphFilename)
                  << " (" << format_bytes(graphInfo.str().size()) << ")" << endl;
             extractedFiles++;
         }
         else
         {
               cout << coutColorText(ColorName::Red, "[FAIL] ")
                  << "Failed to create: " << graphFilename << endl;
         }
     }

     // Extract rmap_info JSON files
     const auto& rmapInfoList = modelData.deepx_binary.rmap_info();
     for (size_t i = 0; i < rmapInfoList.size(); ++i)
     {
         const auto& rmapInfo = rmapInfoList[i];
         if (!rmapInfo.str().empty())
         {
             std::string rmapFilename = baseName + "_rmap_info_" + std::to_string(i);
             if (!rmapInfo.name().empty())
             {
                 rmapFilename = baseName + "_rmap_info_" + rmapInfo.name();
             }
             rmapFilename += ".json";

             ofstream rmapFile(rmapFilename, std::ios::binary);
             if (rmapFile.is_open())
             {
                 rmapFile.write(rmapInfo.str().data(), rmapInfo.str().size());
                 rmapFile.close();
                  cout << coutColorText(ColorName::Green, "[OK] ")
                      << "Extracted rmap info [" << i << "]: " << coutColorText(ColorName::Cyan, rmapFilename)
                      << " (" << format_bytes(rmapInfo.str().size()) << ")" << endl;
                 extractedFiles++;
             }
             else
             {
                  cout << coutColorText(ColorName::Red, "[FAIL] ")
                      << "Failed to create: " << rmapFilename << endl;
             }
         }
     }

     cout << endl;
     if (extractedFiles > 0)
     {
         cout << coutColorText(ColorName::Green, "Successfully extracted " + std::to_string(extractedFiles) + " JSON files.") << endl;
     }
     else
     {
         cout << coutColorText(ColorName::Yellow, "No JSON string data found in the model.") << endl;
     }

     return 0;
 }


 }  // namespace dxrt
