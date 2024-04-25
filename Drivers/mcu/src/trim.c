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
 * @file   trim.c
 *
 * @brief  Source file for TRIM functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "trim.h"
/**
 *******************************************************************************
 ** \addtogroup TrimGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
#define IS_VALID_TRIMINT(x)             (TrimStop     == (x) ||\
                                         TrimCalCntOf == (x) ||\
                                         TrimXTLFault == (x) ||\
                                         TrimXTHFault == (x) ||\
                                         TrimPLLFault == (x))


/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 *****************************************************************************
 ** \brief Trim中断标志获取
 **
 **
 ** \param [in]  enIntType           中断类型（RefStop、CalCntOf、XTAL32KFault、XTAL32MFault）
 ** 
 ** \retval TRUE or FALSE                                      
 *****************************************************************************/
boolean_t Trim_GetIntFlag(en_trim_inttype_t enIntType)
{  
    ASSERT(IS_VALID_TRIMINT(enIntType));
    
    if(M0P_CLK_TRIM->IFR&enIntType)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
     
}

/**
 *****************************************************************************
 ** \brief Trim中断标志清除
 **
 **
 ** \param [in]  enIntType           中断类型（RefStop、CalCntOf、XTAL32KFault、XTAL32MFault）
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
en_result_t Trim_ClearIntFlag(en_trim_inttype_t enIntType)
{
    en_result_t enResult = Error;
  
    ASSERT(IS_VALID_TRIMINT(enIntType));
    
    M0P_CLK_TRIM->ICLR &= ~(uint32_t)enIntType;
    
    return enResult;
}



/**
 *****************************************************************************
 ** \brief Trim中断使能
 **
 **
 ** 
 ** \retval Null                                      
 *****************************************************************************/
void Trim_EnableIrq (void)
{
    M0P_CLK_TRIM->CR_f.IE = TRUE;
}

/**
 *****************************************************************************
 ** \brief Trim中断禁止
 **
 **
 **
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
void Trim_DisableIrq(void)
{
    M0P_CLK_TRIM->CR_f.IE = FALSE;
}

/**
 *****************************************************************************
 ** \brief Trim初始化配置
 **
 **
 ** \param [in]  pstcCfg           Trim配置结构体指针
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
en_result_t Trim_Init(stc_trim_cfg_t* pstcCfg)
{
    en_result_t enResult = Error;

    M0P_CLK_TRIM->CR = 0;
    
    M0P_CLK_TRIM->CR = (uint32_t)pstcCfg->enCALCLK |
                       (uint32_t)pstcCfg->enREFCLK |
                       (uint32_t)pstcCfg->enMON;  
    
    M0P_CLK_TRIM->REFCON = pstcCfg->u32RefCon;
    M0P_CLK_TRIM->CALCON = pstcCfg->u32CalCon;
    
    enResult = Ok;
    
    return enResult;
}

/**
 *****************************************************************************
 ** \brief Trim校准/监测启动运行
 **
 **
 ** 
 ** \retval Null                                     
 *****************************************************************************/
void Trim_Run(void)
{
    M0P_CLK_TRIM->CR_f.TRIM_START = TRUE;
}

/**
 *****************************************************************************
 ** \brief Trim校准/监测停止
 **
 **
 **
 ** 
 ** \retval Null                                      
 *****************************************************************************/
void Trim_Stop(void)
{
    M0P_CLK_TRIM->CR_f.TRIM_START = FALSE;
}

/**
 *****************************************************************************
 ** \brief Trim参考计数器计数值获取
 **
 ** 
 ** \retval u32Data                参考计数器计数值                      
 *****************************************************************************/
uint32_t Trim_RefCntGet(void)
{
    return (uint32_t)M0P_CLK_TRIM->REFCNT;
}

/**
 *****************************************************************************
 ** \brief Trim校准计数器计数值获取
 **
 **
 ** 
 ** 
 ** \retval u32Data                校准计数器计数值                      
 *****************************************************************************/
uint32_t Trim_CalCntGet(void)
{
    return (uint32_t)M0P_CLK_TRIM->CALCNT;
}

//@} // TrimGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
