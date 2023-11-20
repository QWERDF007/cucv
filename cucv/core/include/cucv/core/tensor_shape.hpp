#pragma once

#include "base.hpp"
#include "cucvdef.h"
#include "shape.hpp"
#include "tensor_layout.hpp"

//! @addtogroup core_basic
//! @{

namespace cucv
{

//! Tensor 形状
class TensorShape
{
public:
    using DimType                 = int64_t;
    using ShapeType               = Shape<DimType, TENSOR_MAX_RANK>;
    constexpr static int MAX_RANK = ShapeType::MAX_RANK;

    //! 默认构造函数
    TensorShape() = default;

    //! 从给定的形状、布局构造
    TensorShape(ShapeType shape, TensorLayout layout) : shape_(std::move(shape)), layout_(std::move(layout))
    {
        if (layout_ != TENSOR_NONE && shape_.rank() != layout_.rank())
        {
            CUCV_Error(CUCV_ERROR_INVALID_ARGUMENT, "Layout dimensions must match shape dimensions");
        }
    }

    //! 从指定大小、布局构造
    TensorShape(int size, TensorLayout layout) : TensorShape(ShapeType(size), std::move(layout))
    {
    }

    //! 显示地从给定布局构造
    explicit TensorShape(TensorLayout layout) : TensorShape(layout.rank(), std::move(layout))
    {
    }

    //! 从一组形状数据、布局构造
    TensorShape(const DimType *data, int32_t size, TensorLayout layout)
        : TensorShape(ShapeType(data, size), std::move(layout))
    {
    }

    //! 从一组形状数据、布局描述字符串构造
    TensorShape(const DimType *data, int32_t size, const char *layout)
        : TensorShape(ShapeType(data, size), TensorLayout{layout})
    {
    }

    //! 从给定形状、布局描述字符串构造
    TensorShape(ShapeType shape, const char *layout) : TensorShape(std::move(shape), TensorLayout{layout})
    {
    }

    //! 返回形状的常引用
    const ShapeType &shape() const
    {
        return shape_;
    }

    //! 返回布局的常引用
    const TensorLayout &layout() const
    {
        return layout_;
    }

    //! 获取形状的第 i 个数据
    const DimType &operator[](int i) const
    {
        return shape_[i];
    }

    //! 返回形状的秩 (维数)
    int rank() const
    {
        return shape_.rank();
    }

    //! 返回形状的大小
    int size() const
    {
        return shape_.size();
    }

    //! 判断形状是否为空
    bool empty() const
    {
        return shape_.empty();
    }

    //! 判断形状是否等于另一形状
    bool operator==(const TensorShape &that) const
    {
        return std::tie(shape_, layout_) == std::tie(that.shape_, that.layout_);
    }

    //! 判断形状是否不等于另一形状
    bool operator!=(const TensorShape &that) const
    {
        return !(*this == that);
    }

    //! 判断形状是否小于另一形状
    bool operator<(const TensorShape &that) const
    {
        // 将多个变量绑定到一个 tuple 中进行比较
        return std::tie(shape_, layout_) < std::tie(that.shape_, that.layout_);
    }

    //! 将形状打印到输出流
    friend std::ostream &operator<<(std::ostream &out, const TensorShape &ts)
    {
        if (ts.layout_ == TENSOR_NONE)
        {
            return out << ts.shape_;
        }
        else
        {
            return out << ts.layout_ << '{' << ts.shape_ << '}';
        }
    }

private:
    ShapeType shape_;
    TensorLayout layout_;
};

/**
 * @brief 将输入形状按给定的布局重新排列为新的布局
 *
 * @param[in] src_layout 源形状的布局
 * @param[in] src_shape 被重新排列的形状
 *                      + 必须不为 NULL
 *                      + 维数必须等于 @p src_layout 中的维数
 * @param[in] dst_layout 目标形状的布局
 * @param[out] dst_shape 重新排列后的形状
 *                       + 必须不为 NULL
 *                       + 维数必须等于 @p dst_layout 中的维数
 * @retval #CUCV_ERROR_INVALID_ARGUMENT 部分参数超出有效范围
 * @retval #CUCV_SUCCESS 操作成功
 */
CUCV_API Status TensorShapePermute(TensorLayout_t src_layout, const int64_t *src_shape, TensorLayout_t dst_layout,
                                   int64_t *dst_shape);

//! 将输入形状按给定的布局重新排列为新的布局
inline TensorShape Permute(const TensorShape &src, TensorLayout dstLayout)
{
    TensorShape::ShapeType dst(dstLayout.rank());
    TensorShapePermute(src.layout(), &src[0], dstLayout, &dst[0]);
    // TODO: 检查所有的 detail::CheckThrow();
    // detail::CheckThrow();

    return {std::move(dst), std::move(dstLayout)};
}

} // namespace cucv

//! @} core_basic
