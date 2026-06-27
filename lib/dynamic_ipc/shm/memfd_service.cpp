/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */
#include "memfd_service.h"
#include <fcntl.h>
#include <cstring>
#include <atomic>


namespace dxrt {
namespace shm {

namespace block_id_detail {
std::atomic<uint64_t> g_block_id_counter{1};
}

MemFDService::MemFDService() = default;

MemFDService::~MemFDService() {
    CleanupAll();
}

std::shared_ptr<SharedMemoryInfo> MemFDService::CreateSharedMemory(
    size_t size,
    int task_id,
    int device_id) {

    if (size == 0) {
        throw MemFDException(MemFDErrorCode::INVALID_SIZE, "Size must be greater than 0");
    }

    std::lock_guard<std::mutex> lock(mutex_);
    int blockId = static_cast<int>(block_id_detail::g_block_id_counter.fetch_add(1));

    try {
        // Create memfd.
        dxrt::shm::MemFDHandle fd = SharedMemorySyscallAdapter::CreateMemFD(size);
        if (fd == dxrt::shm::kInvalidMemFDHandle) {
            throw MemFDException(MemFDErrorCode::CREATE_FAILED, "Failed to create memfd");
        }

        // Map memory.
        void* addr = SharedMemorySyscallAdapter::MapMemFD(fd, size);
        if (!addr) {
            SharedMemorySyscallAdapter::CloseMemFD(fd);
            throw MemFDException(MemFDErrorCode::MMAP_FAILED, "Failed to map memory");
        }

        // Fix the memory size.

        if (!SharedMemorySyscallAdapter::SealMemFD(fd)) {
            SharedMemorySyscallAdapter::UnmapMemFD(addr, size);
            SharedMemorySyscallAdapter::CloseMemFD(fd);
            throw MemFDException(MemFDErrorCode::SEAL_FAILED, "Failed to seal memory");
        }

        // Create SharedMemoryInfo.
        auto info = std::make_shared<SharedMemoryInfo>();
        info->block_id = blockId;
        info->fd = fd;
        info->ptr = addr;
        info->size = size;
        info->pid = getpid();
        info->set_phys_addr(0);
        info->deviceid = device_id;
        info->taskId = task_id;

        // Register.
        memories_[fd] = info;
        block_id_to_fd_[info->block_id] = fd;

        return info;
    } catch (const MemFDException&) {
        throw;
    }
}

std::shared_ptr<SharedMemoryInfo> MemFDService::GetMemoryInfo(dxrt::shm::MemFDHandle fd) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = memories_.find(fd);
    if (it == memories_.end()) {
        return nullptr;
    }

    return it->second;
}

std::shared_ptr<SharedMemoryInfo> MemFDService::GetMemoryInfoByBlockId(uint64_t block_id) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto block_it = block_id_to_fd_.find(block_id);
    if (block_it == block_id_to_fd_.end()) {
        return nullptr;
    }

    auto mem_it = memories_.find(block_it->second);
    if (mem_it == memories_.end()) {
        return nullptr;
    }

    return mem_it->second;
}

std::vector<std::shared_ptr<SharedMemoryInfo>> MemFDService::GetAllMemories() {
    std::lock_guard<std::mutex> lock(mutex_);

    std::vector<std::shared_ptr<SharedMemoryInfo>> result;
    for (auto& pair : memories_) {
        result.push_back(pair.second);
    }

    return result;
}

bool MemFDService::ReleaseSharedMemory(dxrt::shm::MemFDHandle fd) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = memories_.find(fd);
    if (it == memories_.end()) {
        return false;
    }

    auto& info = it->second;

    // Unmap.
    SharedMemorySyscallAdapter::UnmapMemFD(info->ptr, info->size);

    // Close.
    SharedMemorySyscallAdapter::CloseMemFD(info->fd);

    // Remove.
    block_id_to_fd_.erase(info->block_id);
    memories_.erase(it);

    return true;
}

void MemFDService::CleanupAll() {
    std::lock_guard<std::mutex> lock(mutex_);

    for (auto& pair : memories_) {
        auto& info = pair.second;
        SharedMemorySyscallAdapter::UnmapMemFD(info->ptr, info->size);
        SharedMemorySyscallAdapter::CloseMemFD(info->fd);
    }

    memories_.clear();
    block_id_to_fd_.clear();
}

MemFDService::Statistics MemFDService::GetStatistics() const {
    std::lock_guard<std::mutex> lock(mutex_);

    Statistics stats = {0, 0, getpid()};

    for (const auto& pair : memories_) {
        stats.total_memories++;
        stats.total_size += pair.second->size;
    }

    return stats;
}

} // namespace shm
} // namespace dxrt
