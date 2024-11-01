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
 * @file   dac.h
 *
 * @brief  Header file for DAC functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/

#ifndef __DAC_H__
#define __DAC_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "ddl.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
  
/**
 ******************************************************************************
 ** \brief 使能或者禁止指令
 ******************************************************************************/ 
typedef enum 
{
    DacDisable  = 0u,         //禁止
    DacEnable   = 1u          //使能
}en_en_state_t;

/**
 ******************************************************************************
 ** \brief 使能或者禁止DAC0通道输出缓冲器         DAC_CR0：BOFF0
 ******************************************************************************/ 
typedef enum 
{
    DacBoffDisable = 1u,
    DacBoffEnable  = 0u
}en_dac_boff_t;

/**
 ******************************************************************************
 ** \brief 使能或者禁止DAC0通道触发使能           DAC_CR0: TEN0
 ******************************************************************************/ 
typedef enum
{
    DacTenDisable = 0u,
    DacTenEnable  = 1u
}en_dac_ten_t;

/**
 ******************************************************************************
 ** \brief DAC0通道触发选择                       DAC_CR0: TSEL0
 ******************************************************************************/ 
typedef enum 
{
    DacTim0Tradc = 0u,      //TIM0_TRADC触发
    DacTim1Tradc = 1u,      //TIM1_TRADC触发
    DacTim2Tradc = 2u,      //TIM2_TRADC触发
    DacTim3Tradc = 3u,      //TIM3_TRADC触发
    DacTim4Tradc = 4u,      //TIM4_TRADC触发
    DacTim5Tradc = 5u,      //TIM5_TRADC触发
    DacSwTriger  = 6u,      //软件触发
    DacExPortTriger = 7u    //外部端口触发
}en_dac_tsel_t;

/**
 ******************************************************************************
 ** \brief DAC0通道噪声/三角波生产使能或禁止      DAC_CR0: WAVE0
 ******************************************************************************/ 
typedef enum
{
    DacWaveDisable  = 0u,   //禁止生产波
    DacNoiseEnable  = 1u,   //使能生成噪声波
    DacTrWaveEnable = 2u    //使能生产三角波
}en_dac_wave_t;

/**
 ******************************************************************************
 ** \brief DACx通道掩码/振幅选择器                DAC_CR0: MAMP0 & MAMP1
 ******************************************************************************/
typedef enum
{
    DacMemp01   = 0u,
    DacMenp03   = 1u,
    DacMenp07   = 2u,
    DacMenp15   = 3u,
    DacMenp31   = 4u,
    DacMenp63   = 5u,
    DacMenp127  = 6u,
    DacMenp255  = 7u,
    DacMenp511  = 8u,
    DacMenp1023 = 9u,
    DacMenp2047 = 10u,
    DacMenp4095 = 11u
}en_dac_mamp_t;

/**
 ******************************************************************************
 ** \brief DACx通道DMA使能或禁止                 DAC_CR0: DMAEN0 & DMAEN1
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief DACx通道DMA下溢中断使能或禁止         DAC_CR0: DMAUDRIE0 & DMAUDRIE1
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief DACx参考电压选择                       DAC_CR0: SREF0 & SREF1
 ******************************************************************************/
typedef enum
{
    DacVoltage1V5   = 0u,   //内部1.5V
    DacVoltage2V5   = 1u,   //内部2.5V
    DacVoltageExRef = 2u,   //外部参考电压ExRef(PB01)
    DacVoltageAvcc  = 3u    //AVCC电压
}en_dac_sref_t;

/**
 ******************************************************************************
 ** \brief DACx触发选择寄存器DAC_ETRS              DAC_ETRS
 ******************************************************************************/
typedef enum
{
    DacPortTrigPA9 = 0u,      //触发端口为PA9
    DacPortTrigPB9 = 1u,      //触发端口为PB9
    DacPortTrigPC9 = 2u,      //触发端口为PC9
    DacPortTrigPD9 = 3u,      //触发端口为PD9
    DacPortTrigPE9 = 4u,      //触发端口为PE9
    DacPortTrigPF9 = 5u       //触发端口为PF9
}en_port_trig_t;

/**
 ******************************************************************************
 ** \brief 对齐方式              
 ******************************************************************************/
typedef enum
{
    DacRightAlign = 0u,        //右对齐
    DacLeftAlign  = 1u         //左对齐
}en_align_t;

/**
 ******************************************************************************
 ** \brief 数据位数          
 ******************************************************************************/
typedef enum
{
    DacBit8  = 0u,               //8位
    DacBit12 = 1u                //12位
}en_bitno_t;

/**
 ******************************************************************************
 ** \brief 初始化配置结构体         
 ******************************************************************************/
typedef struct
{
    en_dac_boff_t  boff_t;
    en_dac_ten_t   ten_t;
    en_dac_tsel_t  tsel_t;
    en_dac_wave_t  wave_t;
    en_dac_mamp_t  mamp_t;
    en_dac_sref_t  sref_t;
    en_port_trig_t port_trig_t;
    en_align_t     align;
    uint16_t       dhr12;
    uint8_t        dhr8;
}stc_dac_cfg_t;



extern void Dac_DmaCmd(boolean_t NewState);
extern void Dac_DmaITCfg(boolean_t NewState);
extern boolean_t Dac_GetITStatus(void);
extern void Dac_Cmd(boolean_t NewState);
extern void Dac_SoftwareTriggerCmd(void);
extern void Dac_Init(stc_dac_cfg_t* DAC_InitStruct);
extern void Dac_SetChannelData(en_align_t DAC_Align, en_bitno_t DAC_Bit, uint16_t Data);
extern uint16_t Dac_GetDataOutputValue(void);

#ifdef __cplusplus
}
#endif
#endif //__DAC_H__

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
