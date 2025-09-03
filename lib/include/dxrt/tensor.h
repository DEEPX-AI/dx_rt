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
#include "dxrt/datatype.h"

namespace dxrt {

enum DataType;
class Device;
class Task;
class InferenceEngine;

/** \brief This class abstracts DXRT tensor object, which defines data array composed of uniform elements.
 * \details Generally, this should be connected to any inference engine objects.
 * \headerfile "dxrt/dxrt_api.h"
*/
class DXRT_API Tensor
{
public:
    Tensor(std::string name_, std::vector<int64_t> shape_, DataType type_, void *data_=nullptr);
    Tensor(const Tensor &tensor_, void *data_=nullptr);
    Tensor& operator=(const dxrt::Tensor&) = default;
    Tensor& operator= (Tensor&& tensor) = default;
    Tensor (Tensor&& tensor) = default;
    ~Tensor();    
    const std::string &name() const;
    std::vector<int64_t> &shape();
    DataType &type();    
    void* &data(); // data pointer
    uint64_t &phy_addr(); // physical address of data
    uint32_t &elem_size();
    uint64_t size_in_bytes() const {
        uint64_t num_elements = 1ULL;
        for (const auto& dim : _shape) {
            if (dim < 0) {
                // negative dimension means dynamic size, so skip calculation
                // actual size is determined at runtime
                continue;
            }
            num_elements *= static_cast<uint64_t>(dim);
        }
        return num_elements * _elemSize;
    }
    /** \brief Get pointer of specific element by tensor index. (for NHWC data type)
     * \param[in] height height index
     * \param[in] width width index
     * \param[in] channel channel index
     * \return address of the element [N, height, width, channel] (N=1 for current ver.)
    */
    void* data(int height, int width, int channel);

    friend DXRT_API std::ostream& operator<<(std::ostream&, const Tensor&);
    friend InferenceEngine;

private:
    void setDataReleaseFlag(bool flag);

private:
    std::string _name;
    std::vector<int64_t> _shape;
    DataType _type;
    void *_data;
    uint64_t _phyAddr = 0; // Physical address - need to verify usage
    uint32_t _inc; // addr. increasement for shape[2]
    uint32_t _elemSize;

    // release flag
    bool _dataReleaseFlag = false;
};
using Tensors = std::vector<Tensor>;
using TensorPtr = std::shared_ptr<Tensor>;
using TensorPtrs = std::vector<std::shared_ptr<Tensor>>;

DXRT_API void DataDumpBin(std::string filename, std::vector<dxrt::Tensor> tensors);
DXRT_API void DataDumpBin(std::string filename, std::vector<std::shared_ptr<dxrt::Tensor>> tensors);

} // namespace dxrt
