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
#include <iostream>

namespace dxrt {

WindowsDriverAdapter::WindowsDriverAdapter(const char* fileName)
{
    _fd = CreateFile(fileName,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED,
	NULL);
#if 0
    if (_fd == INVALID_HANDLE_VALUE)	{
        cout << "Error: Can't open " << _file << endl;
        return;
    }
#endif
}

int32_t WindowsDriverAdapter::IOControl(dxrt_cmd_t request, void* data, uint32_t size , uint32_t sub_cmd)
{
    OVERLAPPED overlappedSend1 = {};
    overlappedSend1.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    int ret = 0;
    dxrt_message_t msg;
    msg.cmd = static_cast<int32_t>(request);
    msg.sub_cmd = static_cast<int32_t>(sub_cmd),
    msg.data = data;
    msg.size = size;

    DWORD bytesReturned;
    BOOL success = DeviceIoControl(
        _fd,
        static_cast<DWORD>(dxrt::dxrt_ioctl_t::DXRT_IOCTL_MESSAGE),
        &msg,
        sizeof(msg),
        NULL,
        0,
        &bytesReturned,
        &overlappedSend1);
    if ( !success )
    {
        if (GetLastError() == ERROR_IO_PENDING)
        {
#if 0
            for (int i = 0; i < 10000; i++) {
                DWORD e = WaitForSingleObject(overlappedSend1.hEvent, 6);  // INFINITE
                if (e == WAIT_OBJECT_0)
                    break;
                if (e == WAIT_TIMEOUT)
                    continue;
                break;  // error
            }
            GetOverlappedResult(_fd, &overlappedSend1, &bytesReturned, FALSE);
            // CloseHandle(_fd.hEvent);
            success = true;
#endif
            DWORD e = WaitForSingleObject(overlappedSend1.hEvent, INFINITE);
            if (e == WAIT_OBJECT_0) {
                GetOverlappedResult(_fd, &overlappedSend1, &bytesReturned, FALSE);
                success = true;
            }
            if (e == WAIT_TIMEOUT)
                return -1;
        }
        else
        {
            LOG_DXRT_ERR("e:" << GetLastError());
        }
    }
    CloseHandle(overlappedSend1.hEvent);
    switch (request)
    {
    case dxrt_cmd_t::DXRT_CMD_UPDATE_FIRMWARE:
        if (!success) {
            ret = bytesReturned;
        }
        else
        {
            ret = 0;
        }
        break;
    default:
        if (!success) {
            // PrintLastErrorString();
            ret = GetLastError();
            std::cout << "GetLastError() = " << ret  << std::endl;
        }
        else
        {
            ret = 0;
        }
        break;
    }
    return ret;
}

int32_t WindowsDriverAdapter::Write(const void* buffer, uint32_t size)
{
    // int ret = write(_fd, buffer, size);
    // if (ret < 0) return ret;
    // return 0;
    int ret;
    DWORD bytesWritten;
    BOOL success = WriteFile(_fd, buffer, size, &bytesWritten, NULL);
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
    BOOL success = ReadFile(_fd, buffer, size, &bytesRead, NULL);
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
    return 0 ;
#endif
}

WindowsDriverAdapter::~WindowsDriverAdapter()
{
    CloseHandle(_fd);
}


}  // namespace dxrt

#endif // _WIN32
