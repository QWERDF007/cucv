#pragma once

#include "tensor_layout.h"

//! @addtogroup core_basic
//! @{

namespace cucv
{

//! Tensor 布局
class TensorLayout final
{
public:
    using const_iterator = const char *;
    using iterator       = const_iterator;
    using value_type     = char;

    //! 默认构造函数
    TensorLayout() = default;

    //! 通过布局构造
    constexpr TensorLayout(const TensorLayout_t &layout) : layout_(layout)
    {
    }

    //! 通过布局描述字符串构造
    explicit TensorLayout(const char *descr)
    {
        TensorLayoutMake(descr, &layout_);
        // detail::CheckThrow();
    }

    //! 通过随机访问迭代器构造，传入的迭代器不能被推导为随机访问迭代器时不会被定义
    template<class IT, class = detail::IsRandomAccessIterator<IT>>
    explicit TensorLayout(IT itbeg, IT itend)
    {
        TensorLayoutMakeRange(&*itbeg, &*itend, &layout_);
        // detail::CheckThrow();
    }

    //! 返回给定索引的布局标签
    constexpr char operator[](int idx) const;

    //! 返回给定 Tensor 布局的秩 (维数)
    constexpr int rank() const;

    //! 查找给定标签在输入的布局中的维度索引
    int find(char dim_label, int start = 0) const;

    //! 判断布局是是否以给定布局开头
    bool startsWith(const TensorLayout &test) const
    {
        return TensorLayoutStartsWith(layout_, test.layout_) != 0;
    }

    //! 判断布局是是否以给定布局结尾
    bool endsWith(const TensorLayout &test) const
    {
        return TensorLayoutEndsWith(layout_, test.layout_) != 0;
    }

    //! 从布局的子范围创建一个布局
    TensorLayout subRange(int begin, int end) const
    {
        TensorLayout out;
        TensorLayoutMakeSubRange(layout_, begin, end, &out.layout_);
        // detail::CheckThrow();
        return out;
    }

    //! 从布局的前 n 个标签创建一个布局
    TensorLayout first(int n) const
    {
        TensorLayout out;
        TensorLayoutMakeFirst(layout_, n, &out.layout_);
        // detail::CheckThrow();
        return out;
    }

    //! 从布局的后 n 个标签创建一个布局
    TensorLayout last(int n) const
    {
        TensorLayout out;
        TensorLayoutMakeLast(layout_, n, &out.layout_);
        // detail::CheckThrow();
        return out;
    }

    //! 比较两个布局是否相等
    friend bool operator==(const TensorLayout &a, const TensorLayout &b);

    //! 比较两个布局是否不相等
    bool operator!=(const TensorLayout &that) const;

    //! 比较布局是否小于另一个布局
    bool operator<(const TensorLayout &that) const;

    constexpr const_iterator begin() const;
    constexpr const_iterator end() const;
    constexpr const_iterator cbegin() const;
    constexpr const_iterator cend() const;

    //! 返回 Tensor 布局底层 C 数据的常引用
    constexpr operator const TensorLayout_t &() const;

    //! 打印布局信息到输出流
    friend std::ostream &operator<<(std::ostream &out, const TensorLayout &that);

    // Tensor 布局底层 C 数据
    TensorLayout_t layout_;
};

constexpr char TensorLayout::operator[](int idx) const
{
    return TensorLayoutGetLabel(layout_, idx);
}

constexpr int TensorLayout::rank() const
{
    return TensorLayoutGetNumDim(layout_);
}

inline int TensorLayout::find(char dim_label, int start) const
{
    return TensorLayoutFindDimIndex(layout_, dim_label, start);
}

constexpr TensorLayout::operator const TensorLayout_t &() const
{
    return layout_;
}

inline bool operator==(const TensorLayout &a, const TensorLayout &b)
{
    return a.layout_ == b.layout_;
}

inline bool TensorLayout::operator!=(const TensorLayout &that) const
{
    return !(*this == that);
}

inline bool TensorLayout::operator<(const TensorLayout &that) const
{
    return layout_ < that.layout_;
}

constexpr auto TensorLayout::begin() const -> const_iterator
{
    return TensorLayoutGetName(&layout_);
}

constexpr inline auto TensorLayout::end() const -> const_iterator
{
    return this->begin() + this->rank();
}

constexpr auto TensorLayout::cbegin() const -> const_iterator
{
    return this->begin();
}

constexpr auto TensorLayout::cend() const -> const_iterator
{
    return this->end();
}

inline std::ostream &operator<<(std::ostream &out, const TensorLayout &that)
{
    return out << that.layout_;
}

inline bool operator==(const TensorLayout &lhs, const TensorLayout_t &rhs)
{
    return TensorLayoutCompare(lhs.layout_, rhs) == 0;
}

inline bool operator!=(const TensorLayout &lhs, const TensorLayout_t &rhs)
{
    return !operator==(lhs, rhs);
}

inline bool operator<(const TensorLayout &lhs, const TensorLayout_t &rhs)
{
    return TensorLayoutCompare(lhs.layout_, rhs) < 0;
}

} // namespace cucv

//! @} core_basic