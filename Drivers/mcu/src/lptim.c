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
 * @file   lptim.c
 *
 * @brief  Source file for LPTIM functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/

#include "lptim.h"

/**
 ******************************************************************************
 ** \addtogroup PCNTGroup
 ******************************************************************************/
//@{

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/


/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/**
******************************************************************************
    ** \brief  定时器LPTIMx中断使能控制
    ** @param  Lptimx    : LPTIM0 或LPTIM1
    ** @param  NewStatus : TRUE 或 FALSE
    ** \retval 无
    **
******************************************************************************/
void Lptim_ConfIt(M0P_LPTIMER_TypeDef* Lptimx, boolean_t NewStatus)
{
    SetBit((uint32_t)(&(Lptimx->CR)), 10, NewStatus);
}

/**
******************************************************************************
    ** \brief  定时器LPTIMx的启动/停止控制
    ** @param  Lptimx    : LPTIM0 或LPTIM1
    ** @param  NewStatus : TRUE 或 FALSE
    ** \retval 无
    **
******************************************************************************/
void Lptim_Cmd(M0P_LPTIMER_TypeDef* Lptimx, boolean_t NewStatus)
{
    SetBit((uint32_t)(&(Lptimx->CR)), 0, NewStatus);
}

/**
******************************************************************************
    ** \brief  定时器LPTIMx的中断标志位获取
    ** @param  Lptimx    : LPTIM0 或LPTIM1
    ** \retval TRUE 或 FALSE
    **
******************************************************************************/
boolean_t Lptim_GetItStatus(M0P_LPTIMER_TypeDef* Lptimx)
{
    return GetBit((uint32_t)(&(Lptimx->IFR)), 0);
}

/**
******************************************************************************
    ** \brief  定时器LPTIMx的中断标志位清除
    ** @param  Lptimx    : LPTIM0 或LPTIM1
    ** \retval 无
    **
******************************************************************************/
void Lptim_ClrItStatus(M0P_LPTIMER_TypeDef* Lptimx)
{
    SetBit((uint32_t)(&(Lptimx->ICLR)), 0, 0);
}

/**
******************************************************************************
    ** \brief  定时器LPTIMx的初始化配置
    ** @param  Lptimx    : LPTIM0 或LPTIM1
    ** @param  InitStruct : 初始化LPTIMx的结构体
    ** \retval en_result_t类型数据
    **
******************************************************************************/
en_result_t Lptim_Init(M0P_LPTIMER_TypeDef* Lptimx, stc_lptim_cfg_t* InitStruct)
{
    uint16_t u16TimeOut;
    u16TimeOut = 1000;
    Lptimx->CR_f.PRS      = InitStruct->enPrs;
    Lptimx->CR_f.TCK_SEL  = InitStruct->enTcksel;
    Lptimx->CR_f.GATE_P   = InitStruct->enGatep;
    Lptimx->CR_f.GATE     = InitStruct->enGate;
    Lptimx->CR_f.TOG_EN   = InitStruct->enTogen;
    Lptimx->CR_f.CT       = InitStruct->enCt;
    Lptimx->CR_f.MD       = InitStruct->enMd;
    while(u16TimeOut--)
    {
        if(Lptimx->CR_f.WT_FLAG)
        {
            break;
        }
    }
    if(u16TimeOut == 0)
    {
        return ErrorTimeout;        
    }
    Lptimx->ARR_f.ARR = InitStruct->u16Arr;
    return Ok;
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
