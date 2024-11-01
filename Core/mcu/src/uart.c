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
 * @file   uart.c
 *
 * @brief  Source file for UART functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "uart.h"
/**
 ******************************************************************************
 ** \addtogroup UartGroup
 ******************************************************************************/
//@{
/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/

/******************************************************************************/
/* Local variable definitions ('static')                                       */
/******************************************************************************/

/**
 ******************************************************************************
 ** \brief  UART通信中断使能函数设置
 **
 ** \param [in] UARTx通道号，enIrqSel发送or接收中断使能
 **
 ** \retval OK配置成功
 ** \retval ErrorInvalidParameter配置失败
 ******************************************************************************/
en_result_t Uart_EnableIrq(M0P_UART_TypeDef* UARTx, en_uart_irq_sel_t enIrqSel)
{
    SetBit((uint32_t)(&(UARTx->SCON)), enIrqSel, TRUE);

    return Ok;
}
/**
 ******************************************************************************
 ** \brief  UART通信中断禁止函数设置
 **
 ** \param [in] UARTx通道号，enIrqSel发送or接收中断禁止
 **
 ** \retval OK配置成功
 ** \retval ErrorInvalidParameter配置失败
 ******************************************************************************/
en_result_t Uart_DisableIrq(M0P_UART_TypeDef* UARTx, en_uart_irq_sel_t enIrqSel)
{
    SetBit((uint32_t)(&(UARTx->SCON)), enIrqSel, FALSE);
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief  UART通道多主机模式配置
 **
 ** \param [in] UARTx通道号，stcMultiCfg多主机模式结构
 **
 ** \retval OK配置成功
 ** \retval ErrorInvalidParameter配置失败
 ******************************************************************************/
en_result_t Uart_SetMultiMode(M0P_UART_TypeDef* UARTx, stc_uart_multimode_t* pstcMultiCfg)
{
    if(NULL != pstcMultiCfg)
    {
        UARTx->SCON_f.ADRDET = TRUE;   
        UARTx->SADDR = pstcMultiCfg->u8SlaveAddr;
        UARTx->SADEN = pstcMultiCfg->u8SaddEn;

    }
    else
    {
        return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief  UART单线半双工模式使能
 **
 ** \param [in] UARTx 通道号
 **
 ** \retval Null
 ******************************************************************************/
void Uart_HdModeEnable(M0P_UART_TypeDef* UARTx)
{
    UARTx->SCON_f.HDSEL = TRUE;
}

/**
 ******************************************************************************
 ** \brief  UART单线半双工模式关闭
 **
 ** \param [in] UARTx 通道号
 **
 ** \retval Null
 ******************************************************************************/
void Uart_HdModeDisable(M0P_UART_TypeDef* UARTx)
{
    UARTx->SCON_f.HDSEL = FALSE;
}

/**
 ******************************************************************************
 ** \brief  UART通道多机模式发送数据/地址帧配置TB8
 **
 ** \param [in] UARTx 通道号
 ** \param [in] TRUE-TB8为地址帧标志；FALSE-TB8为数据帧标志；
 **
 ** \retval Null
 ******************************************************************************/
void Uart_SetTb8(M0P_UART_TypeDef* UARTx, boolean_t bTB8Value)
{
    UARTx->SCON_f.B8CONT = bTB8Value;
    
}

/**
 ******************************************************************************
 ** \brief 获取RB8数值
 **
 ** \param [in] UARTx通道号
 **
 ** \retval RB8
 ******************************************************************************/
boolean_t Uart_GetRb8(M0P_UART_TypeDef* UARTx)
{
    return (UARTx->SBUF_f.DATA8);
}

/**
 ******************************************************************************
 ** \brief  UART通道多主机模式从机地址配置函数
 **
 ** \param [in] UARTx通道号，addr地址
 **
 ** \retval OK配置成功
 ** \retval ErrorInvalidParameter配置失败
 ******************************************************************************/
en_result_t Uart_SetSaddr(M0P_UART_TypeDef* UARTx,uint8_t u8Addr)
{
    UARTx->SADDR = u8Addr;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief  UART通道发送或接收等功能使能设置
 **
 ** \param [in] UARTx通道号，enFunc功能
 **
 ** \retval OK配置成功
 ** \retval ErrorInvalidParameter配置失败
 ******************************************************************************/
en_result_t Uart_EnableFunc(M0P_UART_TypeDef* UARTx, en_uart_func_t enFunc)
{    
    SetBit((uint32_t)(&(UARTx->SCON)), enFunc, TRUE);
    
    return Ok;
}
/**
 ******************************************************************************
 ** \brief  UART通道发送或接收等功能禁止设置
 **
 ** \param [in] UARTx通道号，enFunc功能
 **
 ** \retval OK配置成功
 ** \retval ErrorInvalidParameter配置失败
 ******************************************************************************/
en_result_t Uart_DisableFunc(M0P_UART_TypeDef* UARTx, en_uart_func_t enFunc)
{
    SetBit((uint32_t)(&(UARTx->SCON)), enFunc, FALSE);
    
    return Ok;
}
/**
 ******************************************************************************
 ** \brief  UART通道通信状态获取
 **
 ** \param [in] UARTx通道号
 **
 ** \retval 状态值
 ******************************************************************************/
uint8_t Uart_GetIsr(M0P_UART_TypeDef* UARTx)
{
    return (UARTx->ISR);
}

/**
 ******************************************************************************
 ** \brief  UART通道通信状态获取
 **
 ** \param [in] UARTx通道号，enStatus获取哪个状态
 **
 ** \retval 状态值
 ******************************************************************************/
boolean_t Uart_GetStatus(M0P_UART_TypeDef* UARTx, en_uart_status_t enStatus)
{
    boolean_t bStatus = FALSE;

     
    ASSERT(IS_VALID_STATUS(enStatus));

    bStatus =  GetBit((uint32_t)(&(UARTx->ISR)), enStatus);
    
    return bStatus;
}
/**
 ******************************************************************************
 ** \brief  UART通道通信状态清除
 **
 ** \param [in] UARTx通道号
 **
 ** \retval OK
 ** \retval ErrorInvalidParameter清除失败
 ******************************************************************************/
en_result_t Uart_ClrIsr(M0P_UART_TypeDef* UARTx)
{
    UARTx->ICR = 0u;
    return Ok;
}
/**
 ******************************************************************************
 ** \brief  UART通道通信状态清除
 **
 ** \param [in] UARTx通道号，enStatus清除哪个状态
 **
 ** \retval OK
 ** \retval ErrorInvalidParameter清除失败
 ******************************************************************************/
en_result_t Uart_ClrStatus(M0P_UART_TypeDef* UARTx,en_uart_status_t enStatus)
{
    ASSERT(IS_VALID_STATUS(enStatus));

    SetBit((uint32_t)(&(UARTx->ICR)), enStatus, FALSE);
    
    return Ok;
}
/**
 ******************************************************************************
 ** \brief  UART通道发送数据函数,查询方式调用此函数，中断方式发送不适用
 **
 ** \param [in] UARTx通道号，Data发送数据
 **
 ** \retval Ok发送成功
 ** \retval ErrorInvalidParameter发送失败
 ******************************************************************************/
en_result_t Uart_SendDataPoll(M0P_UART_TypeDef* UARTx, uint8_t u8Data)
{
    while(FALSE == Uart_GetStatus(UARTx,UartTxe))
    {}
    UARTx->SBUF_f.DATA = u8Data;
    while(FALSE == Uart_GetStatus(UARTx,UartTC))
    {}
    Uart_ClrStatus(UARTx,UartTC);       
    return Ok;
}

/**
 ******************************************************************************
 ** \brief  UART通道发送数据函数,查询方式调用此函数，中断方式发送不适用
 **
 ** \param [in] UARTx通道号，Data发送数据
 **
 ** \retval @ref en_result_t
 ******************************************************************************/
en_result_t Uart_SendDataPollTimeOut(M0P_UART_TypeDef* UARTx, uint8_t u8Data, uint32_t u32TimeOut)
{
    uint32_t u32Cnt = 0;
    
    while(FALSE == Uart_GetStatus(UARTx,UartTxe))
    {
        if(u32Cnt > u32TimeOut)
        {
            return ErrorTimeout;
        }
        u32Cnt++;
    }
    UARTx->SBUF_f.DATA = u8Data;
    
    u32Cnt = 0;
    while(FALSE == Uart_GetStatus(UARTx,UartTC))
    {
        if(u32Cnt > u32TimeOut)
        {
            return ErrorTimeout;
        }
        u32Cnt++;
    }
    Uart_ClrStatus(UARTx,UartTC);       
    return Ok;
}

/**
 ******************************************************************************
 ** \brief  UART通道发送数据函数,中断方式调用此函数
 **
 ** \param [in] UARTx通道号，Data发送数据
 **
 ** \retval Ok发送成功
 ** \retval ErrorInvalidParameter发送失败
 ******************************************************************************/
en_result_t Uart_SendDataIt(M0P_UART_TypeDef* UARTx, uint8_t u8Data)
{ 
    UARTx->SBUF_f.DATA = u8Data; 
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief  UART通道接收数据函数
 **
 ** \param [in] UARTx通道号
 **
 ** \retval 接收数据
 ******************************************************************************/
uint8_t Uart_ReceiveData(M0P_UART_TypeDef* UARTx)
{
    return (UARTx->SBUF_f.DATA);
}

/**
 ******************************************************************************
 ** \brief  UART通道初始化函数
 **
 ** \param [in] UARTx通道号，pstcCfg初始化结构体 @ref stc_uart_cfg_t
 **
 ** \retval OK配置成功
 ** \retval ErrorInvalidParameter配置失败
 ******************************************************************************/
en_result_t Uart_Init(M0P_UART_TypeDef* UARTx, stc_uart_cfg_t* pstcCfg)
{
    en_result_t enRet = Error;
    uint32_t u32Over[2] = {0x4, 0x3};
    uint16_t u16OverShift;
    float32_t f32Scnt=0;
    
    if(NULL == pstcCfg)
    {
        return ErrorInvalidParameter;
    }
    
    UARTx->SCON = 0;
    
    UARTx->SCON = (uint32_t)pstcCfg->enStopBit |
                  (uint32_t)pstcCfg->enMmdorCk |
                  (uint32_t)pstcCfg->stcBaud.enClkDiv |
                  (uint32_t)pstcCfg->enRunMode;
    
    if((UartMskMode1 == pstcCfg->enRunMode) || (UartMskMode3 == pstcCfg->enRunMode))
    {
        u16OverShift = u32Over[pstcCfg->stcBaud.enClkDiv/UartMsk8Or16Div];
        f32Scnt = (float32_t)(pstcCfg->stcBaud.u32Pclk)/(float32_t)(pstcCfg->stcBaud.u32Baud<<u16OverShift);        
        UARTx->SCNT = (uint16_t)(float32_t)(f32Scnt + 0.5f);
        Uart_EnableFunc(UARTx,UartRenFunc);       ///<使能收发
    }
    
    
 
    enRet = Ok;
    return enRet;
}
//@} // UartGroup      
