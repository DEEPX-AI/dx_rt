// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <signal.h>
#include <mutex>
#include <atomic>
#include <thread>
#include <condition_variable>
#include "dxrt/common.h"
#include "dxrt/request.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"
#include "dxrt/worker.h"
#include "dxrt/driver_adapter/driver_adapter.h"
#include "dxrt/util.h"
#include "dxrt/common.h"
#include "dxrt/device.h"
#include "dxrt/memory.h"
#include "dxrt/task.h"
#include "dxrt/worker.h"
#include "dxrt/buffer.h"
#include "dxrt/profiler.h"
#include "dxrt/util.h"
#include "dxrt/filesys_support.h"
#include "dxrt/device_version.h"
#include "dxrt/fw.h"
#include "dxrt/multiprocess_memory.h"
#include "dxrt/driver_adapter/linux_driver_adapter.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#ifdef __linux__
    #include <unistd.h>
#endif
#include <errno.h>
#include <string.h>
#ifdef __linux__
    #include <sys/mman.h>
    #include <sys/ioctl.h>
#endif
#include <sys/types.h>
#include <limits>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <string>
#include <memory>




#ifdef __linux__
    #include <poll.h>
#elif _WIN32
    #include <windows.h>

#endif

namespace dxrt {


class Worker;
class Memory;
class InferenceOption;
class TaskData;
class Profiler;
class Buffer;
class FwLog;
class DXRT_API ServiceDevice
{
 public:
    explicit ServiceDevice(const std::string &);
    virtual ~ServiceDevice(void);
    std::string name() const { return _name; }
    int id() const { return _id; }
    int load();

    dxrt_device_info_t info() const{ return _info;}
    dxrt_device_status_t status();
    int Process(dxrt_cmd_t, void*, uint32_t size = 0, uint32_t sub_cmd = 0);


    virtual int InferenceRequest(dxrt_request_acc_t* req);
    int Write(dxrt_meminfo_t &);
    int Read(dxrt_meminfo_t &);
    int Wait();
    void Identify(int id_, SkipMode skip = NONE, uint32_t subCmd = 0);
    void SetSubMode(uint32_t cmd) { _subCmd = cmd; }
    void Terminate();
    int BoundOption(dxrt_sche_sub_cmd_t subCmd, npu_bound_op boundOp);

    void CallBack();

    int WaitThread(int ids);
    void Identify(int id_, dxrt::SkipMode skip);
    void SetCallback(std::function<void(const dxrt_response_t&)> f);
    static vector<shared_ptr<ServiceDevice>> CheckServiceDevices(SkipMode skip = SkipMode::NONE, uint32_t subCmd = 0);

 protected:
    int _id = 0;
    DeviceType _type = DeviceType::ACC_TYPE; /* 0: ACC type, 1: STD type */

    npu_bound_op _boundOp;
    uint32_t _variant;
    int _devFd = -1;
#ifdef __linux__
    struct pollfd _devPollFd;
#elif _WIN32
    HANDLE _devHandle = INVALID_HANDLE_VALUE;
#endif
    std::string _file;
    std::string _name;
    dxrt_device_info_t _info;
    dxrt_device_status_t _status;
    uint32_t _subCmd;
    int _load = 0;
    int _inferenceCnt = 0;
    bool _hasWorkers = false;
    Profiler &_profiler;

    std::thread _thread[3];

    
    std::mutex _lock;
    std::atomic<bool> _stop {false};


    std::shared_ptr<DriverAdapter> _driverAdapter;

    std::function<void(const dxrt_response_t&)> _callBack;
};



}
