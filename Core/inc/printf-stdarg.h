#ifndef __PRINTF_STDARG_H
#define __PRINTF_STDARG_H

#include <stdint.h>
#include <stdbool.h>

#define PRINTF_DMA_QUEUE_SIZE 1024
#define PRINTF_DMA_BUF_SIZE 128


extern char printf_dma_buf[PRINTF_DMA_BUF_SIZE];

void printf_dma_done_irq_handle(void);

bool print_isdone(void);

#endif /* __PRINTF_STDARG_H */
