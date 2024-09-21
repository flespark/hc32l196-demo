#ifndef __HAL_LPUART_H
#define __HAL_LPUART_H

#include "lpuart.h"

#define PRINTF_DMA_BUF_SIZE 128

void Hal_UartInit(void);

#ifdef LOG_LPUART_USE_DMA

void Hal_UartDmaCfg(void);
extern char printf_dma_buf[PRINTF_DMA_BUF_SIZE];

#endif /* LOG_LPUART_USE_DMA */

#endif /* __HAL_LPUART_H */
