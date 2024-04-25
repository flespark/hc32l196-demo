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
 * @file   opa.h
 *
 * @brief  Header file for OPA functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/

#ifndef __OPA_H__
#define __OPA_H__

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
 ** \defgroup OPAGroup  (OPA)
  **
 ******************************************************************************/
//@{

/**
 *******************************************************************************
 ** function prototypes.
 ******************************************************************************/

typedef enum en_opa_oenx
{
    OpaOen1  = 3u,            //OUT1
    OpaOen2  = 4u,            //OUT2
    OpaOen3  = 5u,            //OUT3
    OpaOen4  = 6u             //OUT4
}en_opa_oenx_t;
 
typedef enum en_opa_clksrc    // 自动校准脉冲宽度设置
{
    OpaM1Pclk     = 0u,       // 1个PCLK周期
    OpaM2Pclk     = 1u,       // 2个PCLK周期
    OpaM4Pclk     = 2u,       // 4个PCLK周期
    OpaM8Pclk     = 3u,       // 8个PCLK周期
    OpaM16Pclk    = 4u,       // 16个PCLK周期
    OpaM32Pclk    = 5u,       // 32个PCLK周期
    OpaM64Pclk    = 6u,       // 64个PCLK周期
    OpaM128Pclk   = 7u,       // 128个PCLK周期
    OpaM256Pclk   = 8u,       // 256个PCLK周期
    OpaM512Pclk   = 9u,       // 512个PCLK周期
    OpaM1024Pclk  = 10u,      // 1024个PCLK周期
    OpaM2048Pclk  = 11u,      // 2048个PCLK周期
    OpaM4096Pclk  = 12u       // 4096个PCLK周期
}en_opa_clksrc_t;

typedef enum en_opa_set0ctrl
{
    enClkSwSet = 3u,          // 软件校准使能
    enAzPulse  = 2u,          // 软件校准
    enTrigger  = 1u,          // 软件触发自动教零
    enAdctr    = 1u           // 使能ADC启动触发OPA自动校准功能
}en_opa_set0ctrl_t;

typedef struct
{
    boolean_t         bAzen;        // 自动教零配置
    boolean_t         bClk_sw_set;  // 自动教零选择
    boolean_t         bAz_pulse;    // 软件校准设置
    boolean_t         bTrigger;     // 软件触发校准设置
    boolean_t         bAdctr_en;    // ADC启动触发OPA自动校准设置
    en_opa_clksrc_t   enClksrc;     // 自动校准脉冲宽度设置
}stc_opa_zcfg_t;    


/******************************************************************************
 * Global variable declarations ('extern', definition in C source)
 ******************************************************************************/

/******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
//OPA 通道使能
extern void Opa_Cmd(boolean_t NewStatus);
extern void Opa_CmdBuf(boolean_t NewStatus);

//OPA零点校准配置
extern void Opa_SetZero(stc_opa_zcfg_t* InitZero);

//使能输出OUTX
extern void Opa_CmdOnex(en_opa_oenx_t onex, boolean_t NewState);

//配置校零相关的位
extern void Opa_ZeroBitCtrl(en_opa_set0ctrl_t CtrlBit, boolean_t NewState);

//@} // OPA Group

#ifdef __cplusplus
#endif

#endif /* __OPA_H__ */
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


