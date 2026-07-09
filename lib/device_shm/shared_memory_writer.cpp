/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "shared_memory_writer.h"
#include "dxrt/common.h"

#include <algorithm>

#ifdef __linux__
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <csignal>
#include <chrono>
#elif _WIN32
#include <windows.h>
#include <chrono>
#endif

namespace dxrt {

SharedMemoryWriter::~SharedMemoryWriter()
{
    Cleanup();
}

bool SharedMemoryWriter::Initialize()
{
    if (_initialized)
    {
        return true;
    }

    _lastError.clear();

#ifdef __linux__
    const char* shm_name = GetMonitorShmName();

    // Open-or-create shared memory (reader-first/writer-first both supported).
    _shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, MONITOR_SHM_PERMS);
    if (_shm_fd != -1)
    {
        // Best effort: enforce permissions regardless of process umask.
        // Failure is non-fatal (e.g. we don't own the SHM), but log for diagnosis
        // since it can lead to reader EACCES later.
        if (fchmod(_shm_fd, MONITOR_SHM_PERMS) == -1)
        {
            LOG_DXRT_DBG << "fchmod on shared memory failed: " << shm_name
                         << " (" << strerror(errno) << ")";
        }
    }
    if (_shm_fd == -1)
    {
        _lastError = "shm_open failed";
        LOG_DXRT_ERR("Failed to open/create shared memory: " << shm_name
                     << " (" << strerror(errno) << ")");
        return false;
    }

    // Enforce single-writer policy across processes.
    // Keep this fd open for the writer lifetime so the lock remains held.
    struct flock writer_lock{};
    writer_lock.l_type = F_WRLCK;
    writer_lock.l_whence = SEEK_SET;
    writer_lock.l_start = 0;
    writer_lock.l_len = 0;  // lock entire object
    if (fcntl(_shm_fd, F_SETLK, &writer_lock) == -1)
    {
        if (errno == EACCES || errno == EAGAIN)
        {
            _lastError = "single-writer lock is already held";
            LOG_DXRT_ERR("Another monitor writer is already active for shared memory: "
                         << shm_name);
        }
        else
        {
            _lastError = "fcntl writer lock failed";
            LOG_DXRT_ERR("Failed to acquire writer lock for shared memory: "
                         << shm_name << " (" << strerror(errno) << ")");
        }
        close(_shm_fd);
        _shm_fd = -1;
        return false;
    }

    // Set size (always set to ensure correct size)
    if (ftruncate(_shm_fd, sizeof(MonitorSharedMemory)) == -1)
    {
        _lastError = "ftruncate failed";
        LOG_DXRT_ERR("Failed to set shared memory size");
        close(_shm_fd);
        _shm_fd = -1;
        return false;
    }

    // Map to memory
    _shm_ptr = static_cast<MonitorSharedMemory*>(
        mmap(nullptr, sizeof(MonitorSharedMemory),
             PROT_READ | PROT_WRITE, MAP_SHARED, _shm_fd, 0));

    if (_shm_ptr == MAP_FAILED)
    {
        _lastError = "mmap failed";
        LOG_DXRT_ERR("Failed to map shared memory");
        close(_shm_fd);
        _shm_fd = -1;
        _shm_ptr = nullptr;
        return false;
    }

    // Initialize or reuse existing shared memory
    if (_shm_ptr->magic != MONITOR_SHM_MAGIC || _shm_ptr->version != MONITOR_SHM_VERSION)
    {
        LOG_DXRT_DBG << "Initializing new shared memory";
        new (_shm_ptr) MonitorSharedMemory();
    }
    else if (_shm_ptr->writer_pid != 0 && kill(static_cast<pid_t>(_shm_ptr->writer_pid), 0) != 0 && errno != EPERM)
    {
        // Previous writer process is dead (stale SHM from crash/SIGKILL).
        // Reset device data and take ownership.
        LOG_DXRT_DBG << "Previous writer (pid=" << _shm_ptr->writer_pid
                     << ") is dead, reclaiming shared memory";
        new (_shm_ptr) MonitorSharedMemory();
    }
    else
    {
        LOG_DXRT_DBG << "Reusing existing shared memory";
    }

    _shm_ptr->writer_pid = getpid();

    _initialized = true;
    LOG_DXRT_DBG << "Shared memory writer initialized: " << shm_name;
    return true;

#elif _WIN32
    // Env override bypasses Global/Local namespace selection entirely.
    const char* env_override = std::getenv("DXRT_MONITOR_SHM_NAME");
    const bool use_env = (env_override != nullptr && env_override[0] != '\0');

    bool already_existed = false;
    if (use_env)
    {
        _win_shm_name = env_override;
        _shm_handle = CreateFileMappingA(
            INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE,
            0, static_cast<DWORD>(sizeof(MonitorSharedMemory)),
            _win_shm_name.c_str());
        if (_shm_handle != nullptr)
            already_existed = (::GetLastError() == ERROR_ALREADY_EXISTS);
    }
    else
    {
        // Try Global\ first — granted to services/SYSTEM/admin (SeCreateGlobalObjects).
        // On ERROR_ACCESS_DENIED (standard user process), fall back to Local\.
        _shm_handle = CreateFileMappingA(
            INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE,
            0, static_cast<DWORD>(sizeof(MonitorSharedMemory)),
            MONITOR_SHM_NAME_WIN_GLOBAL);
        if (_shm_handle != nullptr)
        {
            already_existed = (::GetLastError() == ERROR_ALREADY_EXISTS);
            _win_shm_name = MONITOR_SHM_NAME_WIN_GLOBAL;
        }
        else
        {
            LOG_DXRT_DBG << "Global\\ shared memory failed; falling back to Local\\" << std::endl;
            _shm_handle = CreateFileMappingA(
                INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE,
                0, static_cast<DWORD>(sizeof(MonitorSharedMemory)),
                MONITOR_SHM_NAME_WIN_LOCAL);
            if (_shm_handle != nullptr)
            {
                already_existed = (::GetLastError() == ERROR_ALREADY_EXISTS);
                _win_shm_name = MONITOR_SHM_NAME_WIN_LOCAL;
            }
        }
    }

    if (_shm_handle == nullptr)
    {
        LOG_DXRT_ERR("Failed to create shared memory: " << _win_shm_name
                     << " error=" << ::GetLastError());
        return false;
    }

    _shm_ptr = static_cast<MonitorSharedMemory*>(
        MapViewOfFile(
            static_cast<HANDLE>(_shm_handle),
            FILE_MAP_ALL_ACCESS,
            0, 0,
            sizeof(MonitorSharedMemory)
        )
    );

    if (_shm_ptr == nullptr)
    {
        LOG_DXRT_ERR("Failed to map view of shared memory, error=" << ::GetLastError());
        CloseHandle(static_cast<HANDLE>(_shm_handle));
        _shm_handle = nullptr;
        return false;
    }

    // 새로 생성되었거나 magic/version이 유효하지 않으면(손상/비호환) 초기화
    if (!already_existed || _shm_ptr->magic != MONITOR_SHM_MAGIC || _shm_ptr->version != MONITOR_SHM_VERSION)
    {
        LOG_DXRT << "Initializing new shared memory";
        new (_shm_ptr) MonitorSharedMemory();
    }
    else
    {
        LOG_DXRT << "Reusing existing shared memory";
    }

    _shm_ptr->writer_pid = static_cast<uint32_t>(GetCurrentProcessId());

    _initialized = true;
    LOG_DXRT << "Shared memory writer initialized: " << _win_shm_name;
    return true;

#else
    return false;
#endif
}

void SharedMemoryWriter::Cleanup()
{
    if (!_initialized)
    {
        return;
    }

#ifdef __linux__
    const char* shm_name = GetMonitorShmName();

    if (_shm_ptr != nullptr && _shm_ptr != MAP_FAILED)
    {
        // Use sequence lock to safely reset all device data
        BeginWrite();

        // Reset all device data before cleanup
        for (size_t i = 0; i < _shm_ptr->device_count; ++i)
        {
            auto* device = &_shm_ptr->devices[i];
            device->is_active = false;

            // Reset utilization to 0
            device->utilization[0] = 0.0;
            device->utilization[1] = 0.0;
            device->utilization[2] = 0.0;

            // Reset memory stats (keep total, reset used to 0)
            device->memory_used = 0;
            device->memory_free = device->memory_total;

            device->spec = dxrt_device_info_t{};
            device->dev_info = dxrt_dev_info_t{};
            device->status = dxrt_device_status_t{};
        }

        _shm_ptr->writer_pid = 0;  // Signal that writer is no longer active

        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        _shm_ptr->last_update_timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();

        EndWrite();

        munmap(_shm_ptr, sizeof(MonitorSharedMemory));
        _shm_ptr = nullptr;
    }

    if (_shm_fd != -1)
    {
        close(_shm_fd);
        _shm_fd = -1;
    }

    // Keep SHM object persistent so reader/writer can attach independently.
    // A future writer will reopen and reuse/reinitialize this segment as needed.
    LOG_DXRT_DBG << "Shared memory writer cleaned up: " << shm_name;
#elif _WIN32
    if (_shm_ptr != nullptr)
    {
        BeginWrite();

        for (size_t i = 0; i < _shm_ptr->device_count; ++i)
        {
            auto* device = &_shm_ptr->devices[i];
            device->is_active = false;
            device->utilization[0] = 0.0;
            device->utilization[1] = 0.0;
            device->utilization[2] = 0.0;
            device->memory_used = 0;
            device->memory_free = device->memory_total;
            device->spec = dxrt_device_info_t{};
            device->dev_info = dxrt_dev_info_t{};
            device->status = dxrt_device_status_t{};
        }

        _shm_ptr->writer_pid = 0;

        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        _shm_ptr->last_update_timestamp =
            std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();

        EndWrite();

        UnmapViewOfFile(_shm_ptr);
        _shm_ptr = nullptr;
    }

    if (_shm_handle != nullptr)
    {
        CloseHandle(static_cast<HANDLE>(_shm_handle));
        _shm_handle = nullptr;
    }
    // Windows: 모든 핸들/뷰 해제 시 자동 소멸 (shm_unlink 불필요)
    LOG_DXRT_DBG << "Shared memory writer cleaned up: " << _win_shm_name;
#endif

    _initialized = false;
}

void SharedMemoryWriter::UpdateTimestamp()
{
    if (!_initialized || _shm_ptr == nullptr)
    {
        return;
    }

    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    _shm_ptr->last_update_timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
    _shm_ptr->update_count++;
}

MonitorDeviceData* SharedMemoryWriter::GetDeviceData(int deviceId)
{
    if (!_initialized || _shm_ptr == nullptr)
    {
        return nullptr;
    }

    // Find existing device
    for (size_t i = 0; i < _shm_ptr->device_count; ++i)
    {
        if (_shm_ptr->devices[i].device_id == static_cast<uint32_t>(deviceId))
        {
            return &_shm_ptr->devices[i];
        }
    }

    // Add new device if space available
    if (_shm_ptr->device_count < MAX_MONITOR_DEVICES)
    {
        auto* newDevice = &_shm_ptr->devices[_shm_ptr->device_count];
        newDevice->device_id = static_cast<uint32_t>(deviceId);
        newDevice->is_active = true;
        _shm_ptr->device_count++;
        return newDevice;
    }

    return nullptr;
}

void SharedMemoryWriter::BeginWrite()
{
    if (!_initialized || _shm_ptr == nullptr)
    {
        return;
    }

    // Increment sequence to make it odd (signals "update in progress")
    // fetch_add returns the previous value
    auto prev = _shm_ptr->sequence.fetch_add(1, std::memory_order_release);

    // Verify that write is not already in progress (previous value should be even)
    if ((prev & 1) != 0)
    {
        LOG_DXRT_DBG << "BeginWrite called while write already in progress (sequence=" << prev << ")" ;
    }
}

void SharedMemoryWriter::EndWrite()
{
    if (!_initialized || _shm_ptr == nullptr)
    {
        return;
    }

    // Increment sequence to make it even (signals "update complete")
    // fetch_add returns the previous value
    auto prev = _shm_ptr->sequence.fetch_add(1, std::memory_order_release);

    // Verify that write was in progress (previous value should be odd)
    if ((prev & 1) == 0)
    {
        LOG_DXRT_DBG << "EndWrite called without corresponding BeginWrite (sequence=" << prev << ")" ;
    }
}

void SharedMemoryWriter::UpdateDeviceUtilization(int deviceId, const std::array<double, 3>& utilization)
{
    auto* device = GetDeviceData(deviceId);
    if (device == nullptr)
    {
        return;
    }

    BeginWrite();
    std::copy(utilization.begin(), utilization.end(), device->utilization.begin());
    UpdateTimestamp();
    EndWrite();
}

void SharedMemoryWriter::UpdateDeviceMemory(int deviceId, uint64_t total, uint64_t used, uint64_t free)
{
    auto* device = GetDeviceData(deviceId);
    if (device == nullptr)
    {
        return;
    }
    BeginWrite();
    device->memory_total = total;
    device->memory_used = used;
    device->memory_free = free;
    UpdateTimestamp();
    EndWrite();
}

void SharedMemoryWriter::UpdateDeviceSpec(int deviceId, const dxrt_device_info_t& spec, const dxrt_dev_info_t& devInfo)
{
    auto* device = GetDeviceData(deviceId);
    if (device == nullptr)
    {
        return;
    }
    BeginWrite();
    device->spec = spec;
    device->dev_info = devInfo;
    UpdateTimestamp();
    EndWrite();
}

void SharedMemoryWriter::UpdateDeviceFullStatus(int deviceId, const dxrt_device_status_t& deviceStatus)
{
    auto* device = GetDeviceData(deviceId);
    if (device == nullptr)
    {
        return;
    }
    BeginWrite();
    device->status = deviceStatus;
    UpdateTimestamp();
    EndWrite();
}

void SharedMemoryWriter::UpdateDeviceCoreStats(int deviceId, const std::array<uint32_t, 3>& voltage, const std::array<uint32_t, 3>& clock, const std::array<uint32_t, 3>& temperature)
{
    auto* device = GetDeviceData(deviceId);
    if (device == nullptr)
    {
        return;
    }
    BeginWrite();
    std::copy(voltage.begin(), voltage.end(), device->status.voltage);
    std::copy(clock.begin(), clock.end(), device->status.clock);
    std::copy(temperature.begin(), temperature.end(), device->status.temperature);
    device->status.voltage[3] = 0;
    device->status.clock[3] = 0;
    device->status.temperature[3] = 0;
    UpdateTimestamp();
    EndWrite();
}

void SharedMemoryWriter::SetDeviceActive(int deviceId, bool active)
{
    auto* device = GetDeviceData(deviceId);
    if (device == nullptr)
    {
        return;
    }

    BeginWrite();
    device->is_active = active;
    UpdateTimestamp();
    EndWrite();
}

void SharedMemoryWriter::IncrementInferenceCount(int deviceId)
{
    auto* device = GetDeviceData(deviceId);
    if (device == nullptr)
    {
        return;
    }

    BeginWrite();
    device->inference_count++;
    UpdateTimestamp();
    EndWrite();
}

} // namespace dxrt
