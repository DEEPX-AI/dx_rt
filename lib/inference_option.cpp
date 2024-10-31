// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/inference_option.h"

using namespace std;

namespace dxrt
{

InferenceOption DefaultInferenceOption;

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    os << "[";
    for (size_t i = 0; i < v.size(); ++i)
    {
        os << v[i];
        if (i + 1 != v.size())
        {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

ostream& operator<<(ostream& os, const InferenceOption& option)
{
    os << "          inference option: "
        << option.devices << "/"
        << option.boundOption;
    return os;
}

} // namespace dxrt