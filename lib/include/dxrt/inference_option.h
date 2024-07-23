// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "dxrt/common.h"

namespace dxrt {
enum DXRT_API InferenceMode;
/** @brief This struct specifies inference options applied to dxrt::InferenceEngine.
 * @details User can configure which npu device is used to inference.
 * @headerfile "dxrt/dxrt_api.h"
*/
struct DXRT_API InferenceOption
{
    /** @brief device ID list to use 
     * @details make a list which contains list of device ID to use. if it is empty(or use default value), then all devices are used.
     */
    std::vector<int> devices = {};///< list of device ID to use (it is empty by default, then all devices are used.)
    // TODO : add data format
};

DXRT_API std::ostream& operator<<(std::ostream&, const InferenceOption&);

/** @brief Default inference option 
*/
extern DXRT_API InferenceOption DefaultInferenceOption;

} // namespace dxrt
