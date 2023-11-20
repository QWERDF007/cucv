#include "cucv/imgproc/ioperator.h"

namespace cucv
{

IOperator *ToOperatorPtr(void *handle)
{
    // 首先将 handle 转换为 Operator 指针
    if (IOperator *op = reinterpret_cast<IOperator *>(handle))
    {
        // if (op->version().major() == CURRENT_VERSION.major())
        // {
        //     return op;
        // }
        // else
        // {
        //     std::ostringstream ss;
        //     ss << "Object version " << op->version() << " not compatible with NVCV OP version " << CURRENT_VERSION;
        //     throw nvcv::Exception(nvcv::Status::ERROR_NOT_COMPATIBLE, "%s", ss.str().c_str());
        // }

        // TODO: 返回 Operator
        return op;
    }
    else
    {
        return nullptr;
    }
}

Status CUCVOperatorDestroy(OperatorHandle handle)
{
    // return nvcv::ProtectCall([&] { delete priv::ToOperatorPtr(handle); });
    if (handle)
    {
        delete ToOperatorPtr(handle);
    }
    return CUCV_SUCCESS;
}

} // namespace cucv
