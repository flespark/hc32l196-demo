#ifndef __TINY_PRINTF_H
#define __TINY_PRINTF_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include "hal_lpuart.h"

#define PRINTF_DMA_QUEUE_SIZE 1024


int __wrap_printf(const char *format, ...);

int __wrap_sprintf(char *buffer, const char *format, ...);

int __wrap_snprintf(char *buffer, size_t count, const char *format, ...);

int __wrap_vprintf(const char *format, va_list va);

int __wrap_vsnprintf(char *buffer, size_t count, const char *format, va_list va);

void printf_dma_done_irq_handle(void);

bool print_isdone(void);

#endif /* __TINY_PRINTF_H */
