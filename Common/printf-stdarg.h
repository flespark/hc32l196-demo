#ifndef __PRINTF_STDARG_H
#define __PRINTF_STDARG_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

#define PRINTF_DMA_QUEUE_SIZE 1024
#define PRINTF_DMA_BUF_SIZE 128


extern char printf_dma_buf[PRINTF_DMA_BUF_SIZE];

int __wrap_printf(const char *format, ...);

int __wrap_sprintf(char *buffer, const char *format, ...);

int __wrap_snprintf(char *buffer, size_t count, const char *format, ...);

int __wrap_vprintf(const char *format, va_list va);

int __wrap_vsnprintf(char *buffer, size_t count, const char *format, va_list va);

void printf_dma_done_irq_handle(void);

bool print_isdone(void);

#endif /* __PRINTF_STDARG_H */
