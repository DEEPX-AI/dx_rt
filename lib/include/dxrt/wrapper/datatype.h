/*
 * Backward-compatibility shim — DataType enum for SDK users.
 * In C ABI, data types are passed as int. This enum provides named constants.
 * Prefer #include "dxrt/dxrt_api.h" for new code.
 */
#pragma once
#include <cstdint>
#include <string>

namespace dxrt {

enum DataType
{
    NONE_TYPE = 0,
    FLOAT,
    UINT8,
    INT8,
    UINT16,
    INT16,
    INT32,
    INT64,
    UINT32,
    UINT64,
    BBOX,
    FACE,
    POSE,
    MAX_TYPE,
};

inline std::string DataTypeToString(DataType type)
{
    switch (type)
    {
    case NONE_TYPE: return "NONE";
    case FLOAT:    return "FLOAT";
    case UINT8:    return "UINT8";
    case INT8:     return "INT8";
    case UINT16:   return "UINT16";
    case INT16:    return "INT16";
    case INT32:    return "INT32";
    case INT64:    return "INT64";
    case UINT32:   return "UINT32";
    case UINT64:   return "UINT64";
    case BBOX:     return "BBOX";
    case FACE:     return "FACE";
    case POSE:     return "POSE";
    default:       return "UNKNOWN";
    }
}

inline std::string DataTypeToString(int type)
{
    return DataTypeToString(static_cast<DataType>(type));
}

typedef struct _DeviceBoundingBox {
    float x;
    float y;
    float w;
    float h;
    uint8_t grid_y;
    uint8_t grid_x;
    uint8_t box_idx;
    uint8_t layer_idx;
    float score;
    uint32_t label;
    char padding[4];
} DeviceBoundingBox_t;

typedef struct _DeviceFace {
    float x;
    float y;
    float w;
    float h;
    uint8_t grid_y;
    uint8_t grid_x;
    uint8_t box_idx;
    uint8_t layer_idx;
    float score;
    float kpts[5][2];
} DeviceFace_t;

typedef struct _DevicePose {
    float x;
    float y;
    float w;
    float h;
    uint8_t grid_y;
    uint8_t grid_x;
    uint8_t box_idx;
    uint8_t layer_idx;
    float score;
    uint32_t label;
    float kpts[17][3];
    char padding[24];
} DevicePose_t;

enum class DeviceType : uint32_t
{
    ACC_TYPE = 0,
    STD_TYPE = 1,
};

}  // namespace dxrt
