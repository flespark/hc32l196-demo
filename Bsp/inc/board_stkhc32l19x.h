/******************************************************************************
 * Copyright (C) 2021, Xiaohua Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by XHSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************/

/******************************************************************************
 * @file   board_stkhc32l19x.h
 *
 * @brief  Header file for BSP functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/
#ifndef __BOARD_STKHC32L19X_H__
#define __BOARD_STKHC32L19X_H__

///< STK GPIO DEFINE
///< USER KEY
#if defined ( __HC32_HOLOSENSOR_ )
#define STK_USER_PORT       GpioPortB
#define STK_USER_PIN        GpioPin2
#define STK_USER_IRQn       PORTB_IRQn
#define PortGPIO_IRQHandler PortB_IRQHandler
///< LED
#define STK_LED_PORT        GpioPortA
#define STK_LED_PIN         GpioPin3
#else
#define STK_USER_PORT       GpioPortA
#define STK_USER_PIN        GpioPin7
#define STK_USER_IRQn       PORTA_IRQn
#define PortGPIO_IRQHandler PortA_IRQHandler
///< LED
#define STK_LED_PORT        GpioPortD
#define STK_LED_PIN         GpioPin14
#endif
///< XTH
#define SYSTEM_XTH          (32*1000*1000u)     ///< 32MHz

#define STK_XTHI_PORT       GpioPortF
#define STK_XTHI_PIN        GpioPin0
#define STK_XTHO_PORT       GpioPortF
#define STK_XTHO_PIN        GpioPin1

///< XTL
#define SYSTEM_XTL          (32768u)            ///< 32768Hz
#define STK_XTLI_PORT       GpioPortC
#define STK_XTLI_PIN        GpioPin14
#define STK_XTLO_PORT       GpioPortC
#define STK_XTLO_PIN        GpioPin15

///< LCD
#define STK_LCD_COM0_PORT   GpioPortA
#define STK_LCD_COM0_PIN    GpioPin9
#define STK_LCD_COM1_PORT   GpioPortA
#define STK_LCD_COM1_PIN    GpioPin10
#define STK_LCD_COM2_PORT   GpioPortA
#define STK_LCD_COM2_PIN    GpioPin11
#define STK_LCD_COM3_PORT   GpioPortA
#define STK_LCD_COM3_PIN    GpioPin12
#define STK_LCD_SEG0_PORT   GpioPortA
#define STK_LCD_SEG0_PIN    GpioPin8
#define STK_LCD_SEG1_PORT   GpioPortC
#define STK_LCD_SEG1_PIN    GpioPin9
#define STK_LCD_SEG2_PORT   GpioPortC
#define STK_LCD_SEG2_PIN    GpioPin8
#define STK_LCD_SEG3_PORT   GpioPortC
#define STK_LCD_SEG3_PIN    GpioPin7
#define STK_LCD_SEG4_PORT   GpioPortC
#define STK_LCD_SEG4_PIN    GpioPin6
#define STK_LCD_SEG5_PORT   GpioPortB
#define STK_LCD_SEG5_PIN    GpioPin15
#define STK_LCD_SEG6_PORT   GpioPortB
#define STK_LCD_SEG6_PIN    GpioPin14
#define STK_LCD_SEG7_PORT   GpioPortB
#define STK_LCD_SEG7_PIN    GpioPin13

///< I2C EEPROM
#define EVB_I2C0_EEPROM_SCL_PORT    GpioPortB
#define EVB_I2C0_EEPROM_SCL_PIN     GpioPin6
#define EVB_I2C0_EEPROM_SDA_PORT    GpioPortB
#define EVB_I2C0_EEPROM_SDA_PIN     GpioPin7

///< SPI0
#define STK_SPI0_CS_PORT      GpioPortE
#define STK_SPI0_CS_PIN       GpioPin12
#define STK_SPI0_SCK_PORT     GpioPortE
#define STK_SPI0_SCK_PIN      GpioPin13
#define STK_SPI0_MISO_PORT    GpioPortE
#define STK_SPI0_MISO_PIN     GpioPin14
#define STK_SPI0_MOSI_PORT    GpioPortE
#define STK_SPI0_MOSI_PIN     GpioPin15

///< SPI1
#define STK_SPI1_CS_PORT      GpioPortB
#define STK_SPI1_CS_PIN       GpioPin12
#define STK_SPI1_SCK_PORT     GpioPortB
#define STK_SPI1_SCK_PIN      GpioPin13
#define STK_SPI1_MISO_PORT    GpioPortB
#define STK_SPI1_MISO_PIN     GpioPin14
#define STK_SPI1_MOSI_PORT    GpioPortB
#define STK_SPI1_MOSI_PIN     GpioPin15

#endif
