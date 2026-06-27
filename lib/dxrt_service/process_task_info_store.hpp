/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <cstdint>
#include <map>
#include <mutex>
#include <sys/types.h>
#include <utility>
#include <vector>

#include "process_with_device_info.h"

namespace dxrt {

class ProcessTaskInfoStore
{
 public:
    bool addTask(pid_t pid, int deviceId, int taskId, dxrt::npu_bound_op bound, uint64_t memUsage, const dxrt::InferenceContext &context)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        auto &info = _taskInfoMap[std::make_pair(pid, deviceId)];
        if (info.hasTask(taskId))
        {
            return false;
        }

        ProcessWithDeviceInfo::eachTaskInfo taskInfo{};
        taskInfo.pid = pid;
        taskInfo.deviceId = deviceId;
        taskInfo.bound = bound;
        taskInfo.mem_usage = memUsage;
        taskInfo.inferenceContext = context;
        info.InsertTaskInfo(taskId, taskInfo);
        return true;
    }

   /// Update the device I/O buffer fields of an existing task's InferenceContext.
   /// Called after HandleAllocate assigns a per-task staging buffer on the device.
    /// Returns false if (pid, deviceId, taskId) is not found.
    bool setIoBuffer(pid_t pid, int deviceId, int taskId,
                        uint64_t ioPhysAddrBase, uint64_t ioPhysAddrOffset, uint32_t ioBufferSize)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        auto it = _taskInfoMap.find(std::make_pair(pid, deviceId));
        if (it == _taskInfoMap.end() || !it->second.hasTask(taskId))
        {
            return false;
        }
        auto *ctx = it->second.mutableTaskInfo(taskId);
        if (ctx == nullptr) { return false; }
        ctx->inferenceContext.io_phys_addr_base   = ioPhysAddrBase;
        ctx->inferenceContext.io_phys_addr_offset = ioPhysAddrOffset;
        ctx->inferenceContext.io_buffer_size      = ioBufferSize;
        return true;
    }

    bool removeTask(pid_t pid, int deviceId, int taskId, dxrt::npu_bound_op *boundOut)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        auto it = _taskInfoMap.find(std::make_pair(pid, deviceId));
        if (it == _taskInfoMap.end() || !it->second.hasTask(taskId))
        {
            return false;
        }

        if (boundOut != nullptr)
        {
            *boundOut = it->second.getTaskBound(taskId);
        }

        it->second.deleteTaskFromMap(taskId);
        if (it->second.taskCount() == 0)
        {
            _taskInfoMap.erase(it);
        }
        return true;
    }

    bool hasTask(pid_t pid, int deviceId, int taskId) const
    {
        std::lock_guard<std::mutex> lock(_mutex);
        auto it = _taskInfoMap.find(std::make_pair(pid, deviceId));
        if (it == _taskInfoMap.end())
        {
            return false;
        }
        return it->second.hasTask(taskId);
    }

    std::vector<int> extractTaskIds(pid_t pid, int deviceId)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        auto it = _taskInfoMap.find(std::make_pair(pid, deviceId));
        if (it == _taskInfoMap.end())
        {
            return {};
        }

        std::vector<int> taskIds = it->second.getTaskIds();
        _taskInfoMap.erase(it);
        return taskIds;
    }

 private:
    using Key = std::pair<pid_t, int>;

    mutable std::mutex _mutex;
    std::map<Key, ProcessWithDeviceInfo> _taskInfoMap;
};

}  // namespace dxrt
