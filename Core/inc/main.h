#ifndef __MAIN_H
#define __MAIN_H

#include "sysctrl.h"
#include "flash.h"
#include "reset.h"
#include "gpio.h"
#include "rtc.h"
#include "dmac.h"
#include "lpm.h"
#include "lcd.h"
#include "ddl.h"

#define LOG_TX_Pin GpioPin0
#define LOG_TX_Port GpioPortA

#define LOG_LPUART_SEL M0P_LPUART1
#define LOG_LPUART_AF GpioAf2
#define LOG_LPUART_CLOCK SysctrlPeripheralLpUart1
#define LOG_LPUART_RESET ResetMskLpUart1
#define LOG_LPUART_BAUDRATE 115200
#define LOG_LPUART_CLOCK_DIV LPUartMsk8Or16Div
#define LOG_LPUART_DMA_WIDTH 1
#define LOG_LPUART_DMA_CHANNEL DmaCh1
#define LOG_LPUART_DMA_IRQ DMAC_IRQn
#define LOG_LPUART_DMA_IRQ_LEVEL IrqLevel2
#define LOG_LPUART_DMA_TRIG DmaLpUart1TxTrig
#define LOG_LPUART_DMA_RESET ResetMskDma

extern void Error_Handler(void);

#endif /* __MAIN_H */
