#include "cucv/core/tensor_layout.h"
#include "cucv/core/base.hpp"
#include <sstream>

namespace cucv
{

TensorLayout_t CreateLayout(const char *descr)
{
    TensorLayout_t out;

    if (descr == nullptr)
    {
        out = {};
    }
    else
    {
        const char *cur = descr;
        for (int i = 0; i < TENSOR_MAX_RANK && *cur; ++i, ++cur)
        {
            out.data[i] = *cur;
        }

        if (*cur != '\0')
        {
            // 避免遍历整个 descr 缓冲区，这可能会构成安全隐患
            char buf[32];
            strncpy(buf, descr, 31);
            buf[31] = 0;
            std::stringstream msg;
            msg << "Tensor layout description is too big, must have at most 16 labels: " << buf
                << (strlen(buf) <= 31 ? "" : "...");
            CUCV_Error(CUCV_ERROR_INVALID_ARGUMENT, msg.str());
        }

        out.rank = cur - descr;
        CUCV_Assert(0 <= out.rank && (size_t)out.rank < sizeof(out.data) / sizeof(out.data[0]));
        out.data[out.rank] = '\0'; // add null terminator
    }
    return out;
}

TensorLayout_t CreateLayout(const char *begin, const char *end)
{
    if (begin == nullptr || end == nullptr)
    {
        CUCV_Error(CUCV_ERROR_INVALID_ARGUMENT, "Range pointers must not be NULL");
    }

    if (end - begin < 0)
    {
        CUCV_Error(CUCV_ERROR_INVALID_ARGUMENT, "Range must not have negative length");
    }

    if (end - begin > TENSOR_MAX_RANK)
    {
        std::stringstream msg;
        msg << "Range length " << end - begin << " too large, must be <= " << TENSOR_MAX_RANK;
        CUCV_Error(CUCV_ERROR_INVALID_ARGUMENT, msg.str());
    }

    TensorLayout_t out;
    out.rank = end - begin;
    memcpy(out.data, begin, static_cast<size_t>(out.rank));

    return out;
}

TensorLayout_t CreateFirst(const TensorLayout_t &layout, int n)
{
    if (n >= 0)
    {
        TensorLayout_t out;
        out.rank = std::min(n, layout.rank);
        memcpy(out.data, layout.data, static_cast<size_t>(out.rank));
        return out;
    }
    else
    {
        return CreateLast(layout, -n);
    }
}

TensorLayout_t CreateLast(const TensorLayout_t &layout, int n)
{
    if (n >= 0)
    {
        TensorLayout_t out;
        out.rank = std::min(n, layout.rank);
        memcpy(out.data, layout.data + layout.rank - out.rank, out.rank);
        return out;
    }
    else
    {
        return CreateFirst(layout, -n);
    }
}

TensorLayout_t CreateSubRange(const TensorLayout_t &layout, int begin, int end)
{
    if (begin < 0)
    {
        begin = std::max(0, layout.rank + begin);
    }
    else
    {
        begin = std::min(begin, layout.rank);
    }

    if (end < 0)
    {
        end = std::max(0, layout.rank + end);
    }
    else
    {
        end = std::min(end, layout.rank);
    }

    TensorLayout_t out;

    out.rank = end - begin;
    if (out.rank > 0)
    {
        memcpy(out.data, layout.data + begin, out.rank);
    }
    else
    {
        out.rank = 0;
    }

    return out;
}

int FindDimIndex(const TensorLayout_t &layout, char dim_label)
{
    if (const void *p = memchr(layout.data, dim_label, layout.rank))
    {
        return std::distance(reinterpret_cast<const std::byte *>(layout.data), reinterpret_cast<const std::byte *>(p));
    }
    else
    {
        return -1;
    }
}

bool IsChannelLast(const TensorLayout_t &layout)
{
    return layout.rank == 0 || layout.data[layout.rank - 1] == 'C';
}

Status TensorLayoutMake(const char *descr, TensorLayout_t *layout)
{
    if (layout == nullptr)
    {
        CUCV_Error(CUCV_ERROR_INVALID_ARGUMENT, "Pointer to layout output must not be NULL");
    }
    CreateLayout(descr);
}

Status TensorLayoutMakeRange(const char *begin, const char *end, TensorLayout_t *layout)
{
    if (layout == nullptr)
    {
        CUCV_Error(CUCV_ERROR_INVALID_ARGUMENT, "Pointer to layout output must not be NULL");
    }
    CreateLayout(begin, end);
}

Status TensorLayoutMakeFirst(TensorLayout_t in, int32_t n, TensorLayout_t *out)
{
    if (out == nullptr)
    {
        CUCV_Error(CUCV_ERROR_INVALID_ARGUMENT, "Pointer to layout output must not be NULL");
    }
    *out = CreateFirst(in, n);
}

Status TensorLayoutMakeLast(TensorLayout_t in, int32_t n, TensorLayout_t *out)
{
    if (out == nullptr)
    {
        CUCV_Error(CUCV_ERROR_INVALID_ARGUMENT, "Pointer to layout output must not be NULL");
    }
    *out = CreateLast(in, n);
}

Status TensorLayoutMakeSubRange(TensorLayout_t in, int32_t begin, int32_t end, TensorLayout_t *out)
{
    if (out == nullptr)
    {
        CUCV_Error(CUCV_ERROR_INVALID_ARGUMENT, "Pointer to layout output must not be NULL");
    }
    *out = CreateSubRange(in, begin, end);
}

} // namespace cucv
