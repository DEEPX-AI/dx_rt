/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/common.h"
#include "process_with_device_info.h"
#include <unordered_set>
#include <array>
#include "dxrt/driver.h"

bool dxrt::operator==(const dxrt::dxrt_custom_weight_info_t& a, const dxrt::dxrt_custom_weight_info_t& b) {
    return (a.address == b.address &&
            a.size == b.size &&
            a.checksum == b.checksum);
}


// use code segments from boost::hash_combile
template <class T>
inline void hash_combine(std::size_t & seed, const T & v)
{
  std::hash<T> hasher;
  seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}


size_t std::hash<dxrt::dxrt_custom_weight_info_t>::
    operator()(const dxrt::dxrt_custom_weight_info_t& info) const
{
    size_t seed = 0x2C19FABB;
    hash_combine(seed, info.address);
    hash_combine(seed, info.size);
    hash_combine(seed, info.checksum);
    return seed;
}


void ProcessWithDeviceInfo::deleteTaskFromMap(int taskId)
{
    auto taskIt = _taskInfo.find(taskId);
    if (taskIt != _taskInfo.end()) {
        _taskInfo.erase(taskIt);
    } else {
        LOG_DXRT_S_ERR("Task " + std::to_string(taskId) + " not found for cleanup");
    }
}
const ProcessWithDeviceInfo::eachTaskInfo& ProcessWithDeviceInfo::getTaskInfo(int taskId) const
{
    return _taskInfo.at(taskId);
}
int ProcessWithDeviceInfo::firstTaskNumber() const
{

    return getTaskIdByIndex(0);
}
int ProcessWithDeviceInfo::getTaskIdByIndex(int index) const
{
    if (index < 0)
    {
        return -1;
    }
    if (_taskInfo.size() >= static_cast<size_t>(index))
    {
        return -1;
    }
    auto it = _taskInfo.begin();
    for (int i = 0; i < index; i++)
    {
        it++;
    }
    return it->first;
}
void ProcessWithDeviceInfo::InsertTaskInfo(int taskId, const eachTaskInfo& info)
{
    _taskInfo.insert(std::make_pair(taskId, info));
}
std::vector<int> ProcessWithDeviceInfo::getTaskIds() const
{
    std::vector<int> retval;
    for (auto it : _taskInfo)
    {
        retval.push_back(it.first);
    }
    return retval;
}
std::array<int, ProcessWithDeviceInfo::BOUND_NUM> ProcessWithDeviceInfo::getBoundCounts() const
{
    std::array<int, BOUND_NUM> retval;
    retval.fill(0);

    for (auto it : _taskInfo)
    {
        int bound = it.second.bound;
        retval[bound]++;
    }
    return retval;
}