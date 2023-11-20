#pragma once

#include "itensor.hpp"

namespace cucv
{

class Tensor : public ITensor
{
public:
    using Requirements = TensorRequirements;
    static Requirements CalcRequirements(const TensorShape &shape, DataType dtype, const MemAlignment &bufAlign = {});
    static Requirements CalcRequirements(int numImages, Size2D imgSize, ImageFormat fmt,
                                         const MemAlignment &bufAlign = {});

    explicit Tensor(const Requirements &reqs, IAllocator *alloc = nullptr);
    explicit Tensor(const TensorShape &shape, DataType dtype, const MemAlignment &bufAlign = {},
                    IAllocator *alloc = nullptr);
    explicit Tensor(int numImages, Size2D imgSize, ImageFormat fmt, const MemAlignment &bufAlign = {},
                    IAllocator *alloc = nullptr);
    ~Tensor();

    Tensor(const Tensor &) = delete;

private:
    NVCVTensorHandle doGetHandle() const final override;

    NVCVTensorHandle m_handle;
};

} // namespace cucv
