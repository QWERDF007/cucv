/**
 * @file cucvdef.h
 * @brief 一些宏定义
 * @version 0.1
 * @date 2023-03-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef CUCV_CORE_CUCVDEF_H
#define CUCV_CORE_CUCVDEF_H

#ifdef CUCV_Func
// keep current value (through OpenCV port file)
#elif defined __GNUC__ || (defined(__cpluscplus) && (__cpluscplus >= 201103))
#define CUCV_Func __func__
#elif defined __clang__ && (__clang_minor__ * 100 + __clang_major__ >= 305)
#define CUCV_Func __func__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION >= 199901)
#define CUCV_Func __func__
#elif defined _MSC_VER
#define CUCV_Func __FUNCTION__
#elif defined(__INTEL_COMPILER) && (_INTEL_COMPILER >= 600)
#define CUCV_Func __FUNCTION__
#elif defined __IBMCPP__ && __IBMCPP__ >= 500
#define CUCV_Func __FUNCTION__
#elif defined __BORLAND__ && (__BORLANDC__ >= 0x550)
#define CUCV_Func __FUNC__
#else
#define CUCV_Func "<unknown>"
#endif

// clang-format off

// #ifndef CUCV_API
// #   if (defined _WIN32 || defined WINCE || defined __CYGWIN__) && defined(CUCV_EXPORTS)
// #       define CUCV_API __declspec(dllexport)
// #   elif defined __GNUC__ && __GNUC__ >= 4 && defined(CUCV_EXPORTS)
// #       define CUCV_API __attribute__ ((visibility ("default")))
// #   endif
// #endif

#ifndef CUCV_API
#   if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#       define CUCV_API __declspec(dllexport)
#   elif defined __GNUC__ && __GNUC__ >= 4
#       define CUCV_API __attribute__ ((visibility ("default")))
#   endif
#endif



/****************************************************************************************\
*                                CUCV_NORETURN attribute                                 *
\****************************************************************************************/

#ifndef CUCV_NORETURN
#  if defined(__GNUC__)
#    define CV_NORETURN __attribute__((__noreturn__))
#  elif defined(_MSC_VER) && (_MSC_VER >= 1300)
#    define CUCV_NORETURN __declspec(noreturn)
#  else
#    define CUCV_NORETURN /* nothing by default */
#  endif
#endif

// clang-format on

#endif // CUCV_CORE_CUCVDEF_H