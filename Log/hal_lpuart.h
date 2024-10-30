#ifndef __HAL_LPUART_H
#define __HAL_LPUART_H

#include "lpuart.h"

#define PRINTF_DMA_BUF_SIZE 128

extern char printf_dma_buf[PRINTF_DMA_BUF_SIZE];

void hal_lpuart_init(void);

void hal_lpuart_dma_config(void);

#endif /* __HAL_LPUART_H */
