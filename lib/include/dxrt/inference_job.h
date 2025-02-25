// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <string>
#include <unordered_map>
#include <mutex>
#include <vector>
#include <memory>
#include <atomic>

#include "dxrt/common.h"
#include "dxrt/tensor.h"
#include "dxrt/request.h"
#include "dxrt/driver.h"
#include "dxrt/inference_timer.h"

#define INFERENCE_ID_MAX_VALUE 5000
#define JOB_ASYNC_LOAD    (5)

namespace dxrt { 
class Task;
class TaskData;
struct TimePoint;
using TimePointPtr = std::shared_ptr<TimePoint>;

class InferenceJob
{
 public:
    InferenceJob(int id) noexcept ;
    ~InferenceJob();

    void SetInferenceJob(std::vector<std::shared_ptr<Task>>& tasks_, std::shared_ptr<Task> head_, std::vector<string> lastOutputOrder);

    void onRequestComplete(RequestPtr req);

    int startJob(void *inputPtr, void *userArg, void *outputPtr);
    // void endRequest(RequestPtr req);

    TensorPtrs getOutput();
    // void setStatus(RequestStatus status);
    Request::Status getStatus(){return _status;}

    int latency() const {return _latency;}
    uint32_t inference_time() const {return _infTime;}
    void setInferenceEngineInterface(InferenceTimer* ptr);
    void setCallBack(std::function<int(TensorPtrs &outputs, void *userArg, int jobIc)> func);

    void Clear();

    static std::shared_ptr<InferenceJob> GetById(int id);
    static void InitInferenceJob();
    static std::shared_ptr<InferenceJob> Pick();
    void SetStoreResult(bool storeResult);

 private:
    std::vector<RequestWeakPtr> _requests;
    std::unordered_map<std::string, Tensor> _tensors;
    //std::unordered_map<int, std::weak_ptr<Task>> _tasks;
    RequestWeakPtr _head;
    std::weak_ptr<Task> _headTask;
    volatile Request::Status _status = Request::Status::REQ_IDLE;
    std::atomic<int> _outputCount = {0};
    std::atomic<int> _doneCount = {0};
    std::vector<std::string> _outputs;
    void* _userArg;
    int _latency;
    uint32_t _infTime;
    int _jobId;

    std::function<void(RequestPtr)> onRequestCompleteFunction();

    void onAllRequestComplete();
    InferenceTimer* _inferenceEnginePtr;
    std::function<int(TensorPtrs &outputs, void *userArg, int jobId)> _infEngCallback;
    bool _storeResult = false;
    TensorPtrs _returnOutputs = {};
    void setReturnOutputs();
    void ReleaseAllOutputBuffer();
    void* _outputPtr;
    

   static std::vector<std::shared_ptr<InferenceJob> > _sInferenceJobs;

   static std::mutex _sInferenceJobsLock;
   static std::mutex _sInferenceJobsMapLock;
   static std::mutex _sTensorsMapLock;
   static std::mutex _sRequestsLock;

   static std::atomic<int> _sNextInferenceJobId;
};

using InferenceJobPtr = std::shared_ptr<InferenceJob>;


}  // namespace dxrt
