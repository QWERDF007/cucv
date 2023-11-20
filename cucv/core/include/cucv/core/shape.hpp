/**
 * @file shape.h
 *
 * @brief 用数组来实现表示和操作形状
 *
 * @attention
 * The number of dimensions a tensor has is called its rank and the length in each dimension describes its shape. \n
 * https://improbable-research.github.io/keanu/docs/tensors/
 *
 */

#pragma once

#include "base.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <cassert>

namespace cucv
{

template<class T, int N>
class Shape
{
    using Data = std::array<T, N>;

public:
    using value_type      = typename Data::value_type;
    using size_type       = int;
    using reference       = typename Data::reference;
    using const_reference = typename Data::const_reference;
    using iterator        = typename Data::iterator;
    using const_iterator  = typename Data::const_iterator;

    constexpr static int MAX_RANK = N;

    /**
     * @brief 构造一个 Shape
     *
     */
    Shape();

    /**
     * @brief 用 0 填充构造一个 Shape
     *
     * @param size Shape 的秩 (维度)
     */
    explicit Shape(size_type size);

    /**
     * @brief 拷贝构造一个 Shape
     *
     */
    Shape(const Shape &that);

    /**
     * @brief 从数组构造一个 Shape
     *
     * @param data 构造 Shape 的数组
     * @param n Shape 的秩
     */
    Shape(const T *data, size_t n);

    /**
     *  @brief 用初始化列表来构造一个 Shape
     *
     */
    Shape(std::initializer_list<value_type> shape);

    /**
     * @brief 获取 Shape 的第 i 数据的引用
     *
     */
    reference operator[](int i);

    /**
     * @brief 获取 Shape 的第 i 数据的常引用
     *
     */
    const_reference operator[](int i) const;

    /**
     * @brief Shape 的秩 (维数)
     *
     */
    size_type rank() const;

    /**
     * @brief Shape 的秩 (维数)
     *
     */
    size_type size() const;

    bool empty() const;

    /**
     * @brief 指向容器开头的迭代器
     *
     */
    iterator begin();

    /**
     * @brief 指向容器末尾的迭代器
     *
     */
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    /**
     * @brief 指向容器开头的常量迭代器
     *
     */
    const_iterator cbegin() const;

    /**
     * @brief 指向容器末尾的常量迭代器
     *
     */
    const_iterator cend() const;

    bool operator==(const Shape &that) const;
    bool operator!=(const Shape &that) const;

    bool operator<(const Shape &that) const;

private:
    Data data_;
    size_type size_;
};

template<class T, int N>
Shape<T, N>::Shape() : size_(0)
{
}

template<class T, int N>
Shape<T, N>::Shape(size_type size) : size_(size)
{
    std::fill(this->begin(), this->end(), 0);
}

template<class T, int N>
Shape<T, N>::Shape(const Shape &that) : size_(that.size_)
{
    std::copy(that.begin(), that.end(), data_.begin());
}

template<class T, int N>
Shape<T, N>::Shape(std::initializer_list<value_type> shape) : Shape(shape.begin(), shape.size())
{
}

template<class T, int N>
Shape<T, N>::Shape(const T *data, size_t n) : size_(n)
{
    if (data == nullptr)
    {
        CUCV_Error(CUCV_ERROR_INVALID_ARGUMENT, "Shape data must not be NULL");
    }

    if (n > data_.size())
    {
        CUCV_Error(CUCV_ERROR_INVALID_ARGUMENT, "Shape ranks is too big");
    }

    std::copy_n(data, n, data_.begin());
}

template<class T, int N>
auto Shape<T, N>::operator[](int i) -> reference
{
    assert(0 <= i && i < size_);
    return data_[i];
}

template<class T, int N>
auto Shape<T, N>::operator[](int i) const -> const_reference
{
    assert(0 <= i && i < size_);
    return data_[i];
}

template<class T, int N>
bool Shape<T, N>::operator==(const Shape &that) const
{
    if (size_ == that.size_)
    {
        return std::equal(this->begin(), this->end(), that.begin());
    }
    else
    {
        return false;
    }
}

template<class T, int N>
bool Shape<T, N>::operator!=(const Shape &that) const
{
    return !operator==(that);
}

template<class T, int N>
bool Shape<T, N>::operator<(const Shape &that) const
{
    // lexicographical_compare 泛型算法函数，比较两个容器的字典序
    return std::lexicographical_compare(this->begin(), this->end(), that.begin(), that.end());
}

template<class T, int N>
auto Shape<T, N>::rank() const -> size_type
{
    return size_;
}

template<class T, int N>
auto Shape<T, N>::size() const -> size_type
{
    return size_;
}

template<class T, int N>
bool Shape<T, N>::empty() const
{
    return size_ == 0;
}

template<class T, int N>
auto Shape<T, N>::begin() -> iterator
{
    return data_.begin();
}

template<class T, int N>
auto Shape<T, N>::end() -> iterator
{
    return data_.begin() + size_;
}

template<class T, int N>
auto Shape<T, N>::begin() const -> const_iterator
{
    return data_.begin();
}

template<class T, int N>
auto Shape<T, N>::end() const -> const_iterator
{
    return data_.begin() + size_;
}

template<class T, int N>
auto Shape<T, N>::cbegin() const -> const_iterator
{
    return data_.cbegin();
}

template<class T, int N>
auto Shape<T, N>::cend() const -> const_iterator
{
    return data_.cend() + size_;
}

/**
 * @brief 输出 Shape 的元素到 ostream
 *
 */
template<class T, int N>
std::ostream &operator<<(std::ostream &out, const Shape<T, N> &shape)
{
    if (shape.empty())
    {
        return out << "(empty)";
    }
    else
    {
        out << shape[0];
        for (int i = 1; i < shape.size(); ++i)
        {
            out << 'x' << shape[i];
        }
        return out;
    }
}

} // namespace cucv
