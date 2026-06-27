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
#include <map>
#include <memory>
#include <mutex>
#include <vector>

namespace dxrt {
namespace shm {

/**
 * @brief memfd shared memory client
 *
 * Accesses memory created by MemFDService.
 * Accesses memory received from the service via the SharedMemoryInfo struct.
 */
class MemFDClient {
public:
    MemFDClient();
    ~MemFDClient();

    MemFDClient(const MemFDClient&) = delete;
    MemFDClient& operator=(const MemFDClient&) = delete;

    /**
    * @brief Register memory metadata received from the service
    * @param info SharedMemoryInfo received from the service
    * @return true on success
     *
    * Maps memory using the fd and size information for service-created memory.
     */
    bool RegisterSharedMemory(const SharedMemoryInfo& info);

    /**
    * @brief Get registered memory metadata
    * @param fd file descriptor
    * @return SharedMemoryInfo pointer, or nullptr if not found
     */
    std::shared_ptr<SharedMemoryInfo> GetMemoryInfo(dxrt::shm::MemFDHandle fd);

    /**
    * @brief Get registered memory by block_id
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
    * @brief Access mapped memory
    * @param fd file descriptor
    * @return memory pointer, or nullptr if not found
     */
    void* GetMemoryPtr(dxrt::shm::MemFDHandle fd);

    /**
    * @brief Get memory size
    * @param fd file descriptor
    * @return memory size, or 0 if not found
     */
    size_t GetMemorySize(dxrt::shm::MemFDHandle fd);

    /**
    * @brief Unregister memory
    * @param fd file descriptor
    * @return true on success
     */
    bool UnregisterSharedMemory(dxrt::shm::MemFDHandle fd);

    /**
    * @brief Release all registered memory
     */
    void CleanupAll();

    /**
    * @brief Client statistics
     */
    struct Statistics {
        size_t registered_memories;
        size_t total_size;
        pid_t client_pid;
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
