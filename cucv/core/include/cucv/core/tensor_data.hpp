#pragma once

#include "itensor_data.hpp"
#include "tensor_shape.hpp"
#include "data_type.hpp"

//! @addtogroup core_basic
//! @{

namespace cucv
{

//! CUDA Tensor 数据实现类
class TensorDataStridedCuda : public ITensorDataStridedCuda
{
public:
    using Buffer = TensorBufferStrided_t;

    //! 从形状、数据类型、数据缓冲区构造
    explicit TensorDataStridedCuda(const TensorShape &shape, const DataType &dtype, const Buffer &data);
    //! 从数据缓冲区构造
    explicit TensorDataStridedCuda(const TensorData_t &data);
};

inline TensorDataStridedCuda::TensorDataStridedCuda(const TensorShape &tshape, const DataType &dtype,
                                                    const Buffer &buffer)
{
    TensorData_t &data = this->cdata();

    std::copy(tshape.shape().begin(), tshape.shape().end(), data.shape);
    data.rank   = tshape.rank();
    data.dtype  = dtype;
    data.layout = tshape.layout();

    data.buffer_type    = TENSOR_BUFFER_STRIDED_CUDA;
    data.buffer.strided = buffer;
}

inline TensorDataStridedCuda::TensorDataStridedCuda(const TensorData_t &data) : ITensorDataStridedCuda(data)
{
}

} // namespace cucv

//! @} core_basic