/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include <cstdint>
#include <cstring>

int ptrCastToInt(const void* ptr)
{
    uintptr_t ret;
    memcpy(&ret, &ptr, sizeof(uintptr_t));
    return static_cast<int>(ret);
}
void* intCastToPtr(int value)
{
    auto value_store = static_cast<uintptr_t>(value);
    void* ret;
    memcpy(&ret, &value_store, sizeof(void*));
    return ret;
}
