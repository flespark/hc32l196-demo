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
 * @file   spi.h
 *
 * @brief  Header file for SPI functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/

#ifndef __SPI_H__
#define __SPI_H__

/******************************************************************************
 * Include files
 *****************************************************************************/
#include "ddl.h"


/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

//@{

/******************************************************************************
 * Global type definitions
 *****************************************************************************/
 /**
 ******************************************************************************
 ** \brief SPI 功能通道选择设置
 ******************************************************************************/ 

/**
 ******************************************************************************
 ** \brief SPI 功能使能设置
 ******************************************************************************/ 
typedef enum en_spi_en
{
    SpiMskEnable     = 0x4u,    ///< SPI模块使能
    SpiMskDisable    = 0u,      ///< SPI模块禁止
}en_spi_en_t;
/**
 ******************************************************************************
 ** \brief SPI 模式配置
 ******************************************************************************/ 
typedef enum en_spi_mode
{
    SpiMskMaster = 0x10u,  ///<主机
    SpiMskSlave  = 0x00u,  ///<从机
}en_spi_mode_t;
/**
 ******************************************************************************
 ** \brief SPI 时钟极性设置
 ******************************************************************************/ 
typedef enum en_spi_cpol
{
    SpiMskcpollow  = 0u,       ///<极性为低
    SpiMskcpolhigh = 0x08u,    ///<极性为高
}en_spi_cpol_t;
/**
 ******************************************************************************
 ** \brief SPI 时钟相位设置
 ******************************************************************************/ 
typedef enum en_spi_cpha
{
    SpiMskCphafirst  = 0u,      ///<第一边沿采样
    SpiMskCphasecond = 0x4u,    ///<第二边沿采样
}en_spi_cpha_t;
/**
 ******************************************************************************
 ** \brief SPI 时钟分频配置
 *****************************************************************************/
typedef enum en_spi_clk_div
{
    SpiClkMskDiv2   = 0x00u,   ///<PCLK 2分频       
    SpiClkMskDiv4   = 0x01u,   ///<PCLK 4分频            
    SpiClkMskDiv8   = 0x02u,   ///<PCLK 8分频            
    SpiClkMskDiv16  = 0x03u,   ///<PCLK 16分频            
    SpiClkMskDiv32  = 0x80u,   ///<PCLK 32分频            
    SpiClkMskDiv64  = 0x81u,   ///<PCLK 64分频            
    SpiClkMskDiv128 = 0x82u,   ///<PCLK 128分频            
}en_spi_clk_div_t;

/**
 ******************************************************************************
 ** \brief SPI 片选脚电平选择
 *****************************************************************************/
typedef enum en_spi_cspin
{
    SpiCsLow  = 0u,      ///<片选低电平         
    SpiCsHigh = 1u,      ///<片选高电平            
}en_spi_cspin_t;

/**
 ******************************************************************************
 ** \brief SPI 状态
 *****************************************************************************/
typedef enum en_spi_status
{
    SpiIf              = 0x80u,   ///<传输结束中断标志       
    SpiSserr           = 0x20u,   ///<从机模式错误标志    
    SpiMdf             = 0x10u,   ///<主机模式错误标志 
    SpiBusy            = 0x08u,   ///<SPI总线忙标志
    SpiTxe             = 0x04u,   ///<发送缓冲器器空标志
    SpiRxne            = 0x02u,   ///<接受缓冲器非空标志
}en_spi_status_t;
/**
 ******************************************************************************
 ** \brief SPI 功能开启函数
 *****************************************************************************/
typedef enum en_spi_func
{
    SpiMskRxNeIe          = 0x40u,   ///<接收缓冲器非空中断使能       
    SpiMskTxEIe           = 0x20u,   ///<发送缓冲器空中断使能    
    SpiMskDmaTxEn         = 0x10u,   ///<DMA硬件访问发送使能
    SpiMskDmaRxEn         = 0x08u,   ///<DMA硬件访问接收使能
}en_spi_func_t;
/**
 ******************************************************************************
 ** \brief SPI 总体配置结构体
 *****************************************************************************/
typedef struct stc_spi_cfg
{
    en_spi_mode_t       enSpiMode;          ///< 主从模式选择
    en_spi_clk_div_t    enPclkDiv;          ///< PCLK分频系数(波特率分频)
    en_spi_cpol_t       enCPOL;             ///< 时钟极性选择
    en_spi_cpha_t       enCPHA;             ///< 时钟相位选择
}stc_spi_cfg_t;

//SPI 状态获取  
boolean_t Spi_GetStatus(M0P_SPI_TypeDef* SPIx,en_spi_status_t enStatus);

///<SPI 中断使能/禁止
en_result_t Spi_IrqEnable(M0P_SPI_TypeDef* SPIx);
en_result_t Spi_IrqDisable(M0P_SPI_TypeDef* SPIx);
//SPI 清除中断标记
en_result_t Spi_ClearStatus(M0P_SPI_TypeDef* SPIx);
//SPI初始化函数
en_result_t Spi_Init(M0P_SPI_TypeDef* SPIx,stc_spi_cfg_t* pstcSpiCfg);
//SPI 功能使能禁止函数
en_result_t Spi_FuncEnable(M0P_SPI_TypeDef* SPIx, en_spi_func_t enFunc);
en_result_t Spi_FuncDisable(M0P_SPI_TypeDef* SPIx, en_spi_func_t enFunc);
//SPI关闭函数
en_result_t Spi_DeInit(M0P_SPI_TypeDef* SPIx);

//SPI 配置主发送的电平
void Spi_SetCS(M0P_SPI_TypeDef* SPIx,boolean_t bFlag);

//SPI 数据字节数据收发
uint8_t Spi_RWByte(M0P_SPI_TypeDef* SPIx, uint8_t u8Data);
//SPI 数据发送
en_result_t Spi_SendData(M0P_SPI_TypeDef* SPIx, uint8_t u8Data);
void Spi_Slave_DummyWriteData(M0P_SPI_TypeDef* SPIx, uint8_t u8Data);
en_result_t Spi_SendBuf(M0P_SPI_TypeDef* SPIx, uint8_t* pu8Buf, uint32_t u32Len);
//SPI 数据接收
uint8_t Spi_ReceiveData(M0P_SPI_TypeDef* SPIx);
en_result_t Spi_ReceiveBuf(M0P_SPI_TypeDef* SPIx, uint8_t* pu8Buf, uint32_t u32Len);
//@} // Spi Group

#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */
/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/

