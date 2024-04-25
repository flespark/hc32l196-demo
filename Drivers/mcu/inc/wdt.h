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
 * @file   wdt.h
 *
 * @brief  Header file for WDT functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/

#ifndef __WDT_H__
#define __WDT_H__

#include "ddl.h"


/**
 ******************************************************************************
 ** \defgroup WdtGroup Watchdog Timer (WDT)
 **
 ******************************************************************************/
//@{

/******************************************************************************/
/* Global pre-processor symbols/macros ('#define')                            */
/******************************************************************************/

/******************************************************************************
 * Global type definitions
 ******************************************************************************/
/**
 ******************************************************************************
 ** \brief wdt溢出后复位或中断配置
 *****************************************************************************/
typedef enum en_wdt_func
{
    WdtResetEn    = 0,   ///<复位使能
    WdtIntEn      = 1,   ///<中断使能
}en_wdt_func_t;

/**
 ******************************************************************************
 ** \brief wdt当前运行时间配置数据类型定义
 *****************************************************************************/

typedef enum en_wdt_time
{
    WdtT1ms6   = 0u,       ///<1.6ms
    WdtT3ms2   = 1u,       ///<3.2ms
    WdtT6ms4   = 2u,       ///<6.4ms
    WdtT13ms   = 3u,       ///<13ms
    WdtT26ms   = 4u,       ///<26ms
    WdtT51ms   = 5u,       ///<51ms
    WdtT102ms  = 6u,       ///<102ms
    WdtT205ms  = 7u,       ///<205ms
    WdtT500ms  = 8u,       ///<500ms
    WdtT820ms  = 9u,       ///<820ms
    WdtT1s64   = 10u,      ///<1.64s
    WdtT3s28   = 11u,      ///<3.28s
    WdtT6s55   = 12u,      ///<6.55s
    WdtT13s1   = 13u,      ///<13.1s
    WdtT26s2   = 14u,      ///<26.2s
    WdtT52s4   = 15u,      ///<52.4s
}en_wdt_time_t;


//wdt初始化
en_result_t Wdt_Init(en_wdt_func_t enFunc, en_wdt_time_t enTime);

//wdt开始和停止
void Wdt_Start(void);
//喂狗处理
void Wdt_Feed(void);
//喂狗处理
void Wdt_IrqClr(void);
//wdt溢出时间设置
void Wdt_WriteWdtLoad(uint8_t u8LoadValue);
///< 当前计数时间获取
uint8_t Wdt_ReadWdtValue(void);
//< 中断状态获取
boolean_t Wdt_GetIrqStatus(void);
///< 运行状态获取
boolean_t Wdt_ReadwdtStatus(void);

//@} // WdtGroup

#ifdef __cplusplus
#endif

#endif /* __WDT_H__ */



