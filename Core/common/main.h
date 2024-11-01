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
#include "lpuart.h"
#include "hal_gpio.h"

#define LOG_TX_Pin               PA_0
#define LOG_LPUART_SEL           M0P_LPUART1
#define LOG_LPUART_AF            HAL_GPIO_AF_DIGITAL_2
#define LOG_LPUART_CLOCK         SysctrlPeripheralLpUart1
#define LOG_LPUART_RESET         ResetMskLpUart1
#define LOG_LPUART_BAUDRATE      115200
#define LOG_LPUART_CLOCK_DIV     LPUartMsk8Or16Div
#define LOG_LPUART_DMA_WIDTH     1
#define LOG_LPUART_DMA_CHANNEL   DmaCh1
#define LOG_LPUART_DMA_IRQ       DMAC_IRQn
#define LOG_LPUART_DMA_IRQ_LEVEL IrqLevel2
#define LOG_LPUART_DMA_TRIG      DmaLpUart1TxTrig
#define LOG_LPUART_DMA_RESET     ResetMskDma

#define LCD_COM0_Pin  PA_9
#define LCD_COM1_Pin  PA_10
#define LCD_COM2_Pin  PA_11
#define LCD_COM3_Pin  PA_12
#define LCD_SEG0_Pin  PA_8
#define LCD_SEG1_Pin  PC_9
#define LCD_SEG2_Pin  PC_8
#define LCD_SEG3_Pin  PC_7
#define LCD_SEG4_Pin  PC_6
#define LCD_SEG5_Pin  PB_15
#define LCD_SEG6_Pin  PB_14
#define LCD_SEG7_Pin  PB_13
#define LCD_VLCDH_Pin PB_3
#define LCD_VLCD3_Pin PB_4
#define LCD_VLCD2_Pin PB_5
#define LCD_VLCD1_Pin PB_6

#define BSP_KEY_Pin      PA_7
#define BSP_KEY_PP_MODE  HAL_GPIO_PULL_MODE_UP
#define BSP_KEY_IRQ_MODE HAL_GPIO_IRQ_MODE_FALLING
#define BSP_LED_Pin      PD_14
#define BSP_LED_PP_MODE  HAL_GPIO_PULL_MODE_NONE

extern void lcd_display_time(void);
extern void lcd_mask_colon(void);

extern void Error_Handler(void);

#endif /* __MAIN_H */
