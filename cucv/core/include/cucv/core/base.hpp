#pragma once

#include "cucvdef.h"
#include "status.hpp"
#include <ostream>
#include <string>

/**
@defgroup core Core functionality
@brief 核心功能
@{
    @defgroup core_basic Basic structures
    @defgroup core_utils Utility and system functions and macros
    @brief 实用程序和系统函数及宏
    @{

    @}
@}
*/

//! @addtogroup core_utils
//! @{

namespace cucv
{

// TODO: 获取调用栈
std::string getCompleteTraceBack();

/**
 * @brief 异常类
 *
 * 这个类封装了所有或几乎所有关于程序中发生的错误的必要信息。
 * 异常通常是通过 CUCV_Error 和 CUCV_Error_ 宏来隐式构建和抛出的。
 * \see error
 */
class Exception : public std::exception
{
public:
    Exception();

    /**
     * @brief 完整的构造函数, 通常不直接调用，而是使用宏 CV_Error(), CV_Error_() and CV_Assert() 代替
     */
    Exception(int _code, const std::string &_err, const std::string &_func, const std::string &_file, int _line);
    virtual ~Exception() throw();

    /**
     * @brief 错误信息描述
     * @return 错误信息描述和上下文
     */
    virtual const char *what() const throw() override;

    void formatMessage();

    std::string msg;  //!< 格式化后的错误信息
    int code;         //!< 错误码, 见 \ref Status
    std::string err;  //!< 错误描述
    std::string func; //!< 函数名, 需要编译器支持
    std::string file; ///< 错误发生的源文件
    int line;         ///< 错误发生的行号
};

/**
 * @brief 发出错误信号并引发异常
 * @details 默认情况下，该函数将错误的信息打印到 stderr。然后，如果之前调用过
 * setBreakOnError()，它要么停止，要么引发异常。 可以通过使用 redirectError() 来交替进行错误处理。
 * @param exc
 */
CUCV_API CUCV_NORETURN void error(const Exception &exc);

/**
 * @brief 发出错误信号并引发异常。
 * @details 默认情况下，该函数将错误的信息打印到 `stderr`。然后，如果之前调用过 setBreakOnError()，
 *          它要么停止，要么引发异常。可以通过使用 redirectError() 来交替进行错误处理。
 * @param _code 错误代码
 * @param _err 错误描述
 * @param _func 函数名称。编译器支持时才可用
 * @param _file 发生错误的源文件名
 * @param _line 发生错误的源文件的行数
 * @see CUCV_Error, CUCV_Error_, CUCV_Assert, CUCV_DbgAssert
 */
CUCV_API CUCV_NORETURN void error(int _code, const std::string &_err, const char *_func, const char *_file, int _line);

/**
 * @brief 调用错误处理程序
 * @details 目前，错误处理程序将错误代码和错误信息打印到标准的错误流`stderr`。
 *          在Debug配置中，它将引发内存访问违规，以便执行堆栈和所有的参数可以被调试器分析。
 *          在发布配置中配置中，异常被抛出。
 * @param code
 * @param msg
 */
#define CUCV_Error(code, msg) cucv::error(code, msg, CUCV_Func, __FILE__, __LINE__)

// clang-format off

/**
 * @brief 运行时断言，如果失败则抛出异常
 * @details 宏 CUCV_Assert 评估表达式。如果它是 0，该宏返回一个异常 (\see cv::error)。
 */
#define CUCV_Assert(expr)                                                                                              \
do                                                                                                                     \
{                                                                                                                      \
    if (!!(expr))                                                                                                      \
        ;                                                                                                              \
    else                                                                                                               \
        cucv::error(cucv::Error::ERROR_ASSERT_FAILED, #expr, CUCV_Func, __FILE__, __LINE__);                           \
} while (0)

// clang-format on

} // namespace cucv

//! @} core_utils
