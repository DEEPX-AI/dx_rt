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
#include <functional>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

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

class DXRT_API DeviceTaskLayer {
 public:
    explicit DeviceTaskLayer(std::shared_ptr<DeviceCore> core, std::shared_ptr<ServiceLayerInterface> service_interface);

    virtual ~DeviceTaskLayer() = default;

    int load();
    void pick();
    int infCnt();

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
    int64_t Allocate(uint64_t size);
    void Deallocate(uint64_t addr);

    void RegisterCallback(std::function<void()> f);

    dxrt_model_t npu_model(int taskId);
    virtual std::vector<Tensors> inputs(int taskId) = 0;
    Tensors outputs(int taskId);

    //virtual void *peekInference(uint32_t requestId) = 0;
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
    void ProcessErrorFromService(dxrt_server_err_t err, int value);
    void SetProcessResponseHandler(std::function<void(int deviceId, int reqId, dxrt_response_t *response)> handler) {
        _processResponseHandler = handler;
    }
    std::shared_ptr<DeviceCore> core() { return _core; }

    const std::shared_ptr<DeviceCore> core() const { return _core; }
 protected:
    // Accessor for derived classes
    std::shared_ptr<DeviceCore> _core;
    std::atomic<int> _load{0};
    std::atomic<int> _inferenceCnt{0};
    std::mutex _lock;
    std::atomic<bool> _stop{false};
    std::shared_ptr<ServiceLayerInterface> _serviceLayer;
    std::mutex _npuInferenceLock;
    std::unordered_map<int, dxrt::dxrt_model_t> _npuModel;
    std::function<void()> _onCompleteInferenceHandler;
    NpuMemoryCacheManager _npuMemoryCacheManager;
    std::function<void(int deviceId, int reqId, dxrt_response_t *response)> _processResponseHandler;
};

class DXRT_API StdDeviceTaskLayer : public DeviceTaskLayer {
public:
    explicit StdDeviceTaskLayer(std::shared_ptr<DeviceCore> dev, std::shared_ptr<ServiceLayerInterface> service_interface) : DeviceTaskLayer(dev,service_interface) {}
    int RegisterTask(TaskData* task) override;
    int InferenceRequest(RequestData* req, npu_bound_op boundOp) override;
    ~StdDeviceTaskLayer();

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
    explicit AccDeviceTaskLayer(std::shared_ptr<DeviceCore> dev, std::shared_ptr<ServiceLayerInterface> service_interface)
    : DeviceTaskLayer(dev, service_interface), _inputHandlerQueue(dev->name()+"_input", 3,
        std::bind(&AccDeviceTaskLayer::InputHandler, this, std::placeholders::_1, std::placeholders::_2)),
     _outputHandlerQueue(dev->name()+"_output", 4,
        std::bind(&AccDeviceTaskLayer::OutputHandler, this, std::placeholders::_1, std::placeholders::_2))
     {}
     int RegisterTask(TaskData* task) override;
     int InferenceRequest(RequestData* req, npu_bound_op boundOp) override;


     void EventThread();
     void OutputReceiverThread(int id);

     int InputHandler(const int& reqId, int ch);
     int OutputHandler(const dxrt_response_t& resp, int ch);

     int Release(TaskData *task) override;
     void StartThread() override;

     virtual ~AccDeviceTaskLayer();

     // Test accessors
     const dxrt_request_acc_t* test_getInferenceAcc(int taskId) const {
         auto it = _npuInferenceAcc.find(taskId);
         return (it == _npuInferenceAcc.end()) ? nullptr : &it->second;
     }
     const dxrt_request_acc_t* test_getOngoing(int reqId) const {
         auto it = _ongoingRequests.find(reqId);
         return (it == _ongoingRequests.end()) ? nullptr : &it->second;
     }

     int getFullLoad() const override { return DXRT_TASK_MAX_LOAD;}

     void ProcessResponseFromService(const dxrt_response_t &resp) override;
    std::vector<Tensors> inputs(int taskId) override { return {_inputTensorFormats[taskId]}; }


 private:
     dxrt_request_acc_t peekInference(int id);
     int InferenceRequestACC(RequestData *req, npu_bound_op boundOp);

     dxrt_meminfo_t SetMemInfo_PPCPU(const dxrt_meminfo_t& rmap_output,
                                      size_t ppu_filter_num,
                                      DataType dtype,
                                      void* output_ptr);

     std::unordered_map<int, dxrt_request_acc_t> _npuInferenceAcc;
     std::unordered_map<int, dxrt_request_acc_t> _ongoingRequests;

     SharedMutex requestsLock;

     SharedMutex _taskDataLock;

     std::thread _eventThread;
     std::atomic<bool> _eventThreadTerminateFlag{false};
     std::atomic<bool> _eventThreadStartFlag{false};
     std::vector<std::thread> _outputDispatcher;

     HandlerQueueThread<int> _inputHandlerQueue;
     HandlerQueueThread<dxrt_response_t> _outputHandlerQueue;

    std::unordered_map<int, Tensors> _inputTensorFormats;
    std::unordered_map<int, Tensors> _outputTensorFormats;

    std::atomic<bool> _outputDispatcherTerminateFlag[4];

#ifdef DXRT_USE_DEVICE_VALIDATION
    void ReadValidationOutput(std::shared_ptr<Request> req);
#endif
};

}  // namespace dxrt
