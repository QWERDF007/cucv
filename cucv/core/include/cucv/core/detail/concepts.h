#pragma once

#include <iterator>
#include <type_traits>

namespace cucv
{
namespace detail
{

//! 判断迭代器是否为随机访问迭代器，在编译期间进行类型检查，以确保迭代器是随机访问迭代器，否则不会被定义
template<class IT>
using IsRandomAccessIterator = typename std::enable_if<
    std::is_same<typename std::iterator_traits<IT>::iterator_category, std::random_access_iterator_tag>::value>::type;

} // namespace detail
} // namespace cucv
