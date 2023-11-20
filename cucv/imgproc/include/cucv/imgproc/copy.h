#pragma once

#include "cucv/core/itensor.hpp"
#include "cucv/imgproc/ioperator.h"
#include "cuda_runtime.h"

namespace cucv
{

CUCV_API void copyMakeBorder(cudaStream_t stream, const ITensor &src, const ITensor &dst, const int top, const int bottom, const int left,
                             const int right, const int border_type, const int value = 0);

class CUCV_API CopyMakeBorder final : public IOperator
{
public:
    explicit CopyMakeBorder();

    ~CopyMakeBorder();

private:
    OperatorHandle handle_;
};

} // namespace cucv
