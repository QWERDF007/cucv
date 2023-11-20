/**
 * @file tensor_data.h
 * @brief 表示 Tensor 数据的 C 数据结构
 * @version 0.1
 * @date 2023-04-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include "data_type.hpp"
#include "tensor_layout.h"

//! @addtogroup core_basic
//! @{

namespace cucv
{

//! Tensor 缓冲区，可通过基指针加上偏移量 (步长) 访问元素
typedef struct TensorBufferStridedRec
{
    int64_t strides[TENSOR_MAX_RANK];

    /**
     * Tensor 数据基地址, 类型 T 的数据访问:
     * pelem = base + shape[0] * strides[0] + ... + shape[rank-1] * strides[rank-1]
     */
    uint8_t *base;
} TensorBufferStrided_t;

//! Tensor 缓冲区类型
enum TensorBufferType
{
    /* 没有 type 被选中 */
    TENSOR_BUFFER_NONE = 0,
    /**
     * GPU-accessible with equal-shape planes in pitch-linear layout
     * GPU 可访问的线性布局的正方形平面
     * pitch 是由用户提供的 2D 尺寸计算出来的线性内存分配，有必要的填充以确保行的主要访问正确
     */
    TENSOR_BUFFER_STRIDED_CUDA = 1,
};

//! Tensor 缓冲区。表示访问图像批次内容的有效方法。正确的方法取决于 \ref TensorData::bufferType
typedef struct TensorBufferRec
{
    /**
     * 存储在线性布局中的 Tensor image batch.
     * 在 \ref TensorData::buffer_type 是 \ref TENSOR_BUFFER_STRIDED_CUDA 时使用
     */
    TensorBufferStrided_t strided;
} TensorBuffer_t;

//! Tensor 数据。存储关于图像批次特征和内容的信息
typedef struct TensorDataRec
{
    DataType dtype;
    TensorLayout_t layout;
    int32_t rank;
    int64_t shape[TENSOR_MAX_RANK];
    TensorBufferType buffer_type;
    TensorBuffer_t buffer;
} TensorData_t;

} // namespace cucv

//! @} core_basic
