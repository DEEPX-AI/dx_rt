// Copyright (c) 2022 DEEPX Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "dxrt/common.h"
#include "dxrt/datatype.h"

namespace dxrt {
enum DataType;
class Device;
class Task;
/** \brief This class abstracts DXRT tensor object, which defines data array composed of uniform elements.
 * \details Generally, this should be connected to any inference engine objects.
 * \headerfile "dxrt/dxrt_api.h"
*/
class Tensor
{
public:
    Tensor(std::string name_, std::vector<int64_t> shape_, DataType type_, void *data_=nullptr);
    Tensor(const Tensor &tensor_, void *data_=nullptr);
    ~Tensor();    
    std::string &name();
    std::vector<int64_t> &shape();
    DataType &type();    
    void* &data(); // data pointer
    uint64_t &phy_addr(); // physical address of data
    uint32_t &elem_size();
    /** \brief Get pointer of specific element by tensor index. (for NHWC data type)
     * \param[in] height height index
     * \param[in] width width index
     * \param[in] channel channel index
     * \return address of the element [N, height, width, channel] (N=1 for current ver.)
    */
    void* data(int height, int width, int channel);
    friend std::ostream& operator<<(std::ostream&, const Tensor&);
private:
    std::string _name;
    std::vector<int64_t> _shape;
    DataType _type;
    void *_data;
    uint64_t _phyAddr = 0;
    uint32_t _inc; // addr. increasement for shape[2]
    uint32_t _elemSize;
};
using Tensors = std::vector<Tensor>;
using TensorPtr = std::shared_ptr<Tensor>;
using TensorPtrs = std::vector<std::shared_ptr<Tensor>>;

} // namespace dxrt