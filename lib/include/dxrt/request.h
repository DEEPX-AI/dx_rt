// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "dxrt/common.h"
#include "dxrt/tensor.h"
#include "dxrt/driver.h"
#include <unordered_map>
#include <mutex>

#define REQUEST_ID_INIT_VALUE 1
// #define REQUEST_ID_MAX_VALUE 5000
#define REQUEST_ID_MAX_VALUE 50000
// #define REQUEST_ID_MAX_VALUE 50

namespace dxrt {
class Task;
class Device;
class Request;
using RequestPtr = std::shared_ptr<Request>;
struct TimePoint;
using TimePointPtr = std::shared_ptr<TimePoint>;
class Request
{
public:
    enum Status
    {
        REQ_IDLE,
        REQ_BUSY,
        REQ_DONE,
    };
    Request(void);
    Request(Task *task_, Tensors &inputs_, Tensors &outputs_);
    ~Request(void);
    static void Init();
    static RequestPtr Create(Task *task_, Tensors inputs_, Tensors outputs_, void *userArg, void *lastOutput);
    static RequestPtr Create(Task *task_, void *input, void *output, void *userArg, void *lastOutput);
    static RequestPtr GetById(int id);
    static RequestPtr Pick();    
    static void ShowAll();
    static void Clear();
    static void SaveTaskStats(Task *task);
    void Wait();
    void SetStatus(Status s);
    void NotifyCompletion();
    void CheckTimePoint(int opt);
    int &id();
    RequestPtr &head();
    Task* &task();
    Task* &requestor();
    Tensors &inputs();
    Tensors &outputs();
    void* &input_ptr();
    void* &output_ptr();
    void* &last_output_ptr();
    void* &user_arg();
    void* &dev_arg();
    dxrt_request_t &npu_inference();
    dxrt_request_t* &npu_inference_ptr();
    dxrt_request_acc_t &npu_inference_acc();
    uint32_t &inference_time();
    int &complete_cnt();
    TimePointPtr time_point();
    Status status();
    int &latency();
    bool &latency_valid();
    bool &validate_device();
    int16_t &model_type();
    friend std::ostream& operator<<(std::ostream&, const Request&);
private:
    int _id;
    RequestPtr _head;
    Task *_task;
    Task *_requestor;
    Tensors _inputs;
    Tensors _outputs;
    void *_inputPtr;
    void *_outputPtr;
    void *_lastOutputPtr;
    void *_userArg;
    void *_devArg;
    dxrt_request_t _npuInference;
    dxrt_request_t *_npuInferencePtr;
    dxrt_request_acc_t _npuInferenceAcc;
    Status _status = REQ_IDLE;
    std::mutex _statusLock;
    std::shared_ptr<TimePoint> _timePoint;
    int _latency;
    bool _latencyValid;
    bool _validateDevice = false;
    int16_t _modelType;
    uint32_t _infTime;
    int _completeCnt;
    std::mutex _completeCntLock;
    static int _nextId;
    static std::mutex _idLock;
    static std::vector<RequestPtr> _requestMap;
    static std::mutex _mapLock;
};
class RequestMap
{
public:
    RequestMap(void);
    ~RequestMap(void);
    RequestPtr GetById(int id);
    int Add(RequestPtr req);
private:
    std::unordered_map<int, RequestPtr> _map;
    std::mutex _lock;
};
std::ostream& operator<<(std::ostream&, const Request::Status&);

} // namespace dxrt