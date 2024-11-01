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
 * @file   dac.c
 *
 * @brief  Source file for DAC functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "dac.h"

/**
 ******************************************************************************
 ** \addtogroup AdcGroup
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

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 *****************************************************************************/

/**
******************************************************************************
    ** \brief  使能相关通道的DMA DMA_CR0中的DMAEN0
    ** 
  ** @param  NewState  :   TRUE 或者 FALSE
    ** \retval 无
    **
******************************************************************************/
void Dac_DmaCmd(boolean_t NewState)
{
    SetBit((uint32_t)(&(M0P_DAC->CR0)), 12, NewState);
}

/**
******************************************************************************
    ** \brief  配置DAC的DMA下溢中断， DMA_CR0中的DMAUDRIE0
    ** 
  ** @param  NewState  :   TRUE 或者 FALSE
    ** \retval 无
    **
******************************************************************************/
void Dac_DmaITCfg(boolean_t NewState)
{
    SetBit((uint32_t)(&(M0P_DAC->CR0)), 13, NewState);
}

/**
******************************************************************************
    ** \brief  获取DAC的DMA下溢中断标志位状态， DMA_SR中的DMAUDR0
    ** 
    ** @param  无
    ** \retval TRUE 或 FALSE
    **
******************************************************************************/
boolean_t Dac_GetITStatus(void)
{  
    return GetBit((uint32_t)(&(M0P_DAC->SR)), 13);
}

/**
******************************************************************************
    ** \brief  配置DAC的使能与禁止， DMA_CR0中的EN0
    ** 
  ** @param  NewState  :   TRUE 或者 FALSE
    ** \retval 无
    **
******************************************************************************/
void Dac_Cmd(boolean_t NewState)
{
    SetBit((uint32_t)(&(M0P_DAC->CR0)), 0, NewState);
}

/**
******************************************************************************
    ** \brief  软件触发寄存器，触发DAC转换 DMA_SWTRIGR中的SWTRIG0
    ** 
    ** @param  无
    ** \retval 无
    **
******************************************************************************/
void Dac_SoftwareTriggerCmd(void)
{
    SetBit((uint32_t)(&(M0P_DAC->SWTRIGR)), 0, 1);
}
    
/**
******************************************************************************
** \brief  初始化DAC0
    ** 
    ** @param  DAC_InitStruct  :   用于初始化DAC0的结构体
    ** \retval 无
    **
******************************************************************************/
void Dac_Init(stc_dac_cfg_t* DAC_InitStruct)
{
    M0P_DAC->CR0_f.BOFF0 = DAC_InitStruct->boff_t;
    M0P_DAC->CR0_f.TEN0  = DAC_InitStruct->ten_t;
    M0P_DAC->CR0_f.TSEL0 = DAC_InitStruct->tsel_t;
    M0P_DAC->CR0_f.WAVE0 = DAC_InitStruct->wave_t;
    M0P_DAC->CR0_f.MAMP0 = DAC_InitStruct->mamp_t;
    M0P_DAC->CR0_f.SREF0 = DAC_InitStruct->sref_t;
    
    if(DAC_InitStruct->align == DacLeftAlign)
    {
        M0P_DAC->DHR12L0_f.DHR0 = DAC_InitStruct->dhr12;
    }
    else if(DAC_InitStruct->align == DacRightAlign)
    {
        M0P_DAC->DHR12R0_f.DHR0 = DAC_InitStruct->dhr12;
    }
    else
    {
        M0P_DAC->DHR8R0_f.DHR0 = DAC_InitStruct->dhr8;
    }
}

/**
******************************************************************************
** \brief  向DAC0的数据保持寄存器写数据
    ** 
    ** @param  DAC_Channel:   Dac_0
    ** @param  DAC_Align  :   Right_Align 与Left_Align
    ** @param  DAC_Bit    :   Bit8 与Bit12
    ** @param  Data       :   所要发送的数据
    ** \retval 无
    **
******************************************************************************/
void Dac_SetChannelData(en_align_t DAC_Align, en_bitno_t DAC_Bit, uint16_t Data)
{  
    if(DAC_Align == DacRightAlign)
    {
        if(DAC_Bit == DacBit8)
        {
            M0P_DAC->DHR8R0_f.DHR0 = (uint8_t)Data;
        }
        else if(DAC_Bit == DacBit12)
        {
            M0P_DAC->DHR12R0_f.DHR0 = Data;
        }
        else
        {
          return;
        }
    }
    else if(DAC_Align == DacLeftAlign)
    {
        if(DAC_Bit == DacBit8)
        {
            return;
        }
        else if(DAC_Bit == DacBit12)
        {
            M0P_DAC->DHR12L0_f.DHR0 = Data;
        }
        else
        {
          return;
        }
    }
    else
    {
        return;
    }
}



/**
******************************************************************************
    ** \brief  获取DAC数据输出寄存器DAC_DOR0
    ** 
    ** @param  无
    ** \retval DAC_DOR0的值
    **
******************************************************************************/
uint16_t Dac_GetDataOutputValue(void)
{
    uint16_t tmp;

    tmp = M0P_DAC->DOR0_f.DOR0;
    return tmp&0x0fff;
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


