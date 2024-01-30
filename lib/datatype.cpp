// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#include "dxrt/common.h"
#include "dxrt/datatype.h"

using namespace std;

namespace dxrt {

string DataTypeToString(DataType type)
{
    switch (type) {
        case NONE_TYPE: return "NONE_TYPE";
        case UINT8: return "UINT8";
        case UINT16: return "UINT16";
        case UINT32: return "UINT32";
        case INT8: return "INT8";
        case INT16: return "INT16";
        case INT32: return "INT32";
        case FLOAT: return "FLOAT";
        case BBOX: return "BBOX";
        case FACE: return "FACE";
        case POSE: return "POSE";
        case MAX_TYPE: return "MAX_TYPE";
        default: return "NONE_TYPE";
    }
}

std::ostream& operator<<(std::ostream& os, const DataType& type)
{
    os << DataTypeToString(type);
    return os;
}

} // namespace dxrt
