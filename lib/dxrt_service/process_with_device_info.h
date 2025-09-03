/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include "dxrt/common.h"
#include <unordered_map>
#include <unordered_set>
#include <array>
#include "dxrt/driver.h"

namespace dxrt{
    bool operator==(const dxrt::dxrt_custom_weight_info_t& a, const dxrt::dxrt_custom_weight_info_t& b);
}


template <>
struct std::hash<dxrt::dxrt_custom_weight_info_t> {
    size_t operator()(const dxrt::dxrt_custom_weight_info_t& info) const;
};

class ProcessWithDeviceInfo
{
 public:
    struct eachTaskInfo
    {
        int pid;
        int deviceId;
        dxrt::npu_bound_op bound;
        uint64_t mem_usage;
    };
    static constexpr int BOUND_NUM = static_cast<int>(dxrt::npu_bound_op::N_BOUND_INF_MAX);

    int taskCount() const{return _taskInfo.size();}
    void InsertWeightInfo(dxrt::dxrt_custom_weight_info_t info){_weightInfo.insert(info);}
    void EraseWeightInfo(dxrt::dxrt_custom_weight_info_t info){_weightInfo.erase(info);}
    bool isEmpty() const{ return _weightInfo.empty() && _taskInfo.empty();}
    dxrt::npu_bound_op getTaskBound(int taskId) const
    {
        return getTaskInfo(taskId).bound;
    }
    const eachTaskInfo& getTaskInfo(int taskId) const;
    void deleteTaskFromMap(int taskId);
    bool hasTask(int taskId) const{return _taskInfo.find(taskId) != _taskInfo.end();}
    int firstTaskNumber() const;
    int getTaskIdByIndex(int index) const;
    std::vector<int> getTaskIds() const;
    std::array<int, BOUND_NUM> getBoundCounts() const;
    void InsertTaskInfo(int taskId, const eachTaskInfo& info);

 private:
    std::unordered_set<dxrt::dxrt_custom_weight_info_t> _weightInfo;
    std::unordered_map<int, eachTaskInfo> _taskInfo;
};

