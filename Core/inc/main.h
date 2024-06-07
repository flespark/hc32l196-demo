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
#include "uart.h"
#include "ddl.h"

#define LOG_TX_Pin GpioPin2
#define LOG_TX_Port GpioPortA

#define LOG_UART_SEL M0P_UART1
#define LOG_UART_AF GpioAf1
#define LOG_UART_CLOCK SysctrlPeripheralUart1
#define LOG_UART_RESET ResetMskUart1
#define LOG_UART_BAUDRATE 115200
#define LOG_UART_DMA_WIDTH 1
#define LOG_UART_DMA_CHANNEL DmaCh1
#define LOG_UART_DMA_IRQ DMAC_IRQn
#define LOG_UART_DMA_IRQ_LEVEL IrqLevel3
#define LOG_UART_DMA_TRIG DmaUart1TxTrig
#define LOG_UART_DMA_RESET ResetMskDma

extern void Error_Handler(void);

#endif /* __MAIN_H */
