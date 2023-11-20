/**
 * @file itensor_data.hpp
 * @brief Tensor 抽象数据类和接口
 * @version 0.1
 * @date 2023-04-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include "data_type.hpp"
#include "detail/optional.h"
#include "tensor_data.h"
#include "tensor_shape.hpp"
#include <sstream>

//! @addtogroup core_basic
//! @{

namespace cucv
{

//! Tensor 数据抽象基类
class ITensorData
{

public:
    //! 纯虚析构函数
    virtual ~ITensorData() = 0;

    //! Tensor 的秩 (维数)
    int rank() const;

    //! Tensor 的形状的常引用
    const TensorShape &shape() const;

    //! Tensor 的秩 (维数) 的第 d 个元素
    const TensorShape::DimType &shape(int d) const;

    //! Tensor 的布局的常引用
    const TensorLayout &layout() const;

    //! Tensor 的数据类型
    DataType dtype() const;

    //! Tensor C 底层数据的常引用
    const TensorData_t &cdata() const;

protected:
    //! 默认构造
    ITensorData() = default;

    //! 通过 Tensor 数据构造
    ITensorData(const TensorData_t &data);

    //! Tensor C 底层数据的引用
    TensorData_t &cdata();

private:
    //! Tensor C 底层数据
    TensorData_t data_;
    //! 可变的 Tensor 形状指针
    mutable detail::Optional<TensorShape> cache_shape_;
};

//! Tensor 数据抽象类，可通过基指针加上偏移量 (步长) 访问元素
class ITensorDataStrided : public ITensorData
{
public:
    //! 纯虚析构函数
    virtual ~ITensorDataStrided() = 0;

    //! 返回 Tensor 数据的基地址
    Byte *base() const;

    const int64_t &stride(int d) const;

protected:
    // 表示使用 ITensorData 的构造函数作为构造函数
    using ITensorData::ITensorData;
};

//! CUDA Tensor 数据抽象类，可通过基指针加上偏移量 (步长) 访问元素
class ITensorDataStridedCuda : public ITensorDataStrided
{
public:
    //! 纯虚析构函数
    virtual ~ITensorDataStridedCuda() = 0;

protected:
    // 表示使用 ITensorDataStrided 的构造函数作为构造函数
    using ITensorDataStrided::ITensorDataStrided;
};

/////////////////////////////// ITensorData implementation ////////////////////////////////////////

inline ITensorData::ITensorData(const TensorData_t &data) : data_(data)
{
}

inline ITensorData::~ITensorData()
{
    // required dtor implementation
}

inline int ITensorData::rank() const
{
    return this->cdata().rank;
}

inline const TensorShape &ITensorData::shape() const
{
    if (!cache_shape_)
    {
        const TensorData_t &data = this->cdata();
        cache_shape_.emplace(data.shape, data.rank, data.layout);
    }

    return *cache_shape_;
}

inline auto ITensorData::shape(int d) const -> const TensorShape::DimType &
{
    const TensorData_t &data = this->cdata();

    if (d < 0 || d >= data.rank)
    {
        std::stringstream ss;
        ss << "Index of shape dimension " << d << " is out of bounds [0;" << data.rank - 1 << "]";
        CUCV_Error(CUCV_ERROR_INVALID_ARGUMENT, ss.str());
    }
    return data.shape[d];
}

inline const TensorLayout &ITensorData::layout() const
{
    return this->shape().layout();
}

inline DataType ITensorData::dtype() const
{
    const TensorData_t &data = this->cdata();
    return DataType{data.dtype};
}

inline const TensorData_t &ITensorData::cdata() const
{
    return data_;
}

inline TensorData_t &ITensorData::cdata()
{
    // data contents might be modified, must reset cache
    cache_shape_.reset();
    return data_;
}

/////////////////////////////// ITensorDataStrided implementation ////////////////////////////////////////

inline ITensorDataStrided::~ITensorDataStrided()
{
    // required dtor implementation
}

inline Byte *ITensorDataStrided::base() const
{
    const TensorBufferStrided_t &buffer = this->cdata().buffer.strided;
    return reinterpret_cast<Byte *>(buffer.base);
}

inline const int64_t &ITensorDataStrided::stride(int d) const
{
    const TensorData_t &data = this->cdata();
    if (d < 0 || d >= data.rank)
    {
        std::stringstream ss;
        ss << "Index of pitch " << d << " is out of bounds [0;" << data.rank - 1 << "]";
        CUCV_Error(CUCV_ERROR_INVALID_ARGUMENT, ss.str());
    }

    return data.buffer.strided.strides[d];
}

/////////////////////////////// ITensorDataStridedCuda implementation ////////////////////////////////////////

inline ITensorDataStridedCuda::~ITensorDataStridedCuda()
{
    // required dtor implementation
}

} // namespace cucv

//! @} core_basic
