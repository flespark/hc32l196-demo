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
 * @file   i2c.h
 *
 * @brief  Header file for I2C functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/

#ifndef __I2C_H__
#define __I2C_H__

#include "ddl.h"


/**
 *******************************************************************************
 ** \defgroup I2cGroup Inter-Integrated Circuit (I2C)
 **
 **
 ******************************************************************************/
//@{

/******************************************************************************/
/* Global pre-processor symbols/macros ('#define')                            */
/******************************************************************************/
/******************************************************************************
 * Global type definitions
 ******************************************************************************/

 /**
 ******************************************************************************
 ** \brief I2C功能配置
 *****************************************************************************/
typedef enum en_i2c_func
{
    I2cModule_En = 6u,  ///<I2C模块使能
    I2cStart_En  = 5u, ///<开始信号
    I2cStop_En   = 4u, ///<结束信号
    I2cAck_En    = 2u, ///<应答信号
    I2cHlm_En    = 0u, ///<高速使能
}en_i2c_func_t; 

 /**
 ******************************************************************************
 ** \brief I2C模式配置
 *****************************************************************************/
typedef enum en_i2c_mode
{
    I2cMasterMode = 0x40u,  ///<I2C主机模式
    I2cSlaveMode  = 0x44u,  ///<I2C从机模式
}en_i2c_mode_t;

/**
 ******************************************************************************
 ** \brief I2C初始化配置结构
 *****************************************************************************/
typedef struct stc_i2c_cfg
{
    uint32_t        u32Pclk;        ///<Pclk 设置(Hz)
    uint32_t        u32Baud;        ///<I2C通信波特率(Hz)
    en_i2c_mode_t   enMode;         ///<I2C主从模式配置
    uint8_t         u8SlaveAddr;    ///<从机地址配置(如果需要)
    boolean_t       bGc;            ///<广播地址使能(如果需要)
}stc_i2c_cfg_t;

/******************************************************************************
 * Global variable declarations ('extern', definition in C source)
 *****************************************************************************/

/******************************************************************************
 * Global function prototypes (definition in C source)
 *****************************************************************************/
 //I2C初始化函数
 en_result_t I2C_Init(M0P_I2C_TypeDef* I2Cx,stc_i2c_cfg_t *pstcI2CCfg);
 //设置波特率配置寄存器
 en_result_t I2C_SetBaud(M0P_I2C_TypeDef* I2Cx,uint8_t u8Tm);
 //I2C功能设置函数
 en_result_t I2C_SetFunc(M0P_I2C_TypeDef* I2Cx,en_i2c_func_t enFunc);
  //I2C功能清除函数
 en_result_t I2C_ClearFunc(M0P_I2C_TypeDef* I2Cx,en_i2c_func_t enFunc);
 //获取中断标记SI
 boolean_t I2C_GetIrq(M0P_I2C_TypeDef* I2Cx);
  //清除中断标记SI
 en_result_t I2C_ClearIrq(M0P_I2C_TypeDef* I2Cx);
 //获取状态
 uint8_t I2C_GetState(M0P_I2C_TypeDef* I2Cx);

 //字节写函数
 en_result_t I2C_WriteByte(M0P_I2C_TypeDef* I2Cx,uint8_t u8Data);

//字节读函数
uint8_t I2C_ReadByte(M0P_I2C_TypeDef* I2Cx);
 
//@} // I2cGroup

#ifdef __cplusplus
#endif

#endif /* __I2C_H__ */
/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/


