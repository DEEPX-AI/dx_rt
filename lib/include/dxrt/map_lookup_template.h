/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers
 * who are supplied with DEEPX NPU (Neural Processing Unit).
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#pragma once

#include<array>
#include<string>

namespace dxrt {

constexpr int CHARBUFFER_SIZE = 128;
using pair_type = std::pair<int, const char*>;
template<typename T, size_t size>
static std::string map_lookup(const std::array<pair_type, size>& m, T n)
{
    int key = static_cast<int>(n);
    for (const auto& pair : m)
    {
        if (pair.first == key)
            return std::string(pair.second);
        else continue;
    }
    return "-ERROR lookup type"+std::string(typeid(n).name())+"("+std::to_string(n)+")-";
}
template<typename T, size_t size, typename STRTYPE>
static std::string map_lookup(const std::array<pair_type, size>& m, T n, STRTYPE defaultStr)
{
    int key = static_cast<int>(n);
    for (const auto& pair : m)
    {
        if (pair.first == key)
            return std::string(pair.second);
        else continue;
    }
    return std::string(defaultStr);
}

}  // namespace dxrt
