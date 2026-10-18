/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once
#ifdef _WIN32

#include <condition_variable>
#include <memory>
#include <mutex>
#include <vector>
#include "driver_adapter.h"
#include <cfgmgr32.h>

namespace dxrt {

class WindowsDriverAdapter : public DriverAdapter {
 public:
    explicit WindowsDriverAdapter(const char* fileName);
    int32_t IOControl(dxrt_cmd_t request, void* data, uint32_t size = 0, uint32_t sub_cmd = 0) override;
    int32_t Write(const void* buffer, uint32_t size) override;
    int32_t Read(void* buffer, uint32_t size) override;
    void* MemoryMap(void *__addr, size_t __len, off_t __offset = 0) override;
    int32_t Poll() override;
    int GetFd() const override;

    ~WindowsDriverAdapter() override;
    std::string GetName() const override { return _name;  }
    void Close() override;

    // Rejects new requests with ERROR_DEVICE_REMOVED, cancels pending ones and closes the
    // handle once they finish. Returns false (handle kept open) if they do not finish in time.
    bool Quiesce(uint32_t timeoutMs);
    // Accepts requests again after a Quiesce that timed out.
    void Resume();
    // Opens the device path again after a completed Quiesce or Close. Not for use inside the removal handler.
    bool Reopen();
    void SetRemovalHandler(DeviceRemovalHandler handler);

    static constexpr uint32_t kRemovalDrainTimeoutMs = 2000;
    static constexpr uint32_t kRemovalQuiesceTimeoutMs = 1000;

 protected:
    // Test seam: issues one request on the given handle.
    virtual BOOL startIo(HANDLE fd, dxrt_message_t* msg, DWORD* bytesReturned, OVERLAPPED* overlapped);
    // Handles one CM_NOTIFY_ACTION for the device handle; returns the callback result.
    DWORD onDeviceNotify(int action);

 private:
     enum class GateState { NotOpen, Open, Closing, Closed };
     struct NotifyContext;

     HANDLE AcquireEvent();
     void ReleaseEvent(HANDLE hEvent);
     int32_t enterIo(HANDLE* fd);
     void leaveIo();
     bool waitForIdle(std::unique_lock<std::mutex>& lock, uint32_t timeoutMs);
     bool reopenHandle();
     void registerNotification();
     void releaseNotification();
     static void releaseContext(const std::shared_ptr<NotifyContext>& ctx);
     static DWORD CALLBACK notifyCallback(HCMNOTIFICATION notify, PVOID context, CM_NOTIFY_ACTION action,
         PCM_NOTIFY_EVENT_DATA eventData, DWORD eventDataSize);

     HANDLE _fd = INVALID_HANDLE_VALUE;
     std::string _name;

     std::mutex _eventPoolMutex;
     std::vector<HANDLE> _eventPool;

     mutable std::mutex _gateMutex;
     std::condition_variable _idleCv;
     GateState _state = GateState::NotOpen;
     std::vector<DWORD> _ioThreads;  // one entry per in-flight request
     HANDLE _stopEvent = NULL;

     // Lock order: NotifyContext::lock -> _removalMutex -> handler locks -> _gateMutex.
     std::mutex _removalMutex;
     DeviceRemovalHandler _removalHandler;
     std::shared_ptr<NotifyContext> _notify;
};

}  // namespace dxrt

#endif // _WIN32
