/*
 * Copyright (C) 2018- DEEPX Ltd.
 * All rights reserved.
 *
 * This software is the property of DEEPX and is provided exclusively to customers 
 * who are supplied with DEEPX NPU (Neural Processing Unit). 
 * Unauthorized sharing or usage is strictly prohibited by law.
 */

#include "dxrt/common.h"
#include "dxrt/datatype.h"
#include <string>
#include <iostream>

namespace dxrt {

DXRT_API std::string DataTypeToString(DataType type)
{
    switch (type) {
        case dxrt::DataType::NONE_TYPE: return "NONE_TYPE";
        case dxrt::DataType::UINT8: return "UINT8";
        case dxrt::DataType::UINT16: return "UINT16";
        case dxrt::DataType::UINT32: return "UINT32";
        case dxrt::DataType::UINT64: return "UINT64";
        case dxrt::DataType::INT8: return "INT8";
        case dxrt::DataType::INT16: return "INT16";
        case dxrt::DataType::INT32: return "INT32";
        case dxrt::DataType::INT64: return "INT64";
        case dxrt::DataType::FLOAT: return "FLOAT";
        case dxrt::DataType::BBOX: return "BBOX";
        case dxrt::DataType::FACE: return "FACE";
        case dxrt::DataType::POSE: return "POSE";
        case dxrt::DataType::MAX_TYPE: return "MAX_TYPE";
        default: return "NONE_TYPE";
    }
}

std::ostream& operator<<(std::ostream& os, const DataType& type)
{
    os << DataTypeToString(type);
    return os;
}

} // namespace dxrt
