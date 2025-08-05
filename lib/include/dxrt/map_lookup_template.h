// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

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
    }
    return "-ERROR("+std::to_string(n)+")-";
}

}  // namespace dxrt
