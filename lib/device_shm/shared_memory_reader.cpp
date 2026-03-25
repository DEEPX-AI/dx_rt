/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "shared_memory_reader.h"
#include "dxrt/common.h"
#include <cstdint>

#ifdef __linux__
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <csignal>
#elif _WIN32
#include <windows.h>
#endif

namespace dxrt {

SharedMemoryReader::SharedMemoryReader() = default;

SharedMemoryReader::~SharedMemoryReader()
{
    Close();
}

bool SharedMemoryReader::Open()
{
    if (_opened) 
    {
        return true;
    }

#ifdef __linux__
    // Try to open existing shared memory (read-only)
    _shm_fd = shm_open(MONITOR_SHM_NAME, O_RDONLY, 0);
    
    // If it doesn't exist, create it for dxtop-first scenarios
    if (_shm_fd == -1) 
    {
        LOG_DXRT_DBG << "Shared memory doesn't exist, creating placeholder for Writer..." << std::endl;
        
        // Create with read-write to initialize
        _shm_fd = shm_open(MONITOR_SHM_NAME, O_CREAT | O_RDWR, 0666);
        if (_shm_fd == -1) 
        {
            LOG_DXRT_ERR("Failed to create shared memory");
            return false;
        }
        
        // Set size
        if (ftruncate(_shm_fd, sizeof(MonitorSharedMemory)) == -1) 
        {
            LOG_DXRT_ERR("Failed to set shared memory size");
            close(_shm_fd);
            _shm_fd = -1;
            return false;
        }
        
        // Map with read-write to initialize
        auto* init_ptr = static_cast<MonitorSharedMemory*>(
            mmap(nullptr, sizeof(MonitorSharedMemory), 
                 PROT_READ | PROT_WRITE, MAP_SHARED, _shm_fd, 0));
        
        if (init_ptr == MAP_FAILED) 
        {
            LOG_DXRT_ERR("Failed to map shared memory for initialization");
            close(_shm_fd);
            _shm_fd = -1;
            return false;
        }
        
        // Initialize the structure (writer_pid = 0 means no Writer yet)
        new (init_ptr) MonitorSharedMemory();
        
        // Unmap and reopen as read-only
        munmap(init_ptr, sizeof(MonitorSharedMemory));
        close(_shm_fd);
        
        // Reopen as read-only
        _shm_fd = shm_open(MONITOR_SHM_NAME, O_RDONLY, 0);
        if (_shm_fd == -1) 
        {
            LOG_DXRT_ERR("Failed to reopen shared memory as read-only");
            return false;
        }
    }

    // Map to memory (read-only)
    _shm_ptr = mmap(nullptr, sizeof(MonitorSharedMemory), 
                    PROT_READ, MAP_SHARED, _shm_fd, 0);
    
    if (_shm_ptr == MAP_FAILED) 
    {
        LOG_DXRT_ERR("Failed to map shared memory");
        close(_shm_fd);
        _shm_fd = -1;
        _shm_ptr = nullptr;
        return false;
    }

    // Verify magic number
    auto* shm = static_cast<const MonitorSharedMemory*>(_shm_ptr);
    if (shm->magic != MONITOR_SHM_MAGIC) 
    {
        LOG_DXRT_ERR("Invalid shared memory magic number");
        munmap(_shm_ptr, sizeof(MonitorSharedMemory));
        close(_shm_fd);
        _shm_fd = -1;
        _shm_ptr = nullptr;
        return false;
    }

    _opened = true;
    return true;

#elif _WIN32
    // 기존 read-only 매핑 열기 시도
    HANDLE ro_handle = OpenFileMappingA(FILE_MAP_READ, FALSE, MONITOR_SHM_NAME);

    if (ro_handle == nullptr)
    {
        // dxtop-first 시나리오: Writer 없이 Reader가 먼저 시작된 경우
        LOG_DXRT_DBG << "Shared memory doesn't exist, creating placeholder for Writer..." << std::endl;

        HANDLE init_handle = CreateFileMappingA(
            INVALID_HANDLE_VALUE,
            nullptr,
            PAGE_READWRITE,
            0,
            static_cast<DWORD>(sizeof(MonitorSharedMemory)),
            MONITOR_SHM_NAME
        );

        if (init_handle == nullptr)
        {
            LOG_DXRT_ERR("Failed to create shared memory placeholder, error=" << GetLastError());
            return false;
        }

        auto* init_ptr = static_cast<MonitorSharedMemory*>(
            MapViewOfFile(init_handle, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(MonitorSharedMemory)));

        if (init_ptr == nullptr)
        {
            LOG_DXRT_ERR("Failed to map shared memory for initialization, error=" << GetLastError());
            CloseHandle(init_handle);
            return false;
        }

        // writer_pid = 0: 아직 Writer 없음
        new (init_ptr) MonitorSharedMemory();
        UnmapViewOfFile(init_ptr);

        // init_handle이 열린 상태에서 read-only 핸들 획득 후 init_handle 해제
        ro_handle = OpenFileMappingA(FILE_MAP_READ, FALSE, MONITOR_SHM_NAME);
        CloseHandle(init_handle);

        if (ro_handle == nullptr)
        {
            LOG_DXRT_ERR("Failed to reopen shared memory as read-only, error=" << GetLastError());
            return false;
        }
    }

    _shm_handle = ro_handle;

    _shm_ptr = MapViewOfFile(
        static_cast<HANDLE>(_shm_handle),
        FILE_MAP_READ,
        0, 0,
        sizeof(MonitorSharedMemory)
    );

    if (_shm_ptr == nullptr)
    {
        LOG_DXRT_ERR("Failed to map view of shared memory, error=" << GetLastError());
        CloseHandle(static_cast<HANDLE>(_shm_handle));
        _shm_handle = nullptr;
        return false;
    }

    // magic 검증
    auto* shm = static_cast<const MonitorSharedMemory*>(_shm_ptr);
    if (shm->magic != MONITOR_SHM_MAGIC)
    {
        LOG_DXRT_ERR("Invalid shared memory magic number");
        UnmapViewOfFile(_shm_ptr);
        CloseHandle(static_cast<HANDLE>(_shm_handle));
        _shm_handle = nullptr;
        _shm_ptr = nullptr;
        return false;
    }

    _opened = true;
    return true;

#else
    return false;
#endif
}

void SharedMemoryReader::Close()
{
    if (!_opened) 
    {
        return;
    }

#ifdef __linux__
    // Check if we need to cleanup placeholder before unmapping
    bool should_cleanup_placeholder = false;
    if (_shm_ptr != nullptr && _shm_ptr != MAP_FAILED) 
    {
        // If writer never initialized (writer_pid == 0), we should cleanup
        auto* shm = static_cast<const MonitorSharedMemory*>(_shm_ptr);
        should_cleanup_placeholder = (shm->writer_pid == 0);
        
        munmap(_shm_ptr, sizeof(MonitorSharedMemory));
        _shm_ptr = nullptr;
    }

    if (_shm_fd != -1) 
    {
        close(_shm_fd);
        _shm_fd = -1;
    }
    
    // If we created a placeholder and Writer never initialized it, clean it up
    if (should_cleanup_placeholder)
    {
        shm_unlink(MONITOR_SHM_NAME);
    }
#elif _WIN32
    if (_shm_ptr != nullptr)
    {
        UnmapViewOfFile(_shm_ptr);
        _shm_ptr = nullptr;
    }

    if (_shm_handle != nullptr)
    {
        CloseHandle(static_cast<HANDLE>(_shm_handle));
        _shm_handle = nullptr;
    }
    // Windows: 모든 핸들/뷰 해제 시 자동 소멸 (shm_unlink 불필요)
#endif

    _opened = false;
}

bool SharedMemoryReader::ReadDeviceData(int deviceId, MonitorDeviceData& outData) const
{
    if (!_opened || _shm_ptr == nullptr) 
    {
        return false;
    }

    auto* shm = static_cast<const MonitorSharedMemory*>(_shm_ptr);
    
    // Use sequence lock to ensure consistent read
    uint64_t seq1;
    uint64_t seq2;
    do {
        // Read sequence before data
        seq1 = shm->sequence.load(std::memory_order_acquire);
        
        // If sequence is odd, writer is updating - retry
        if (seq1 & 1) 
        {
            continue;
        }
        
        // Find and copy device data
        bool found = false;
        for (uint32_t i = 0; i < shm->device_count; ++i) 
        {
            if (shm->devices[i].device_id == static_cast<uint32_t>(deviceId)) 
            {
                outData = shm->devices[i];
                found = true;
                break;
            }
        }
        
        // Read sequence after data
        seq2 = shm->sequence.load(std::memory_order_acquire);
        
        // If sequences match and even, data is consistent
        if (seq1 == seq2) 
        {
            return found;
        }
        
        // Otherwise, writer updated during read - retry
    } while (true);
}

bool SharedMemoryReader::GetAllDevices(MonitorDeviceData* outDevices, uint32_t& outCount, uint32_t maxCount) const
{
    if (!_opened || _shm_ptr == nullptr || outDevices == nullptr) 
    {
        return false;
    }

    auto* shm = static_cast<const MonitorSharedMemory*>(_shm_ptr);
    
    // Use sequence lock to ensure consistent read
    uint64_t seq1;
    uint64_t seq2;
    do {
        // Read sequence before data
        seq1 = shm->sequence.load(std::memory_order_acquire);
        
        // If sequence is odd, writer is updating - retry
        if (seq1 & 1) 
        {
            continue;
        }
        
        // Copy device data
        auto count = shm->device_count;
        if (count > maxCount) 
        {
            count = maxCount;
        }

        for (uint32_t i = 0; i < count; ++i) 
        {
            outDevices[i] = shm->devices[i];
        }
        
        // Read sequence after data
        seq2 = shm->sequence.load(std::memory_order_acquire);
        
        // If sequences match and even, data is consistent
        if (seq1 == seq2) 
        {
            outCount = count;
            return true;
        }
        
        // Otherwise, writer updated during read - retry
    } while (true);

}

bool SharedMemoryReader::IsWriterAlive() const
{
    if (!_opened || _shm_ptr == nullptr) 
    {
        return false;
    }

#ifdef __linux__
    auto* shm = static_cast<const MonitorSharedMemory*>(_shm_ptr);
    // Check if writer process is still running
    auto writer_pid = static_cast<pid_t>(shm->writer_pid);
    if (writer_pid == 0) 
    {
        return false;
    }

    // Send signal 0 to check if process exists
    if (kill(writer_pid, 0) == 0)
    {
        return true;
    }

    return false;
#elif _WIN32
    auto* shm = static_cast<const MonitorSharedMemory*>(_shm_ptr);
    DWORD writer_pid = static_cast<DWORD>(shm->writer_pid);
    if (writer_pid == 0)
    {
        return false;
    }

    HANDLE process_handle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, writer_pid);
    if (process_handle == nullptr)
    {
        return false;
    }

    DWORD exit_code = 0;
    bool alive = GetExitCodeProcess(process_handle, &exit_code) && (exit_code == STILL_ACTIVE);
    CloseHandle(process_handle);
    return alive;
#else
    return false;
#endif
}

uint32_t SharedMemoryReader::GetWriterPid() const
{
    if (!_opened || _shm_ptr == nullptr)
    {
        return 0;
    }

    auto* shm = static_cast<const MonitorSharedMemory*>(_shm_ptr);
    return shm->writer_pid;
}

uint64_t SharedMemoryReader::GetUpdateCount() const
{
    if (!_opened || _shm_ptr == nullptr) 
    {
        return 0;
    }

    auto* shm = static_cast<const MonitorSharedMemory*>(_shm_ptr);
    return shm->update_count;
}

} // namespace dxrt
