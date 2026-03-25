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

#include <atomic>
#include <unordered_map>
#include <map>
#include <cstdarg>
#include <mutex>
#include <memory>
#include <string>
#include <vector>

#include "dxrt/tensor.h"
#include "dxrt/request_data.h"
#include "dxrt/driver.h"
#include "dxrt/profiler.h"
#include "dxrt/model.h"
#include "dxrt/task_data.h"
#include "dxrt/inference_timer.h"
#include "dxrt/fixed_size_buffer.h"

namespace dxrt {
using rmapinfo = deepx_rmapinfo::RegisterInfoDatabase;
using TaskPtr = std::shared_ptr<Task>;
using TaskPtrs = std::vector<std::shared_ptr<Task>>;

struct DXRT_API TaskStats
{
    static TaskStats& GetInstance(int id);
    std::string name;
    int id = 0;
    float latency_us = 0.0f;
    float inference_time_us = 0.0f;
    std::vector<int> latency_data;
    std::vector<uint32_t> inference_time_data;
};
class CpuHandle;

// Struct for atomically allocating and freeing buffers
struct BufferSet {
    void* encoded_input = nullptr;
    void* output = nullptr;
    void* encoded_output = nullptr;

#ifdef USE_VNPU
    // Physical addresses for zero-copy DMA (CMA buffers only, 0 if not applicable)
    uint64_t encoded_input_phy = 0;
    uint64_t output_phy = 0;
    uint64_t encoded_output_phy = 0;
#endif // USE_VNPU

    BufferSet() = default;
};

class Request;
using RequestPtr = std::shared_ptr<Request>;
class DXRT_API Task // NOSONAR
{
public:
    Task(const std::string& name_, const rmapinfo& rmapInfo_, int bufferCount_,
        std::vector<std::vector<uint8_t>>&&, npu_bound_op boundOp = N_BOUND_NORMAL, bool hasPpuBinary = false);
    Task(const std::string& name_, const rmapinfo& rmapInfo_, int bufferCount_,
        std::vector<std::vector<uint8_t>>&&, npu_bound_op boundOp, const std::vector<int>& deviceIds, bool hasPpuBinary = false);

    Task();
    ~Task(void);

    void RegisterCallBack(const std::function<int(TensorPtrs&, void*)>& f);
    int id() const;
    std::string name() const;
    Tensors inputs(void *ptr = nullptr, uint64_t phyAddr = 0);
    Tensors outputs(void *ptr = nullptr, uint64_t phyAddr = 0);
    Processor processor() const;
    uint32_t input_size() const;
    uint32_t output_size() const;
    uint32_t output_mem_size() const;
    std::map<int, std::vector<int>> &input_index();
    std::map<int, std::vector<int>> &output_index();
    void input_name_order(const std::vector<std::string>& order);
    const std::vector<std::string>& input_name_order() const;
    std::atomic<int> &inference_count();
    rmapinfo npu_param() const;
    dxrt_model_t npu_model() const;

    TaskPtr &next();
    TaskPtrs &prevs();
    TaskPtrs &nexts();
    void set_head();
    void set_tail();
    bool &is_head();
    bool &is_tail();
    bool &is_PPU();
    bool &is_argmax();
    bool has_next() const;
    std::function<int(TensorPtrs&, void*)> callback() const;
    void PushLatency(int latency);
    void PushInferenceTime(uint32_t infTime);
    InferenceTimer& GetTaskTimer();
    int GetLatency();
    uint32_t GetNpuInferenceTime();
    int &GetCompleteCnt();
    void IncrementCompleteCount();
    void SetInferenceEngineTimer(InferenceTimer* ie);
    void SetEncodedInputBuffer(int size);
    void* GetEncodedInputBuffer();
    void ReleaseEncodedInputBuffer(void* ptr);
    void ClearEncodedInputBuffer();
    void SetOutputBuffer(int size);
    void* GetOutputBuffer();
    void* GetEncodedOutputBuffer();
    void ReleaseOutputBuffer(void* ptr);
    void ReleaseEncodedOutputBuffer(void* ptr);
    void ClearOutputBuffer();
#ifdef USE_VNPU
    void FlushEncodedOutputCache(void* ptr, uint32_t size, bool invalidate);
#endif // USE_VNPU
    BufferSet AcquireAllBuffers();
    void ReleaseAllBuffers(const BufferSet& buffers);

    const std::vector<int>& getDeviceIds() const;
    CpuHandle* getCpuHandle();
    int getNpuBoundOp() const;

    TaskData* getData() {return &_taskData;}
    void setLastOutput(Tensors t);
    Tensors getLastOutput();

    void setTailOffset(int64_t n);
    int64_t getTailOffset() const;

    // Unified Task-based service integration
    void InitializeTaskWithService(int device_id) const;
    void CleanupTaskFromService(int device_id) const;

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
    std::vector<std::string> _inputNameOrder;

    std::mutex _reqLock;
    std::mutex _completeCntLock;
    std::mutex _lastOutputLock;

    std::mutex _bufferMutex;

    bool _isHead = false;
    bool _isTail = false;

    std::atomic<int> _inferenceCnt {0};
    std::function<int(TensorPtrs&, void*)> _callBack;

    std::shared_ptr<CpuHandle> _cpuHandle;
    InferenceTimer _taskTimer;
    InferenceTimer* _inferenceEngineTimer;
    std::shared_ptr<FixedSizeBuffer> _taskOutputBuffer;
    Tensors _lastOutput;

    std::shared_ptr<FixedSizeBuffer> _taskEncodedInputBuffer;
    std::shared_ptr<FixedSizeBuffer> _taskEncodedOutputBuffer;

    int _completeCnt = 1;
    int _boundOp = 0;
    int64_t _tailOffset = 0;
    static int nextId;
    static std::mutex _nextIdLock;
    static int getNextId();
};

}  // namespace dxrt
