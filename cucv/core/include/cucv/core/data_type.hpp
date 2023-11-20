/**
 * @file data_type.hpp
 * @brief 定义数据类型和处理数据的相关函数
 * @version 0.1
 * @date 2023-04-04
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef CUCV_CORE_DATATYPE_H
#define CUCV_CORE_DATATYPE_H

#include <stdint.h>

namespace cucv
{

typedef uint64_t DataType;

//! Byte 类型，与  C++17 的 std::byte 相似
enum class Byte : uint8_t
{
};

} // namespace cucv

#endif // #ifndef CUCV_CORE_DATATYPE_H