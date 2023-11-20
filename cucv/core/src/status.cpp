#include "cucv/core/status.hpp"
#include <string>
#include <unordered_map>

namespace cucv
{

namespace
{

// Visual Studio error C2001:常量中有换行符（解决办法）：1. 改成UTF-8 with BOM 格式；2. 中文字符常量后添加英文句号 '.'

/////////////////////////////// 错误描述信息 ////////////////////////////////////////

const std::unordered_map<int, std::string> kErrorStr = {
    {CUCV_SUCCESS, "操作成功."},
    {CUCV_ERROR_NOT_IMPLEMENTED, "操作没有实现."},
    {CUCV_ERROR_INVALID_ARGUMENT, "无效的参数, 错误的范围或者不接受的值."},
    {CUCV_ERROR_INVALID_IMAGE_FORMAT, "不接受的图像类型."},
    {CUCV_ERROR_INVALID_OPERATION, "在当前情况下, 操作是无效的."},
    {CUCV_ERROR_DEVICE, "设备后端错误."},
    {CUCV_ERROR_NOT_READY, "操作尚未完成, 稍后再试."},
    {CUCV_ERROR_OUT_OF_MEMORY, "没有足够的可用内存来分配对象."},
    {CUCV_ERROR_INTERNAL, "内部错误, 坏的状态."},
    {CUCV_ERROR_NOT_COMPATIBLE, "实现的版本不兼容."},
    {CUCV_ERROR_OVERFLOW, "结果大于可接受的最大值."},
    {CUCV_ERROR_UNDERFLOW, "结果小于可接受的最小值."},
    {CUCV_ERROR_ASSERT_FAILED, "断言失败."},
};

} // namespace

std::string getErrorStr(int status)
{
#define CASE(ERR)                                                                                                      \
    case ERR:                                                                                                          \
        return std::string(#ERR) + ": " + kErrorStr.at(ERR)

    switch (status)
    {
        CASE(CUCV_SUCCESS);
        CASE(CUCV_ERROR_NOT_IMPLEMENTED);
        CASE(CUCV_ERROR_INVALID_ARGUMENT);
        CASE(CUCV_ERROR_INVALID_IMAGE_FORMAT);
        CASE(CUCV_ERROR_INVALID_OPERATION);
        CASE(CUCV_ERROR_DEVICE);
        CASE(CUCV_ERROR_NOT_READY);
        CASE(CUCV_ERROR_OUT_OF_MEMORY);
        CASE(CUCV_ERROR_INTERNAL);
        CASE(CUCV_ERROR_NOT_COMPATIBLE);
        CASE(CUCV_ERROR_OVERFLOW);
        CASE(CUCV_ERROR_UNDERFLOW);
    }
    return std::to_string(status) + ": 未定义错误.";
#undef CASE
}

} // namespace cucv