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
 * @file   lvd.h
 *
 * @brief  Header file for Low Voltage Detector functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/

#ifndef __LVD_H__
#define __LVD_H__

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"


/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 ******************************************************************************
 ** \defgroup LvdGroup Low Voltage Detector (LVD)
 **
 ******************************************************************************/
//@{

/******************************************************************************
 * Global type definitions
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief LVD监测电压源选择
 **
 ** \note
 ******************************************************************************/
typedef enum en_lvd_input_src
{
    LvdInputSrcMskVCC  = 0u<<2,                ///< Vcc
    LvdInputSrcMskPC13 = 1u<<2,                ///< PC13
    LvdInputSrcMskPB08 = 2u<<2,                ///< PB08
    LvdInputSrcMskPB07 = 3u<<2,                ///< PB07
}en_lvd_input_src_t;

/**
 ******************************************************************************
 ** \brief LVD阈值电压选择
 **
 ** \note
 ******************************************************************************/
typedef enum en_lvd_threshold
{
    LvdMskTH1_8V = 0u<<4,                  ///< 1.8V
    LvdMskTH1_9V = 1u<<4,                  ///< 1.9V
    LvdMskTH2_0V = 2u<<4,                  ///< 2.0V
    LvdMskTH2_1V = 3u<<4,                  ///< 2.1V
    LvdMskTH2_2V = 4u<<4,                  ///< 2.2V
    LvdMskTH2_3V = 5u<<4,                  ///< 2.3V
    LvdMskTH2_4V = 6u<<4,                  ///< 2.4V
    LvdMskTH2_5V = 7u<<4,                  ///< 2.5V
    LvdMskTH2_6V = 8u<<4,                  ///< 2.6V
    LvdMskTH2_7V = 9u<<4,                  ///< 2.7V
    LvdMskTH2_8V = 10u<<4,                 ///< 2.8V
    LvdMskTH2_9V = 11u<<4,                 ///< 2.9V
    LvdMskTH3_0V = 12u<<4,                 ///< 3.0V
    LvdMskTH3_1V = 13u<<4,                 ///< 3.1V
    LvdMskTH3_2V = 14u<<4,                 ///< 3.2V
    LvdMskTH3_3V = 15u<<4,                 ///< 3.3V
}en_lvd_threshold_t;

/**
 ******************************************************************************
 ** \brief LVD输出滤波时间
 **
 ** \note
 ******************************************************************************/
typedef enum en_lvd_filter_time
{
    LvdFilterMsk7us    = 0u<<9,                   ///< 7us
    LvdFilterMsk14us   = 1u<<9,                   ///< 14us
    LvdFilterMsk28us   = 2u<<9,                   ///< 28us
    LvdFilterMsk112us  = 3u<<9,                   ///< 112us
    LvdFilterMsk450us  = 4u<<9,                   ///< 450us
    LvdFilterMsk1_8ms  = 5u<<9,                   ///< 1.8ms
    LvdFilterMsk7_2ms  = 6u<<9,                   ///< 7.3ms
    LvdFilterMsk28_8ms = 7u<<9,                   ///< 28.8ms
}en_lvd_filter_time_t;

/**
 ******************************************************************************
 ** \brief LVD中断触发方式
 **
 ** \note
 ******************************************************************************/
typedef enum en_lvd_irq_type
{
    LvdIrqMskRiseFall = 3u<<12,                 ///< 上升/下降沿触发
    LvdIrqMskHigh     = 1u<<14,                 ///< 高电平触发
    LvdIrqMskRise     = 1u<<13,                 ///< 上升沿触发
    LvdIrqMskFall     = 1u<<12,                 ///< 下降沿触发
}en_lvd_irq_type_t;

/**
 ******************************************************************************
 ** \brief LVD触发动作选择
 **
 ** \note
 ******************************************************************************/
typedef enum en_lvd_act
{
    LvdActMskReset = 1u<<1,              ///< 系统复位
    LvdActMskInt   = 0u,                 ///< NVIC中断
}en_lvd_act_t;

/**
 ******************************************************************************
 ** \brief LVD数字滤波使能配置
 **
 ** \note
 ******************************************************************************/
typedef enum en_lvd_filter
{
    LvdFilterMskEnable  = 1u<<8,         ///< 数字滤波使能
    LvdFilterMskDisable = 0u,            ///< 数字滤波禁止
}en_lvd_filter_t;

/**
 ******************************************************************************
 ** \brief LVD配置
 ** \note
 ******************************************************************************/
typedef struct stc_lvd_cfg
{
    en_lvd_act_t            enAct;          ///< LVD触发动作
    en_lvd_input_src_t      enInputSrc;     ///< LVD输入电压源
    en_lvd_threshold_t      enThreshold;    ///< LVD监测电压
    en_lvd_filter_t         enFilter;       ///< 是否使用输出滤波
    en_lvd_filter_time_t    enFilterTime;   ///< 输出滤波时间
    en_lvd_irq_type_t       enIrqType;      ///< 中断触发方式
}stc_lvd_cfg_t;


/******************************************************************************
 * Global definitions
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
//irq enable/disable
void Lvd_EnableIrq(void);
void Lvd_DisableIrq(void);

// irq function
void Lvd_ClearIrq(void);
boolean_t Lvd_GetIrqStat(void);

// fliter function
boolean_t Lvd_GetFilterResult(void);

//init/deinit function
void Lvd_Init(stc_lvd_cfg_t *pstcCfg);

//LVD function enable/disable
void Lvd_Enable(void);
void Lvd_Disable(void);

//@} // LvdGroup

#ifdef __cplusplus
}
#endif

#endif /* __LVD_H__ */
/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
