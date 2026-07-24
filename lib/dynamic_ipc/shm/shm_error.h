/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */
#pragma once

#include <exception>
#include <string>
#include <cstring>

namespace dxrt {
namespace shm {

/**
 * @brief memfd-related error codes
 */
enum class MemFDErrorCode {
    // Success
    SUCCESS = 0,

    // Creation-related
    CREATE_FAILED = 1,
    INVALID_NAME = 2,
    INVALID_SIZE = 3,
    MEMORY_EXHAUSTED = 4,

    // Access-related
    OPEN_FAILED = 5,
    ACCESS_DENIED = 6,
    MEMORY_NOT_FOUND = 7,
    PROCESS_MISMATCH = 8,

    // Mapping-related
    MMAP_FAILED = 9,
    MUNMAP_FAILED = 10,
    INVALID_ADDRESS = 11,

    // Resize-related
    RESIZE_FAILED = 12,
    SHRINK_NOT_ALLOWED = 13,

    // Sealing-related
    SEAL_FAILED = 14,
    ALREADY_SEALED = 15,

    // Pool-related
    POOL_FULL = 16,
    POOL_NOT_INITIALIZED = 17,
    INVALID_BLOCK = 18,

    // Miscellaneous
    FD_CLOSE_FAILED = 19,
    UNKNOWN_ERROR = 99,
};

/**
 * @brief memfd exception class
 */
class MemFDException : public std::exception {
public:
    explicit MemFDException(MemFDErrorCode code, const std::string& message = "")
        : code_(code), message_(message) {
        if (message_.empty()) {
            message_ = GetErrorMessage(code_);
        }
    }

    const char* what() const noexcept override {
        return message_.c_str();
    }

    MemFDErrorCode GetErrorCode() const {
        return code_;
    }

    /**
    * @brief Constructor with a system error code
     */
    explicit MemFDException(MemFDErrorCode code, int system_errno)
        : code_(code) {
        message_ = GetErrorMessage(code_) + " (errno: " + std::string(std::strerror(system_errno)) + ")";
    }

private:
    MemFDErrorCode code_;
    std::string message_;

    static std::string GetErrorMessage(MemFDErrorCode code) {
        switch (code) {
            case MemFDErrorCode::SUCCESS:
                return "Success";
            case MemFDErrorCode::CREATE_FAILED:
                return "Failed to create memfd";
            case MemFDErrorCode::INVALID_NAME:
                return "Invalid memory name";
            case MemFDErrorCode::INVALID_SIZE:
                return "Invalid memory size";
            case MemFDErrorCode::MEMORY_EXHAUSTED:
                return "Insufficient memory";
            case MemFDErrorCode::OPEN_FAILED:
                return "Failed to open memfd";
            case MemFDErrorCode::ACCESS_DENIED:
                return "Access denied to memory";
            case MemFDErrorCode::MEMORY_NOT_FOUND:
                return "Memory not found";
            case MemFDErrorCode::PROCESS_MISMATCH:
                return "Process ID mismatch";
            case MemFDErrorCode::MMAP_FAILED:
                return "Failed to map memory";
            case MemFDErrorCode::MUNMAP_FAILED:
                return "Failed to unmap memory";
            case MemFDErrorCode::INVALID_ADDRESS:
                return "Invalid memory address";
            case MemFDErrorCode::RESIZE_FAILED:
                return "Failed to resize memory";
            case MemFDErrorCode::SHRINK_NOT_ALLOWED:
                return "Memory shrinking not allowed";
            case MemFDErrorCode::SEAL_FAILED:
                return "Failed to seal memory";
            case MemFDErrorCode::ALREADY_SEALED:
                return "Memory already sealed";
            case MemFDErrorCode::POOL_FULL:
                return "Memory pool exhausted";
            case MemFDErrorCode::POOL_NOT_INITIALIZED:
                return "Memory pool not initialized";
            case MemFDErrorCode::INVALID_BLOCK:
                return "Invalid memory block";
            case MemFDErrorCode::FD_CLOSE_FAILED:
                return "Failed to close file descriptor";
            default:
                return "Unknown error";
        }
    }
};

} // namespace shm
} // namespace dxrt
