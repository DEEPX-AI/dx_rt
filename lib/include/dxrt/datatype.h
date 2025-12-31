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

/** \brief DXRT C++ APIs are provided in this namespace
 * 
*/
namespace dxrt {

enum DXRT_API DataType
{
    NONE_TYPE = 0,
    FLOAT,   ///< 32bit float
    UINT8,   ///< 8bit unsigned integer
    INT8,    ///< 8bit signed integer
    UINT16,  ///< 16it unsigned integer
    INT16,   ///< 16bit signed integer
    INT32,   ///< 32bit signed integer
    INT64,   ///< 64bit signed integer
    UINT32,  ///< 32bit unsigned integer
    UINT64,  ///< 64bit unsigned integer
    BBOX,   ///< custom structure for bounding boxes from device
    FACE,   ///< custom structure for faces from device
    POSE,   ///< custom structure for poses boxes from device
    MAX_TYPE,
};
DXRT_API std::string DataTypeToString(DataType type);
DXRT_API std::ostream& operator<<(std::ostream&, const DataType&);

typedef struct DXRT_API _DeviceBoundingBox {
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

/// @cond
/** \brief face detection data format from device 
 * \headerfile "dxrt/dxrt_api.h"
*/
/// @endcond
typedef struct DXRT_API _DeviceFace {
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

/// @cond
/** \brief pose estimation data format from device 
 * \headerfile "dxrt/dxrt_api.h"
*/
/// @endcond
typedef struct DXRT_API _DevicePose {
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

} // namespace dxrt