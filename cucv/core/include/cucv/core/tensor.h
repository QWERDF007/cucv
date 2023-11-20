#pragma once

#include "data_type.hpp"
#include "alloc/requirements.h"
#include "tensor_layout.h"


//! @addtogroup core_basic
//! @{

namespace cucv
{

typedef struct TensorRequirementsRec
{
    /*< Type of each element */
    DataType dtype;

    /*< Tensor dimension layout.
     * It's optional. If layout not available, set it to NVCV_TENSOR_NONE. */
    TensorLayout_t layout;

    /*< Rank, a.k.a number of dimensions */
    int32_t rank;

    /*< Shape of the tensor */
    int64_t shape[TENSOR_MAX_RANK];

    /*< Distance in bytes between each element of a given dimension. */
    int64_t strides[TENSOR_MAX_RANK];

    /*< Alignment/block size in bytes */
    int32_t alignBytes;

    /*< Tensor resource requirements. */
    Requirements_t mem;
} TensorRequirements_t;

} // namespace cucv

//! @} core_basic
