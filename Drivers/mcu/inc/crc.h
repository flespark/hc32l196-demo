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
 * @file   crc.h
 *
 * @brief  Header file for CRC functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/

#ifndef __CRC_H__
#define __CRC_H__

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "sysctrl.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 ******************************************************************************
 ** \defgroup CrcGroup Cyclic Redundancy Check (CRC)
  **
 ******************************************************************************/
//@{

/******************************************************************************
 * Global type definitions
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
//CRC16 编码值获取
uint16_t CRC16_Get8(uint8_t* pu8Data, uint32_t u32Len);
uint16_t CRC16_Get16(uint16_t* pu16Data, uint32_t u32Len);
uint16_t CRC16_Get32(uint32_t* pu32Data, uint32_t u32Len);
//CRC16 校验
en_result_t CRC16_Check8(uint8_t* pu8Data, uint32_t u32Len, uint16_t u16CRC);
en_result_t CRC16_Check16(uint16_t* pu16Data, uint32_t u32Len, uint16_t u16CRC);
en_result_t CRC16_Check32(uint32_t* pu32Data, uint32_t u32Len, uint16_t u16CRC);


//CRC32 编码值获取
uint32_t CRC32_Get8(uint8_t* pu8Data, uint32_t u32Len);
uint32_t CRC32_Get16(uint16_t* pu16Data, uint32_t u32Len);
uint32_t CRC32_Get32(uint32_t* pu32Data, uint32_t u32Len);
//CRC32 校验
en_result_t CRC32_Check8(uint8_t* pu8Data, uint32_t u32Len, uint32_t u32CRC);
en_result_t CRC32_Check16(uint16_t* pu16Data, uint32_t u32Len, uint32_t u32CRC);
en_result_t CRC32_Check32(uint32_t* pu32Data, uint32_t u32Len, uint32_t u32CRC);
//@} // CRC Group

#ifdef __cplusplus
}
#endif

#endif /* __CRC_H__ */
/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/

