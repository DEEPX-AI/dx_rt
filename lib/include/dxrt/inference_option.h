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


#ifdef USE_ORT
#define ORT_OPTION_DEFAULT true
#else
#define ORT_OPTION_DEFAULT false
#endif

namespace dxrt {
enum DXRT_API InferenceMode;
/** @brief This struct specifies inference options applied to dxrt::InferenceEngine.
 * @details User can configure which npu device is used to inference.
 * @headerfile "dxrt/dxrt_api.h"
*/
class DXRT_API InferenceOption
{
 public:
    enum BOUND_OPTION {
        NPU_ALL = 0,
        NPU_0,
        NPU_1,
        NPU_2,
        NPU_01,
        NPU_12,
        NPU_02
    };

    /** @brief device ID list to use 
     * @details make a list which contains list of device ID to use. if it is empty(or use default value), then all devices are used.
     */
    std::vector<int> devices = {};///< list of device ID to use (it is empty by default, then all devices are used.)

    /** @brief Select the NPU core inside the device
     * @details NPU_ALL is an option that uses all NPU cores simultaneously. NPU_0, NPU_1, and NPU_2 are options that allow using only a single NPU core.
     */
    uint32_t    boundOption = BOUND_OPTION::NPU_ALL;
    /** @brief Select which uses ORT task or not
     * @details if this is true, all task will works. if false, only npu task works.
     */

    bool useORT = ORT_OPTION_DEFAULT;

};



DXRT_API std::ostream& operator<<(std::ostream&, const InferenceOption&);

/** @brief Default inference option 
*/
extern DXRT_API InferenceOption DefaultInferenceOption;

} // namespace dxrt
