#include "cucv/core/base.hpp"
#include "cucv/core/precomp.h"
#include "cucv/core/status.hpp"
#include "cucv/core/utility.h"
#include "cucv/core/version.h"

// #include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <unordered_map>

namespace cucv
{

static Mutex *__initialization_mutex = NULL;
Mutex &getInitializationMutex()
{
    if (__initialization_mutex == NULL)
    {
        __initialization_mutex = new Mutex();
    }
    return *__initialization_mutex;
}
// force initialization (single-threaded environment)
Mutex *__initialization_mutex_initializer = &getInitializationMutex();

} // namespace cucv

#ifndef CUCV_ERROR_SET_TERMINATE_HANDLER // build config option
#if defined(_WIN32)
#define CUCV_ERROR_SET_TERMINATE_HANDLER 1
#endif
#endif
#if defined(CUCV_ERROR_SET_TERMINATE_HANDLER) && !CUCV_ERROR_SET_TERMINATE_HANDLER
#undef CUCV_ERROR_SET_TERMINATE_HANDLER
#endif

namespace cucv
{

std::string format(const char *fmt, ...)
{
    AutoBuffer<char, 1024> buf;

    for (;;)
    {
        va_list va;
        va_start(va, fmt);
        int bsize = static_cast<int>(buf.size());
        int len = cucv_vsnprintf(buf.data(), bsize, fmt, va);
        va_end(va);

        CUCV_Assert(len >= 0 && "Check format string for errors");
        if (len >= bsize)
        {
            buf.resize(len + 1);
            continue;
        }
        buf[bsize - 1] = 0;
        return std::string(buf.data(), static_cast<size_t>(len));
    }
}

Exception::Exception()
{
    code = 0;
    line = 0;
}

Exception::Exception(int _code, const std::string &_err, const std::string &_func, const std::string &_file, int _line)
    : code(_code), err(_err), func(_func), file(_file), line(_line)
{
    formatMessage();
}

Exception::~Exception() throw()
{
}

// TODO: 返回异常产生时的调用栈
const char *Exception::what() const throw()
{
    return msg.c_str();
}

void Exception::formatMessage()
{
    size_t pos = err.find('\n');
    bool multiline = pos != std::string::npos;
    if (multiline)
    {
        std::stringstream ss;
        size_t prev_pos = 0;
        while (pos != std::string::npos)
        {
            ss << "> " << err.substr(prev_pos, pos - prev_pos) << std::endl;
            prev_pos = pos + 1;
            pos = err.find('\n', prev_pos);
        }
        ss << "> " << err.substr(prev_pos);
        if (err[err.size() - 1] != '\n')
            ss << std::endl;
        err = ss.str();
    }
    if (func.size() > 0)
    {
        if (multiline)
        {
            msg = format("CUCV(%s) %s:%d: error: (%d:%s) in function '%s'\n%s", CUCV_VERSION_STRING, file.c_str(), line,
                         code, getErrorStr(code), func.c_str(), err.c_str());
        }
        else
        {
            msg = format("CUCV(%s) %s:%d: error: (%d:%s) %s in function '%s'\n", CUCV_VERSION_STRING, file.c_str(),
                         line, code, getErrorStr(code), err.c_str(), func.c_str());
        }
    }
    else
    {
        msg = format("CUCV(%s) %s:%d: error: (%d:%s) %s%s", CUCV_VERSION_STRING, file.c_str(), line, code,
                     getErrorStr(code), err.c_str(), multiline ? "" : "\n");
    }
}

static bool param_dumpErrors = true;
static ErrorCallback customErrorCallback = 0;
static void *customErrorCallbackData = 0;
static bool breakOnError = false;

bool setBreakOnError(bool value)
{
    bool prevVal = breakOnError;
    breakOnError = value;
    return prevVal;
}

int cucv_snprintf(char *buf, int len, const char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    int res = cucv_vsnprintf(buf, len, fmt, va);
    va_end(va);
    return res;
}

int cucv_vsnprintf(char *buf, int len, const char *fmt, va_list args)
{
#if defined _MSC_VER
    if (len <= 0)
        return len == 0 ? 1024 : -1;
    int res = _vsnprintf_s(buf, len, _TRUNCATE, fmt, args);
    // ensure null terminating on VS
    if (res >= 0 && res < len)
    {
        buf[res] = 0;
        return res;
    }
    else
    {
        buf[len - 1] = 0; // truncate happened
        return res >= len ? res : (len * 2);
    }
#else
    return vsnprintf(buf, len, fmt, args);
#endif
}

static void dumpException(const Exception &exc)
{
    // const char *errorStr = cvErrorStr(exc.code);
    const char *errorStr = "";
    char buf[1 << 12];

    cucv_snprintf(buf, sizeof(buf), "CUCV(%s) Error: %s (%s) in %s, file %s, line %d", CUCV_VERSION_STRING, errorStr,
                  exc.err.c_str(), exc.func.size() > 0 ? exc.func.c_str() : "unknown function", exc.file.c_str(),
                  exc.line);
    fflush(stdout);
    fflush(stderr);
    fprintf(stderr, "%s\n", buf);
    fflush(stderr);
}

#ifdef CUCV_ERROR_SET_TERMINATE_HANDLER
static bool cucv_terminate_handler_installed = false;
static std::terminate_handler cucv_old_terminate_handler;
static cucv::Exception cucv_terminate_handler_exception;
static void cucv_terminate_handler()
{
    std::cerr << "CUCV: terminate handler is called! The last CUCV error is:\n";
    dumpException(cucv_terminate_handler_exception);
    if (false /*cv_old_terminate_handler*/) // buggy behavior is observed with doubled "abort/retry/ignore" windows
        cucv_old_terminate_handler();
    abort();
}

#endif

#ifdef __GNUC__
#if defined __clang__ || defined __APPLE__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-noreturn"
#endif
#endif

void error(const Exception &exc)
{
#ifdef CUCV_ERROR_SET_TERMINATE_HANDLER
    {
        cucv::AutoLock lock(getInitializationMutex());
        if (!cucv_terminate_handler_installed)
        {
            cucv_old_terminate_handler = std::set_terminate(cucv_terminate_handler);
            cucv_terminate_handler_installed = true;
        }
        cucv_terminate_handler_exception = exc;
    }
#endif

    if (customErrorCallback != 0)
        customErrorCallback(exc.code, exc.func.c_str(), exc.err.c_str(), exc.file.c_str(), exc.line,
                            customErrorCallbackData);
    else if (param_dumpErrors)
    {
        dumpException(exc);
    }

    if (breakOnError)
    {
        static volatile int *p = 0;
        *p = 0;
    }

    throw exc;
#ifdef __GNUC__
#if !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
#endif
#endif
}

void error(int _code, const std::string &_err, const char *_func, const char *_file, int _line)
{
    error(cucv::Exception(_code, _err, _func, _file, _line));
#ifdef __GNUC__
#if !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
#endif
#endif
}

ErrorCallback redirectError(ErrorCallback errCallback, void *userdata, void **prevUserdata)
{
    if (prevUserdata)
        *prevUserdata = customErrorCallbackData;

    ErrorCallback prevCallback = customErrorCallback;

    customErrorCallback = errCallback;
    customErrorCallbackData = userdata;

    return prevCallback;
}

} // namespace cucv
