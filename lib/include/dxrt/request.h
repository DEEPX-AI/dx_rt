// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "dxrt/common.h"
#include "dxrt/tensor.h"
#include "dxrt/driver.h"
#include "dxrt/task_data.h"
#include "dxrt/request_data.h"
#include <unordered_map>
#include <mutex>

#define REQUEST_ID_INIT_VALUE 1
// #define REQUEST_ID_MAX_VALUE 5000
#define REQUEST_ID_MAX_VALUE 50000
// #define REQUEST_ID_MAX_VALUE 50

namespace dxrt {
class Task;

class Request;
using RequestPtr = std::shared_ptr<Request>;
using RequestWeakPtr = std::weak_ptr<Request>;
struct TimePoint;
using TimePointPtr = std::shared_ptr<TimePoint>;


class DXRT_API Request
{
public:
    enum Status
    {
        REQ_IDLE,
        REQ_BUSY,
        REQ_DONE,
    };
    Request(void);
    Request(int id);
    Request(Task *task_, Tensors &inputs_, Tensors &outputs_);
    ~Request(void);
    static void Init();
    static RequestPtr Create(Task *task_, Tensors inputs_, Tensors outputs_, void *userArg, int jobId=0);
    static RequestPtr Create(Task *task_, void *input, void *output, void *userArg, int jobId=0);
    static RequestPtr GetById(int id);
    static RequestPtr Pick();
    static void ShowAll();
    static void Clear();
    static void SaveTaskStats(Task *task);
    void Wait();
    void SetStatus(Status s);
    void NotifyCompletion();
    void CheckTimePoint(int opt);
    int id() const;
    int job_id() const;
    void set_processed_unit(std::string processedPU, int processedId);
    std::string processed_pu() const;
    int processed_id() const;
    void Reset();

    TaskData* taskData();
    Task* task();
    std::string requestor_name() const;
    Tensors &inputs();
    Tensors &outputs();
    void* input_ptr();
    void* output_ptr();
    void* user_arg() const;
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

    void setNpuInferenceAcc(dxrt_request_acc_t npuInferenceAcc);
    void setCallback(std::function<void(RequestPtr)> func);  // works for start next request or complete whole inference
    void onRequestComplete(RequestPtr req);

    RequestData* getData();
    const RequestData* getData() const;
    friend DXRT_API std::ostream& operator<<(std::ostream&, const Request&);
private:

    RequestData _data;

    std::string _requestorName;

    Task* _task;

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
    std::function<void(RequestPtr)> _callback;
    

};
class DXRT_API RequestMap
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
DXRT_API std::ostream& operator<<(std::ostream&, const Request::Status&);


int InferenceRequest(RequestPtr req);
int ProcessResponse(RequestPtr req, dxrt_response_t *response=nullptr);

} // namespace dxrt