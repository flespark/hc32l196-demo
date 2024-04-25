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
 * @file   flash.h
 *
 * @brief  Header file for FLASH functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/

#ifndef __FLASH_H__
#define __FLASH_H__

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
 ** \defgroup FlashGroup Flash Controller (Flash)
 **
 **
 ******************************************************************************/
//@{

/******************************************************************************
 * Global type definitions
 ******************************************************************************/
/**
 ******************************************************************************
  ** \brief Flash中断类型重定义
 *****************************************************************************/
typedef enum en_flash_int_type
{
    FlashPCInt    = 0x01u,          ///<擦写PC地址报警中断
    FlashSlockInt = 0x02u,          ///<擦写保护报警中断
} en_flash_int_type_t;


/**
 ******************************************************************************
  ** \brief Flash读等待周期类型重定义
 *****************************************************************************/
typedef enum en_flash_waitcycle
{
    FlashWaitCycle0 = 0u,       ///< 读等待周期设置为0-即读周期为1（当HCLK小于等于24MHz时）
    FlashWaitCycle1 = 1u,       ///< 读等待周期设置为1-即读周期为2（当HCLK大于24MHz时必须至少为1）
    FlashWaitCycle2 = 2u,       ///< 读等待周期设置为2-即读周期为3（当HCK大于48MHz时必须至少为2）
} en_flash_waitcycle_t;

/**
 ******************************************************************************
  ** \brief Flash擦写保护范围重定义
 *****************************************************************************/
typedef enum en_flash_lock
{
    FlashLock0 = 0u,           ///<LOCK0    
    FlashLock1 = 1u,           ///<LOCK1   
    FlashLock2 = 4u,           ///<LOCK2    
    FlashLock3 = 5u,           ///<LOCK3     
} en_flash_lock_t;

/**
 ******************************************************************************
  ** \brief Flash操作模式配置
 *****************************************************************************/
typedef enum en_flash_op_mode
{
    FlashReadMode        = 0u,           ///<Flash 读模式    
    FlashWriteMode       = 1u,           ///<Flash 写（编程）模式   
    FlashSectorEraseMode = 2u,           ///<Flash 扇区（页）擦除模式    
    FlashChipEraseMode   = 3u,           ///<Flash 全片擦除模式     
} en_flash_op_mode_t;

/**
 ******************************************************************************
 ** \brief Redefinition of FLASH register structure 
 ******************************************************************************/

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
///<Flash 初始化配置(中断函数、编程时间参数及休眠模式配置）
en_result_t Flash_Init(uint8_t u8FreqCfg, boolean_t bDpstbEn);

///<Flash 模式配置
en_result_t Flash_OpModeConfig(en_flash_op_mode_t enFlashOpMode);

///<Flash 页/全片擦除
en_result_t Flash_SectorErase(uint32_t u32SectorAddr);

///<Flash 字节/半字/字写
en_result_t Flash_Write8(uint32_t u32Addr, uint8_t pu8Data[], uint32_t u32Len);
en_result_t Flash_Write16(uint32_t u32Addr, uint16_t pu16Data[], uint32_t u32Len);
en_result_t Flash_Write32(uint32_t u32Addr, uint32_t pu32Data[], uint32_t u32Len);

///<Flash 编程保护加锁/解锁
en_result_t Flash_LockAll(void);
en_result_t Flash_UnlockAll(void);
en_result_t Flash_LockSet(en_flash_lock_t enLock, uint32_t u32LockValue);

///<Flash 读等待周期设定
en_result_t Flash_WaitCycle(en_flash_waitcycle_t enWaitCycle);

///<中断相关函数
///<中断使能/禁止
en_result_t Flash_EnableIrq(en_flash_int_type_t enFlashIntType);
en_result_t Flash_DisableIrq(en_flash_int_type_t enFlashIntType);
///<中断标志获取
boolean_t Flash_GetIntFlag(en_flash_int_type_t enFlashIntType);
///<中断标志清除
en_result_t Flash_ClearIntFlag(en_flash_int_type_t enFlashIntType);

//@} // FlashGroup

#ifdef __cplusplus
}
#endif

#endif /* __FLASH_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
