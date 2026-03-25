/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

 #include <stdlib.h>
 #include <stdio.h>
 #include <stdint.h>
 #include <fcntl.h>
 #ifdef __linux__
 #include <cxxabi.h>
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

 // Forward declarations
 int ParseModelJSONExtract(const std::string& file);
 int ParseModelDetailed(const std::string& file, const ParseOptions& options);


 // ANSI escape codes for terminal text colors
 namespace Color {
    static bool& get_color_enabled()
    {
        static bool color_enabled = true;
        return color_enabled;
    }

#ifdef _WIN32
    static bool& get_windows_color_initialized()
    {
        static bool windows_color_initialized = false;
        return windows_color_initialized;
    }

     // Initialize Windows console for ANSI color support
    static void init_windows_console()
    {
         if (!get_windows_color_initialized())
         {
             HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
             if (hOut != INVALID_HANDLE_VALUE)
             {
                 DWORD dwMode = 0;
                 if (GetConsoleMode(hOut, &dwMode))
                 {
                     dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                     SetConsoleMode(hOut, dwMode);
                 }
             }
             get_windows_color_initialized() = true;
         }
     }
#endif

    static std::string get_color(const std::string& color_code)
    {
        if (!get_color_enabled())
        {
            return "";
        }
        else
        {
            return color_code;
        }
    }

     const std::string RESET       = "\033[0m";
     const std::string BOLD        = "\033[1m";
     const std::string YELLOW      = "\033[1;33m";
     const std::string GREEN       = "\033[1;32m";
     const std::string BLUE        = "\033[1;34m";
     const std::string RED         = "\033[1;31m";
     const std::string PURPLE      = "\033[1;35m";
     const std::string CYAN        = "\033[1;36m";
     const std::string GRAY        = "\033[90m";

     void enable_color(bool enable)
     {
        get_color_enabled() = enable;
     }

     std::string reset() { return get_color(RESET); }
     std::string bold() { return get_color(BOLD); }
     std::string yellow() { return get_color(YELLOW); }
     std::string green() { return get_color(GREEN); }
     std::string blue() { return get_color(BLUE); }
     std::string red() { return get_color(RED); }
     std::string purple() { return get_color(PURPLE); }
     std::string cyan() { return get_color(CYAN); }
     std::string gray() { return get_color(GRAY); }
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
     cout << "  +- " << Color::bold() << title << ":" << Color::reset() << endl;
     if (tensors.empty())
     {
         cout << (is_outputs ? "     " : "  |  ") << "+- (None)" << endl;
         return;
     }
     for (size_t i = 0; i < tensors.size(); ++i)
     {
         const auto& tensor = tensors[i];
         const char* prefix = is_outputs ? "     +- " : "  |  +- ";

         cout << prefix << Color::cyan() << tensor.name() << Color::reset();

         // Show detailed info in verbose mode
         if (verbose)
         {
             cout << Color::gray() << " {shape: " << format_tensor_shape(tensor)
                  << ", dtype: " << get_tensor_dtype_string(tensor)
                  << ", size: " << format_bytes(calculate_tensor_bytes(tensor)) << "}" << Color::reset();
         }

         // Show layout/transpose info only in verbose mode
         if (verbose && is_npu && tensorInfos && i < tensorInfos->size())
         {
             auto layout = static_cast<deepx_rmapinfo::Layout>((*tensorInfos)[i]._layout);
             std::string layout_str = deepx_rmapinfo::LayoutToString(layout);
             cout << Color::gray() << " [layout: " << layout_str;
             if (layout == deepx_rmapinfo::ALIGNED)
             {
                 auto transpose = static_cast<deepx_rmapinfo::Transpose>((*tensorInfos)[i]._transpose);
                 std::string transpose_str = deepx_rmapinfo::TransposeToString(transpose);
                 cout << ", transpose: " << transpose_str;
             }
             cout << "]" << Color::reset();
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
     // Set color mode
     Color::enable_color(!options.no_color);

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

    cout << "\n" << Color::bold() << "===================== Model Information ======================" << Color::reset() << endl;
    cout << Color::bold() << " Model File Path        : " << Color::cyan() << file << Color::reset() << endl;
    cout << Color::bold() << " .dxnn Format Version   : " << Color::green() <<"v"<< modelData.deepx_binary._dxnnFileFormatVersion << Color::reset() << endl;
    cout << Color::bold() << " DX-COM Version         : " << Color::green() <<"v"<< modelData.deepx_binary._compilerVersion << Color::reset() << endl;
    cout << endl;
    cout << Color::bold() << " Model Input Tensors:" << Color::reset() << endl;
    for (const auto& input : modelData.deepx_graph.inputs())
    {
        cout << "  - " << Color::cyan() << input << Color::reset() << endl;
    }
    cout << endl;
    cout << Color::bold() << " Model Output Tensors:" << Color::reset() << endl;
    for (const auto& output : modelData.deepx_graph.outputs())
    {
        cout << "  - " << Color::cyan() << output << Color::reset() << endl;
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
        const bool is_v8_format = (modelData.deepx_binary._dxnnFileFormatVersion == 8);

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
    cout << Color::bold() << " Model Memory Usage:" << Color::reset() << endl;
    cout << "  - " << Color::bold() << "Total             : " << Color::purple() << format_bytes(totalModelMemory) << Color::reset() << endl;
    cout << "  - " << Color::bold() << "Buffers           : " << Color::purple() << format_bytes(totalBufferMemory) << Color::reset() << endl;
    cout << "  - " << Color::bold() << "NPU Tasks Count   : " << Color::purple() << npuTaskCount << Color::reset() << endl;
    cout << "  - " << Color::bold() << "Buffer Pool Size  : " << Color::purple() << "x" << npu_buffer_count << Color::reset() << endl;

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
               cout << Color::bold() << Color::yellow()
                    << " ⚠ Warning: Model size exceeds Device " << i << " memory (" << format_bytes(deviceMemSizes[i]) << "), but can fit by reducing buffer pool size to x" << recommendedPoolSize << " or less."
                    << Color::reset() << endl;
           }
           else
           {
               cout << Color::bold() << Color::red()
                    << " ✗ Error: Model size exceeds Device " << i << " memory (" << format_bytes(deviceMemSizes[i]) << ") - cannot fit even with minimum buffer pool size."
                    << Color::reset() << endl;
           }
       }
    }
#endif

    cout << "\n" << Color::bold() << "================== Task Graph Information ====================" << Color::reset() << endl;

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
                cout << Color::gray() << ", ";
            }
            cout << Color::gray() << *it;
        }
    };

    // Helper lambda to print dependency list
    auto print_dependency_list = [](const set<string>& items) {
        for (auto it = items.begin(); it != items.end(); ++it)
        {
            cout << Color::cyan() << *it << Color::reset() << (std::next(it) == items.end() ? "" : ", ");
        }
    };

    // Helper lambda to print NPU memory usage details
    auto print_npu_memory_usage = [](const TaskData& taskData) {
        uint64_t model_bytes = taskData._npuModel.rmap.size + taskData._npuModel.weight.size;
        uint64_t buffers_total = (static_cast<uint64_t>(taskData._encodedInputSize) + taskData._outputMemSize) * taskData.get_buffer_count();
        uint64_t input_device_mem = static_cast<uint64_t>(taskData._encodedInputSize) * taskData.get_buffer_count();
        uint64_t output_device_mem = static_cast<uint64_t>(taskData._outputMemSize) * taskData.get_buffer_count();

        cout << "  +- Memory Usage (NPU Device)" << endl;
        cout << "  |  +- Total        : " << Color::bold() << format_bytes(taskData._memUsage) << Color::reset() << endl;
        cout << "  |  +- Model        : " << format_bytes(model_bytes) << endl;
        cout << "  |  +- Buffers (x" << taskData.get_buffer_count() << ") : " << format_bytes(buffers_total) << endl;
        cout << "  |     +- Input buffers  : " << format_bytes(input_device_mem)
             << " " << Color::gray() << "(" << format_bytes(taskData._encodedInputSize) << " x " << taskData.get_buffer_count() << ")" << Color::reset() << endl;
        cout << "  |     +- Output buffers : " << format_bytes(output_device_mem)
             << " " << Color::gray() << "(" << format_bytes(taskData._outputMemSize) << " x " << taskData.get_buffer_count() << ")" << Color::reset() << endl;
    };

    // Helper lambda to print logical vs device memory differences
    auto print_memory_differences = [](const TaskData& taskData) {
        const bool has_difference = (taskData._outputMemSize != taskData._outputSize ||
                                   taskData._encodedInputSize != taskData._inputSize);
        if (!has_difference) return;

        cout << "  |" << endl;
        cout << "  |  " << Color::gray() << "Logical tensor size vs Device footprint:" << Color::reset() << endl;

        if (taskData._encodedInputSize != taskData._inputSize)
        {
            cout << "  |     +- Input  (logical) : " << format_bytes(taskData._inputSize) << endl;
            cout << "  |     +- Input  (device)  : " << format_bytes(taskData._encodedInputSize)
                 << " " << Color::yellow() << "(NPU format conversion)" << Color::reset() << endl;
        }
        else
        {
            cout << "  |     +- Input  (logical) : " << format_bytes(taskData._inputSize) << endl;
        }

        if (taskData._outputMemSize != taskData._outputSize)
        {
            cout << "  |     +- Output (logical) : " << format_bytes(taskData._outputSize) << endl;
            cout << "  |     +- Output (device)  : " << format_bytes(taskData._outputMemSize)
                 << " " << Color::yellow() << "(includes scratch memory)" << Color::reset() << endl;
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

    cout << "\n" << Color::bold() << "-------------------- Task Dependencies -----------------------\n" << Color::reset() << endl;

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

        string procType = (taskData._processor == dxrt::Processor::NPU) ?
                          Color::green() + "[NPU]" + Color::reset() :
                          Color::blue() + "[CPU]" + Color::reset();

        string tag = "";
        if (entryTasks.count(taskName))
        {
            tag += Color::yellow() + " (model input)" + Color::reset();
        }
        if (outputTasks.count(taskName))
        {
            tag += Color::yellow() + " (model output)" + Color::reset();
        }

        const auto& predecessors = taskPredecessors[taskName];
        if (predecessors.empty())
        {
            cout << "  " << Color::cyan() << taskName << Color::reset() << " " << procType << tag << endl;
        }
        else
        {
            cout << "  ";
            print_predecessors(predecessors);
            cout << Color::gray() << " -> " << Color::cyan() << taskName << Color::reset() << " " << procType << tag << endl;
        }
    }

    cout << "\n" << Color::bold() << "---------------------- Task Details --------------------------" << Color::reset() << endl;

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
        string procType = (taskData._processor == dxrt::Processor::NPU) ?
                          Color::green() + "[NPU]" + Color::reset() :
                          Color::blue() + "[CPU]" + Color::reset();
        string taskColor = (taskData._processor == dxrt::Processor::NPU) ? Color::green() : Color::blue();

        string tag = "";
        if (entryTasks.count(taskName))
        {
            tag += Color::yellow() + " (model input)" + Color::reset();
        }
        if (outputTasks.count(taskName))
        {
            tag += Color::yellow() + " (model output)" + Color::reset();
        }

        cout << "\n"
             << Color::bold() << taskColor << "Task[" << task_idx << "]" << Color::reset() << ": "
             << Color::cyan() << taskName << Color::reset() << " " << procType << tag << endl;
        task_idx++;

        // Dependencies
        const auto& predecessors = taskPredecessors[taskName];
        const auto& successors = taskSuccessors[taskName];

        // Dependencies one-line (arrow style) - only show if verbose
        if (options.verbose)
        {
            cout << "  +- Dependencies: [";
            print_dependency_list(predecessors);
            cout << "] " << Color::gray() << "->" << Color::reset() << " "
                 << Color::cyan() << taskName << Color::reset() << " "
                 << Color::gray() << "->" << Color::reset() << " [";
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

     cout << Color::bold() << "JSON Binary Data Extraction" << Color::reset() << endl;
     cout << Color::cyan() << "Model: " << file << Color::reset() << endl;
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
             cout << Color::green() << "[OK] " << Color::reset()
                  << "Extracted graph info: " << Color::cyan() << graphFilename << Color::reset()
                  << " (" << format_bytes(graphInfo.str().size()) << ")" << endl;
             extractedFiles++;
         }
         else
         {
             cout << Color::red() << "[FAIL] " << Color::reset()
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
                 cout << Color::green() << "[OK] " << Color::reset()
                      << "Extracted rmap info [" << i << "]: " << Color::cyan() << rmapFilename << Color::reset()
                      << " (" << format_bytes(rmapInfo.str().size()) << ")" << endl;
                 extractedFiles++;
             }
             else
             {
                 cout << Color::red() << "[FAIL] " << Color::reset()
                      << "Failed to create: " << rmapFilename << endl;
             }
         }
     }

     cout << endl;
     if (extractedFiles > 0)
     {
         cout << Color::bold() << Color::green() << "Successfully extracted "
              << extractedFiles << " JSON files." << Color::reset() << endl;
     }
     else
     {
         cout << Color::yellow() << "No JSON string data found in the model." << Color::reset() << endl;
     }

     return 0;
 }


 }  // namespace dxrt
