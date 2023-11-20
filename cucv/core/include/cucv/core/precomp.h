#pragma once

#include <cstdarg>

namespace cucv
{

int cucv_snprintf(char *buf, int len, const char *fmt, ...);
int cucv_vsnprintf(char *buf, int len, const char *fmt, va_list args);

} // namespace cucv
