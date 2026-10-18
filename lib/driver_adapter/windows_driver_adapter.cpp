/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#ifdef _WIN32 // all or nothing

#include "dxrt/common.h"
#include "dxrt/driver.h"
#include "dxrt/device_struct.h"
#include "dxrt/driver_adapter/windows_driver_adapter.h"
#include <algorithm>
#include <chrono>
#include <iostream>

namespace dxrt {

namespace {

constexpr uint32_t kCloseDrainTimeoutMs = 3000;
constexpr uint32_t kLivenessPollMs = 50;

HANDLE openDevice(const std::string& fileName)
{
    return CreateFile(fileName.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED,
        NULL);
}

// Threads terminated at process exit never leave the gate; do not wait for them.
bool isThreadAlive(DWORD threadId)
{
    HANDLE thread = OpenThread(SYNCHRONIZE, FALSE, threadId);
    if (thread == NULL) {
        return false;
    }
    const bool alive = (WaitForSingleObject(thread, 0) == WAIT_TIMEOUT);
    CloseHandle(thread);
    return alive;
}

}  // namespace

struct WindowsDriverAdapter::NotifyContext : std::enable_shared_from_this<NotifyContext> {
    std::mutex lock;
    WindowsDriverAdapter* adapter = nullptr;
    HCMNOTIFICATION handle = nullptr;
    bool released = false;
};

WindowsDriverAdapter::WindowsDriverAdapter(const char* fileName)
: _name(fileName)
{
    _stopEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    if (_stopEvent == NULL) {
        LOG_DXRT_ERR("CreateEvent failed. GLE=" << GetLastError() << ", device: " << _name);
        return;
    }
    _fd = openDevice(_name);
    if (_fd != INVALID_HANDLE_VALUE) {
        _state = GateState::Open;
        registerNotification();
    }
}

size_t g_eventPoolAllocCount = 0;
size_t g_eventFullCount = 0;

HANDLE WindowsDriverAdapter::AcquireEvent()
{
    std::lock_guard<std::mutex> lock(_eventPoolMutex);

    size_t pool_size = _eventPool.size();

    if (pool_size > g_eventPoolAllocCount)
    {
        g_eventPoolAllocCount = pool_size;
        //LOG_DXRT_INFO("WindowsDriverAdapter::AcquireEvent() -> _eventPool.size() = " << pool_size);
    }

    if (!_eventPool.empty()) {
        HANDLE hEvent = _eventPool.back();
        _eventPool.pop_back();
        ResetEvent(hEvent);
        return hEvent;
    }

    return CreateEvent(nullptr, TRUE, FALSE, nullptr);
}

void WindowsDriverAdapter::ReleaseEvent(HANDLE hEvent)
{
    if (hEvent == NULL) return;

    std::lock_guard<std::mutex> lock(_eventPoolMutex);

    size_t pool_size = _eventPool.size();

    if (pool_size < 64) {
        _eventPool.push_back(hEvent);
    }
    else {
        if (g_eventFullCount < 3)
        {
            LOG_DXRT_INFO("WindowsDriverAdapter::ReleaseEvent -> _eventPool.size() = " << pool_size);
        }
        else if(g_eventFullCount == 3)
        {
            LOG_DXRT_INFO("Further event pool full messages suppressed.");
		}

        g_eventFullCount++;

        CloseHandle(hEvent);
    }
}

int32_t WindowsDriverAdapter::enterIo(HANDLE* fd)
{
    std::lock_guard<std::mutex> lock(_gateMutex);
    if (_state == GateState::Open) {
        *fd = _fd;
        _ioThreads.push_back(GetCurrentThreadId());
        return ERROR_SUCCESS;
    }
    return (_state == GateState::NotOpen) ? ERROR_INVALID_HANDLE : ERROR_DEVICE_REMOVED;
}

void WindowsDriverAdapter::leaveIo()
{
    std::lock_guard<std::mutex> lock(_gateMutex);
    auto it = std::find(_ioThreads.begin(), _ioThreads.end(), GetCurrentThreadId());
    if (it != _ioThreads.end()) {
        _ioThreads.erase(it);
    }
    if (_ioThreads.empty()) {
        _idleCv.notify_all();
    }
}

bool WindowsDriverAdapter::waitForIdle(std::unique_lock<std::mutex>& lock, uint32_t timeoutMs)
{
    const auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeoutMs);
    while (_state == GateState::Closing && !_ioThreads.empty()) {
        _ioThreads.erase(std::remove_if(_ioThreads.begin(), _ioThreads.end(),
            [](DWORD threadId) { return !isThreadAlive(threadId); }), _ioThreads.end());
        if (_ioThreads.empty()) {
            break;
        }
        const auto now = std::chrono::steady_clock::now();
        if (now >= deadline) {
            return false;
        }
        _idleCv.wait_for(lock, std::min<std::chrono::steady_clock::duration>(
            deadline - now, std::chrono::milliseconds(kLivenessPollMs)));
    }
    return true;
}

BOOL WindowsDriverAdapter::startIo(HANDLE fd, dxrt_message_t* msg, DWORD* bytesReturned, OVERLAPPED* overlapped)
{
    return DeviceIoControl(
        fd,
        static_cast<DWORD>(dxrt::dxrt_ioctl_t::DXRT_IOCTL_MESSAGE),
        msg,
        sizeof(*msg),
        NULL,
        0,
        bytesReturned,
        overlapped);
}

int32_t WindowsDriverAdapter::IOControl(dxrt_cmd_t request, void* data, uint32_t size, uint32_t sub_cmd)
{
    HANDLE fd = INVALID_HANDLE_VALUE;
    const int32_t gateError = enterIo(&fd);
    if (gateError == ERROR_INVALID_HANDLE) {
        LOG_DXRT_ERR("IOControl called with invalid handle. Device: " << _name);
    }
    if (gateError != ERROR_SUCCESS) {
        return gateError;
    }
    struct IoScope {
        WindowsDriverAdapter* self;
        ~IoScope() { self->leaveIo(); }
    } ioScope{this};

    HANDLE hEvent = AcquireEvent();
    if (hEvent == NULL) {
        LOG_DXRT_ERR("AcquireEvent failed. GLE=" << GetLastError());
        return -1;
    }

    OVERLAPPED overlapped = {};
    overlapped.hEvent = hEvent;

    int ret = 0;
    dxrt_message_t msg;
    msg.cmd = static_cast<int32_t>(request);
    msg.sub_cmd = static_cast<int32_t>(sub_cmd);
    msg.data = data;
    msg.size = size;

    DWORD bytesReturned = 0;
    BOOL success = startIo(fd, &msg, &bytesReturned, &overlapped);

    DWORD lastError = GetLastError();
    const bool pended = (!success && lastError == ERROR_IO_PENDING);

    if (pended)
    {
        const HANDLE waits[2] = { hEvent, _stopEvent };
        if (WaitForMultipleObjects(2, waits, FALSE, INFINITE) != WAIT_OBJECT_0) {
            CancelIoEx(fd, &overlapped);
        }
        // The driver owns overlapped and the caller's buffers until the request completes.
        success = GetOverlappedResult(fd, &overlapped, &bytesReturned, TRUE);
        lastError = success ? ERROR_SUCCESS : GetLastError();
    }

    if (!success && (lastError == ERROR_OPERATION_ABORTED || lastError == ERROR_DEVICE_REMOVED))
    {
        ReleaseEvent(hEvent);
        return ERROR_DEVICE_REMOVED;
    }

    // Failures after pending were silent before their result was checked; callers handle the code.
    if (!success && !pended)
    {
        LOG_DXRT_ERR("DeviceIoControl failed. GLE=" << lastError
            << ", cmd=" << static_cast<int>(request)
            << ", handle=" << reinterpret_cast<uint64_t>(fd));

        if (lastError == ERROR_INVALID_HANDLE) {
            LOG_DXRT_ERR("Handle appears to be invalid or closed. Device: " << _name);
        }
    }

    switch (request)
    {
    case dxrt_cmd_t::DXRT_CMD_UPDATE_FIRMWARE:
        ret = success ? 0 : bytesReturned;
        break;
    default:
        if (!success) {
            ret = lastError;
            if (!pended) {
                std::cout << "GetLastError() = " << ret << std::endl;
            }
        }
        else {
            ret = 0;
		}
        break;
    }

    ReleaseEvent(hEvent);
    return ret;
}

int32_t WindowsDriverAdapter::Write(const void* buffer, uint32_t size)
{
    // int ret = write(_fd, buffer, size);
    // if (ret < 0) return ret;
    // return 0;
    HANDLE fd = INVALID_HANDLE_VALUE;
    if (enterIo(&fd) != ERROR_SUCCESS) {
        return -1;
    }
    int ret;
    DWORD bytesWritten;
    BOOL success = WriteFile(fd, buffer, size, &bytesWritten, NULL);
    leaveIo();
    if (!success) {
        ret = -1;
    }
    else {
        // ret = bytesWritten;
        ret = 0;
    }
    return ret;
}

int32_t WindowsDriverAdapter::Read(void* buffer, uint32_t size)
{
    int ret;
	// int ret = read(_fd, buffer, size);
    // if (ret < 0) return ret;
    // return 0;
    DWORD bytesRead;
    HANDLE fd = INVALID_HANDLE_VALUE;
    if (enterIo(&fd) != ERROR_SUCCESS) {
        return -1;
    }
    BOOL success = ReadFile(fd, buffer, size, &bytesRead, NULL);
    leaveIo();
    if (!success || bytesRead != size)
    {
        return -1;
    }
    ret = bytesRead;
    return 0 ;
}

void* WindowsDriverAdapter::MemoryMap(void *__addr, size_t __len, off_t __offset)
{
    // void* ret = mmap(__addr, __len, PROT_READ|PROT_WRITE, MAP_SHARED, _fd, __offset);
    // return ret;
	return nullptr ;
}

#define DEVICE_POLL_LIMIT_MS 3*1000*1000

int32_t WindowsDriverAdapter::Poll()
{
    return 0 ;  // unused in windows
#if 0
	pollfd _devPollFd = {
        .fd = _fd,
        .events = POLLIN,
        // .events = POLLIN|POLLHUP,
        .revents = 0,
    };
    return poll(&_devPollFd, 1, DEVICE_POLL_LIMIT_MS);
#endif
#if 0
    DWORD waitResult = WaitForSingleObject(_devHandle, DEVICE_POLL_LIMIT_MS);
    if (waitResult == WAIT_FAILED)
    {
        cout << "Error: Device " << _id << " WaitForSingleObject fail." << endl;
        return -1;
    }
    else if (waitResult == WAIT_TIMEOUT)
    {
        // Timeout occurred, you might want to handle this case
        cout << "Warning: Device " << _id << " wait timeout." << endl;
        return -1;
    }
    else
    {
        return 0;
    }
#endif
}

WindowsDriverAdapter::~WindowsDriverAdapter()
{
    releaseNotification();
    Close();

    for (HANDLE hEvent : _eventPool) {
        CloseHandle(hEvent);
    }
    _eventPool.clear();

    if (_stopEvent != NULL) {
        CloseHandle(_stopEvent);
    }
}

int WindowsDriverAdapter::GetFd() const
{
    std::lock_guard<std::mutex> lock(_gateMutex);
    return reinterpret_cast<uint64_t>(_fd);
}

bool WindowsDriverAdapter::Quiesce(uint32_t timeoutMs)
{
    std::unique_lock<std::mutex> lock(_gateMutex);
    if (_state != GateState::Open && _state != GateState::Closing) {
        return true;
    }
    _state = GateState::Closing;
    SetEvent(_stopEvent);
    CancelIoEx(_fd, NULL);
    const bool idle = waitForIdle(lock, timeoutMs);
    if (_state == GateState::Closed) {
        return true;
    }
    if (!idle || _state != GateState::Closing) {
        return false;
    }
    CloseHandle(_fd);
    _fd = INVALID_HANDLE_VALUE;
    _state = GateState::Closed;
    return true;
}

void WindowsDriverAdapter::Resume()
{
    std::lock_guard<std::mutex> lock(_gateMutex);
    if (_state == GateState::Closing) {
        ResetEvent(_stopEvent);
        _state = GateState::Open;
    }
}

bool WindowsDriverAdapter::Reopen()
{
    if (!reopenHandle()) {
        return false;
    }
    registerNotification();
    return true;
}

bool WindowsDriverAdapter::reopenHandle()
{
    std::lock_guard<std::mutex> lock(_gateMutex);
    if (_state == GateState::Open) {
        return true;
    }
    if (_state == GateState::Closing || _stopEvent == NULL) {
        return false;
    }
    HANDLE fd = openDevice(_name);
    if (fd == INVALID_HANDLE_VALUE) {
        return false;
    }
    _fd = fd;
    ResetEvent(_stopEvent);
    _state = GateState::Open;
    return true;
}

void WindowsDriverAdapter::Close()
{
    if (Quiesce(kCloseDrainTimeoutMs)) {
        return;
    }
    LOG_DXRT_ERR("Closing " << _name << " with requests still in flight");
    std::lock_guard<std::mutex> lock(_gateMutex);
    if (_fd != INVALID_HANDLE_VALUE) {
        CloseHandle(_fd);
    }
    _fd = INVALID_HANDLE_VALUE;
    _state = GateState::Closed;
}

void WindowsDriverAdapter::SetRemovalHandler(DeviceRemovalHandler handler)
{
    std::lock_guard<std::mutex> lock(_removalMutex);
    _removalHandler = std::move(handler);
}

void WindowsDriverAdapter::registerNotification()
{
    {
        std::lock_guard<std::mutex> lock(_removalMutex);
        if (_notify) {
            return;
        }
    }
    HANDLE fd = INVALID_HANDLE_VALUE;
    {
        std::lock_guard<std::mutex> gate(_gateMutex);
        fd = _fd;
    }
    if (fd == INVALID_HANDLE_VALUE) {
        return;
    }
    auto ctx = std::make_shared<NotifyContext>();
    ctx->adapter = this;
    CM_NOTIFY_FILTER filter = {};
    filter.cbSize = sizeof(filter);
    filter.FilterType = CM_NOTIFY_FILTER_TYPE_DEVICEHANDLE;
    filter.u.DeviceHandle.hTarget = fd;
    {
        std::lock_guard<std::mutex> ctxLock(ctx->lock);
        const CONFIGRET cr = CM_Register_Notification(&filter, ctx.get(), notifyCallback, &ctx->handle);
        if (cr != CR_SUCCESS) {
            LOG_DXRT_DBG << _name << ": CM_Register_Notification failed, cr=" << cr << std::endl;
            return;
        }
    }
    {
        std::lock_guard<std::mutex> lock(_removalMutex);
        if (!_notify) {
            _notify = ctx;
            return;
        }
    }
    std::lock_guard<std::mutex> ctxLock(ctx->lock);
    releaseContext(ctx);
}

void WindowsDriverAdapter::releaseNotification()
{
    std::shared_ptr<NotifyContext> ctx;
    {
        std::lock_guard<std::mutex> lock(_removalMutex);
        ctx = std::move(_notify);
    }
    if (!ctx) {
        return;
    }
    // Destruction may run during process exit, where the thread pool cannot take work. Detach the
    // callback and keep the registration and its context alive instead of unregistering.
    std::lock_guard<std::mutex> lock(ctx->lock);
    if (!ctx->released) {
        ctx->released = true;
        ctx->adapter = nullptr;
        new std::shared_ptr<NotifyContext>(ctx);  // NOSONAR: intentional leak, see above
    }
}

// Caller holds ctx->lock. CM_Unregister_Notification waits for running callbacks, so it runs on a pool thread.
void WindowsDriverAdapter::releaseContext(const std::shared_ptr<NotifyContext>& ctx)
{
    if (ctx->released) {
        return;
    }
    ctx->released = true;
    ctx->adapter = nullptr;
    auto* keep = new std::shared_ptr<NotifyContext>(ctx);
    const BOOL submitted = TrySubmitThreadpoolCallback(
        [](PTP_CALLBACK_INSTANCE, PVOID context) {
            auto* owned = static_cast<std::shared_ptr<NotifyContext>*>(context);
            CM_Unregister_Notification((*owned)->handle);
            delete owned;
        },
        keep, nullptr);
    if (!submitted) {
        // The registration may still call back into ctx, so it is kept alive.
        LOG_DXRT_ERR("TrySubmitThreadpoolCallback failed. GLE=" << GetLastError());
    }
}

DWORD CALLBACK WindowsDriverAdapter::notifyCallback(HCMNOTIFICATION notify, PVOID context, CM_NOTIFY_ACTION action,
    PCM_NOTIFY_EVENT_DATA eventData, DWORD eventDataSize)
{
    (void)notify;
    (void)eventData;
    (void)eventDataSize;
    auto* raw = static_cast<NotifyContext*>(context);
    std::lock_guard<std::mutex> lock(raw->lock);
    if (raw->released || raw->adapter == nullptr) {
        return ERROR_SUCCESS;
    }
    std::shared_ptr<NotifyContext> self = raw->shared_from_this();
    WindowsDriverAdapter* adapter = raw->adapter;
    const DWORD result = adapter->onDeviceNotify(action);
    if (action == CM_NOTIFY_ACTION_DEVICEREMOVEPENDING || action == CM_NOTIFY_ACTION_DEVICEREMOVECOMPLETE) {
        releaseContext(self);
    }
    return result;
}

DWORD WindowsDriverAdapter::onDeviceNotify(int action)
{
    std::lock_guard<std::mutex> lock(_removalMutex);
    switch (action)
    {
    case CM_NOTIFY_ACTION_DEVICEQUERYREMOVE:
        if (_removalHandler && !_removalHandler(DeviceRemovalEvent::QueryRemove, kRemovalDrainTimeoutMs)) {
            LOG_DXRT_INFO(_name << ": removal refused, submitted jobs did not finish");
            return ERROR_CANCELLED;
        }
        if (!Quiesce(kRemovalQuiesceTimeoutMs)) {
            Resume();
            if (_removalHandler) {
                _removalHandler(DeviceRemovalEvent::RemoveCancelled, 0);
            }
            LOG_DXRT_INFO(_name << ": removal refused, device requests did not finish");
            return ERROR_CANCELLED;
        }
        LOG_DXRT_INFO(_name << ": handle closed for device removal");
        return ERROR_SUCCESS;

    case CM_NOTIFY_ACTION_DEVICEQUERYREMOVEFAILED:
        if (!reopenHandle()) {
            LOG_DXRT_ERR(_name << ": reopen after cancelled removal failed. GLE=" << GetLastError());
            return ERROR_SUCCESS;
        }
        if (_removalHandler) {
            _removalHandler(DeviceRemovalEvent::RemoveCancelled, 0);
        }
        return ERROR_SUCCESS;

    case CM_NOTIFY_ACTION_DEVICEREMOVEPENDING:
    case CM_NOTIFY_ACTION_DEVICEREMOVECOMPLETE:
        Close();
        // The callback releases this registration; a later Reopen registers a new one.
        _notify.reset();
        if (_removalHandler) {
            _removalHandler(DeviceRemovalEvent::Removed, 0);
        }
        return ERROR_SUCCESS;

    default:
        return ERROR_SUCCESS;
    }
}


}  // namespace dxrt

#endif // _WIN32
