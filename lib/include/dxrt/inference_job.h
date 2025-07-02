// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <string>
#include <unordered_map>
#include <mutex>
#include <atomic>
#include <vector>
#include <memory>
#include <atomic>

#include "dxrt/common.h"
#include "dxrt/tensor.h"
#include "dxrt/request.h"
#include "dxrt/task.h"
#include "dxrt/driver.h"
#include "dxrt/inference_timer.h"

namespace dxrt { 
class Task;
class TaskData;
struct TimePoint;
using TimePointPtr = std::shared_ptr<TimePoint>;

template<typename T>
class CircularDataPool;
class InferenceEngine;

class InferenceJob
{
 public:
   enum Status
   {
      TASK_IDLE,
      TASK_READY,
      TASK_BUSY,
      TASK_DONE,
   };
    InferenceJob(int id) noexcept ;
    ~InferenceJob();

    void SetInferenceJob(std::vector<std::shared_ptr<Task>>& tasks_, std::shared_ptr<Task> head_, std::vector<string> lastOutputOrder);

    void onRequestComplete(RequestPtr req);
    bool checkAndSetTaskReady(TaskPtr taskPtr);
    void processReadyTask(TaskPtr taskPtr);

    int startJob(void *inputPtr, void *userArg, void *outputPtr);
    // void endRequest(RequestPtr req);

    TensorPtrs getOutput();
    void setStatus(Request::Status status);
    Request::Status getStatus();
    int getId();
    int latency() const {return _latency;}
    uint32_t inference_time() const {return _infTime;}
    void setInferenceEngineInterface(InferenceEngine* ptr);
    void setCallBack(std::function<int(TensorPtrs &outputs, void *userArg, int jobIc)> func);

    void Clear();

    void SetStoreResult(bool storeResult);
    friend class CircularDataPool<InferenceJob>;

    // wait for inference job to complete
    void Wait();

    // inference job for IE
    bool GetOccupiedJob() { return _occupiedJob.load(); }
    void SetOccupiedJob(bool occupied) { _occupiedJob.store(occupied); }

 private:
    std::vector<RequestWeakPtr> _requests;
    std::unordered_map<std::string, Tensor> _tensors;
    std::unordered_map<std::string, Status> _taskStatusMap;
    RequestWeakPtr _head;
    std::weak_ptr<Task> _headTask;
    std::atomic<int> _outputCount{0};
    std::atomic<int> _doneCount{0};
    std::vector<std::string> _outputs;
    void* _userArg;
    int _latency;
    uint32_t _infTime;
    int _jobId;

    std::function<void(RequestPtr)> onRequestCompleteFunction();

    void onAllRequestComplete();
    InferenceEngine* _inferenceEnginePtr;
    std::function<int(TensorPtrs &outputs, void *userArg, int jobId)> _infEngCallback;
    bool _storeResult = false;
    TensorPtrs _returnOutputs = {};
    void setReturnOutputs();
    void ReleaseAllOutputBuffer();
    void* _outputPtr;
    std::atomic<bool> _use_flag = {false};

    // Limit access to shared resources to one thread at a time. 
    // Acquire the mutex when reading or writing the _status value, and release it after the operation is complete.
    std::atomic<Request::Status> _status = {Request::Status::REQ_IDLE};
    std::mutex _lock;

    // wait for inference job to complete
    std::condition_variable _waitCV;
    std::mutex _waitMutex;

    std::atomic<bool> _occupiedJob {false};
   
};

using InferenceJobPtr = std::shared_ptr<InferenceJob>;


}  // namespace dxrt
