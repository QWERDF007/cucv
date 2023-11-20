#pragma once

#include "cucv/core/base.hpp"
#include "cucv/core/cucvdef.h"
#include "cucv/core/status.hpp"

namespace cucv
{

typedef struct IOperator *OperatorHandle;

CUCV_API Status CUCVOperatorDestroy(OperatorHandle handle);

class IOperator
{
public:
    using HandleType = OperatorHandle;
    using InterfaceType = IOperator;

    virtual ~IOperator() = default;

    HandleType handle() const
    {
        return reinterpret_cast<HandleType>(const_cast<IOperator *>(static_cast<const IOperator *>(this)));
    }
};

IOperator *ToOperatorPtr(void *handle);

template <class T> inline T *ToDynamicPtr(OperatorHandle handle)
{
    return dynamic_cast<T *>(ToOperatorPtr(handle));
}

template <class T> inline T &ToDynamicRef(OperatorHandle handle)
{
    CUCV_Assert(handle != nullptr && "局部不能为 nullptr");

    // 将 handle 转换为类型 T 的指针并赋给 child，判断 child 是否为 nullptr
    if (T *child = ToDynamicPtr<T>(handle))
    {
        return *child;
    }
    else
    {
        CUCV_Error(CUCV_ERROR_NOT_COMPATIBLE, "句柄与请求的对象不匹配或者已经被销毁.");
    }
}

} // namespace cucv
