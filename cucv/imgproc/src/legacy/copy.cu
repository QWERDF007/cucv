#include "imgproc_legacy.h"

template <typename Ptr2D>
__global__ void copyMakeBroderKernel(const Ptr2D src, Ptr2D dst, int top, int bottom, int left, int right,
                                     int borderType, const int value)
{
    const int x = blockIdx.x * blockDim.x + threadIdx.x;
    const int y = blockIdx.y * blockDim.y + threadIdx.y;
    const int x_shift = x - left;
    const int y_shift = y - top;
    int out_height = dst.rows;
    int out_width = dst.cols;
    if (x < out_width && y < out_height)
    {
        dst[y][x] = 0;
    }
}

template <typename T, int cn> // uchar3 float3 uchar float
void copyMakeBorderImpl(const T src, T dst, int top, int bottom, int left, int right, int borderType, const int value)
{
    // typedef void (*func_t)(const Ptr2D src, Ptr2D dst, int top, int bottom, int left, int right, int borderType)
}

namespace cucv::legacy::cuda
{



} // namespace cucv::legacy::cuda
