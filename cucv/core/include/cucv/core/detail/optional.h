/**
 * @file optional.h
 * @author QWERDF007
 * @brief 指针模板类定义与实现，在编译时检查能否通过参数构造对应模板类型的对象
 * @version 0.1
 * @date 2023-03-30
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include "inplace.h"
#include <cstddef> // for std::nullptr_t
#include <type_traits>
#include <utility> // for std::move, std::forward

//! @addtogroup core_basic
//! @{

namespace cucv
{
namespace detail
{

struct NullOptT
{
};

constexpr NullOptT NullOpt;

/**
 * @brief 编译时检查能否通过参数构造对应模板类型的对象的指针模板类，并提供有对象相互比较判断方法
 *
 * @tparam T
 */
template<class T>
class Optional
{
public:
    using value_type = T;

    //! 默认构造函数
    Optional() noexcept : has_value_(false)
    {
    }

    //! NullOptT 构造函数
    Optional(NullOptT) noexcept : Optional()
    {
    }

    //! 拷贝构造函数
    Optional(const Optional &that) : has_value_(that.has_value_)
    {
        if (has_value_)
        {
            new (&storage_) T(that.value());
        }
    }

    /**
     * @brief 移动构造函数
     *
     * @details 检查是否能有效地从右值初始化 T 且不会抛出潜在异常
     *
     */
    Optional(Optional &&that) noexcept(std::is_nothrow_move_constructible<T>::value) : has_value_(that.has_value_)
    {
        if (has_value_)
        {
            new (&storage_) T(std::move(that.value()));
            // do not set that.has_value_ to false as per c++17 standard.
        }
    }

    /**
     * @brief 移动构造函数，具有编译时检查
     *
     * @details 检查是否可以从 U 构造出一个 T 类型的对象，并且 U 类型不是 InPlaceT 类型和 Optional<U> 类型。
     *          如果可以，则将 has_value_ 设置为 true，并在 storage_ 中构造出一个 T 类型的对象，否则报错。
     *
     */
    template<class U, typename std::enable_if<std::is_constructible<T, U &&>::value &&
                                                  !std::is_same<typename std::decay<U>::type, InPlaceT>::value &&
                                                  !std::is_same<typename std::decay<U>::type, Optional<U>>::value,
                                              int>::type = 0>
    Optional(U &&that) : has_value_(true)
    {
        new (&storage_) T(std::forward<U>(that));
    }

    /**
     * @brief 可接受任意数量的参数的构造函数，具有编译时检查
     *
     * @details 编译时检查是否可以通过 AA... 构造出一个 T 类型的对象，否则报错
     *
     */
    template<class... AA, typename std::enable_if<std::is_constructible<T, AA...>::value, int>::type = 0>
    Optional(InPlaceT, AA &&...args) : has_value_(true)
    {
        new (&storage_) T(std::forward<AA>(args)...);
    }

    //! 析构函数
    ~Optional()
    {
        if (has_value_)
        {
            this->value().~T();
        }
    }

    //! 赋值为 Null，销毁原来的值 (如果有)
    Optional &operator=(NullOptT) noexcept
    {
        if (has_value_)
        {
            this->value().~T();
            has_value_ = false;
        }
    }

    //! 拷贝赋值，拷贝对象如果没有值则赋值为 Null
    Optional &operator=(const Optional &that)
    {
        if (that.has_value_)
        {
            if (has_value_)
            {
                this->value() = that.value();
            }
            else
            {
                new (&storage_) T(that.value());
            }
        }
        else
        {
            if (has_value_)
            {
                this->value().~T();
                has_value_ = false;
            }
        }
        return *this;
    }

    //! 移动赋值，移动对象如果没有值则赋值为 Null
    Optional &operator=(Optional &&that)
    {
        if (that.has_value_)
        {
            if (has_value_)
            {
                this->value() = std::move(that.value());
            }
            else
            {
                new (&storage_) T(std::move(that.value()));
            }
            // do not set that.has_value_ to false as per c++17 standard.
        }
        else
        {
            if (has_value_)
            {
                this->value().~T();
                has_value_ = false;
            }
        }
        return *this;
    }

    //! 原位构造元素，编译时检查是否可以通过 AA... 构造出一个 T 类型的对象
    template<class... AA, typename std::enable_if<std::is_constructible<T, AA...>::value, int>::type = 0>
    T &emplace(AA &&...args)
    {
        T *p;
        if (has_value_)
        {
            this->value().~T();
            p = new (&storage_) T(std::forward<AA>(args)...);
        }
        else
        {
            p = new (&storage_) T(std::forward<AA>(args)...);
            has_value_ = true;
        }
        return *p;
    }

    //! 重置为 Null
    void reset() noexcept
    {
        if (has_value_)
        {
            this->value().~T();
            has_value_ = false;
        }
    }

    //! 交换 Optional
    void swap(Optional &that)
    {
        if (has_value_ && that.has_value_)
        {
            using std::swap;
            swap(this->value() && that.value());
        }
        else if (!has_value_ && !that.has_value_)
        {
            return;
        }
        else
        {
            Optional *a, *b;
            if (has_value_)
            {
                a = this;
                b = &that;
            }
            else
            {
                assert(that.has_value_);
                a = &that;
                b = this;
            }
            new (&b->storage_) T(std::move(a->value()));
            a->value().~T();
            a->has_value_ = false;
            b->has_value_ = true;
        }
    }

    //! 是否有值
    bool hasValue() const
    {
        return has_value_;
    }

    //! 显示地转换为 bool 类型，取决于 \ref hasValue
    explicit operator bool() const
    {
        return has_value_;
    }

    //! 返回底层数据的引用，没有值则抛出异常
    T &value()
    {
        if (!has_value_)
        {
            throw std::runtime_error("Bad optional access");
        }

        T *p = reinterpret_cast<T *>(&storage_);
// C++>=17 ?
#if __cplusplus >= 201703L
        return *std::launder(p);
#else
        return *p;
#endif
    }

    //! 返回底层数据的常引用，没有值则抛出异常
    const T &value() const
    {
        if (!has_value_)
        {
            throw std::runtime_error("Bad optional access");
        }

        const T *p = reinterpret_cast<const T *>(&storage_);
#if __cplusplus >= 201703L
        return *std::launder(p);
#else
        return *p;
#endif
    }

    //! 返回指向 value 的指针
    T *operator->()
    {
        return &this->value();
    }

    //! 返回指向 value 的常指针
    const T *operator->() const
    {
        return &this->value();
    }

    //! 返回 value 的引用
    T &operator*()
    {
        return this->value();
    }

    //! 返回 value 的常引用
    const T &operator*() const
    {
        return this->value();
    }

private:
    //! 是否有数据
    bool has_value_;
    //! 用来分配大小为 T 的内存块，并将其对齐到 T 的对齐方式
    typename std::aligned_storage<sizeof(T), alignof(T)>::type storage_;
};

//! 判断 a 是否为 NullOpt
template<class T>
bool operator==(const Optional<T> &a, NullOptT)
{
    return !a;
}

//! 判断 b 是否为 NullOpt
template<class T>
bool operator==(NullOptT, const Optional<T> &b)
{
    return !b;
}

//! 判断 a 是否为 nullptr
template<class T>
bool operator==(const Optional<T> &a, std::nullptr_t)
{
    return !a;
}

//! 判断 b 是否为 nullptr
template<class T>
bool operator==(std::nullptr_t, const Optional<T> &b)
{
    return !b;
}

//! 判断 a 非空且 a 指向的内容是否和 b 相等
template<class T>
bool operator==(const Optional<T> &a, const T &b)
{
    return a && *a == b;
}

//! 判断 b 非空且 a 和 b 指向的内容是否相等
template<class T>
bool operator==(const T &a, const Optional<T> &b)
{
    return b && a == *b;
}

//! 判断 a 指向的内容和 b 指向的内容是否不相等
template<class T>
bool operator!=(const Optional<T> &a, const Optional<T> &b)
{
    return !(a == b);
}

//! 判断 a 是否不是 nullptr
template<class T>
bool operator!=(const Optional<T> &a, std::nullptr_t)
{
    return !(a == nullptr);
}

//! 判断 b 是否不是 nullptr
template<class T>
bool operator!=(std::nullptr_t, const Optional<T> &b)
{
    return !(nullptr == b);
}

//! 判断 a 指向的内容是否不等于 b
template<class T>
bool operator!=(const Optional<T> &a, const T &b)
{
    return !(a == b);
}

//! 判断 a 是否不等于 b 指向的内容
template<class T>
bool operator!=(const T &a, const Optional<T> &b)
{
    return !(a == b);
}

} // namespace detail
} // namespace cucv

//! @} core_basic
