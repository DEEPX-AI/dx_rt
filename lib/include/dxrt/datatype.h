// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "dxrt/common.h"

/** \brief DXRT C++ APIs are provided in this namespace
 * 
*/
namespace dxrt {

enum DataType
{
    NONE_TYPE = 0,
    UINT8,   ///< 8bit unsigned integer
    UINT16,  ///< 16it unsigned integer
    UINT32,  ///< 32bit unsigned integer
    INT8,    ///< 8bit signed integer
    INT16,   ///< 16bit signed integer
    INT32,   ///< 32bit signed integer
    FLOAT,   ///< 32bit float
    BBOX,   ///< custom structure for bounding boxes from device
    FACE,   ///< custom structure for faces from device
    POSE,   ///< custom structure for poses boxes from device
    MAX_TYPE,
};
extern std::string DataTypeToString(DataType type);
std::ostream& operator<<(std::ostream&, const DataType&);

typedef struct {
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

/** \brief face detection data format from device 
 * \headerfile "dxrt/dxrt_api.h"
*/
typedef struct {
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
/** \brief pose estimation data format from device 
 * \headerfile "dxrt/dxrt_api.h"
*/
typedef struct {
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