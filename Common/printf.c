#include <stdint.h>
#include <stdarg.h>

#if defined(LOG_INTERFACE_NONE)
int __wrap_printf(const char *format, ...)
{
    return 0;
}

// int __wrap_sprintf(char *str, const char *format, ...)
// {
// #error undef
// }

// int __wrap_snprintf(char *str, size_t size, const char *format, ...)
// {
// #error undef
// }

int __wrap_vprintf(const char *format, va_list ap)
{
    return 0;
}

// int __wrap_vsnprintf(char *str, size_t size, const char *format, va_list ap)
// {
// #error undef
// }

#elif defined(LOG_INTERFACE_USE_RTT)

#include "SEGGER_RTT.h"

int __wrap_printf(const char *format, ...)
{
    int     r;
    va_list ap;

    va_start(ap, format);
    r = SEGGER_RTT_vprintf(0, format, &ap);
    va_end(ap);
    return r;
}

// int __wrap_sprintf(char *str, const char *format, ...)
// {
// #error undef
// }

// int __wrap_snprintf(char *str, size_t size, const char *format, ...)
// {
// #error undef
// }

int __wrap_vprintf(const char *format, va_list ap)
{
    return SEGGER_RTT_vprintf(0, format, &ap);
}

// int __wrap_vsnprintf(char *str, size_t size, const char *format, va_list ap)
// {
// #error undef
// }
#endif /* LOG_INTERFACE */