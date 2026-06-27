/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */
#include "memfd_client.h"

namespace dxrt {
namespace shm {

MemFDClient::MemFDClient() = default;

MemFDClient::~MemFDClient() {
    CleanupAll();
}

bool MemFDClient::RegisterSharedMemory(const SharedMemoryInfo& info) {
    if (info.fd == dxrt::shm::kInvalidMemFDHandle || info.size == 0 || info.block_id == 0) {
        throw MemFDException(MemFDErrorCode::INVALID_NAME, "Invalid SharedMemoryInfo");
    }

    std::lock_guard<std::mutex> lock(mutex_);

    // Check whether it is already registered.
    if (memories_.find(info.fd) != memories_.end()) {
        throw MemFDException(MemFDErrorCode::CREATE_FAILED, "Memory already registered");
    }

    if (block_id_to_fd_.find(info.block_id) != block_id_to_fd_.end()) {
        throw MemFDException(MemFDErrorCode::CREATE_FAILED, "Block ID already registered");
    }

    try {
        // Validate the file descriptor.
        if (!SharedMemorySyscallAdapter::OpenMemFD(info.fd)) {
            throw MemFDException(MemFDErrorCode::OPEN_FAILED, "Failed to open memfd");
        }

        // Map the shared memory.
        void* addr = SharedMemorySyscallAdapter::MapMemFD(info.fd, info.size);
        if (!addr) {
            throw MemFDException(MemFDErrorCode::MMAP_FAILED, "Failed to map memory");
        }

        // Copy SharedMemoryInfo and update its mapped address.
        auto registered_info = std::make_shared<SharedMemoryInfo>(info);
        registered_info->ptr = addr;
        registered_info->pid = getpid();  // Update with the client PID.

        // Register the mapping.
        memories_[info.fd] = registered_info;
        block_id_to_fd_[info.block_id] = info.fd;

        return true;
    } catch (const MemFDException&) {
        throw;
    }
}

std::shared_ptr<SharedMemoryInfo> MemFDClient::GetMemoryInfo(dxrt::shm::MemFDHandle fd) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = memories_.find(fd);
    if (it == memories_.end()) {
        return nullptr;
    }

    return it->second;
}

std::shared_ptr<SharedMemoryInfo> MemFDClient::GetMemoryInfoByBlockId(uint64_t block_id) {
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

std::vector<std::shared_ptr<SharedMemoryInfo>> MemFDClient::GetAllMemories()
{
    std::lock_guard<std::mutex> lock(mutex_);

    std::vector<std::shared_ptr<SharedMemoryInfo>> result;
    for (const auto& pair : memories_)
    {
        result.push_back(pair.second);
    }

    return result;
}

void* MemFDClient::GetMemoryPtr(dxrt::shm::MemFDHandle fd)
{
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = memories_.find(fd);
    if (it == memories_.end())
    {
        return nullptr;
    }

    return it->second->ptr;
}

size_t MemFDClient::GetMemorySize(dxrt::shm::MemFDHandle fd) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = memories_.find(fd);
    if (it == memories_.end()) {
        return 0;
    }

    return it->second->size;
}

bool MemFDClient::UnregisterSharedMemory(dxrt::shm::MemFDHandle fd) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = memories_.find(fd);
    if (it == memories_.end()) {
        return false;
    }

    const auto& info = it->second;

    // Unmap the memory.
    SharedMemorySyscallAdapter::UnmapMemFD(info->ptr, info->size);
    // With SCM_RIGHTS, the kernel duplicates the fd into this process, so it must be closed here.
    // Even if the server closes the original fd, the client's duplicated copy remains valid.
    SharedMemorySyscallAdapter::CloseMemFD(info->fd);

    block_id_to_fd_.erase(info->block_id);
    memories_.erase(it);

    return true;
}

void MemFDClient::CleanupAll() {
    std::lock_guard<std::mutex> lock(mutex_);

    for (const auto& pair : memories_) {
        const auto& info = pair.second;
        SharedMemorySyscallAdapter::UnmapMemFD(info->ptr, info->size);
        SharedMemorySyscallAdapter::CloseMemFD(info->fd);
    }

    memories_.clear();
    block_id_to_fd_.clear();
}

MemFDClient::Statistics MemFDClient::GetStatistics() const {
    std::lock_guard<std::mutex> lock(mutex_);

    Statistics stats = {0, 0, getpid()};

    for (const auto& pair : memories_) {
        stats.registered_memories++;
        stats.total_size += pair.second->size;
    }

    return stats;
}

}  // namespace shm
}  // namespace dxrt
