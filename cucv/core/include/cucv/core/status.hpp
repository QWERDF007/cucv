#pragma once

#include <iostream>
#include <string>

//! @addtogroup core_utils
//! @{

namespace cucv
{

/**
 * @brief 状态码
 */
enum Status
{
    CUCV_SUCCESS = 0,                //!< 操作成功
    CUCV_ERROR_NOT_IMPLEMENTED,      //!< 操作没有实现
    CUCV_ERROR_INVALID_ARGUMENT,     //!< 无效的参数, 错误的范围或者不接受的值
    CUCV_ERROR_INVALID_IMAGE_FORMAT, //!< 不接受的图像类型
    CUCV_ERROR_INVALID_OPERATION,    //!< 在当前情况下, 操作是无效的
    CUCV_ERROR_DEVICE,               //!< 设备后端错误
    CUCV_ERROR_NOT_READY,            //!< 操作尚未完成, 稍后再试
    CUCV_ERROR_OUT_OF_MEMORY,        //!< 没有足够的可用内存来分配对象
    CUCV_ERROR_INTERNAL,             //!< 内部错误, 坏的状态
    CUCV_ERROR_NOT_COMPATIBLE,       //!< 实现的版本不兼容
    CUCV_ERROR_OVERFLOW,             //!< 结果大于可接受的最大值
    CUCV_ERROR_UNDERFLOW,            //!< 结果小于可接受的最小值
    CUCV_ERROR_ASSERT_FAILED,        //!< 断言失败
};

namespace Error
{
//! 错误码
enum Code
{
    SUCCESS = CUCV_SUCCESS,                                       //!< \ref CUCV_SUCCESS
    ERROR_NOT_IMPLEMENTED = CUCV_ERROR_NOT_IMPLEMENTED,           //!< \ref CUCV_ERROR_NOT_IMPLEMENTED
    ERROR_INVALID_ARGUMENT = CUCV_ERROR_INVALID_ARGUMENT,         //!< \ref CUCV_ERROR_INVALID_ARGUMENT
    ERROR_INVALID_IMAGE_FORMAT = CUCV_ERROR_INVALID_IMAGE_FORMAT, //!< \ref CUCV_ERROR_INVALID_IMAGE_FORMAT
    ERROR_INVALID_OPERATION = CUCV_ERROR_INVALID_OPERATION,       //!< \ref CUCV_ERROR_INVALID_OPERATION
    ERROR_DEVICE = CUCV_ERROR_DEVICE,                             //!< \ref CUCV_ERROR_DEVICE
    ERROR_NOT_READY = CUCV_ERROR_NOT_READY,                       //!< \ref CUCV_ERROR_NOT_READY
    ERROR_OUT_OF_MEMORY = CUCV_ERROR_OUT_OF_MEMORY,               //!< \ref CUCV_ERROR_OUT_OF_MEMORY
    ERROR_INTERNAL = CUCV_ERROR_INTERNAL,                         //!< \ref CUCV_ERROR_INTERNAL
    ERROR_NOT_COMPATIBLE = CUCV_ERROR_NOT_COMPATIBLE,             //!< \ref CUCV_ERROR_NOT_COMPATIBLE
    ERROR_OVERFLOW = CUCV_ERROR_OVERFLOW,                         //!< \ref CUCV_ERROR_OVERFLOW
    ERROR_UNDERFLOW = CUCV_ERROR_UNDERFLOW,                       //!< \ref CUCV_ERROR_UNDERFLOW
    ERROR_ASSERT_FAILED = CUCV_ERROR_ASSERT_FAILED,               //!< \ref CUCV_ERROR_ASSERT_FAILED
};

} // namespace Error

/**
 * @brief 获取错误信息
 *
 * @param status 错误码
 * @return 错误信息
 */
std::string getErrorStr(int status);

inline std::ostream &operator<<(std::ostream &out, Status status)
{
    return out << getErrorStr(status);
}

} // namespace cucv

//! @} core_utils
