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

#ifdef __linux__
    // Create or open shared memory
    _shm_fd = shm_open(MONITOR_SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (_shm_fd == -1) 
    {
        LOG_DXRT_ERR("Failed to create shared memory: " << MONITOR_SHM_NAME);
        return false;
    }

    // Set size (always set to ensure correct size)
    if (ftruncate(_shm_fd, sizeof(MonitorSharedMemory)) == -1) 
    {
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
        LOG_DXRT_ERR("Failed to map shared memory");
        close(_shm_fd);
        _shm_fd = -1;
        _shm_ptr = nullptr;
        return false;
    }

    // Initialize structure only if magic number is invalid (first time or corrupted)
    if (_shm_ptr->magic != MONITOR_SHM_MAGIC) 
    {
        LOG_DXRT_DBG << "Initializing new shared memory";
        new (_shm_ptr) MonitorSharedMemory();
    } 
    else 
    {
        LOG_DXRT_DBG << "Reusing existing shared memory";
    }
    
    _shm_ptr->writer_pid = getpid();
    
    _initialized = true;
    LOG_DXRT_DBG << "Shared memory writer initialized: " << MONITOR_SHM_NAME;
    return true;

#elif _WIN32
    _shm_handle = CreateFileMappingA(
        INVALID_HANDLE_VALUE,
        nullptr,
        PAGE_READWRITE,
        0,
        static_cast<DWORD>(sizeof(MonitorSharedMemory)),
        MONITOR_SHM_NAME
    );

    if (_shm_handle == nullptr)
    {
        LOG_DXRT_ERR("Failed to create shared memory: " << MONITOR_SHM_NAME
                     << " error=" << GetLastError());
        return false;
    }

    // GetLastError는 다음 Win32 호출이 덮어쓰기 전에 즉시 캡처
    bool already_existed = (GetLastError() == ERROR_ALREADY_EXISTS);

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
        LOG_DXRT_ERR("Failed to map view of shared memory, error=" << GetLastError());
        CloseHandle(static_cast<HANDLE>(_shm_handle));
        _shm_handle = nullptr;
        return false;
    }

    // 새로 생성되었거나 magic이 유효하지 않으면(손상) 초기화
    if (!already_existed || _shm_ptr->magic != MONITOR_SHM_MAGIC)
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
    LOG_DXRT << "Shared memory writer initialized: " << MONITOR_SHM_NAME;
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
            
            // Reset core stats to 0
            for (size_t j = 0; j < 3; ++j) 
            {
                device->voltage[j] = 0;
                device->clock[j] = 0;
                device->temperature[j] = 0;
            }
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

    // Unlink shared memory to clean up
    // Note: Processes that already have it mapped can continue using it
    // The memory will be freed when the last process unmaps it
    shm_unlink(MONITOR_SHM_NAME);
    LOG_DXRT_DBG << "Shared memory writer cleaned up and unlinked: " << MONITOR_SHM_NAME;
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
            for (size_t j = 0; j < 3; ++j)
            {
                device->voltage[j] = 0;
                device->clock[j] = 0;
                device->temperature[j] = 0;
            }
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
    LOG_DXRT_DBG << "Shared memory writer cleaned up: " << MONITOR_SHM_NAME;
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

void SharedMemoryWriter::UpdateDeviceCoreStats(int deviceId, const std::array<uint32_t, 3>& voltage, const std::array<uint32_t, 3>& clock, const std::array<uint32_t, 3>& temperature)
{
    auto* device = GetDeviceData(deviceId);
    if (device == nullptr) 
    {
        return;
    }
    BeginWrite();
    std::copy(voltage.begin(), voltage.end(), device->voltage.begin());
    std::copy(clock.begin(), clock.end(), device->clock.begin());
    std::copy(temperature.begin(), temperature.end(), device->temperature.begin());
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
