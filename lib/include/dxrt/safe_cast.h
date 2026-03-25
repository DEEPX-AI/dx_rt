/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include "dxrt/common.h"
#include <cstdint>
#include <cstring>
#include <type_traits> // for is_trivially_copyable

namespace dxrt {



class SafeCast {
 public:
    // Must explicitly specify the pointer type (e.g., ToAddr<int*>)
    template <typename T> struct Identity { using type = T; };  // NOSONAR: S5008
    template <typename T_Ptr>
    static inline uintptr_t PointerToInteger(typename Identity<T_Ptr>::type ptr) noexcept {
        // Strictly check if it's a real pointer, not a function pointer
        static_assert(std::is_pointer<T_Ptr>::value, "Error: ToAddr requires a pointer type (e.g., ToAddr<int*>)");
        static_assert(!std::is_function<typename std::remove_pointer<T_Ptr>::type>::value, "Error: Function pointers not allowed"); // NOSONAR: S6020

        return reinterpret_cast<uintptr_t>(ptr);  // NOSONAR: S3630
    }

    // For consistency with FromAddr, specify destination type as is (e.g., FromAddr<int*>)
    template <typename T_Dest, typename T_Src>
    static inline T_Dest IntegerToPointer(T_Src addr) noexcept {
        static_assert(std::is_pointer<T_Dest>::value, "Error: Destination must be a pointer type");
        static_assert(std::is_same<T_Src, uintptr_t>::value, "Error: Source must be uintptr_t");

        return reinterpret_cast<T_Dest>(addr);  // NOSONAR: S3630
    }

    // 3. Arbitrary pointer -> byte pointer (preparation for arithmetic operations)
    template <typename T_Src>
    static inline uint8_t* PtrToBytePtr(T_Src* src) noexcept {
        return static_cast<uint8_t*>(static_cast<void*>(src));
    }

    // 4. Byte pointer -> arbitrary pointer (struct interpretation)
    template <typename T_Dest>
    static inline T_Dest BytePtrToPtr(uint8_t* bptr) noexcept {
        static_assert(std::is_pointer<T_Dest>::value, "Dest must be a pointer!");
        // Adding alignment check here would be ideal
        return static_cast<T_Dest>(static_cast<void*>(bptr));
    }

    // const version
    template <typename T_Src>
    static inline const uint8_t* PtrToBytePtr(const T_Src* src) noexcept {
        return static_cast<const uint8_t*>(static_cast<const void*>(src));
    }

    // 4. Byte pointer -> arbitrary pointer (struct interpretation)
    template <typename T_Dest>
    static inline T_Dest BytePtrToPtr(const uint8_t* bptr) noexcept {
        static_assert(std::is_pointer<T_Dest>::value, "Dest must be a pointer!");
        static_assert(std::is_const<typename std::remove_pointer<T_Dest>::type>::value, "Dest must be a const pointer!");  // NOSONAR: S6020
        // Adding alignment check here would be ideal
        return static_cast<T_Dest>(static_cast<const void*>(bptr));
    }
};


}  // namespace dxrt
