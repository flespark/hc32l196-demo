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
 * @file   debug.h
 *
 * @brief  Header file for DEBUG functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/

#ifndef __DEBUG_H__
#define __DEBUG_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"


#ifdef __cplusplus
extern "C"
{
#endif

/**
 ******************************************************************************
 ** \defgroup DebugGroup (DEBUG)
  **
 ******************************************************************************/
//@{

/**
 *******************************************************************************
 ** function prototypes.
 ******************************************************************************/

/******************************************************************************
 * Global type definitions
 ******************************************************************************/ 
/**
 *******************************************************************************
 ** \brief 调试模式下各模块工作状态类型定义
 ** \note
 ******************************************************************************/
typedef enum en_debug_module_active
{
    DebugMskTim0   = 0x0001u,               ///< TIM0 
    DebugMskTim1   = 0x0002u,               ///< TIM1 
    DebugMskTim2   = 0x0004u,               ///< TIM2 
    DebugMskLpTim0 = 0x0008u,               ///< LPTIM0
    DebugMskTim4   = 0x0010u,               ///< TIM4 
    DebugMskTim5   = 0x0020u,               ///< TIM5 
    DebugMskTim6   = 0x0040u,               ///< TIM6 
    DebugMskPca    = 0x0080u,               ///< PCA  
    DebugMskWdt    = 0x0100u,               ///< WDT  
    DebugMskRtc    = 0x0200u,               ///< RTC  
    DebugMskTim3   = 0x0800u,               ///< TIM3 
    DebugMskLpTim1 = 0x1000u,               ///< LPTIM1 
}en_debug_module_active_t; 
 
/*******************************************************************************
 * Global definitions
 ******************************************************************************/

/******************************************************************************
 * Global variable declarations ('extern', definition in C source)
 ******************************************************************************/

/******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
///< 在SWD调试模式下，使能模块计数功能
en_result_t Debug_ActiveEnable(en_debug_module_active_t enModule);
///< 在SWD调试模式下，暂停模块计数功能
en_result_t Debug_ActiveDisable(en_debug_module_active_t enModule);

//@} // Debug Group

#ifdef __cplusplus
#endif

#endif /* __DEBUG_H__ */
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


