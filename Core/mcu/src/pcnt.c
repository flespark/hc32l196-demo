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
 * @file   pcnt.c
 *
 * @brief  Source file for PCNT functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "pcnt.h"

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
    ** \brief  PCNT的启动和停止控制
    ** @param  NewState : Run_Enable 或者 Run_Disable
    ** @param  NewState : FALSE或者TRUE
    ** \retval 无
    **
******************************************************************************/
boolean_t Pcnt_Cmd(boolean_t NewState)
{
    SetBit((uint32_t)(&(M0P_PCNT->RUN)), 0, NewState);
    return GetBit((uint32_t)(&(M0P_PCNT->RUN)), 0);
}


/**
******************************************************************************
    ** \brief  将BUF中的值同步到CNT
    ** @param  value : 要同步到TOP的数值
    ** \retval ok 或 ErrorTimeout
    **
******************************************************************************/
en_result_t Pcnt_SetB2T(uint16_t value)
{
    uint16_t u16TimeOut;

    u16TimeOut = 1000;
    M0P_PCNT->BUF = value;
    M0P_PCNT->CMD_f.B2T = 1;

    while(u16TimeOut--)
    {
        if(M0P_PCNT->SR2_f.B2T == FALSE)
        {
            break;
        }
    }
    if(u16TimeOut == 0)
    {
        return ErrorTimeout;
    }
        return Ok;
}

/**
******************************************************************************
    ** \brief  将BUF中的值同步到CNT
    ** @param  value : 要同步到CNT的数值
    ** \retval ok 或 ErrorTimeout
    **
******************************************************************************/
en_result_t Pcnt_SetB2C(uint16_t value)
{
    uint16_t u16TimeOut;
    u16TimeOut = 1000;
    M0P_PCNT->BUF = value;
    M0P_PCNT->CMD_f.B2C = 1;

    while(u16TimeOut--)
    {
        if(M0P_PCNT->SR2_f.B2C == FALSE)
        {
            break;
        }
    }
    if(u16TimeOut == 0)
    {
        return ErrorTimeout;
    }
    return Ok;
}

/**
******************************************************************************
    ** \brief  将TOP中的值同步到CNT
    ** @param  value : 要同步到CNT的数值
    ** \retval ok 或 ErrorTimeout
    **
******************************************************************************/
en_result_t Pcnt_SetT2C(void)
{
    uint16_t u16TimeOut;
    u16TimeOut = 1000;
    M0P_PCNT->CMD_f.T2C = 1;
    while(u16TimeOut--)
    {
        if(M0P_PCNT->SR2_f.T2C == FALSE)
        {
            break;
        }
    }
    if(u16TimeOut == 0)
    {
        return ErrorTimeout;
    }
    return Ok;
}


/**
******************************************************************************
    ** \brief  赋值BUF
    ** @param  value : 要赋值给BUF的数值
    ** \retval 无
    **
******************************************************************************/
void Pcnt_SetBuf(uint16_t value)
{
    M0P_PCNT->TOP_f.TOP = value;
}

/**
******************************************************************************
    ** \brief  初始化
    ** @param  start : 要同步到TOP的数值
    ** @param  end   : 要同步到CNT的数值
    ** \retval ok 或 ErrorTimeout
    **
******************************************************************************/
void Pcnt_Init(stc_pcnt_initstruct_t*  InitStruct)
{
    M0P_PCNT->CTRL_f.S1P = InitStruct->Pcnt_S1Sel;
    M0P_PCNT->CTRL_f.S0P = InitStruct->Pcnt_S0Sel;
    M0P_PCNT->CTRL_f.CLKSEL = InitStruct->Pcnt_Clk;
    M0P_PCNT->CTRL_f.MODE = InitStruct->Pcnt_Mode;
    if(InitStruct->Pcnt_Mode == PcntDoubleMode)//如果是双通道正交脉冲计数模式
    {
        M0P_PCNT->SR1_f.DIR = InitStruct->Pcnt_Dir;
    }
    else
    {
        M0P_PCNT->CTRL_f.DIR = InitStruct->Pcnt_Dir;
    }
    M0P_PCNT->FLT_f.EN = InitStruct->Pcnt_FltEn;
    M0P_PCNT->FLT_f.DEBTOP = InitStruct->Pcnt_DebTop;
    M0P_PCNT->FLT_f.CLKDIV = InitStruct->Pcnt_ClkDiv;
    M0P_PCNT->TOCR_f.EN    = InitStruct->Pcnt_TocrEn;
    M0P_PCNT->TOCR_f.TH    = InitStruct->Pcnt_TocrTh;

    M0P_PCNT->DBG_f.DBG = InitStruct->Pcnt_Dbg;
}

/**
******************************************************************************
    ** \brief  配置中断源的使能
    ** @param  IT_Src : 中断源再PCNT_IEN内部的位位置
    ** @param  NewState   : FALSE 或TRUE
    ** \retval 无
    **
******************************************************************************/
void Pcnt_ItCfg(en_pcnt_itfce_t IT_Src, boolean_t NewState)
{
    if(NewState == TRUE)
    {
        M0P_PCNT->IEN |= (uint32_t)(1<<IT_Src);
    }
    else if(NewState == FALSE)
    {
        M0P_PCNT->IEN &= ~(uint32_t)(1<<IT_Src);
    }
    else
    {
        ;
    }
}

/**
******************************************************************************
    ** \brief  获取中断源的标志位
    ** @param  IT_Src : 中断源标志位
    ** \retval FALSE 或TRUE
    **
******************************************************************************/
boolean_t Pcnt_GetItStatus(en_pcnt_itfce_t IT_Src)
{
    return ((M0P_PCNT->IFR >> IT_Src) & 1u) > 0 ? TRUE : FALSE;
}

/**
******************************************************************************
    ** \brief  清除中断源的标志位
    ** @param  IT_Src : 中断源标志位
    ** \retval 无
    **
******************************************************************************/
void Pcnt_ClrItStatus(en_pcnt_itfce_t IT_Src)
{
    M0P_PCNT->ICR &= ~(uint32_t)(1<<(uint32_t)IT_Src);
}


/**
******************************************************************************
    ** \brief  获取PCNT_CNT寄存器的数值
    ** @param  无
    ** \retval PCNT_CNT数值
    **
******************************************************************************/
uint16_t Pcnt_GetCnt(void)
{
    return  (uint16_t)(M0P_PCNT->CNT);
}

/**
******************************************************************************
    ** \brief  获取PCNT_TOP寄存器的数值
    ** @param  无
    ** \retval PCNT_TOP数值
    **
******************************************************************************/
uint16_t Pcnt_GetTop(void)
{
   return  (uint16_t)(M0P_PCNT->TOP);
}

/**
******************************************************************************
    ** \brief  获取PCNT_BUF寄存器的数值
    ** @param  无
    ** \retval PCNT_BUF数值
    **
******************************************************************************/
uint16_t Pcnt_GetBuf(void)
{
    return (uint16_t)(M0P_PCNT->BUF);
}

//@} // Group
/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/

