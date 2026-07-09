/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt_service_v2.hpp"
#include "../device_shm/shared_memory_writing_thread.hpp"


#include <algorithm>
#include <atomic>
#include <cerrno>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <fcntl.h>
#include <limits>
#include <thread>
#include <sstream>
#include <unordered_set>
#include <utility>
#include <vector>

#include "../data/ppcpu.h"
#include "../dynamic_ipc/shm/shared_memory_syscall_adapter.h"

#ifdef __linux__
#include <signal.h>
#include <sys/uio.h>
#include <unistd.h>
#elif _WIN32
#include <windows.h>
#endif

namespace {

std::shared_ptr<SchedulerService> CreateSchedulerV2(
    const std::vector<std::shared_ptr<dxrt::ServiceDevice>> &devices,
    dxrt::DXRTScheduleV2 schedulerOption)
{
    switch (schedulerOption)
    {
        case dxrt::DXRTScheduleV2::RoundRobin:
            return std::make_shared<RoundRobinSchedulerService>(devices);
        case dxrt::DXRTScheduleV2::SJF:
            return std::make_shared<SJFSchedulerService>(devices);
        default:
            return std::make_shared<FIFOSchedulerService>(devices);
    }
}

bool isLikelyDisconnectedSendError(int rc)
{
    const int err = (rc < 0) ? -rc : rc;
    return err == ENOENT ||
           err == EPIPE ||
           err == ECONNRESET ||
           err == ENOTCONN ||
           err == ENOPROTOOPT ||
           err == EBADF;
}

void ResetIpcRegistry(dxrt::IPCPacketHandlerRegistry *registry)
{
    if (registry == nullptr)
    {
        return;
    }

    registry->clearHandlers();
    registry->setOnClientConnectedPid(nullptr);
    registry->setOnClientDisconnectedPid(nullptr);
}

#ifdef __linux__
bool IsProcessRunning(pid_t pid)
{
    if (kill(pid, 0) == 0)
    {
        return true;
    }
    return errno == EPERM;
}
#elif _WIN32
bool IsProcessRunning(pid_t pid)
{
    if (pid == 0)
    {
        return true;
    }

    HANDLE process = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (process == NULL)
    {
        const DWORD error = GetLastError();
        if (error == ERROR_INVALID_PARAMETER)
        {
            return false;
        }
        return true;
    }

    const DWORD result = WaitForSingleObject(process, 0);
    CloseHandle(process);
    return result == WAIT_TIMEOUT;
}
#endif

}  // namespace

namespace dxrt {

struct DxrtServiceV2::IpcServerContext
{
    explicit IpcServerContext(const std::string &name)
        : endpointName(name)
    {
    }

    IPCServerEndpoint endpoint;
    IPCPacketHandlerRegistry registry;
    std::string endpointName;
    std::thread ioThread;
};

struct DxrtServiceV2::IpcServerWorkItem
{
    std::function<int()> handler;
    const char *name{"unknown"};
};

DxrtServiceV2::~DxrtServiceV2()
{
    if (_sharedMemoryWritingThread)
    {
        _sharedMemoryWritingThread->Stop();
    }
    StopIpcServer();
}

void DxrtServiceV2::CacheMemoryServices()
{
    _memoryServices.clear();
    for (const auto &device : _devices)
    {
        const int id = device->id();
        _memoryServices[id] = dxrt::MemoryService::getInstance(id);
    }
}

dxrt::MemoryService *DxrtServiceV2::GetMemoryService(int deviceId) const
{
    auto it = _memoryServices.find(deviceId);
    if (it == _memoryServices.end())
    {
        return nullptr;
    }
    return it->second;
}

int DxrtServiceV2::StartIpcServer(const std::string &endpoint, int backlog)
{
    return StartIpcServers(std::vector<std::string>{endpoint}, backlog);
}

void DxrtServiceV2::setupIpcPacketRegistry(IPCPacketHandlerRegistry *registry, IPCServerEndpoint *server)
{
    if (registry == nullptr || server == nullptr)
    {
        return;
    }

    registry->clearHandlers();

    registry->setOnClientConnectedPid([this](pid_t pid) {
        onClientConnected(pid);
    });
    registry->setOnClientDisconnectedPid([this](pid_t pid) {
        onClientDisconnected(pid);
    });

    auto enqueueWorkItem = [this](IpcServerWorkItem workItem) {
        if (_ipcHandlerQueue == nullptr)
        {
            errno = EPIPE;
            return -1;
        }

        _ipcHandlerQueue->PushWork(workItem);
        return IPCPacketHandlerRegistry::kResponseAlreadySent;
    };

    registry->setAllocateHandler(
        [this, registry, enqueueWorkItem](
            int clientFd,
            pid_t pid,
            int deviceId,
            int32_t seqId,
            int taskId,
            uint32_t bufferSize,
            dxrt::MemoryType memoryType) {
            IpcServerWorkItem workItem;
            workItem.name = "Allocate";
            workItem.handler = [this, registry, clientFd, pid, deviceId, seqId, taskId, bufferSize, memoryType]() {
                bool isBackward = (memoryType == dxrt::MemoryType::Model_rmap
                    || memoryType == dxrt::MemoryType::Model_weight
                    || memoryType == dxrt::MemoryType::Model_ppu_binary);
                const SharedMemoryInfo allocInfo = HandleAllocate(pid, deviceId, taskId, bufferSize, isBackward);
                const int result = (allocInfo.phys_addr() == static_cast<uint64_t>(-1)) ? -1 : 0;
                // bufferAddress carries physAddrOffset (offset in NPU memory).
                // blockId carries the 1-based sequential SHM block identifier.
                const uint64_t bufferAddress = (result == 0) ? allocInfo.phys_addr_offset : 0;
                const int64_t blockId = (result == 0) ? allocInfo.block_id : 0;
                const uint64_t allocatedBufferSize = (result == 0) ? allocInfo.size : 0;
                const intptr_t fdToSend = (result == 0) ? allocInfo.fd : dxrt::shm::kInvalidMemFDHandle;
                const int sendRc = registry->SendAllocResultPacketToClient(
                    clientFd,
                    pid,
                    deviceId,
                    seqId,
                    result,
                    bufferAddress,
                    allocatedBufferSize,
                    blockId,
                    fdToSend);
                if (sendRc < 0)
                {
                    return sendRc;
                }

                return 0;
            };

            return enqueueWorkItem(std::move(workItem));
        });

    registry->setFreeBufferHandler(
        [this, registry, enqueueWorkItem](
            pid_t pid,
            int deviceId,
            int32_t seqId,
            uint64_t bufferAddress) {
            IpcServerWorkItem workItem;
            workItem.name = "FreeBuffer";
            workItem.handler = [this, registry, pid, deviceId, seqId, bufferAddress]() {
                const int result = HandleDeallocate(pid, deviceId, bufferAddress);

                const int sendRc = registry->SendFreeBufferResultPacket(
                    pid,
                    deviceId,
                    seqId,
                    result);
                if (sendRc < 0)
                {
                    return sendRc;
                }

                return 0;
            };

            return enqueueWorkItem(std::move(workItem));
        });

    registry->setInferenceRequestHandler(
        [this, registry, enqueueWorkItem](
            pid_t pid,
            int deviceId,
            int taskId,
            const dxrt_request_acc_t &request) {
            IpcServerWorkItem workItem;
            workItem.name = "InferenceRequest";
            workItem.handler = [this, registry, pid, deviceId, taskId, request]() {
                dxrt_request_acc_t inferenceRequest = request;
                inferenceRequest.task_id = static_cast<uint32_t>(taskId);
                inferenceRequest.proc_id = static_cast<uint32_t>(pid);
                const int result = HandleInferenceRequest(pid, deviceId, inferenceRequest);

                if (result != 0)
                {
                    dxrt_response_t emptyResponse{};
                    emptyResponse.req_id = request.req_id;
                    emptyResponse.proc_id = static_cast<uint32_t>(pid);
                    emptyResponse.status = result;
                    const int sendRc = registry->SendInferenceResultPacket(
                        pid,
                        deviceId,
                        0,
                        result,
                        emptyResponse);
                    if (sendRc < 0)
                    {
                        return sendRc;
                    }
                }

                return 0;
            };

            return enqueueWorkItem(std::move(workItem));
        });

    registry->setTaskInitHandler(
        [this, registry, enqueueWorkItem](
            pid_t pid,
            int deviceId,
            int32_t seqId,
            int taskId,
            int bound,
            uint64_t modelMemorySize,
            const TaskStaticConfig &config) {
            IpcServerWorkItem workItem;
            workItem.name = "TaskInit";
            workItem.handler = [this, registry, pid, deviceId, seqId, taskId, bound, modelMemorySize, config]() {
                auto recvTime = std::chrono::high_resolution_clock::now();

                LOG_DXRT_S_DBG << "IPC_RECV: seqId=" << seqId << " TaskInit taskId=" << taskId
                               << " (pid=" << pid << ",device=" << deviceId << ")" << std::endl;

                LOG_DXRT_S_DBG << "TaskInitHandler pid=" << pid
                               << ", deviceId=" << deviceId
                               << ", taskId=" << taskId
                               << ", bound=" << bound
                               << ", modelMemorySize=" << modelMemorySize << std::endl;

                auto processStart = std::chrono::high_resolution_clock::now();
                const int result = HandleTaskInit(pid, deviceId, taskId, bound, modelMemorySize, config);
                auto processEnd = std::chrono::high_resolution_clock::now();

                auto processDuration = std::chrono::duration_cast<std::chrono::milliseconds>(processEnd - processStart);
                LOG_DXRT_S_DBG << "IPC_PROCESS: seqId=" << seqId << " TaskInit result=" << result
                               << " elapsed_ms=" << processDuration.count() << std::endl;

                const int sendRc = registry->SendTaskInitResultPacket(
                    pid,
                    deviceId,
                    seqId,
                    result);
                if (sendRc < 0)
                {
                    return sendRc;
                }

                auto sendEnd = std::chrono::high_resolution_clock::now();
                auto totalDuration = std::chrono::duration_cast<std::chrono::milliseconds>(sendEnd - recvTime);
                LOG_DXRT_S_DBG << "IPC_SENT: seqId=" << seqId << " TaskInit total_elapsed_ms="
                               << totalDuration.count() << std::endl;

                return 0;
            };

            return enqueueWorkItem(std::move(workItem));
        });

    registry->setTaskDeInitHandler(
        [this, registry, enqueueWorkItem](
            pid_t pid,
            int deviceId,
            int32_t seqId,
            int taskId,
            int) {
            IpcServerWorkItem workItem;
            workItem.name = "TaskDeInit";
            workItem.handler = [this, registry, pid, deviceId, seqId, taskId]() {
                const int result = HandleTaskDeInit(pid, deviceId, taskId);

                const int sendRc = registry->SendTaskDeInitResultPacket(
                    pid,
                    deviceId,
                    seqId,
                    result);
                if (sendRc < 0)
                {
                    return sendRc;
                }

                return 0;
            };

            return enqueueWorkItem(std::move(workItem));
        });

    registry->setViewFreeMemoryLegacyHandler(
        [this, registry, enqueueWorkItem](
            pid_t pid,
            int deviceId,
            int32_t seqId) {
            IpcServerWorkItem workItem;
            workItem.name = "ViewFreeMemoryLegacy";
            workItem.handler = [this, registry, pid, deviceId, seqId]() {
                uint64_t bytes = 0;
                const int result = HandleViewMemory(deviceId, false, &bytes);

                const int sendRc = registry->SendViewFreeMemoryLegacyResultPacket(
                    pid,
                    deviceId,
                    seqId,
                    result,
                    bytes);
                if (sendRc < 0)
                {
                    return sendRc;
                }

                return 0;
            };

            return enqueueWorkItem(std::move(workItem));
        });

    registry->setViewUsedMemoryLegacyHandler(
        [this, registry, enqueueWorkItem](
            pid_t pid,
            int deviceId,
            int32_t seqId) {
            IpcServerWorkItem workItem;
            workItem.name = "ViewUsedMemoryLegacy";
            workItem.handler = [this, registry, pid, deviceId, seqId]() {
                uint64_t bytes = 0;
                const int result = HandleViewMemory(deviceId, true, &bytes);

                const int sendRc = registry->SendViewUsedMemoryLegacyResultPacket(
                    pid,
                    deviceId,
                    seqId,
                    result,
                    bytes);
                if (sendRc < 0)
                {
                    return sendRc;
                }

                return 0;
            };

            return enqueueWorkItem(std::move(workItem));
        });

    registry->setGetUsageLegacyHandler(
        [this, registry, enqueueWorkItem](
            pid_t pid,
            int deviceId,
            int32_t seqId,
            int channel) {
            IpcServerWorkItem workItem;
            workItem.name = "GetUsageLegacy";
            workItem.handler = [this, registry, pid, deviceId, seqId, channel]() {
                double usage = 0.0;
                const int result = HandleGetUsage(deviceId, channel, &usage);

                const int sendRc = registry->SendGetUsageLegacyResultPacket(
                    pid,
                    deviceId,
                    seqId,
                    result,
                    usage);
                if (sendRc < 0)
                {
                    return sendRc;
                }

                return 0;
            };

            return enqueueWorkItem(std::move(workItem));
        });

    registry->setGetDeviceTelemetryHandler(
        [this, registry, enqueueWorkItem](
            pid_t pid,
            int deviceId,
            int32_t seqId) {
            IpcServerWorkItem workItem;
            workItem.name = "GetDeviceTelemetry";
            workItem.handler = [this, registry, pid, deviceId, seqId]() {
                IPCDeviceTelemetryPayload telemetry{};
                const int result = HandleGetDeviceTelemetry(deviceId, &telemetry);
                telemetry.result = result;

                const int sendRc = registry->SendGetDeviceTelemetryResultPacket(
                    pid,
                    deviceId,
                    seqId,
                    telemetry);
                if (sendRc < 0)
                {
                    return sendRc;
                }

                return 0;
            };

            return enqueueWorkItem(std::move(workItem));
        });

    registry->setDMAReadHandler(
        [this, registry, enqueueWorkItem](
            pid_t pid,
            int deviceId,
            int32_t seqId,
            int64_t  blockId,
            uint64_t blockOffset,
            uint64_t size) {
            IpcServerWorkItem workItem;
            workItem.name = "DMARead";
            workItem.handler = [this, registry, pid, deviceId, seqId, blockId, blockOffset, size]()
            {
                const int result = HandleDMARead(pid, seqId, deviceId, blockId, blockOffset, size);
                return result;
            };

            return enqueueWorkItem(std::move(workItem));
        });

    registry->setDMAReadWithFaultInjectionHandler(
        [this, registry, enqueueWorkItem](
            pid_t pid,
            int deviceId,
            int32_t seqId,
            int64_t  blockId,
            uint64_t blockOffset,
            uint64_t size) {
            IpcServerWorkItem workItem;
            workItem.name = "DMAReadWithFaultInjection";
            workItem.handler = [this, registry, pid, deviceId, seqId, blockId, blockOffset, size]()
            {
                const int result = HandleDMAReadWithFaultInjection(pid, deviceId, blockId, blockOffset, size);
                const int sendRc = registry->SendDMAReadResultPacket(pid, deviceId, seqId, result);
                if (sendRc < 0)
                {
                    return sendRc;
                }
                return 0;
            };

            return enqueueWorkItem(std::move(workItem));
        });

    registry->setDMAWriteHandler(
        [this, registry, enqueueWorkItem](
            pid_t pid,
            int deviceId,
            int32_t seqId,
            int64_t  blockId,
            uint64_t blockOffset,
            uint64_t size) {
            IpcServerWorkItem workItem;
            workItem.name = "DMAWrite";
            workItem.handler = [this, registry, pid, deviceId, seqId, blockId, blockOffset, size]()
            {
                const int result = HandleDMAWrite(pid, seqId, deviceId, blockId, blockOffset, size);
                return result;
            };

            return enqueueWorkItem(std::move(workItem));
        });

    registry->attachToServer(server);
}

int DxrtServiceV2::StartIpcServers(const std::vector<std::string> &endpoints, int backlog)
{
    StopIpcServer();

    _ipcHandlerQueue.reset(new HandlerQueueThread<IpcServerWorkItem>(
        "DxrtServiceV2IpcQueue",
        1,
        [this](const IpcServerWorkItem &workItem, int threadId) {
            return handleIpcServerWorkItem(workItem, threadId);
        }));
    _ipcHandlerQueue->Start();

    std::vector<std::string> uniqueEndpoints;
    uniqueEndpoints.reserve(endpoints.size());
    std::unordered_set<std::string> seenEndpoints;
    seenEndpoints.reserve(endpoints.size());
    for (const auto &endpoint : endpoints)
    {
        if (endpoint.empty())
        {
            continue;
        }

        if (seenEndpoints.insert(endpoint).second)
        {
            uniqueEndpoints.push_back(endpoint);
        }
    }

    if (uniqueEndpoints.empty())
    {
        errno = EINVAL;
        return -1;
    }

    _ipcServerRunRc.store(0, std::memory_order_release);
    _ipcServerStarted.store(true, std::memory_order_release);

    int lastErrno = 0;
    size_t failedCount = 0;

    _ipcServers.reserve(uniqueEndpoints.size());
    for (const auto &endpoint : uniqueEndpoints)
    {
        auto context = std::make_unique<IpcServerContext>(endpoint);
        setupIpcPacketRegistry(&context->registry, &context->endpoint);

        const int rc = context->endpoint.start(endpoint, backlog, true);
        if (rc != 0)
        {
            ResetIpcRegistry(&context->registry);
            ++failedCount;
            const int err = errno;
            if (err != 0)
            {
                lastErrno = err;
            }

            LOG_DXRT_S_ERR(
                "StartIpcServers failed endpoint: " + endpoint +
                " (errno=" + std::to_string(err) +
                ": " + std::string(std::strerror(err)) + ")");
            continue;
        }

        IpcServerContext *rawContext = context.get();
        _ipcServers.push_back(std::move(context));

        startIpcPollThread(rawContext);
    }

    if (_ipcServers.empty())
    {
        if (_ipcHandlerQueue != nullptr)
        {
            _ipcHandlerQueue->Stop();
            _ipcHandlerQueue.reset();
        }
        _ipcServerStarted.store(false, std::memory_order_release);
        errno = (lastErrno != 0) ? lastErrno : EADDRNOTAVAIL;
        return -1;
    }

    if (failedCount > 0)
    {
        LOG_DXRT_S << "StartIpcServers partial success: started=" << _ipcServers.size()
                   << " failed=" << failedCount << std::endl;
    }

    return 0;
}

void DxrtServiceV2::startIpcPollThread(IpcServerContext *context)
{
    if (context == nullptr)
    {
        return;
    }

    context->ioThread = std::thread([this, context]() {
        runIpcPollLoop(context);
    });
}

void DxrtServiceV2::runIpcPollLoop(IpcServerContext *context)
{
    if (context == nullptr)
    {
        return;
    }

    constexpr int kIpcPollThreadTimeoutMs = 100;
    while (_ipcServerStarted.load(std::memory_order_acquire) && context->endpoint.isRunning())
    {
        const int pollRc = context->endpoint.pollOnce(kIpcPollThreadTimeoutMs);
        const DxrtServiceV2IpcPollDecision decision = EvaluateDxrtServiceV2IpcPollResult(
            pollRc,
            _ipcServerStarted.load(std::memory_order_acquire));

        switch (decision)
        {
            case DxrtServiceV2IpcPollDecision::ContinuePolling:
                continue;
            case DxrtServiceV2IpcPollDecision::ExitPolling:
                return;
            case DxrtServiceV2IpcPollDecision::StopAllServers:
                handleIpcPollFailure(pollRc);
                return;
        }
    }
}

void DxrtServiceV2::handleIpcPollFailure(int pollRc)
{
    int expected = 0;
    _ipcServerRunRc.compare_exchange_strong(expected, pollRc, std::memory_order_acq_rel);
    requestAllIpcServerStops();
    _ipcServerStarted.store(false, std::memory_order_release);
}

int DxrtServiceV2::RunIpcServer(int timeoutMs)
{
    if (!_ipcServerStarted.load(std::memory_order_acquire) || _ipcServers.empty())
    {
        errno = EBADF;
        return -1;
    }

    constexpr int kDefaultSleepMs = 10;
    constexpr int kMinSleepMs = 1;
    constexpr int kMaxSleepMs = 10;
    int sleepMs = kDefaultSleepMs;
    if (timeoutMs >= 0)
    {
        sleepMs = (std::max)(kMinSleepMs, (std::min)(timeoutMs, kMaxSleepMs));
    }

    while (_ipcServerStarted.load(std::memory_order_acquire))
    {
        const int rc = _ipcServerRunRc.load(std::memory_order_acquire);
        if (rc < 0)
        {
            return rc;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(sleepMs));
    }

    return _ipcServerRunRc.load(std::memory_order_acquire);
}

void DxrtServiceV2::StopIpcServer()
{
    const bool wasStarted = _ipcServerStarted.exchange(false, std::memory_order_acq_rel);
    if (!wasStarted)
    {
        if (_ipcHandlerQueue != nullptr)
        {
            _ipcHandlerQueue->Stop();
            _ipcHandlerQueue.reset();
        }
        _ipcServers.clear();
        _ipcServerRunRc.store(0, std::memory_order_release);
        return;
    }

    requestAllIpcServerStops();

    for (auto &context : _ipcServers)
    {
        if (context->ioThread.joinable())
        {
            context->ioThread.join();
        }
    }

    if (_ipcHandlerQueue != nullptr)
    {
        _ipcHandlerQueue->Stop();
        _ipcHandlerQueue.reset();
    }

    for (const auto &context : _ipcServers)
    {
        context->endpoint.stop();
        ResetIpcRegistry(&context->registry);
    }

    _ipcServers.clear();
    _ipcServerRunRc.store(0, std::memory_order_release);
}

int DxrtServiceV2::handleIpcServerWorkItem(const IpcServerWorkItem &workItem, int threadId)
{
    (void)threadId;

    if (!workItem.handler)
    {
        errno = EINVAL;
        return -1;
    }

    const int rc = workItem.handler();
    if (rc < 0)
    {
        LOG_DXRT_S_ERR(
            "DxrtServiceV2::handleIpcServerWorkItem failed: name=" + std::string(workItem.name) +
            ", rc=" + std::to_string(rc) +
            ", errno=" + std::to_string(errno));
    }

    return rc;
}

void DxrtServiceV2::requestAllIpcServerStops()
{
    for (const auto &context : _ipcServers)
    {
        context->endpoint.requestStop();
    }
}

int DxrtServiceV2::sendToPidAcrossServers(pid_t pid, const uint8_t *data, size_t size, int fd)
{
    if ((_ipcServerStarted.load(std::memory_order_acquire) == false) || (_ipcServers.empty()))
    {
        errno = EBADF;
        return -1;
    }

    int lastRc = -1;
    for (const auto &context : _ipcServers)
    {
        const int sendRc = context->endpoint.sendToPid(pid, data, size, fd);
        if (sendRc >= 0)
        {
            return sendRc;
        }

        lastRc = sendRc;
    }

    return lastRc;
}

DxrtServiceV2::DxrtServiceV2(
    std::vector<std::shared_ptr<dxrt::ServiceDevice>> devices,
    DXRTScheduleV2 schedulerOption)
    : _devices(std::move(devices)),
      _scheduler(CreateSchedulerV2(_devices, schedulerOption))
{
    for (const auto &device : _devices)
    {
        const int id = device->id();

        // Do NOT force a device recovery on every startup. By the time we reach
        // this constructor, CheckServiceDevices() has already completed a full
        // IDENTIFY on each device (device_core.cpp throws/asserts on any failure),
        // so every device here is proven healthy and READY. Issuing an
        // unconditional DXRT_CMD_RECOVERY maps to a full FW reboot (~1s blocking
        // wait) in the driver, which is:
        //   - redundant: the device is already READY;
        //   - harmful: under rapid restart cycling (e.g. cpu_reset recovery test)
        //     systemd's SIGTERM can interrupt the blocking reboot wait, the ioctl
        //     returns -ERESTARTSYS (-512), and the driver parks the device in
        //     'waiting_user' -> permanent wedge requiring a PCIe/cold reset.
        // All legitimate recovery paths already complete BEFORE dxrtd restarts:
        //   - error/DMA-abort path: EventLoop calls TriggerRecovery() then quick_exit;
        //   - driver-owned (link_flap/cpu_reset): the kernel finishes recovery and
        //     emits 'recovery DONE' before the service comes back up.
        // Only recover here if a device somehow reached this point still blocked.
        if (device->isBlocked())
        {
            LOG_DXRT_S << id << ": device blocked at startup; issuing recovery." << std::endl;
            _devices[id]->Process(dxrt::dxrt_cmd_t::DXRT_CMD_RECOVERY, nullptr);
        }

        device->SetCallback([id, this](const dxrt::dxrt_response_t &response) {
            _scheduler->FinishJobs(id, response);
        });
        device->SetErrorCallback(
            [this](dxrt::dxrt_server_err_t err, uint32_t errCode, int deviceId, const dx_pcie_dev_err_t *errorInfo) {
                ErrorBroadCastToClient(err, errCode, deviceId, errorInfo);
        });
        device->SetRecoveryAdapter(std::make_shared<ServiceRecoveryAdapter>(this));
        device->SetThrottleCallback([this](dxrt::dx_pcie_dev_ntfy_throt_t throtInfo, int deviceId) {
            BroadcastThrottlingEventToClient(throtInfo, deviceId);
        });
        device->SetDMACompletionCallback([this](bool isRead, pid_t pid, int deviceId, int seqId, int result) {
            // Decrement pending DMA counter when completion is delivered
            DecrementPendingDMA();

            IPCPacketHeader requestHeader{};
            requestHeader.pid = pid;
            requestHeader.deviceId = deviceId;
            requestHeader.seqId = seqId;

            std::vector<uint8_t> packet;
            const int assembleRc = isRead
                ? IPCPacketHandlerRegistry::assembleDMAReadResponsePacket(requestHeader, result, &packet)
                : IPCPacketHandlerRegistry::assembleDMAWriteResponsePacket(requestHeader, result, &packet);
            if (assembleRc != 0)
            {
                LOG_DXRT_S_ERR(
                    std::string("DMA completion packet assemble failed: op=") + (isRead ? "read" : "write") +
                    ", pid=" + std::to_string(pid) +
                    ", deviceId=" + std::to_string(deviceId) +
                    ", seqId=" + std::to_string(seqId) +
                    ", rc=" + std::to_string(assembleRc));
                return;
            }

            const int sendRc = sendToPidAcrossServers(
                pid,
                packet.empty() ? nullptr : packet.data(),
                packet.size(),
                -1);
            if (sendRc < 0)
            {
                if (isLikelyDisconnectedSendError(sendRc))
                {
                    LOG_DXRT_S_DBG << "DMA completion dropped for disconnected client: op="
                                   << (isRead ? "read" : "write")
                                   << ", pid=" << pid
                                   << ", deviceId=" << deviceId
                                   << ", seqId=" << seqId
                                   << ", rc=" << sendRc
                                   << std::endl;
                }
                else
                {
                    LOG_DXRT_S_ERR(
                        std::string("DMA completion send failed: op=") + (isRead ? "read" : "write") +
                        ", pid=" + std::to_string(pid) +
                        ", deviceId=" + std::to_string(deviceId) +
                        ", seqId=" + std::to_string(seqId) +
                        ", rc=" + std::to_string(sendRc));
                }
            }
        });
    }

    _scheduler->SetListener(this);

    CacheMemoryServices();

    _sharedMemoryWritingThread = std::make_unique<SharedMemoryWritingThread>();
    for (const auto &device : _devices)
    {
        if (!device)
        {
            continue;
        }

        auto* dispatcher = device->dispatcher();
        if (dispatcher == nullptr)
        {
            continue;
        }

        const int deviceId = device->id();
        dispatcher->SetMemoryStatsProvider([deviceId](uint64_t* total, uint64_t* used, uint64_t* free) {
            if (total == nullptr || used == nullptr || free == nullptr)
            {
                return false;
            }

            auto* memService = dxrt::MemoryService::getInstance(deviceId);
            if (memService == nullptr)
            {
                return false;
            }

            const uint64_t usedSize = memService->used_size();
            const uint64_t freeSize = memService->free_size();
            *used = usedSize;
            *free = freeSize;
            *total = usedSize + freeSize;
            return true;
        });

        _sharedMemoryWritingThread->RegisterDevice(dispatcher);
    }

    if (!_sharedMemoryWritingThread->Start())
    {
        LOG_DXRT_S_ERR("Failed to start SharedMemoryWritingThread");
    }

    // Initialize PPCPU Firmware for all devices
    {
        size_t ppuDataSize = dxrt::PPCPUDataLoader::GetDataSize();
        for (const auto &device : _devices)
        {
            int id = device->id();
            auto memService = GetMemoryService(id);
            if (memService != nullptr)
            {
                uint64_t memOffset = memService->Allocate(ppuDataSize, getpid());
                device->LoadPPCPUFirmware(memOffset);
            }
        }
    }
}

DxrtServiceV2::DxrtServiceV2(DXRTScheduleV2 schedulerOption)
    : DxrtServiceV2(dxrt::ServiceDevice::CheckServiceDevices(), schedulerOption)
{
}

void DxrtServiceV2::onClientConnected(pid_t pid)
{
    {
        std::lock_guard<std::mutex> lock(_pidSetMutex);
        _pidSet.insert(pid);
    }
}

void DxrtServiceV2::onClientDisconnected(pid_t pid)
{
    LOG_DXRT_S << "Client disconnected: pid=" << pid << std::endl;
    // Clean up resources for all devices when client disconnects
    for (size_t i = 0; i < _devices.size(); ++i)
    {
        HandleProcessDeInit(pid, static_cast<int>(i));
    }

    // Remove from pid tracking set
    {
        std::lock_guard<std::mutex> lock(_pidSetMutex);
        _pidSet.erase(pid);
    }
}

SharedMemoryInfo DxrtServiceV2::HandleAllocate(pid_t pid, int deviceId, int taskId, size_t size, bool forModel)
{
    auto *mem = GetMemoryService(deviceId);
    if (mem == nullptr)
    {
        LOG_DXRT_S_ERR(
            "HandleAllocate rejected request: pid=" + std::to_string(pid) +
            ", deviceId=" + std::to_string(deviceId) +
            ", taskId=" + std::to_string(taskId) +
            ", size=" + std::to_string(size) +
            ", forModel=" + std::to_string(forModel) +
            ", mem=" + std::string(mem == nullptr ? "null" : "ok"));
        SharedMemoryInfo invalid{};
        invalid.phys_addr_base = 0;
        invalid.phys_addr_offset = static_cast<uint64_t>(-1);
        invalid.fd = -1;
        return invalid;
    }

    LOG_DXRT_S_DBG << "HandleAllocate request: pid=" << pid
                   << ", deviceId=" << deviceId
                   << ", taskId=" << taskId
                   << ", size=" << size
                   << ", forModel=" << forModel << std::endl;

    auto allocStart = std::chrono::high_resolution_clock::now();
    SharedMemoryInfo allocInfo{};
    uint64_t physAddrOffset = static_cast<uint64_t>(-1);
    if (taskId != -1)
    {
        physAddrOffset = forModel
            ? mem->BackwardAllocateForTask(size, pid, taskId)
            : mem->AllocateForTask(size, pid, taskId);
    }
    else
    {
        physAddrOffset = forModel
            ? mem->BackwardAllocate(size, pid)
            : mem->Allocate(size, pid);
    }

    if (physAddrOffset == static_cast<uint64_t>(-1))
    {
        allocInfo.phys_addr_base = 0;
        allocInfo.phys_addr_offset = static_cast<uint64_t>(-1);
        allocInfo.fd = -1;
        return allocInfo;
    }

    try
    {
        auto shmInfo = _memfdService.CreateSharedMemory(size, taskId, deviceId);
        allocInfo = *shmInfo;
        allocInfo.phys_addr_base = _devices[deviceId]->info().mem_addr;
        allocInfo.phys_addr_offset = physAddrOffset;
        allocInfo.pid = pid;
    }
    catch (const dxrt::shm::MemFDException& e)
    {
        (void)mem->DeallocateAddress(physAddrOffset, pid);
        allocInfo.phys_addr_base = 0;
        allocInfo.phys_addr_offset = static_cast<uint64_t>(-1);
        allocInfo.fd = -1;
        allocInfo.block_id = static_cast<int>(e.GetErrorCode());
        return allocInfo;
    }
    TrackSharedMemoryHandle(deviceId, pid, allocInfo);

    auto allocEnd = std::chrono::high_resolution_clock::now();
    auto allocDuration = std::chrono::duration_cast<std::chrono::milliseconds>(allocEnd - allocStart);

    if (allocInfo.fd == -1)
    {
        LOG_DXRT_S_ERR(
            "HandleAllocate failed: pid=" + std::to_string(pid) +
            ", deviceId=" + std::to_string(deviceId) +
            ", taskId=" + std::to_string(taskId) +
            ", size=" + std::to_string(size) +
            ", forModel=" + std::to_string(forModel) +
            ", elapsed_ms=" + std::to_string(allocDuration.count()));
        return allocInfo;
    }

    LOG_DXRT_S_DBG << "HandleAllocate success: pid=" << pid
                   << ", deviceId=" << deviceId
                   << ", taskId=" << taskId
                   << ", size=" << size
                   << ", forModel=" << forModel
                   << ", address=0x" << std::hex << allocInfo.phys_addr() << std::dec
                   << ", block_id=" << allocInfo.block_id
                   << ", alloc_elapsed_ms=" << allocDuration.count() << std::endl;

    onClientConnected(pid);
    return allocInfo;
}

int DxrtServiceV2::HandleDeallocate(pid_t pid, int deviceId, uint64_t address)
{
    auto *mem = GetMemoryService(deviceId);
    if (mem == nullptr)
    {
        return -1;
    }

    SharedMemoryInfo info{};
    if (TryResolveSharedMemoryHandle(deviceId, pid, address, &info))
    {
        ReleaseSharedMemoryHandle(info);
        return mem->DeallocateAddress(info.phys_addr_offset, pid) ? 0 : -1;
    }

    // Fallback for old callers that pass either offset or base+offset.
    const uint64_t base = _devices[deviceId]->info().mem_addr;
    const uint64_t offset = (address >= base) ? (address - base) : address;
    return mem->DeallocateAddress(offset, pid) ? 0 : -1;
}

int DxrtServiceV2::HandleInferenceRequest(pid_t pid, int deviceId, const dxrt_request_acc_t &request)
{
    if (!IsTaskValid(pid, deviceId, request.task_id))
    {
        return -1;
    }

    // req_id is used as scheduler key per process; reject duplicates while still pending.
    if (_scheduler->HasPendingRequest(pid, request.req_id))
    {
        LOG_DXRT_S_ERR(
            "HandleInferenceRequest rejected duplicate req_id: pid=" + std::to_string(pid) +
            ", deviceId=" + std::to_string(deviceId) +
            ", req_id=" + std::to_string(request.req_id));
        return -EALREADY;
    }

    // Reject new inference requests while device recovery is in progress.
    // Clients are expected to retry after receiving S_ERR_DEVICE_EVENT_FAULT.
    if (_recoveryInProgress.load(std::memory_order_acquire))
    {
        return -EAGAIN;
    }

    {
        std::lock_guard<std::mutex> lock(_deviceMutex);
        if (_devices[deviceId]->isBlocked())
        {
            return -1;
        }
    }

    _scheduler->AddScheduler(request, deviceId);
    return 0;
}

int DxrtServiceV2::HandleTaskInit(pid_t pid, int deviceId, int taskId, int bound, uint64_t modelMemorySize, const TaskStaticConfig &config)
{
    return TaskInit(pid, deviceId, taskId, bound, modelMemorySize, config) ? 0 : -1;
}

int DxrtServiceV2::HandleTaskDeInit(pid_t pid, int deviceId, int taskId)
{
    TaskDeInit(deviceId, taskId, pid);
    return 0;
}

int DxrtServiceV2::HandleDMAWrite(
    pid_t pid,
    int seqId,
    int deviceId,
    int64_t blockId,
    uint64_t blockOffset,
    uint64_t size)
{
    if (size == 0)
    {
        return 0;
    }

    if (deviceId < 0 || static_cast<size_t>(deviceId) >= _devices.size())
    {
        return -EINVAL;
    }

    if (_recoveryInProgress.load(std::memory_order_acquire))
    {
        return -EAGAIN;
    }

    // Resolve source data via shared memory block_id.
    SharedMemHandle sharedHandle{};
    if (!TryGetSharedMemoryHandle(deviceId, pid, static_cast<uint64_t>(blockId), &sharedHandle)
        || sharedHandle.ptr == nullptr
        || blockOffset + size > sharedHandle.size)
    {
        LOG_DXRT_S_ERR(
            "HandleDMAWrite: block not found or out of bounds: "
            "pid=" + std::to_string(pid) +
            ", deviceId=" + std::to_string(deviceId) +
            ", blockId=" + std::to_string(blockId) +
            ", blockOffset=" + std::to_string(blockOffset) +
            ", size=" + std::to_string(size) +
            ", handleSize=" + std::to_string(sharedHandle.size));
        return -EINVAL;
    }

    void *dataPtr = static_cast<uint8_t *>(sharedHandle.ptr) + blockOffset;
    const uint64_t deviceOffset = sharedHandle.physAddrOffset + blockOffset;

    dxrt_meminfo_t meminfo{};
    meminfo.data = reinterpret_cast<uint64_t>(dataPtr);
    meminfo.base = _devices[deviceId]->info().mem_addr;
    meminfo.offset = static_cast<uint32_t>(deviceOffset);
    meminfo.size = static_cast<uint32_t>(size);

    dxrt::shm::SharedMemorySyscallAdapter::InvalidateMemory(dataPtr, static_cast<size_t>(size));

    IncrementPendingDMA();
    _devices[deviceId]->DMAWriteAsync(meminfo, pid, seqId);

    return 0;
}

int DxrtServiceV2::HandleDMARead(
    pid_t pid,
    int seqId,
    int deviceId,
    int64_t blockId,
    uint64_t blockOffset,
    uint64_t size)
{
    if (size == 0)
    {
        return 0;
    }

    if (deviceId < 0 || static_cast<size_t>(deviceId) >= _devices.size())
    {
        return -EINVAL;
    }

    if (_recoveryInProgress.load(std::memory_order_acquire))
    {
        return -EAGAIN;
    }

    // Resolve destination buffer via shared memory block_id.
    SharedMemHandle sharedHandle{};
    if (!TryGetSharedMemoryHandle(deviceId, pid, static_cast<uint64_t>(blockId), &sharedHandle)
        || sharedHandle.ptr == nullptr
        || blockOffset + size > sharedHandle.size)
    {
        LOG_DXRT_S_ERR(
            "HandleDMARead: block not found or out of bounds: "
            "pid=" + std::to_string(pid) +
            ", deviceId=" + std::to_string(deviceId) +
            ", blockId=" + std::to_string(blockId) +
            ", blockOffset=" + std::to_string(blockOffset) +
            ", size=" + std::to_string(size) +
            ", handleSize=" + std::to_string(sharedHandle.size));
        return -EINVAL;
    }

    void *dataPtr = static_cast<uint8_t *>(sharedHandle.ptr) + blockOffset;
    const uint64_t deviceOffset = sharedHandle.physAddrOffset + blockOffset;

    dxrt_meminfo_t meminfo{};
    meminfo.data = reinterpret_cast<uint64_t>(dataPtr);
    meminfo.base = _devices[deviceId]->info().mem_addr;
    meminfo.offset = static_cast<uint32_t>(deviceOffset);
    meminfo.size = static_cast<uint32_t>(size);

    IncrementPendingDMA();
    _devices[deviceId]->DMAReadAsync(meminfo, pid, seqId);
    return 0;
}

int DxrtServiceV2::HandleDMAReadWithFaultInjection(
    pid_t pid,
    int deviceId,
    int64_t blockId,
    uint64_t blockOffset,
    uint64_t size)
{
    if (size == 0)
    {
        return 0;
    }

    if (deviceId < 0 || static_cast<size_t>(deviceId) >= _devices.size())
    {
        return -EINVAL;
    }

    // Resolve destination buffer via shared memory block_id.
    SharedMemHandle sharedHandle{};
    if (!TryGetSharedMemoryHandle(deviceId, pid, static_cast<uint64_t>(blockId), &sharedHandle)
        || sharedHandle.ptr == nullptr
        || blockOffset + size > sharedHandle.size)
    {
        LOG_DXRT_S_ERR(
            "HandleDMAReadWithFaultInjection: block not found or out of bounds: "
            "pid=" + std::to_string(pid) +
            ", deviceId=" + std::to_string(deviceId) +
            ", blockId=" + std::to_string(blockId) +
            ", blockOffset=" + std::to_string(blockOffset) +
            ", size=" + std::to_string(size) +
            ", handleSize=" + std::to_string(sharedHandle.size));
        return -EINVAL;
    }

    void *dataPtr = static_cast<uint8_t *>(sharedHandle.ptr) + blockOffset;
    const uint64_t deviceOffset = sharedHandle.physAddrOffset + blockOffset;

    dxrt_req_meminfo_t meminfo{};
    meminfo.data = reinterpret_cast<uint64_t>(dataPtr);
    meminfo.base = _devices[deviceId]->info().mem_addr;
    meminfo.offset = static_cast<uint32_t>(deviceOffset);
    meminfo.size = static_cast<uint32_t>(size);
    meminfo.ch = 0;

    // Test-only fault injection (service mode): the RT client flagged this single
    // output read for corruption (it owns the DXRT_FAULT_INJECT_OUTPUT env var and
    // the read counter). The client cannot corrupt the device address itself
    // because only block_id/offset/size cross the IPC boundary and the service
    // resolves the real address here. Mask the high 32 bits of base so base+offset
    // falls outside the device memory window; with the driver's
    // fault_inject_skip_addr_check=1 this bad address reaches HW and triggers a
    // DMA abort + recovery (same address as the library-mode output.base
    // corruption: base & 0xFFFFFFFF).

    const uint64_t corrupted = meminfo.base & 0x00000000FFFFFFFFULL;
    std::ostringstream faultMsg;
    faultMsg << "[FAULT_INJECT] HandleDMAReadWithFaultInjection: corrupting meminfo.base high 32 bits (0x"
                << std::hex << meminfo.base << " -> 0x" << corrupted
                << "), deviceOffset=0x" << deviceOffset << std::dec;
    LOG_DXRT_S_ERR(faultMsg.str());
    meminfo.base = corrupted;


    int readRc = 0;
    {
        std::lock_guard<std::mutex> lock(_deviceMutex);
        readRc = _devices[deviceId]->Process(dxrt::dxrt_cmd_t::DXRT_CMD_READ_MEM, static_cast<void *>(&meminfo));
    }
    if (readRc != 0)
    {
        const uint32_t errCode = static_cast<uint32_t>(dxrt::dxrt_error_t::ERR_PCIE_DMA_CH0_ABORT);
        LOG_DXRT_S_ERR(
            "HandleDMAReadWithFaultInjection: DXRT_CMD_READ_MEM failed (rc=" + std::to_string(readRc) +
            ") for deviceId=" + std::to_string(deviceId) +
            ". Triggering fallback recovery path.");

        RecoveryBroadcastAndWait(dxrt::dxrt_server_err_t::S_ERR_DEVICE_EVENT_FAULT, errCode, deviceId);

        int recoveryRc = 0;
        {
            std::lock_guard<std::mutex> lock(_deviceMutex);
            recoveryRc = _devices[deviceId]->Process(dxrt::dxrt_cmd_t::DXRT_CMD_RECOVERY, nullptr);
        }
        if (recoveryRc != 0)
        {
            LOG_DXRT_S_ERR(
                "HandleDMAReadWithFaultInjection: fallback DXRT_CMD_RECOVERY failed (rc=" +
                std::to_string(recoveryRc) + ") for deviceId=" + std::to_string(deviceId));
        }
        else
        {
            LOG_DXRT_S << "HandleDMAReadWithFaultInjection: fallback recovery completed for device "
                       << deviceId << std::endl;
        }
        // Keep the same post-recovery semantics as the standard recovery path
        // (ClearAllLoad + recovery flag release).
        ServiceRecoveryAdapter(this).ResumeAfterRecovery(deviceId);
        return readRc;
    }

    dxrt::shm::SharedMemorySyscallAdapter::SyncMemory(dataPtr, static_cast<size_t>(size));

    return 0;
}


int DxrtServiceV2::HandleProcessDeInit(pid_t pid, int deviceId)
{

    LOG_DXRT_S << "HandleProcessDeInit: pid=" << pid << ", deviceId=" << deviceId << std::endl;
    // Identify tasks with in-flight (RUNNING) requests *before* stopping them.
    // Their RMAP/Weight memory must not be freed until the NPU delivers the
    // completion response; register them for deferred cleanup via _onTaskDrained.
    const std::vector<int> runningTaskIds = _scheduler->GetRunningTaskIds(pid);
    const std::set<int> runningTaskIdSet(runningTaskIds.begin(), runningTaskIds.end());
    {
        std::lock_guard<std::mutex> lock(_pendingTaskFreeMutex);
        for (int taskId : runningTaskIds)
        {
            _pendingTaskFree[{pid, taskId}] = deviceId;
        }

        LOG_DXRT_S << "HandleProcessDeInit: pid=" << pid
                   << ", deviceId=" << deviceId
                   << ", runningTaskIds count=" << runningTaskIds.size() << std::endl;
    }

    _scheduler->StopAllInferenceForProcess(pid, deviceId);

    const std::vector<std::pair<int, dxrt::npu_bound_op>> taskEntries =
        _taskInfoStore.extractTaskIdsWithBound(pid, deviceId);
    for (const auto &entry : taskEntries)
    {
        const int taskId = entry.first;
        _scheduler->StopTaskInference(pid, deviceId, taskId);
    }
    _scheduler->ClearRunningRequests(pid, deviceId);

    auto *memService = GetMemoryService(deviceId);
    for (const auto &entry : taskEntries)
    {
        const int taskId = entry.first;
        const dxrt::npu_bound_op bound = entry.second;

        if (memService != nullptr)
        {
            LOG_DXRT_S << "HandleProcessDeInit mem clear: pid=" << pid
                       << ", deviceId=" << deviceId
                       << ", taskId=" << taskId << std::endl;
            // Do not deallocate tasks that still have in-flight requests.
            // Those are deferred and reclaimed in onTaskDrained().
            if (runningTaskIdSet.count(taskId) == 0)
            {
                // No in-flight request: RMAP/Weight is safe to free now.
                (void)memService->DeallocateTask(pid, taskId);
            }
        }

        // Running tasks: DeallocateTask deferred to _onTaskDrained.
        std::lock_guard<std::mutex> lock(_deviceMutex);
        if (deviceId >= 0 && deviceId < static_cast<int>(_devices.size()))
        {
            (void)_devices[deviceId]->DeleteBound(bound);
        }
    }
    ReleaseAllSharedMemoryHandlesForProcess(deviceId, pid);

    {
        std::lock_guard<std::mutex> lock(_pidSetMutex);
        _pidSet.erase(pid);
    }
    return 0;
}

void DxrtServiceV2::TrackSharedMemoryHandle(int deviceId, pid_t pid, const SharedMemoryInfo &info)
{
    std::lock_guard<std::mutex> lock(_sharedMemoryLock);
    SharedMemHandle handle;
    handle.fd = info.fd;
    handle.ptr = info.ptr;
    handle.size = info.size;
    handle.physAddrOffset = info.phys_addr_offset;
    auto &addrMap = _sharedMemoryHandles[deviceId][pid];
    // Support absolute address, offset, and block_id (1-based) addressing styles.
    addrMap[info.phys_addr()] = handle;
    addrMap[info.phys_addr_offset] = handle;
    if (info.block_id > 0)
    {
        addrMap[static_cast<uint64_t>(info.block_id)] = handle;
    }
}

bool DxrtServiceV2::TryGetSharedMemoryHandle(int deviceId, pid_t pid, uint64_t address, SharedMemHandle *handle)
{
    if (handle == nullptr)
    {
        return false;
    }

    std::lock_guard<std::mutex> lock(_sharedMemoryLock);
    auto deviceIt = _sharedMemoryHandles.find(deviceId);
    if (deviceIt == _sharedMemoryHandles.end())
    {
        return false;
    }

    auto pidIt = deviceIt->second.find(pid);
    if (pidIt == deviceIt->second.end())
    {
        return false;
    }

    auto addrIt = pidIt->second.find(address);
    if (addrIt == pidIt->second.end())
    {
        return false;
    }

    *handle = addrIt->second;
    return true;
}

bool DxrtServiceV2::TryResolveSharedMemoryHandle(int deviceId, pid_t pid, uint64_t address, SharedMemoryInfo *info)
{
    if (info == nullptr)
    {
        return false;
    }

    std::lock_guard<std::mutex> lock(_sharedMemoryLock);
    auto deviceIt = _sharedMemoryHandles.find(deviceId);
    if (deviceIt == _sharedMemoryHandles.end())
    {
        return false;
    }
    auto pidIt = deviceIt->second.find(pid);
    if (pidIt == deviceIt->second.end())
    {
        return false;
    }
    auto addrIt = pidIt->second.find(address);
    if (addrIt == pidIt->second.end())
    {
        return false;
    }

    const SharedMemHandle handle = addrIt->second;

    // Remove all alias keys (absolute and offset) that point to this block.
    for (auto it = pidIt->second.begin(); it != pidIt->second.end(); )
    {
        if (it->second.physAddrOffset == handle.physAddrOffset)
        {
            it = pidIt->second.erase(it);
            continue;
        }
        ++it;
    }

    if (pidIt->second.empty())
    {
        deviceIt->second.erase(pidIt);
    }
    if (deviceIt->second.empty())
    {
        _sharedMemoryHandles.erase(deviceIt);
    }

    info->fd = handle.fd;
    info->ptr = handle.ptr;
    info->size = handle.size;
    info->phys_addr_base = _devices[deviceId]->info().mem_addr;
    info->phys_addr_offset = handle.physAddrOffset;
    return true;
}

void DxrtServiceV2::ReleaseSharedMemoryHandle(const SharedMemoryInfo &info)
{
    if (info.fd >= 0)
    {
        (void)_memfdService.ReleaseSharedMemory(info.fd);
    }
}

void DxrtServiceV2::ReleaseAllSharedMemoryHandlesForProcess(int deviceId, pid_t pid)
{
    std::vector<SharedMemoryInfo> pending;
    {
        std::lock_guard<std::mutex> lock(_sharedMemoryLock);
        auto deviceIt = _sharedMemoryHandles.find(deviceId);
        if (deviceIt == _sharedMemoryHandles.end())
        {
            return;
        }
        auto pidIt = deviceIt->second.find(pid);
        if (pidIt == deviceIt->second.end())
        {
            return;
        }

        pending.reserve(pidIt->second.size());
        for (const auto &pair : pidIt->second)
        {
            SharedMemoryInfo info{};
            info.fd = pair.second.fd;
            info.ptr = pair.second.ptr;
            info.size = pair.second.size;
            info.phys_addr_base = _devices[deviceId]->info().mem_addr;
            info.phys_addr_offset = pair.second.physAddrOffset;
            pending.push_back(info);
        }

        deviceIt->second.erase(pidIt);
        if (deviceIt->second.empty())
        {
            _sharedMemoryHandles.erase(deviceIt);
        }
    }

    for (const auto &info : pending)
    {
        ReleaseSharedMemoryHandle(info);
    }
}

int DxrtServiceV2::HandleViewMemory(int deviceId, bool viewUsedMemory, uint64_t *bytes) const
{
    const dxrt::MemoryService *service = GetMemoryService(deviceId);
    if (service == nullptr || bytes == nullptr)
    {
        return -1;
    }

    *bytes = viewUsedMemory ? service->used_size() : service->free_size();
    return 0;
}

int DxrtServiceV2::HandleViewAvailableDevice(uint64_t *availableMask) const
{
    if (availableMask == nullptr)
    {
        return -1;
    }

    uint64_t mask = 1;
    uint64_t available = 0;
    for (const auto &device : _devices)
    {
        if (!device->isBlocked())
        {
            available |= mask;
        }
        mask <<= 1;
    }

    *availableMask = available;
    return 0;
}

int DxrtServiceV2::HandleGetUsage(int deviceId, int channel, double *usage) const
{
    if (usage == nullptr)
    {
        return -1;
    }

    if (deviceId < 0 || deviceId >= static_cast<int>(_devices.size()))
    {
        return -1;
    }

    *usage = _devices[deviceId]->getUsage(channel);
    return 0;
}

int DxrtServiceV2::HandleGetDeviceTelemetry(int deviceId, IPCDeviceTelemetryPayload *telemetry) const
{
    if (telemetry == nullptr)
    {
        return -1;
    }

    if (deviceId < 0 || deviceId >= static_cast<int>(_devices.size()))
    {
        return -1;
    }

    uint64_t usedMemoryBytes = 0;
    uint64_t freeMemoryBytes = 0;
    const int usedMemoryRc = HandleViewMemory(deviceId, true, &usedMemoryBytes);
    const int freeMemoryRc = HandleViewMemory(deviceId, false, &freeMemoryBytes);
    if (usedMemoryRc != 0)
    {
        return usedMemoryRc;
    }
    if (freeMemoryRc != 0)
    {
        return freeMemoryRc;
    }

    const auto deviceInfo = _devices[deviceId]->info();
    const auto deviceStatus = _devices[deviceId]->status();
    const size_t statusCoreCapacity = sizeof(deviceStatus.clock) / sizeof(deviceStatus.clock[0]);
    const uint32_t coreCount = (std::min<uint32_t>)(
        deviceInfo.num_dma_ch,
        static_cast<uint32_t>(statusCoreCapacity));

    telemetry->usedMemoryBytes = usedMemoryBytes;
    telemetry->freeMemoryBytes = freeMemoryBytes;
    telemetry->cores.assign(coreCount, IPCDeviceTelemetryCoreData{});
    for (uint32_t index = 0; index < coreCount; ++index)
    {
        telemetry->cores[index].utilizationPermille = static_cast<uint32_t>(
            _devices[deviceId]->getUsage(static_cast<int>(index)) * 1000.0);
        telemetry->cores[index].temperature = static_cast<int32_t>(deviceStatus.temperature[index]);
        telemetry->cores[index].clock = deviceStatus.clock[index];
        telemetry->cores[index].voltage = deviceStatus.voltage[index];
    }

    return 0;
}

void DxrtServiceV2::onInferenceComplete(const dxrt::dxrt_response_t& response, int deviceId)
{
    onCompleteInference(response, deviceId);
}

void DxrtServiceV2::onSchedulerError(dxrt::dxrt_server_err_t err, uint32_t errCode, int deviceId)
{
    ErrorBroadCastToClient(err, errCode, deviceId, nullptr);
}

bool DxrtServiceV2::validateTask(pid_t pid, int deviceId, int taskId)
{
    return IsTaskValid(pid, deviceId, taskId);
}

void DxrtServiceV2::onTaskDrained(pid_t pid, int taskId)
{
    int deviceId = -1;
    {
        std::lock_guard<std::mutex> lock(_pendingTaskFreeMutex);
        auto it = _pendingTaskFree.find({pid, taskId});
        if (it == _pendingTaskFree.end()) { return; }  // normal idle — nothing to free
        deviceId = it->second;
        _pendingTaskFree.erase(it);
    }
    auto *memService = GetMemoryService(deviceId);
    if (memService != nullptr)
    {
        (void)memService->DeallocateTask(pid, taskId);
    }
}

void DxrtServiceV2::onCompleteInference(const dxrt::dxrt_response_t &response, int deviceId)
{
    LOG_DXRT_S_DBG << "onCompleteInference deviceId=" << deviceId
                   << ", req_id=" << response.req_id
                   << ", proc_id=" << response.proc_id
                   << ", status=" << response.status
                   << ", dma_ch=" << response.dma_ch
                   << ", inf_time=" << response.inf_time
                   << std::endl;


    const int result = (response.status == 0) ? 0 : -1;

    std::vector<uint8_t> packet;
    IPCPacketHeader requestHeader{};
    requestHeader.pid = response.proc_id;
    requestHeader.deviceId = deviceId;
    requestHeader.seqId = 0;

    const int assembleRc = IPCPacketHandlerRegistry::assembleInferenceResponsePacket(
        requestHeader,
        result,
        response,
        &packet);
    if (assembleRc != 0)
    {
        LOG_DXRT_S_ERR(
            "onCompleteInference failed to assemble callback response: req_id=" + std::to_string(response.req_id) +
            ", pid=" + std::to_string(response.proc_id) +
            ", rc=" + std::to_string(assembleRc));
        return;
    }

    const int sendRc = sendToPidAcrossServers(
        response.proc_id,
        packet.empty() ? nullptr : packet.data(),
        packet.size(),
        -1);
    if (sendRc < 0)
    {
        const int err = (sendRc < 0) ? -sendRc : sendRc;
        if (isLikelyDisconnectedSendError(sendRc))
        {
            LOG_DXRT_S_DBG << "onCompleteInference callback dropped for disconnected client: req_id="
                           << response.req_id
                           << ", pid=" << response.proc_id
                           << ", rc=" << sendRc
                           << ", err=" << err
                           << " (" << std::strerror(err) << ")"
                           << std::endl;
        }
        else
        {
            LOG_DXRT_S_ERR(
                "onCompleteInference failed to send callback response: req_id=" + std::to_string(response.req_id) +
                ", pid=" + std::to_string(response.proc_id) +
                ", rc=" + std::to_string(sendRc) +
                ", err=" + std::to_string(err) +
                " (" + std::string(std::strerror(err)) + ")");
        }
    }
}

void DxrtServiceV2::BroadcastThrottlingEventToClient(dxrt::dx_pcie_dev_ntfy_throt_t throtInfo, int deviceId)
{
    std::vector<pid_t> pids;
    {
        std::lock_guard<std::mutex> lock(_pidSetMutex);
        pids.assign(_pidSet.begin(), _pidSet.end());
    }

    if (pids.empty())
    {
        return;
    }

    IPCPacketThrottleNotification packet{};
    initializeIPCPacket(&packet, IPCMessageType::ThrottleNotification, 0);
    packet.header.deviceId = deviceId;
    packet.throtInfo = throtInfo;

    for (pid_t pid : pids)
    {
        const int sendRc = sendToPidAcrossServers(
            pid,
            SafeCast::PtrToBytePtr(&packet),
            sizeof(packet),
            -1);
        if (sendRc < 0)
        {
            LOG_DXRT_S_ERR(
                "BroadcastThrottlingEventToClient failed: pid=" + std::to_string(pid) +
                ", deviceId=" + std::to_string(deviceId) +
                ", ntfy_code=" + std::to_string(packet.throtInfo.ntfy_code) +
                ", sendRc=" + std::to_string(sendRc));
        }
    }
}

void DxrtServiceV2::ErrorBroadCastToClient(
    dxrt::dxrt_server_err_t err,
    uint32_t errCode,
    int deviceId,
    const dx_pcie_dev_err_t *errorInfo)
{
    std::vector<pid_t> pids;
    {
        std::lock_guard<std::mutex> lock(_pidSetMutex);
        pids.assign(_pidSet.begin(), _pidSet.end());
    }

    IPCPacketErrorNotification packet{};
    initializeIPCPacket(&packet, IPCMessageType::ErrorNotification, 0);
    packet.header.deviceId = deviceId;
    packet.serverErr = static_cast<int64_t>(err);
    if (errorInfo != nullptr)
    {
        packet.errorInfo = *errorInfo;
    }
    else
    {
        std::memset(&packet.errorInfo, 0, sizeof(packet.errorInfo));
        packet.errorInfo.err_code = errCode;
        packet.errorInfo.npu_id = static_cast<uint32_t>(deviceId);
    }

    for (pid_t pid : pids)
    {
        const int sendRc = sendToPidAcrossServers(
            pid,
            SafeCast::PtrToBytePtr(&packet),
            sizeof(packet),
            -1);
        if (sendRc < 0)
        {
            LOG_DXRT_S_ERR(
                "ErrorBroadCastToClient failed: pid=" + std::to_string(pid) +
                ", err=" + std::to_string(static_cast<int>(err)) +
                ", errCode=" + std::to_string(errCode) +
                ", packetErrCode=" + std::to_string(packet.errorInfo.err_code) +
                ", deviceId=" + std::to_string(deviceId) +
                ", sendRc=" + std::to_string(sendRc));
        }
    }
}

void DxrtServiceV2::WaitForAllClientsDead(int timeoutMs)
{
    constexpr int pollIntervalMs = 50;
    int elapsed = 0;

    while (elapsed < timeoutMs)
    {
        std::vector<pid_t> pids;
        {
            std::lock_guard<std::mutex> lock(_pidSetMutex);
            pids.assign(_pidSet.begin(), _pidSet.end());
        }

        if (pids.empty())
        {
            LOG_DXRT_S << "All client processes have terminated." << std::endl;
            return;
        }

        std::vector<pid_t> alive;
        alive.reserve(pids.size());
        for (pid_t pid : pids)
        {
            if (IsProcessRunning(pid))
            {
                alive.push_back(pid);
            }
            else
            {
                std::lock_guard<std::mutex> lock(_pidSetMutex);
                _pidSet.erase(pid);
            }
        }

        if (alive.empty())
        {
            LOG_DXRT_S << "All client processes have terminated." << std::endl;
            return;
        }

        LOG_DXRT_S << "Waiting for " << alive.size() << " client(s) to terminate ("
                   << elapsed << "ms / " << timeoutMs << "ms)..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(pollIntervalMs));
        elapsed += pollIntervalMs;
    }

    std::vector<pid_t> remaining;
    {
        std::lock_guard<std::mutex> lock(_pidSetMutex);
        remaining.assign(_pidSet.begin(), _pidSet.end());
    }

    for (pid_t pid : remaining)
    {
        if (!IsProcessRunning(pid))
        {
            continue;
        }

        LOG_DXRT_S_ERR("Client PID " + std::to_string(pid)
            + " did not terminate within " + std::to_string(timeoutMs)
            + "ms. Sending SIGKILL.");

#ifdef __linux__
        kill(pid, SIGKILL);
#elif _WIN32
        HANDLE process = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
        if (process != NULL)
        {
            TerminateProcess(process, 1);
            CloseHandle(process);
        }
#endif
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    LOG_DXRT_S << "Force-killed remaining clients after timeout." << std::endl;
}

// ServiceRecoveryAdapter implementation

bool DxrtServiceV2::ServiceRecoveryAdapter::PauseForRecovery(uint32_t errCode, int deviceId)
{
    _service->RecoveryBroadcastAndWait(
        dxrt_server_err_t::S_ERR_DEVICE_EVENT_FAULT, errCode, deviceId);
    return true;
}

void DxrtServiceV2::ServiceRecoveryAdapter::ResumeAfterRecovery(int deviceId)
{
    // After FW recovery the NPU has lost all in-flight model context. Reset
    // the scheduler load counters so stale counts don't skew scheduling after
    // clients re-initialize their tasks.
    _service->_scheduler->ClearAllLoad();
    _service->_recoveryInProgress.store(false, std::memory_order_release);
    LOG_DXRT_S << "Recovery complete for device " << deviceId
               << ": device ready, service continues." << std::endl;
}

[[noreturn]] void DxrtServiceV2::ServiceRecoveryAdapter::OnRecoveryFailed(int deviceId)
{
    LOG_DXRT_S_ERR("Recovery failed for device " + std::to_string(deviceId) + ". Aborting service.");
    std::abort();
}

void DxrtServiceV2::RecoveryBroadcastAndWait(dxrt::dxrt_server_err_t err, uint32_t errCode, int deviceId)
{
    _recoveryInProgress.store(true, std::memory_order_release);

    LOG_DXRT_S_DBG << "Recovery start (errCode=" << errCode
                   << ", deviceId=" << deviceId << ")" << std::endl;
    LOG_DXRT_S_DBG << "Step 1: Broadcasting error to all clients..." << std::endl;
    ErrorBroadCastToClient(err, errCode, deviceId, nullptr);

    // Wait for in-flight DMA operations to complete instead of killing clients
    // According to DMA_ABORT_RECOVERY_USER_SPACE_REQUIREMENTS.md:
    // Driver will complete aborted channel's in-flight transfers with -EIO
    constexpr int kDmaWaitTimeoutMs = 10000;
    LOG_DXRT_S << "Step 2: Waiting for in-flight DMA operations (timeout: " << kDmaWaitTimeoutMs << "ms)..." << std::endl;

    const bool dmaCompleted = WaitForDMACompletion(kDmaWaitTimeoutMs);
    if (!dmaCompleted)
    {
        LOG_DXRT_S_ERR("DMA wait timed out after " << kDmaWaitTimeoutMs << "ms, proceeding with recovery");
    }
    else
    {
        LOG_DXRT_S_DBG << "All in-flight DMA operations completed." << std::endl;
    }

    LOG_DXRT_S_DBG << "Step 3: Ready for device recovery (clients remain connected)." << std::endl;
}

bool DxrtServiceV2::TaskInit(pid_t pid, int deviceId, int taskId, int bound, uint64_t modelMemorySize, const TaskStaticConfig &config)
{
    if (deviceId < 0 || deviceId >= static_cast<int>(_devices.size()))
    {
        return false;
    }

    auto memService = GetMemoryService(deviceId);
    if (memService == nullptr)
    {
        return false;
    }

    if (memService->free_size() < modelMemorySize)
    {
        memService->OptimizeMemory();
        if (memService->free_size() < modelMemorySize)
        {
            return false;
        }
    }

    const auto boundOp = static_cast<dxrt::npu_bound_op>(bound);
    {
        std::lock_guard<std::mutex> lock(_deviceMutex);
        if (_devices[deviceId]->isBlocked())
        {
            return false;
        }
        if (_devices[deviceId]->CanAcceptBound(boundOp) == false)
        {
            return false;
        }
        if (_devices[deviceId]->AddBound(boundOp) != 0)
        {
            return false;
        }
    }

    if (!_taskInfoStore.addTask(
            pid,
            deviceId,
            taskId,
            boundOp,
            modelMemorySize,
            dxrt::InferenceContext{config}))
    {
        std::lock_guard<std::mutex> lock(_deviceMutex);
        (void)_devices[deviceId]->DeleteBound(boundOp);
        return false;
    }

    return true;
}

void DxrtServiceV2::TaskDeInit(int deviceId, int taskId, int pid)
{
    dxrt::npu_bound_op bound = dxrt::npu_bound_op::N_BOUND_NORMAL;
    const bool hasTask = _taskInfoStore.removeTask(pid, deviceId, taskId, &bound);

    if (!hasTask)
    {
        return;
    }

    _scheduler->StopTaskInference(pid, deviceId, taskId);
    // NOTE: ClearRunningRequests(pid, deviceId) was intentionally removed.
    // StopTaskInference already marks RUNNING requests for this specific taskId as CANCELLED.
    // ClearRunningRequests has no taskId filter and would incorrectly cancel in-flight requests
    // belonging to other tasks of the same (pid, deviceId) — e.g. a second InferenceEngine
    // in a different thread using the same device — causing their callbacks to be silently
    // dropped and the client to hang indefinitely.

    std::lock_guard<std::mutex> lock(_deviceMutex);
    if (deviceId >= 0 && deviceId < static_cast<int>(_devices.size()))
    {
        (void)_devices[deviceId]->DeleteBound(bound);
    }
}

bool DxrtServiceV2::IsTaskValid(pid_t pid, int deviceId, int taskId)
{
    if (!_taskInfoStore.hasTask(pid, deviceId, taskId))
    {
        return false;
    }

    auto memService = GetMemoryService(deviceId);
    return (memService != nullptr) && memService->IsTaskValid(pid, taskId);
}

void DxrtServiceV2::Dispose()
{
    std::vector<SharedMemoryInfo> pending;
    {
        std::lock_guard<std::mutex> lock(_sharedMemoryLock);
        for (const auto &devicePair : _sharedMemoryHandles)
        {
            const int deviceId = devicePair.first;
            const uint64_t base = _devices[deviceId]->info().mem_addr;
            for (const auto &pidPair : devicePair.second)
            {
                for (const auto &addrPair : pidPair.second)
                {
                    SharedMemoryInfo info{};
                    info.fd = addrPair.second.fd;
                    info.ptr = addrPair.second.ptr;
                    info.size = addrPair.second.size;
                    info.phys_addr_base = base;
                    info.phys_addr_offset = addrPair.second.physAddrOffset;
                    pending.push_back(info);
                }
            }
        }
        _sharedMemoryHandles.clear();
    }

    for (const auto &info : pending)
    {
        ReleaseSharedMemoryHandle(info);
    }
}

void DxrtServiceV2::IncrementPendingDMA()
{
    _pendingDmaOperations.fetch_add(1, std::memory_order_acq_rel);
}

void DxrtServiceV2::DecrementPendingDMA()
{
    int current = _pendingDmaOperations.load(std::memory_order_acquire);
    while (current > 0)
    {
        if (_pendingDmaOperations.compare_exchange_weak(
                current, current - 1,
                std::memory_order_acq_rel,
                std::memory_order_acquire))
        {
            if (current == 1)
            {
                std::lock_guard<std::mutex> lock(_dmaMutex);
                _dmaCompletionCv.notify_all();
            }
            return;
        }
    }
    LOG_DXRT_S_ERR("DecrementPendingDMA: counter already 0, underflow prevented");
}

bool DxrtServiceV2::WaitForDMACompletion(int timeoutMs)
{
    std::unique_lock<std::mutex> lock(_dmaMutex);

    const auto startTime = std::chrono::steady_clock::now();
    const auto deadline = startTime + std::chrono::milliseconds(timeoutMs);
    constexpr auto kPollInterval = std::chrono::milliseconds(50);
    constexpr auto kLogInterval  = std::chrono::seconds(1);
    auto nextLogTime = startTime + kLogInterval;

    while (_pendingDmaOperations.load(std::memory_order_acquire) > 0)
    {
        const auto now = std::chrono::steady_clock::now();
        if (now >= deadline)
        {
            break;
        }
        const auto remaining = std::chrono::duration_cast<std::chrono::milliseconds>(deadline - now);
        _dmaCompletionCv.wait_for(lock, (std::min)(remaining, kPollInterval));

        if (_pendingDmaOperations.load(std::memory_order_acquire) == 0)
        {
            return true;
        }

        const auto afterWait = std::chrono::steady_clock::now();
        if (afterWait >= nextLogTime)
        {
            const auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                afterWait - startTime).count();
            LOG_DXRT_S << "Waiting for DMA completion: "
                       << _pendingDmaOperations.load(std::memory_order_acquire)
                       << " operations remaining (" << elapsedMs << "ms / " << timeoutMs << "ms)..." << std::endl;
            nextLogTime += kLogInterval;
        }
    }

    const int pending = _pendingDmaOperations.load(std::memory_order_acquire);
    if (pending == 0)
    {
        return true;
    }

    LOG_DXRT_S_ERR("DMA completion timeout: " + std::to_string(pending) +
                   " operations still pending after " + std::to_string(timeoutMs) + "ms. Proceeding anyway.");
    return false;
}

}  // namespace dxrt
