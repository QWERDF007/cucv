/**
 * @file tensor_layout.h
 * @brief 表示 Tensor 布局数据的 C 数据结构和一些操作、访问数据的函数
 * @version 0.1
 * @date 2023-04-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include "cucvdef.h"
#include "detail/concepts.h"
#include "status.hpp"
#include <cstdint>

//! @addtogroup core_basic
//! @{

namespace cucv
{

//! Tensor 的维度的最大值
#define TENSOR_MAX_RANK (15)

//! Tensor 布局，分配标签给每个维度，诸如 NCHW 或者 NHWC 等
typedef struct TensorLayoutRec
{
    //! Tensor 布局标签
    char data[TENSOR_MAX_RANK + 1]; // +1 for '\0'
    //! Tensor 布局的秩 (维数)
    int32_t rank;
} TensorLayout_t;

enum TensorLabel : char
{
    TLABEL_BATCH   = 'N', //< batch size `N`
    TLABEL_CHANNEL = 'C', //< channel    `C`
    TLABEL_FRAME   = 'F', //< frame      `F`
    TLABEL_DEPTH   = 'D', //< depth      `D`
    TLABEL_HEIGHT  = 'H', //< height     `H`
    TLABEL_WIDTH   = 'W'  //< width      `W`
};

// clang-format off

//! 通过 layout 构建 TensorLayout_t
#define TENSOR_LAYOUT_MAKE(layout)                                                                                     \
TensorLayout_t                                                                                                         \
{                                                                                                                      \
    layout, sizeof(layout) - 1                                                                                         \
}

//! 空 Tensor 布局
constexpr static const TensorLayout_t TENSOR_NONE = TENSOR_LAYOUT_MAKE("");

//! Tensor 的隐式表示方式
constexpr static const TensorLayout_t TENSOR_IMPLICIT[7] =
{
    // 不能直接使用 TENSOR_* 标识符，clang 会抱怨它们不是编译时常量。
    // 必须使用 TENSOR_LAYOUT_MAKE 宏
    TENSOR_LAYOUT_MAKE(""), // none
    TENSOR_LAYOUT_MAKE("W"),
    TENSOR_LAYOUT_MAKE("HW"),
    TENSOR_LAYOUT_MAKE("NHW"),
    TENSOR_LAYOUT_MAKE("NCHW"),
    TENSOR_LAYOUT_MAKE("NCDHW"),
    TENSOR_LAYOUT_MAKE("NCFDHW"),
};

// clang-format on

/**
 * @brief 从布局标签字符串创建一个 Tensor 布局
 *
 * @param [in] descr Tensor 布局标签字符串，例如 NCHW
 * @return Tensor 布局
 */
TensorLayout_t CreateLayout(const char *descr);

/**
 * @brief 从布局标签的子范围创建一个 Tensor 布局
 *
 * @param [in] begin Tensor 布局标签范围起始地址
 * @param [in] end Tensor 布局标签范围终止地址
 * @return Tensor 布局
 */
TensorLayout_t CreateLayout(const char *begin, const char *end);

/**
 * @brief 从输入的布局的前 n 个标签创建一个 Tensor 布局
 *
 * @param [in] layout 输入的布局
 * @param [in] n 标签数量
 * @return Tensor 布局
 */
TensorLayout_t CreateFirst(const TensorLayout_t &layout, int n);

/**
 * @brief 从输入的布局的后 n 个标签创建一个 Tensor 布局
 *
 * @param [in] layout 输入的布局
 * @param [in] n 标签数量
 * @return Tensor 布局
 */
TensorLayout_t CreateLast(const TensorLayout_t &layout, int n);

/**
 * @brief 从输入的布局的子范围创建一个 Tensor 布局
 *
 * @param [in] layout 输入的布局
 * @param [in] begin 起始位置
 * @param [in] end 终止位置
 * @return Tensor 布局
 */
TensorLayout_t CreateSubRange(const TensorLayout_t &layout, int begin, int end);

/**
 * @brief 从标签字符串中创建 Tensor 布局，维数取决于字符串长度。
 *
 * @param [in] descr c 字符串 ('\0' 结尾)
 *              + 最多不超过 \ref TENSOR_MAX_RANK 个字符
 * @param [out] layout 输出的布局
 * @retval #CUCV_ERROR_INVALID_ARGUMENT 部分参数超出范围
 * @retval #CUCV_SUCCESS                操作执行成功
 */
CUCV_API Status TensorLayoutMake(const char *descr, TensorLayout_t *layout);

/**
 * @brief 从标签范围创建 Tensor 布局，维数取决于标签范围的长度
 *
 * @param [in] begin 标签范围起始地址
 *                   + 必须非 NULL
 *                   + 最多不超过 \ref TENSOR_MAX_RANK 个字符
 * @param [in] end 标签范围终止地址
 *                 + 必须非 NULL
 *                 + 最多不超过 \ref TENSOR_MAX_RANK 个字符
 * @param [out] layout 输出的布局
 * @retval #CUCV_ERROR_INVALID_ARGUMENT 部分参数超出范围
 * @retval #CUCV_SUCCESS                操作执行成功
 */
CUCV_API Status TensorLayoutMakeRange(const char *begin, const char *end, TensorLayout_t *layout);

/**
 * @brief 从输入的布局的前 n 个标签制作 Tensor 布局
 *
 * @param [in] in 输入的布局
 * @param [in] n 标签数量
 *               - 如果为 0，则返回空布局
 *               - 如果为负，则返回最后 -n 个标签
 *               - 如果 >= rank 或 <= -rank，则返回输入布局的副本
 * @param [out] out 输出的布局
 * @retval #CUCV_ERROR_INVALID_ARGUMENT 部分参数超出范围
 * @retval #CUCV_SUCCESS                操作执行成功
 */
CUCV_API Status TensorLayoutMakeFirst(TensorLayout_t in, int32_t n, TensorLayout_t *out);

/**
 * @brief 从输入的布局的后 n 个标签制作 Tensor 布局
 *
 * @param [in] in 输入的布局
 * @param [in] n 标签数量
 *          - 如果为 0，则返回空布局
 *          - 如果为负，则返回前 -n 个标签
 *          - 如果 >= rank 或 <= -rank，则返回输入布局的副本
 * @param [out] out 输出的布局
 * @retval #CUCV_ERROR_INVALID_ARGUMENT 部分参数超出范围
 * @retval #CUCV_SUCCESS                操作执行成功
 */
CUCV_API Status TensorLayoutMakeLast(TensorLayout_t in, int32_t n, TensorLayout_t *out);

/**
 * @brief 从输入的布局的子范围的标签构建一个 Tensor 布局
 *
 * @param [in] in 输入的布局
 * @param [in] begin 输入的布局的子范围的起始位置
 * @param [in] end 输入的布局的子范围的终止位置
 * @param [out] out  输出的布局
 * @retval #CUCV_ERROR_INVALID_ARGUMENT 部分参数超出范围
 * @retval #CUCV_SUCCESS                操作执行成功
 */
CUCV_API Status TensorLayoutMakeSubRange(TensorLayout_t in, int32_t begin, int32_t end, TensorLayout_t *out);

/**
 * @brief 返回给定标签在输入的布局中的维度索引
 *
 * @param [in] layout 要搜索的布局
 * @param [in] dim_label 要查询的标签
 * @param [in] idx_start 开始搜索的索引
 * @return 给定标签在布局中的维度索引，-1 表示没找到
 */
inline static int32_t TensorLayoutFindDimIndex(TensorLayout_t layout, char dim_label, int idx_start)
{
    if (idx_start < 0)
    {
        idx_start = layout.rank + idx_start;
    }

    int n = layout.rank - idx_start;
    if (n > 0)
    {
        void *p = memchr(layout.data + idx_start, dim_label, n);
        if (p != NULL)
        {
            return (int32_t)((char *)p - (char *)layout.data);
        }
    }
    return -1;
}

/**
 * @brief 返回给定索引的布局标签
 *
 * @param layout 要查询布局
 * @param idx 标签的索引
 * @return 如果 @p idx >= 0 或者 < @p layout 的大小，返回对应的标签，否则返回 '\0'
 */
constexpr inline static char TensorLayoutGetLabel(TensorLayout_t layout, int idx)
{
    // Must be all a single statement for C++11 compatibility
    return idx < 0 ? (0 <= layout.rank + idx && layout.rank + idx < layout.rank ? layout.data[layout.rank + idx] : '\0')
                   : (0 <= idx && idx < layout.rank ? layout.data[idx] : '\0');
}

/**
 * @brief 返回 Tensor 布局的维数
 *
 * @param layout 要查询的布局
 * @return 给定 Tensor 布局的维数
 */
constexpr inline static int32_t TensorLayoutGetNumDim(TensorLayout_t layout)
{
    return layout.rank;
}

/**
 * @brief 比较两个布局
 *
 * @param [in] a 要比较的布局
 * @param [in] b 要比较的布局
 * @retval <0, @p a < @p b
 * @retval 0, @p a == @p b
 * @retval >0, @p a > @p b
 */
inline static int32_t TensorLayoutCompare(TensorLayout_t a, TensorLayout_t b)
{
    if (a.rank == b.rank)
    {
        return memcmp(a.data, b.data, a.rank);
    }
    else
    {
        return a.rank - b.rank;
    }
}

/**
 * @brief 判断 Tensor 布局是是否以给定布局开头。如果给定布局为空，则始终认为在开头找到
 *
 * @param [in] layout 要查询的布局
 * @param [in] test 要找到的布局
 * @return 找到返回非 0，否则返回 0
 */
inline static int32_t TensorLayoutStartsWith(TensorLayout_t layout, TensorLayout_t test)
{
    if (test.rank <= layout.rank)
    {
        return memcmp(test.data, layout.data, test.rank) == 0;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief 判断 Tensor 布局是是否以给定布局结尾。如果给定布局为空，则始终认为在末尾找到
 *
 * @param [in] layout 要查询的布局
 * @param [in] test 要找到的布局
 * @return 找到返回非 0，否则返回 0
 */
inline static int32_t TensorLayoutEndsWith(TensorLayout_t layout, TensorLayout_t test)
{
    if (test.rank <= layout.rank)
    {
        return memcmp(test.data, layout.data + layout.rank - test.rank, test.rank) == 0;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief 返回 Tensor 布局的字符串表现形式
 *
 * @param layout 输入的布局
 * @return 具有布局名称的以 '\0' 结尾的字符串
 */
constexpr inline static const char *TensorLayoutGetName(const TensorLayout_t *layout)
{
    return layout == NULL ? "" : layout->data;
}

/**
 * @brief 查找指定维度标签在 Tensor 布局中的位置，如果找到了该维度标签，
 *        则返回其在张量布局中的位置，否则返回 -1
 *
 * @param [in] layout 输入的布局
 * @param [in] dim_label 要查找的维度标签
 * @return 指定维度标签在 Tensor 布局中的位置，-1 表示未找到指定标签
 */
int FindDimIndex(const TensorLayout_t &layout, char dim_label);

/**
 * @brief 判断指定布局是否为 channel last 格式
 *
 * @param [in] layout 输入的布局
 */
bool IsChannelLast(const TensorLayout_t &layout);

//! 比较两个布局是否相等
inline bool operator==(const TensorLayout_t &lhs, const TensorLayout_t &rhs)
{
    return TensorLayoutCompare(lhs, rhs) == 0;
}

//! 比较两个布局是否不相等
inline bool operator!=(const TensorLayout_t &lhs, const TensorLayout_t &rhs)
{
    return !operator==(lhs, rhs);
}

//! 比较布局是否小于另一布局
inline bool operator<(const TensorLayout_t &lhs, const TensorLayout_t &rhs)
{
    return TensorLayoutCompare(lhs, rhs) < 0;
}

//! 打印布局信息到输出流
inline std::ostream &operator<<(std::ostream &out, const TensorLayout_t &layout)
{
    return out << TensorLayoutGetName(&layout);
}

} // namespace cucv

//! @} core_basic