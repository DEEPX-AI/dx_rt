// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.



#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <cxxabi.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>

#include "dxrt/common.h"
#include "dxrt/datatype.h"
#include "dxrt/model.h"
#include "dxrt/inference_engine.h"
#include "dxrt/task_data.h"
#include "dxrt/cpu_handle.h"
#include "dxrt/filesys_support.h"


using std::cout;
using std::endl;
using std::vector;
using std::map;
using std::string;



namespace dxrt
{


int ParseModel(string file)
{
    if (dxrt::fileExists(file) == false)
    {
        DXRT_ASSERT(false, "Can't find " + file);
        return -1;
    }

    std::map<std::string, deepx_graphinfo::GraphBindingDatabase> graphMap;

    std::vector<TaskData> dataList;

    string name = getAbsolutePath(file);
    dxrt::ModelDataBase modelData = LoadModelParam(file);
    std::vector<std::string> taskOrder = modelData.deepx_graph.topoSort_order();

    if (taskOrder.empty())
    {
        taskOrder.push_back(
            modelData.deepx_binary.rmap_info(0).name());
    }

    for (auto &order : taskOrder )
    {
        vector<dxrt::rmapinfo> rmapInfos;
        vector<vector<uint8_t>> data;
        bool is_cpu_model = false;
        bool found = false;
        auto graphs = modelData.deepx_graph.m_graph();
        for (auto &graph : graphs)
        {
            if (order == graph.name())
            {
                graphMap[graph.name()] = graph;
                break;
            }
        }

        for (size_t j = 0; j < modelData.deepx_binary.rmap_info().size(); j++)
        {
            if (order == modelData.deepx_binary.rmap_info(j).name())
            {
                auto rmapInfo = modelData.deepx_rmap.m_rmap(j);
                if (graphMap.find(order) != graphMap.end())
                    rmapInfo.input().memory().name() = graphMap[order].inputs().front().key();
                rmapInfos.emplace_back(rmapInfo);
                data.emplace_back(move(vector<uint8_t>(rmapInfo.memorys().memory(0).size(), 0)) );
                memcpy(data.back().data(), modelData.deepx_binary.rmap(j).buffer(), data.back().size());
                DXRT_ASSERT(0 < data.back().size(), "invalid model");
                data.emplace_back(move(vector<uint8_t>(rmapInfo.memorys().memory(1).size(), 0)) );
                memcpy(data.back().data(), modelData.deepx_binary.weight(j).buffer(), data.back().size());
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

                    data.emplace_back(move(vector<uint8_t>(modelData.deepx_binary.cpu_models(j).size(), 0)) );
                    memcpy(data.back().data(),
                        static_cast<void*>(modelData.deepx_binary.cpu_models(j).buffer()), data.back().size());
                    found = true;
                    is_cpu_model = true;
                    break;
                }
            }
        }
#endif
        if (found)
        {
            TaskData taskData(0, order, rmapInfos);
#ifdef USE_ORT
            if (is_cpu_model)
            {
                std::shared_ptr<CpuHandle> handle =
                    std::make_shared<CpuHandle>(data.front().data(), data.front().size(), order);
                taskData.set_from_cpu(handle);
            }
            else
#endif
            {
                taskData.set_from_npu(data);
            }

            dataList.emplace_back(taskData);
#ifndef USE_ORT
            break;  // force single task
#endif
        }
    }

    for (const auto &order : taskOrder )
    {
        auto& graphElem = graphMap[order];
        std::map<string, vector<string>> input_list, output_list;

        for (auto &prev : graphElem.inputs())
        {
            input_list[prev.val()].push_back(prev.key());
        }

        for (auto &next : graphElem.outputs())
        {
            output_list[next.val()].push_back(next.key());
        }

        cout << "[ ";
        for (auto &prev : input_list)
        {
            if (prev.first == "") continue;
            cout << prev.first << ", ";
        }
        cout << " ] -> " << order << " -> [";
        for (auto &next : output_list)
        {
            if (next.first == "") continue;
            cout << next.first << ", ";
        }
        cout << " ]" << endl;
        cout << "  inputs" << endl;
        for (auto &prev : input_list)
        {
            cout << "    " << prev.first << " -> " << order << endl;

            for (auto &data_name : prev.second)
            {
                if (data_name == "")continue;
                cout << "      " << data_name << endl;
            }
        }
        cout << "  outputs" << endl;
        for (auto &next : output_list)
        {
            cout << "    " << order << " -> " << next.first << endl;

            for (auto &data_name : next.second)
            {
                if (data_name == "")continue;
                cout << "      " << data_name << endl;
            }
        }
    }
    for (auto& taskData : dataList)
    {
        cout << dec << "  Task[" <<taskData._id << "] " << taskData._name
            << ", " << taskData._processor << ", " << taskData._memUsage
            << " bytes (input " << taskData._inputSize << ", output " << taskData._outputSize << ")" << endl;
        cout << "    inputs" << endl;
        for (auto &tensor : taskData._inputTensors) cout << "      " << tensor << endl;
        cout << "    outputs" << endl;
        for (auto &tensor : taskData._outputTensors) cout << "      " << tensor << endl;
    }
    return 0;
}




}  // namespace dxrt
