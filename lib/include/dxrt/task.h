// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "dxrt/common.h"
#include "dxrt/tensor.h"
#include "dxrt/request.h"
#include "dxrt/driver.h"
#include "dxrt/profiler.h"
#include "dxrt/circular_buffer.h"
#include "dxrt/model.h"
#include "dxrt/task_data.h"
#include <atomic>
#include <unordered_map>
#include <map>
#include <cstdarg>

namespace dxrt {
using rmapinfo = deepx_rmapinfo::RegisterInfoDatabase;
using TaskPtr = std::shared_ptr<Task>;
using TaskPtrs = std::vector<std::shared_ptr<Task>>;
class DXRT_API InferenceEngine;
struct DXRT_API TaskStats
{
    static TaskStats& GetInstance(int id);
    std::string name;
    int id;
    float latency_us;
    float inference_time_us;
    std::vector<int> latency_data;
    std::vector<uint32_t> inference_time_data;
};
struct CpuHandle;
class DXRT_API Task
{    
public:
    Task(std::string name_, std::vector<rmapinfo>, std::vector<std::vector<uint8_t>>&&);
    Task();
    ~Task(void);
    int Setup(void);
    int Check(void);
    // Tensors Run(Tensors inputs);
    // void Run(Tensors &inputs, Tensors &outputs);
    // RequestPtr InferenceRequest(Tensors inputs, Tensors outputs);
    int InferenceRequest(RequestPtr req);
    int ProcessResponse(RequestPtr req, dxrt_response_t *response=nullptr);
    void RegisterCallBack(std::function<int(TensorPtrs&, void*)>);
    int Release(void);
    int id();
    std::string name();
    void *input_buf(int deviceId, int bufId);
    Tensors inputs(void *ptr = nullptr, uint64_t phyAddr=0);
    Tensors outputs(void *ptr = nullptr, uint64_t phyAddr=0);
    Processor processor();
    uint32_t input_size();
    uint32_t output_size();
    uint32_t output_mem_size();
    std::map<int, std::vector<int>> &input_index();
    std::map<int, std::vector<int>> &output_index();
    void input_name_order(const std::vector<string>& order);
    const std::vector<string>& input_name_order() const;
    std::atomic<int> &inference_count();
    std::vector< rmapinfo > npu_param();
    std::vector< dxrt_model_t > npu_model();
    std::vector< dxrt_request_t > npu_inference(int device);// to be moved to device
    TaskPtr &next();
    TaskPtrs &prevs();
    vector<queue<Tensor>> &input_tensor_queue_set(size_t size);
    map<string, int> &input_tensor_queue_idx();
    vector<queue<Tensor>> &input_tensor_queue();
    TaskPtrs &nexts();
    bool &is_head();
    bool &is_tail();
    bool &is_argmax();
    std::function<int(TensorPtrs&, void*)> callback();
    int latency();
    uint32_t inference_time();
    int &complete_cnt();
    InferenceEngine* &inference_engine();
    friend DXRT_API std::ostream& operator<<(std::ostream&, const Task&);
private:


    TaskData _taskData;

    std::string _onnxFile = "";
    std::vector<std::shared_ptr<Device>> _devices;

    std::vector< std::vector<uint8_t> > _data;

    TaskPtr _next;
    TaskPtrs _prevTasks;
    TaskPtrs _nextTasks;
    std::map<int, std::vector<int>> _inputTensorIdx;
    std::map<int, std::vector<int>> _outputTensorIdx;
    std::vector<string> _inputNameOrder;
    std::vector<std::queue<Tensor>> _inputTensorQueue;
    std::map<std::string, int> _inputTensorQueueIdx;
    std::mutex _inputTensorQueueLock;
    std::mutex _reqLock;

    bool _isHead = false;
    bool _isTail = false;

    std::atomic<int> _inferenceCnt;
    std::function<int(TensorPtrs&, void*)> _callBack;
    CircularBuffer<int> _latency;
    CircularBuffer<uint32_t> _infTime;
    std::shared_ptr<CpuHandle> _cpuHandle;
    InferenceEngine *_ie;
    int _completeCnt = 1;
    static int nextId;
};
} // namespace dxrt