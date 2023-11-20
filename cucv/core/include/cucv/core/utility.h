#pragma once

#include "cucvdef.h"
#include <algorithm>
#include <mutex>

namespace cucv
{

//! @addtogroup core_utils
//! @{

/**
 * @brief 自动分配的缓冲区类
 * @details 该类用于函数和方法中的临时缓冲区。如果一个临时缓冲区通常很小（几K的内存），但它的大小取决于参数，
 *          那么在栈区中创建一个小的固定大小的数组，并在它足够大时使用它。如果需要的缓冲区大小大于固定大小，
 *          另一个足够大的缓冲区被动态分配并在处理后释放。因此，在典型的情况下，当缓冲区的大小很小的时候，
 *          没有与malloc()/free()相关的开销。同时，对处理的数据大小也没有限制。
 */
#ifdef CUCV_ENABLE_MEMORY_SANITIZER
template <typename _Tp, size_t fixed_size = 0> class AutoBuffer
#else
template <typename _Tp, size_t fixed_size = 1024 / sizeof(_Tp) + 8> class AutoBuffer
#endif
{
public:
    typedef _Tp value_type;

    //! 默认构造函数
    AutoBuffer();
    //! 构造指定大小的缓冲区
    explicit AutoBuffer(size_t _size);

    //! 拷贝构造函数
    AutoBuffer(const AutoBuffer<_Tp, fixed_size> &buf);
    //! 赋值运算符
    AutoBuffer<_Tp, fixed_size> &operator=(const AutoBuffer<_Tp, fixed_size> &buf);

    //! 析构函数。调用 deallocate()
    ~AutoBuffer();

    //! 分配大小为_size的新缓冲区。如果_size足够小，则使用堆栈分配的缓冲区
    void allocate(size_t _size);
    //! 如果缓冲区是动态分配的，则释放该缓冲区
    void deallocate();
    //! 调整缓冲区的大小并保留内容
    void resize(size_t _size);
    //! 返回当前缓冲区的大小
    size_t size() const;
    //! 返回指向真正的缓冲区的指针，栈分配的或堆分配的
    inline _Tp *data()
    {
        return ptr;
    }
    //! 返回指向真实缓冲区的只读指针，栈分配的或堆分配的
    inline const _Tp *data() const
    {
        return ptr;
    }

#if !defined(CUCV_DISABLE_DEPRECATED_COMPATIBILITY) // 使用 .data() 代替
    //! 返回指向真正的缓冲区的指针，栈分配的或堆分配的
    operator _Tp *()
    {
        return ptr;
    }
    //! 返回指向真实缓冲区的只读指针，栈分配的或堆分配的
    operator const _Tp *() const
    {
        return ptr;
    }
#else
    //! 返回指定位置的元素的引用。在 Release 版本中不执行边界检查
    inline _Tp &operator[](size_t i)
    {
        CV_DbgCheckLT(i, sz, "out of range");
        return ptr[i];
    }
    //! 返回指定位置的元素的引用。在 Release 版本中不执行边界检查
    inline const _Tp &operator[](size_t i) const
    {
        CV_DbgCheckLT(i, sz, "out of range");
        return ptr[i];
    }
#endif

protected:
    //! 指向真实缓冲区的指针，如果缓冲区足够小，则可以指向 buf
    _Tp *ptr;
    //! 真实缓冲区的大小
    size_t sz;
    //! 预分配的缓冲区。至少有一个元素以满足 C++ 标准要求
    _Tp buf[(fixed_size > 0) ? fixed_size : 1];
};

extern "C" typedef int (*ErrorCallback)(int status, const char *func_name, const char *err_msg, const char *file_name,
                                        int line, void *userdata);

/**
 * @brief 设置新的错误处理程序和可选的用户数据。将由 cv::error() 调用
 * @param [in] errCallback  新的错误处理程序。如果为空，则使用默认的错误处理程序
 * @param [in] userdata 可选的用户数据指针，传递给回调函数
 * @param [out] prevUserdata 可选的输出参数，用于存储先前的用户数据指针
 * @return 先前的错误处理程序
 */
CUCV_API ErrorCallback redirectError(ErrorCallback errCallback, void *userdata = 0, void **prevUserdata = 0);

//! @} core_utils

//! @cond IGNORED

/////////////////////////////// AutoBuffer implementation ////////////////////////////////////////

template <typename _Tp, size_t fixed_size> inline AutoBuffer<_Tp, fixed_size>::AutoBuffer()
{
    ptr = buf;
    sz  = fixed_size;
}

template <typename _Tp, size_t fixed_size> inline AutoBuffer<_Tp, fixed_size>::AutoBuffer(size_t _size)
{
    ptr = buf;
    sz  = fixed_size;
    allocate(_size);
}

template <typename _Tp, size_t fixed_size>
inline AutoBuffer<_Tp, fixed_size>::AutoBuffer(const AutoBuffer<_Tp, fixed_size> &abuf)
{
    ptr = buf;
    sz  = fixed_size;
    allocate(abuf.size());
    for (size_t i = 0; i < sz; i++)
        ptr[i] = abuf.ptr[i];
}

template <typename _Tp, size_t fixed_size>
inline AutoBuffer<_Tp, fixed_size> &AutoBuffer<_Tp, fixed_size>::operator=(const AutoBuffer<_Tp, fixed_size> &abuf)
{
    if (this != &abuf)
    {
        deallocate();
        allocate(abuf.size());
        for (size_t i = 0; i < sz; i++)
            ptr[i] = abuf.ptr[i];
    }
    return *this;
}

template <typename _Tp, size_t fixed_size> inline AutoBuffer<_Tp, fixed_size>::~AutoBuffer()
{
    deallocate();
}

template <typename _Tp, size_t fixed_size> inline void AutoBuffer<_Tp, fixed_size>::allocate(size_t _size)
{
    if (_size <= sz)
    {
        sz = _size;
        return;
    }
    deallocate();
    sz = _size;
    if (_size > fixed_size)
    {
        ptr = new _Tp[_size];
    }
}

template <typename _Tp, size_t fixed_size> inline void AutoBuffer<_Tp, fixed_size>::deallocate()
{
    if (ptr != buf)
    {
        delete[] ptr;
        ptr = buf;
        sz  = fixed_size;
    }
}

template <typename _Tp, size_t fixed_size> inline void AutoBuffer<_Tp, fixed_size>::resize(size_t _size)
{
    if (_size <= sz)
    {
        sz = _size;
        return;
    }
    size_t i, prevsize = sz, minsize = std::min(prevsize, _size);
    _Tp *prevptr = ptr;

    ptr = _size > fixed_size ? new _Tp[_size] : buf;
    sz  = _size;

    if (ptr != prevptr)
        for (i = 0; i < minsize; i++)
            ptr[i] = prevptr[i];
    for (i = prevsize; i < _size; i++)
        ptr[i] = _Tp();

    if (prevptr != buf)
        delete[] prevptr;
}

template <typename _Tp, size_t fixed_size> inline size_t AutoBuffer<_Tp, fixed_size>::size() const
{
    return sz;
}

//! @endcond

typedef std::recursive_mutex Mutex;
typedef std::lock_guard<cucv::Mutex> AutoLock;

} // namespace cucv
