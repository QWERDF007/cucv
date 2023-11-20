#include "cucv/core/tensor_shape.hpp"

namespace cucv
{

Status TensorShapePermute(TensorLayout_t src_layout, const int64_t *src_shape, TensorLayout_t dst_layout,
                          int64_t *dst_shape)
{
    CUCV_Assert(src_shape != nullptr);
    CUCV_Assert(dst_shape != nullptr);

    std::fill_n(dst_shape, dst_layout.rank, 1);

    for (int i = 0; i < src_layout.rank; ++i)
    {
        int dst_idx = FindDimIndex(dst_layout, src_layout.data[i]);
        if (dst_idx >= 0)
        {
            dst_shape[dst_idx] = src_shape[i];
        }
    }
}

} // namespace cucv
