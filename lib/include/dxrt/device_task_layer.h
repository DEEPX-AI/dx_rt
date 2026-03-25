/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */


#pragma once

// project common
#include "dxrt/common.h"

// self header (none)

// C headers (none)

// C++ headers
#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <array>

// project headers
#include "dxrt/device_core.h"
#include "dxrt/device_struct.h"
#include "dxrt/driver.h"
#include "dxrt/exception/server_err.h"
#include "dxrt/handler_que_template.h"
#include "dxrt/memory_interface.h"
#include "dxrt/npu_memory_cache.h"
#include "dxrt/service_abstract_layer.h"
#include "dxrt/npu_memory_cache.h"

namespace dxrt {

class Device;
class TaskData;
class RequestData;

// TODO: Refactor DeviceTaskLayer to reduce field count. Consider extracting
//       service layer, memory cache, and inference state into separate classes.
class DXRT_API DeviceTaskLayer { // NOSONAR: Too many fields - stable as-is, refactoring deferred
 public:
    explicit DeviceTaskLayer(std::shared_ptr<DeviceCore> core, std::shared_ptr<ServiceLayerInterface> service_interface);

    virtual ~DeviceTaskLayer() = default;
    DeviceTaskLayer(const DeviceTaskLayer&) = delete;
    DeviceTaskLayer& operator=(const DeviceTaskLayer&) = delete;
    DeviceTaskLayer(DeviceTaskLayer&&) = delete;
    DeviceTaskLayer& operator=(DeviceTaskLayer&&) = delete;

    int load() const;
    void pick();
    int infCnt() const;

    // connection
    int id() const { return core()->id(); }
    dxrt_device_status_t Status() const { return core()->Status(); }

    // virtual abstrect methods
    virtual int InferenceRequest(RequestData *req, npu_bound_op boundOp = N_BOUND_NORMAL) = 0;
    virtual int RegisterTask(TaskData *task) = 0;
    virtual int Release(TaskData *task) = 0;
    virtual void StartThread() = 0;


    int Response(dxrt_response_t &response);
    void BoundOption(dxrt_sche_sub_cmd_t subCmd, npu_bound_op boundOp);
    void Terminate();
    void Reset(int opt);
    void ResetBuffer(int opt);
    int64_t Allocate(uint64_t size) const;
    void Deallocate(uint64_t addr) const;

    void RegisterCallback(std::function<void()> f);

    dxrt_model_t npu_model(int taskId);
    virtual std::vector<Tensors> inputs(int taskId) = 0;
    Tensors outputs(int taskId);


    void popInferenceStruct(uint32_t requestId);
    void signalToWorker(int channel);
    void Deallocate_npuBuf(int64_t addr, int taskId);
    int64_t AllocateFromCache(int64_t size, int taskId);
    void StartDev(uint32_t option) { core()->StartDev(option); }
    bool isBlocked() const { return core()->isBlocked();  }
    void block() { core()->block(); }
    void unblock() { core()->unblock(); }
    virtual int getFullLoad() const = 0;

    void CallBack();

    virtual void ProcessResponseFromService(const dxrt_response_t &resp) = 0;
    [[noreturn]] void ProcessErrorFromService(dxrt_server_err_t err, int value);

    // DMA Abort Recovery
    bool isRecoveryInProgress() const { return _recoveryInProgress.load(std::memory_order_acquire); }
    void setRecoveryFlag(bool value) { _recoveryInProgress.store(value, std::memory_order_release); }
    uint32_t recoveryEpoch() const { return _recoveryEpoch.load(std::memory_order_acquire); }
    void waitForInflightDmaCompletion(uint32_t timeoutMs);
    int triggerRecovery();
    void reloadModelsIfNeeded();
    void SetProcessResponseHandler(const std::function<void(int deviceId, int reqId, const dxrt_response_t *response)>& handler) {
        _processResponseHandler = handler;
    }
    std::shared_ptr<DeviceCore> core() { return _core; }

    std::shared_ptr<DeviceCore> core() const { return _core; }

 protected:
     bool isStopFlag(std::memory_order order = std::memory_order_seq_cst) const { return _stop.load(order); }
     void setStopFlag(bool value = true, std::memory_order order = std::memory_order_seq_cst) { _stop.store(value, order); }
    std::mutex& stateLock() { return _lock; }
    std::mutex& npuInferenceLock() { return _npuInferenceLock; }
    std::shared_ptr<ServiceLayerInterface>& serviceLayer() { return _serviceLayer; }
    std::unordered_map<int, dxrt::dxrt_model_t>& npuModelMap() { return _npuModel; }
    NpuMemoryCacheManager& memoryCacheManager() { return _npuMemoryCacheManager; }
    std::function<void(int deviceId, int reqId, const dxrt_response_t *response)>& processResponseHandler() {
        return _processResponseHandler;
    }
    std::atomic<int>& loadCounter() { return _load; }
    std::condition_variable& recoveryCondVar() { return _recoveryCondVar; }
    std::mutex& recoveryMutex() { return _recoveryMutex; }

 private:
    std::shared_ptr<DeviceCore> _core;
    std::atomic<int> _load{0};
    std::atomic<int> _inferenceCnt{0};
    std::mutex _lock;
    std::atomic<bool> _stop{false};
    std::shared_ptr<ServiceLayerInterface> _serviceLayer;
    std::mutex _npuInferenceLock;
    std::unordered_map<int, dxrt::dxrt_model_t> _npuModel;
    std::function<void()> _onCompleteInferenceHandler = [](){
        // Default no-op handler
    };
    NpuMemoryCacheManager _npuMemoryCacheManager;
    std::function<void(int deviceId, int reqId, const dxrt_response_t *response)> _processResponseHandler;

    // DMA Abort Recovery state
    std::atomic<bool> _recoveryInProgress{false};
    std::atomic<uint32_t> _recoveryEpoch{0};
    std::mutex _recoveryMutex;
    std::condition_variable _recoveryCondVar;
};

class DXRT_API StdDeviceTaskLayer : public DeviceTaskLayer {
public:
    explicit StdDeviceTaskLayer(std::shared_ptr<DeviceCore> dev, std::shared_ptr<ServiceLayerInterface> service_interface) : DeviceTaskLayer(dev,service_interface) {}
    int RegisterTask(TaskData* task) override;
    int InferenceRequest(RequestData* req, npu_bound_op boundOp) override;
    ~StdDeviceTaskLayer() override;
    StdDeviceTaskLayer(const StdDeviceTaskLayer&) = delete;
    StdDeviceTaskLayer& operator=(const StdDeviceTaskLayer&) = delete;
    StdDeviceTaskLayer(StdDeviceTaskLayer&&) = delete;
    StdDeviceTaskLayer& operator=(StdDeviceTaskLayer&&) = delete;

    int Release(TaskData *task) override;
    void StartThread() override;

    // Test accessors (kept lightweight; could be macro-guarded if needed)
    const std::vector<dxrt_request_t>& test_getInferenceVec(int taskId) const {
        static const std::vector<dxrt_request_t> kEmpty;
        auto it = _npuInference.find(taskId);
        return (it == _npuInference.end()) ? kEmpty : it->second;
    }
    const dxrt_request_t* test_getOngoing(int reqId) const {
        auto it = _ongoingRequestsStd.find(reqId);
        return (it == _ongoingRequestsStd.end()) ? nullptr : &it->second;
    }
    int test_getBufIndex(int taskId) const {
        auto it = _bufIdx.find(taskId);
        return (it == _bufIdx.end()) ? -1 : it->second;
    }

    int getFullLoad() const override { return 1;}
    void ProcessResponseFromService(const dxrt_response_t &resp) override;
    std::vector<Tensors> inputs(int taskId) override{
        auto it = _inputTensors.find(taskId);
        if (it != _inputTensors.end() && !it->second.empty()) {
            return it->second;
        }
        return std::vector<Tensors>();
    }

private:
    std::unordered_map<int, std::vector< dxrt_request_t >> _npuInference;
    std::unordered_map<int, dxrt_request_t> _ongoingRequestsStd;
    std::unordered_map<int, std::vector<Tensors>> _inputTensors;
    std::unordered_map<int, std::vector<Tensors>> _outputTensors;
    std::unordered_map<int, std::vector<uint8_t>> _outputValidateBuffers;
    std::unordered_map<int, int> _bufIdx;

    SharedMutex requestsLock;
    SharedMutex _taskDataLock;

    void ThreadImpl();
    std::thread _thread;
    uint64_t _memoryMapBuffer = 0;
};

class DXRT_API AccDeviceTaskLayer : public DeviceTaskLayer {
 public:
    explicit AccDeviceTaskLayer(std::shared_ptr<DeviceCore> dev, std::shared_ptr<ServiceLayerInterface> service_interface);
    int RegisterTask(TaskData *task) override;
    int InferenceRequest(RequestData *req, npu_bound_op boundOp) override;

    void EventThread();
    void OutputReceiverThread(int id);

    int InputHandler(const int &reqId, int ch);
    int OutputHandler(const dxrt_response_t &resp, int ch);

    // DMA Abort Recovery
    void HandleDmaAbortError(const dx_pcie_dev_err_t *err);
    void HandleDmaFailError(const dx_pcie_dev_err_t *err);
    void HandleFwTimeoutError(const dx_pcie_dev_err_t *err);
    void LogAbortDiagnostics(int channel, const dx_pcie_dev_err_t *err);
    void LogDmaFailDiagnostics(int channel, const dx_pcie_dev_err_t *err);
    void LogFwTimeoutDiagnostics(const dx_pcie_dev_err_t *err);
    void DmaAbortRecoveryThread();

    int Release(TaskData *task) override;
    void StartThread() override;

    ~AccDeviceTaskLayer() override;
    AccDeviceTaskLayer(const AccDeviceTaskLayer&) = delete;
    AccDeviceTaskLayer& operator=(const AccDeviceTaskLayer&) = delete;
    AccDeviceTaskLayer(AccDeviceTaskLayer&&) = delete;
    AccDeviceTaskLayer& operator=(AccDeviceTaskLayer&&) = delete;

    // Test accessors
    const dxrt_request_acc_t *test_getInferenceAcc(int taskId) const
    {
        auto it = _npuInferenceAcc.find(taskId);
        return (it == _npuInferenceAcc.end()) ? nullptr : &it->second;
    }
     const dxrt_request_acc_t* test_getOngoing(int reqId) const {
         auto it = _ongoingRequests.find(reqId);
         return (it == _ongoingRequests.end()) ? nullptr : &it->second;
     }

     int getFullLoad() const override { return DXRT_NPU_FULL_MAX_LOAD;}

     void ProcessResponseFromService(const dxrt_response_t &resp) override;
    std::vector<Tensors> inputs(int taskId) override { return {_inputTensorFormats[taskId]}; }


 private:
     dxrt_request_acc_t peekInference(int id);
     int InferenceRequestACC(RequestData *req, npu_bound_op boundOp);

     dxrt_meminfo_t SetMemInfo_PPCPU(const dxrt_meminfo_t& rmap_output,
                                      size_t ppu_filter_num,
                                      DataType dtype,
                                      void* output_ptr) const;

     std::unordered_map<int, dxrt_request_acc_t> _npuInferenceAcc;
     std::unordered_map<int, dxrt_request_acc_t> _ongoingRequests;

#ifdef USE_PROFILER
     // Track response receive timestamps for queueing delay measurement
     std::unordered_map<uint32_t, uint64_t> _responseReceiveTimestamps;
     std::mutex _responseTimestampLock;

     // Track PCIe Write completion timestamps for accurate NPU Core timing
     std::unordered_map<uint32_t, uint64_t> _writeCompleteTimestamps;
     std::mutex _writeTimestampLock;
#endif

     SharedMutex requestsLock;

     SharedMutex _taskDataLock;

     std::thread _eventThread;
     std::atomic<bool> _eventThreadTerminateFlag{false};
     std::atomic<bool> _eventThreadStartFlag{false};
     std::vector<std::thread> _outputDispatcher;

     // DMA Abort Recovery thread
     std::thread _recoveryThread;
     std::atomic<bool> _recoveryPending{false};

     HandlerQueueThread<int> _inputHandlerQueue;
     HandlerQueueThread<dxrt_response_t> _outputHandlerQueue;

    std::unordered_map<int, Tensors> _inputTensorFormats;
    std::unordered_map<int, Tensors> _outputTensorFormats;

    std::array<std::atomic<bool>, 4> _outputDispatcherTerminateFlag;

#ifdef DXRT_USE_DEVICE_VALIDATION
    void ReadValidationOutput(std::shared_ptr<Request> req);
#endif
};

}  // namespace dxrt
