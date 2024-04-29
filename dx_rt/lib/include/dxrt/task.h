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
#include <atomic>
#include <unordered_map>
#include <map>
#include <cstdarg>

namespace dxrt {
using rmapinfo = deepx_rmapinfo::RegisterInfoDatabase;
using TaskPtr = std::shared_ptr<Task>;
using TaskPtrs = std::vector<std::shared_ptr<Task>>;
class InferenceEngine;
struct TaskStats
{
    static TaskStats& GetInstance(int id);
    std::string name;
    int id;
    float latency_us;
    float inference_time_us;
    std::vector<int> latency_data;
    std::vector<uint32_t> inference_time_data;
};

class CpuHandle;
class Task
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
    std::atomic<int> &inference_count();
    std::vector< rmapinfo > npu_param();
    std::vector< dxrt_model_t > npu_model();
    std::vector< dxrt_request_t > npu_inference(int device);// to be moved to device
    TaskPtr &next();
    TaskPtrs &prevs();
    TaskPtrs &nexts();
    bool &is_head();
    bool &is_tail();
    bool &is_argmax();
    std::function<int(TensorPtrs&, void*)> callback();
    int latency();
    uint32_t inference_time();
    int &complete_cnt();
    InferenceEngine* &inference_engine();
    friend std::ostream& operator<<(std::ostream&, const Task&);
private:
    int _id;
    std::string _name = "EMPTY";
    std::string _onnxFile = "";
    std::vector<std::shared_ptr<Device>> _devices;
    std::vector<rmapinfo> _infos;
    std::vector< std::vector<uint8_t> > _data;
    std::vector< dxrt_model_t > _npuModel;
    
    Processor _processor = Processor::NONE_PROCESSOR;
    uint64_t _memUsage = 0;
    uint32_t _inputSize = 0;
    uint32_t _outputSize = 0;
    uint32_t _outputMemSize = 0;
    DataType _inputDataType;
    DataType _outputDataType;
    int _numInputs = 1;
    int _numOutputs;
    std::vector<std::string> _inputNames;
    std::vector<std::string> _outputNames;
    std::vector<std::vector<int64_t>> _inputShape;
    std::vector<std::vector<int64_t>> _outputShape;
    std::vector<uint64_t> _inputOffsets = {0};
    std::vector<uint64_t> _outputOffsets;
    TaskPtr _next;
    TaskPtrs _prevTasks;
    TaskPtrs _nextTasks;
    std::map<int, std::vector<int>> _inputTensorIdx;
    std::map<int, std::vector<int>> _outputTensorIdx;
    std::vector<std::queue<Tensor>> _inputTensorQueue;
    std::mutex _inputTensorQueueLock;
    Tensors _inputTensors;
    Tensors _outputTensors;
    bool _isHead = false;
    bool _isTail = false;
    bool _isArgMax = false;
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