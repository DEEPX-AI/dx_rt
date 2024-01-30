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
    for (int i = 0; i < v.size(); ++i)
    {
        os << v[i];
        if (i != v.size() - 1)
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
        << option.devices;
    return os;
}

} // namespace dxrt