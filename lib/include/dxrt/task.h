// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "dxrt/common.h"
#include "dxrt/tensor.h"
#include "dxrt/request.h"
#include "dxrt/driver.h"
#include "dxrt/profiler.h"
#include "dxrt/model.h"
#include "dxrt/task_data.h"
#include "dxrt/inference_timer.h"
#include "dxrt/fixed_size_buffer.h"
#include <atomic>
#include <unordered_map>
#include <map>
#include <cstdarg>

namespace dxrt {
using rmapinfo = deepx_rmapinfo::RegisterInfoDatabase;
using TaskPtr = std::shared_ptr<Task>;
using TaskPtrs = std::vector<std::shared_ptr<Task>>;

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
class CpuHandle;
class DXRT_API Task
{    
public:
    Task(std::string name_, std::vector<rmapinfo>, std::vector<std::vector<uint8_t>>&&, npu_bound_op boundOp = N_BOUND_NORMAL);
    Task(std::string name_, std::vector<rmapinfo>, std::vector<std::vector<uint8_t>>&&, npu_bound_op boundOp, std::vector<std::shared_ptr<Device>> devices_);

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
    //void *input_buf(int deviceId, int bufId);
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
    std::shared_ptr<Buffer> OutputBuffer();
    TaskPtr &next();
    TaskPtrs &prevs();
    TaskPtrs &nexts();
    void set_head();
    void set_tail();
    bool &is_head();
    bool &is_tail();
    bool &is_PPU();
    bool &is_argmax();
    std::function<int(TensorPtrs&, void*)> callback();
    void PushLatency(int latency);
    void PushInferenceTime(uint32_t infTime);
    int latency();
    uint32_t inference_time();
    int &complete_cnt();
    void SetInferenceEngineTimer(InferenceTimer* ie);
    void SetOutputBuffer(int size);
    void SetInputBuffer(int size);
    void* GetOutputBuffer();
    void* GetInputBuffer();
    void FreeOutputBuffer(void* ptr);
    void FreeInputBuffer(void* ptr);
    void ClearOutputBuffer();   
    void ClearInputBuffer();

    TaskData* getData() {return &_taskData;}
    friend DXRT_API std::ostream& operator<<(std::ostream&, const Task&);
private:


    TaskData _taskData;

    std::string _onnxFile = "";
    std::vector<int> _device_ids;

    std::vector< std::vector<uint8_t> > _data;

    TaskPtr _next;
    TaskPtrs _prevTasks;
    TaskPtrs _nextTasks;
    std::map<int, std::vector<int>> _inputTensorIdx;
    std::map<int, std::vector<int>> _outputTensorIdx;
    std::vector<string> _inputNameOrder;

    std::mutex _reqLock;

    bool _isHead = false;
    bool _isTail = false;

    std::atomic<int> _inferenceCnt;
    std::function<int(TensorPtrs&, void*)> _callBack;

    std::shared_ptr<CpuHandle> _cpuHandle;
    InferenceTimer _inferenceTimer;
    InferenceTimer* _ie;
    std::shared_ptr<FixedSizeBuffer> _taskOutputBuffer;
    std::shared_ptr<FixedSizeBuffer> _taskInputBuffer;


    int _completeCnt = 1;
    int _boundOp = 0;
    static int nextId;
};
} // namespace dxrt