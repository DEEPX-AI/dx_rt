/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */
#pragma once

#include "shm.h"
#include "shm_error.h"
#include "shared_memory_syscall_adapter.h"
#include <atomic>
#include <map>
#include <memory>
#include <mutex>
#include <vector>

namespace dxrt {
namespace shm {

/**
 * @brief memfd shared memory service
 *
 * Creates and manages memory to be shared with other processes.
 * Shares memory metadata with clients via the SharedMemoryInfo struct.
 */
class MemFDService {
public:
    MemFDService();
    ~MemFDService();

    MemFDService(const MemFDService&) = delete;
    MemFDService& operator=(const MemFDService&) = delete;

    /**
    * @brief Create shared memory
    * @param size memory size to allocate (bytes)
    * @param task_id task ID (optional)
    * @param device_id device ID (optional)
    * @return created SharedMemoryInfo, or nullptr on failure
     *
    * Returns SharedMemoryInfo so it can be delivered to clients.
     */
    std::shared_ptr<SharedMemoryInfo> CreateSharedMemory(
        size_t size,
        int task_id = -1,
        int device_id = -1
    );

    /**
    * @brief Get memory metadata
    * @param fd file descriptor
    * @return SharedMemoryInfo pointer, or nullptr if not found
     */
    std::shared_ptr<SharedMemoryInfo> GetMemoryInfo(dxrt::shm::MemFDHandle fd);

    /**
    * @brief Get memory metadata by block_id
    * @param block_id unique block ID
    * @return SharedMemoryInfo pointer, or nullptr if not found
     */
    std::shared_ptr<SharedMemoryInfo> GetMemoryInfoByBlockId(uint64_t block_id);

    /**
    * @brief Get all registered memory entries
    * @return vector of SharedMemoryInfo entries
     */
    std::vector<std::shared_ptr<SharedMemoryInfo>> GetAllMemories();

    /**
    * @brief Release shared memory
    * @param fd file descriptor
    * @return true on success
     */
    bool ReleaseSharedMemory(dxrt::shm::MemFDHandle fd);

    /**
    * @brief Release all shared memory
     */
    void CleanupAll();

    /**
    * @brief Service statistics
     */
    struct Statistics {
        size_t total_memories;
        size_t total_size;
        pid_t service_pid;
    };

    /**
    * @brief Get statistics
     */
    Statistics GetStatistics() const;

private:
    // Manage memory metadata by fd.
    std::map<dxrt::shm::MemFDHandle, std::shared_ptr<SharedMemoryInfo>> memories_;
    std::map<uint64_t, dxrt::shm::MemFDHandle> block_id_to_fd_;

    // Synchronization.
    mutable std::mutex mutex_;
};

} // namespace shm
} // namespace dxrt
