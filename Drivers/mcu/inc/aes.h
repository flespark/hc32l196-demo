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
 * @file   aes.h
 *
 * @brief  Header file for AES functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/

#ifndef __AES_H__
#define __AES_H__

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
 ** \defgroup AESGroup AES
  **
 ******************************************************************************/
//@{

/******************************************************************************
 * Global type definitions
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief AES密钥长度类型定义
 ** \note
 ******************************************************************************/
typedef enum en_aes_key_type
{
    AesKey128 = 0u,               ///< 128 bits
    AesKey192 = 1u,               ///< 192 bits
    AesKey256 = 2u,               ///< 256 bits
}en_aes_key_type_t;

/**
 *******************************************************************************
 ** \brief AES配置结构体
 ** \note
 ******************************************************************************/
typedef struct
{
    uint32_t*           pu32Cipher;           ///< AES 密文指针
    uint32_t*           pu32Plaintext;        ///< AES 明文指针
    uint32_t*           pu32Key;              ///< AES 密钥指针
    en_aes_key_type_t   enKeyLen;             ///< AES 密钥长度类型

}stc_aes_cfg_t;

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
//AES 加密
en_result_t AES_Encrypt(stc_aes_cfg_t* pstcAesCfg);
//AES 解密
en_result_t AES_Decrypt(stc_aes_cfg_t* pstcAesCfg);


//@} // AES Group

#ifdef __cplusplus
}
#endif

#endif /* __AES_H__ */
/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/

