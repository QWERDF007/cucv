
#include "cucv/imgproc/copy.h"
#include "cucv/imgproc/ioperator.h"
#include "cucv/imgproc/legacy/imgproc_legacy.h"

namespace cucv
{

void copyMakeBorder(cudaStream_t stream, const ITensor &src, const ITensor &dst, const int top, const int bottom, const int left,
                    const int right, const int border_type, const int value)
{
    ;
}

CopyMakeBorder::CopyMakeBorder() {}

CopyMakeBorder::~CopyMakeBorder() {}

} // namespace cucv