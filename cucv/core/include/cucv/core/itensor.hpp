/**
 * @file itensor.hpp
 * @brief Tensor 的抽象类和接口
 * @version 0.1
 * @date 2023-04-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include "data_type.hpp"
#include "tensor_data.hpp"
#include "tensor_layout.hpp"
#include "tensor_shape.hpp"

namespace cucv
{

//! @addtogroup core_basic
//! @{

//! Tensor 抽象基类
class ITensor
{
public:
    //     using HandleType = CUCVTensorHandle;
    using BaseInterface = ITensor;

    virtual ~ITensor() = default;

    //     HandleType handle() const;
    //     static ITensor *cast(HandleType handle);

    int rank() const;
    TensorShape shape() const;
    DataType dtype() const;
    TensorLayout layout() const;

    const ITensorData *exportData() const;

    void setUserPointer(void *ptr);
    void *userPointer() const;

    // private:
    //     virtual NVCVTensorHandle doGetHandle() const = 0;

    // mutable detail::Optional<TensorDataStridedCuda> cache_data_;
};

inline int ITensor::rank() const
{
    int32_t rank = 0;

    return rank;
}

//! @} core_basic

} // namespace cucv
