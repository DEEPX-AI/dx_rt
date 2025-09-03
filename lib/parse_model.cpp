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
//#include <errno.h>
#ifdef __linux__
#include <cxxabi.h>
#endif
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <set>
//#include <algorithm>

#include "dxrt/common.h"
//#include "dxrt/datatype.h"
#include "dxrt/model.h"
//#include "dxrt/inference_engine.h"
#include "dxrt/task_data.h"
#include "dxrt/cpu_handle.h"
#include "dxrt/filesys_support.h"
#include "dxrt/exception/exception.h"
//#include "dxrt/dxrt_api.h"


using std::cout;
using std::endl;
using std::vector;
using std::map;
using std::string;
using std::set;



namespace dxrt
{

// Helper to add thousand separators to a number string
static string add_commas(const string& s) {
    int n = s.length();
    if (n <= 3) return s;
    int first_comma_pos = n % 3;
    if (first_comma_pos == 0) first_comma_pos = 3;
    string res = s.substr(0, first_comma_pos);
    for (int i = first_comma_pos; i < n; i += 3) {
        res += ',';
        res += s.substr(i, 3);
    }
    return res;
}


int ParseModel(string file)
{
    using std::cout;
    using std::endl;
    using std::vector;
    using std::map;
    using std::string;
    using std::to_string;

    if (dxrt::fileExists(file) == false)
    {
        //DXRT_ASSERT(false, "Can't find " + file);
        throw FileNotFoundException(EXCEPTION_MESSAGE(file));
    }

    std::map<std::string, deepx_graphinfo::SubGraph> graphMap;

    std::vector<TaskData> dataList;

    dxrt::ModelDataBase modelData;
    LoadModelParam(modelData, file);
    cout << "\n=======================================================" << endl;
    cout << " * Model File \n   : " << file << endl;
    cout << "-------------------------------------------------------" << endl;
    // Log .dxnn File Format Version
    cout << " * .dxnn Format version : v" << modelData.deepx_binary._dxnnFileFormatVersion << endl;
    cout << " * Compiler version     : v" << modelData.deepx_binary._compilerVersion << endl;
    cout << "=======================================================" << endl;

    cout << "\nModel Input Tensors:" << endl;
    for (const auto& input : modelData.deepx_graph.inputs()) {
        cout << "  - " << input << endl;
    }

    cout << "Model Output Tensors:" << endl;
    for (const auto& output : modelData.deepx_graph.outputs()) {
        cout << "  - " << output << endl;
    }

    std::vector<std::string> taskOrder = modelData.deepx_graph.topoSort_order();

    if (taskOrder.empty())
    {
        taskOrder.push_back(
            modelData.deepx_binary.rmap_info(0).name());
    }

    for (auto &order : taskOrder )
    {
        dxrt::rmapinfo rmapInfo;
        vector<vector<uint8_t>> data;
#ifdef USE_ORT
        bool is_cpu_model = false;
#endif
        bool found = false;
        auto graphs = modelData.deepx_graph.subgraphs();
        for (auto &graph : graphs)
        {
            if (order == graph.name())
            {
                graphMap[graph.name()] = graph;
                break;
            }
        }
        for (size_t j = 0; j < modelData.deepx_binary.rmap_info().size(); j++) {
            if (order == modelData.deepx_binary.rmap_info(j).name()) {
                rmapInfo = modelData.deepx_rmap.rmap_info(j);
                if (graphMap.find(order) != graphMap.end()) {
                    for (size_t k = 0; k < rmapInfo.inputs().size(); k++) {
                        rmapInfo.inputs()[k].memory().name() = graphMap[order].inputs()[k].name();
                    }
                }

                data.emplace_back(vector<uint8_t>(rmapInfo.model_memory().rmap().size()));
                auto& firstMemBuffer = modelData.deepx_binary.rmap(j).buffer();
                memcpy(data.back().data(), firstMemBuffer.data(), firstMemBuffer.size());
                DXRT_ASSERT(0 < data.back().size(), "invalid model");

                data.emplace_back(vector<uint8_t>(rmapInfo.model_memory().weight().size()));
                auto& weightBuffer = modelData.deepx_binary.weight(j).buffer();
                memcpy(data.back().data(), weightBuffer.data(), weightBuffer.size());
                DXRT_ASSERT(0 < data.back().size(), "invalid model");

                found = true;
            }
        }
#ifdef USE_ORT
        if (found == false)
        {
            for (size_t j=0; j < modelData.deepx_binary.cpu_models().size(); j++)
            {
                if (order == modelData.deepx_binary.cpu_models(j).name())
                {
                    const auto& bufferSource = modelData.deepx_binary.cpu_models(j).buffer();
                    data.emplace_back(bufferSource.begin(), bufferSource.end());

                    found = true;
                    is_cpu_model = true;
                    break;
                }
            }
        }
#endif
        if (found)
        {
            TaskData taskData(0, order, rmapInfo);
#ifdef USE_ORT
            if (is_cpu_model)
            {
                std::shared_ptr<CpuHandle> handle =
                    std::make_shared<CpuHandle>(data.front().data(), data.front().size(), order, 1);
                taskData.set_from_cpu(handle);
            }
            else
#endif
            {
                taskData.set_from_npu(data);
            }

            dataList.emplace_back(taskData);
        }
    }

    cout << "\nTasks:" << endl;
    int task_idx = 0;
    for (const auto& taskName : taskOrder) {
        auto graph_it = graphMap.find(taskName);
        if (graph_it == graphMap.end()) continue;
        const auto& subgraph = graph_it->second;

        TaskData* taskData = nullptr;
        for (auto& td : dataList) {
            if (td._name == taskName) {
                taskData = &td;
                break;
            }
        }
        if (!taskData) continue;

        // Determine predecessors and successors
        set<string> predecessors;
        for (const auto& input : subgraph.inputs()) {
            if (!input.owner().empty()) {
                predecessors.insert(input.owner());
            }
        }

        set<string> successors;
        for (const auto& output : subgraph.outputs()) {
            for (const auto& user : output.users()) {
                if (!user.empty()) {
                    successors.insert(user);
                }
            }
        }

        // Print dependencies
        cout << "  [ ";
        for (auto it = predecessors.begin(); it != predecessors.end(); ++it) {
            cout << *it << (std::next(it) == predecessors.end() ? "" : ", ");
        }
        cout << "] -> " << taskName << " -> [";
        for (auto it = successors.begin(); it != successors.end(); ++it) {
            cout << *it << (std::next(it) == successors.end() ? "" : ", ");
        }
        cout << "]" << endl;

        // Print task details
        cout << "  Task[" << task_idx++ << "] " << taskData->_name
            << ", " << taskData->_processor;

        if (taskData->_processor == dxrt::Processor::NPU) {
            cout << ", NPU memory usage " << add_commas(to_string(taskData->_memUsage))
                 << " bytes (input " << add_commas(to_string(taskData->_inputSize))
                 << ", output " << add_commas(to_string(taskData->_outputSize)) << ")" << endl;
        } else if (taskData->_processor == dxrt::Processor::CPU) {
            cout << ", input " << add_commas(to_string(taskData->_inputSize))
                 << " bytes, output " << add_commas(to_string(taskData->_outputSize)) << " bytes" << endl;
        } else {
            cout << " WARNING: Unknown processor type" << endl;
        }

        cout << "  Inputs" << endl;
        if (taskData->_processor == dxrt::Processor::NPU) {
            for (size_t i = 0; i < taskData->_inputTensors.size(); ++i) {
                const auto &tensor = taskData->_inputTensors[i];
                auto layout = static_cast<deepx_rmapinfo::Layout>(taskData->_npuInputTensorInfos[i].layout());
                std::string layout_str = deepx_rmapinfo::LayoutToString(layout);
                if (layout == deepx_rmapinfo::ALIGNED) {
                    auto transpose = static_cast<deepx_rmapinfo::Transpose>(taskData->_npuInputTensorInfos[i].transpose());
                    std::string transpose_str = deepx_rmapinfo::TransposeToString(transpose);
                    cout << "     -  " << tensor << "  [layout: " << layout_str << ", transpose: " << transpose_str << "]" << endl;
                } else {
                    cout << "     -  " << tensor << "  [layout: " << layout_str << "]" << endl;
                }
            }
        } else {
            for (const auto &tensor : taskData->_inputTensors) {
                cout << "     -  " << tensor << endl;
            }
        }
        cout << "  Outputs" << endl;
        if (taskData->_processor == dxrt::Processor::NPU) {
            for (size_t i = 0; i < taskData->_outputTensors.size(); ++i) {
                const auto &tensor = taskData->_outputTensors[i];
                auto layout = static_cast<deepx_rmapinfo::Layout>(taskData->_npuOutputTensorInfos[i].layout());
                std::string layout_str = deepx_rmapinfo::LayoutToString(layout);
                if (layout == deepx_rmapinfo::ALIGNED) {
                    auto transpose = static_cast<deepx_rmapinfo::Transpose>(taskData->_npuOutputTensorInfos[i].transpose());
                    std::string transpose_str = deepx_rmapinfo::TransposeToString(transpose);
                    cout << "     -  " << tensor << "  [layout: " << layout_str << ", transpose: " << transpose_str << "]" << endl;
                } else {
                    cout << "     -  " << tensor << "  [layout: " << layout_str << "]" << endl;
                }
            }
        } else {
            for (const auto &tensor : taskData->_outputTensors) {
                cout << "     -  " << tensor << endl;
            }
        }
        cout << endl;
    }
    return 0;
}


}  // namespace dxrt
