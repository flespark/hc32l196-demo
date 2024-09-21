#ifndef __PRINTF_H
#define __PRINTF_H

#include <stdint.h>
#include <stdarg.h>

int sys_log_init(void);

#if defined(LOG_INTERFACE_NONE) || defined(LOG_INTERFACE_USE_RTT)

int __wrap_printf(const char *format, ...);

int __wrap_vprintf(const char *format, va_list ap);

#elif defined(LOG_INTERFACE_USE_LPUART)

#include "tiny_printf.h"

#endif /* LOG_INTERFACE */

#endif /* __PRINTF_H */