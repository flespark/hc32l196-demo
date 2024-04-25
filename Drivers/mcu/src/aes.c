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
 * @file   aes.c
 *
 * @brief  Source file for AES functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "aes.h"
/**
 *******************************************************************************
 ** \addtogroup AesGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

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
 * \brief   
 *          AES 加密
 *
 * \param   [in]   pstcAesCfg    AES 配置结构体 @ref stc_aes_cfg_t
 *
 * \retval  en_result_t  Ok:  配置成功
 * \retval  en_result_t  ErrorInvalidParameter: 无效参数
 */
en_result_t AES_Encrypt(stc_aes_cfg_t* pstcAesCfg)
{
    if (NULL == pstcAesCfg)
    {
        return ErrorInvalidParameter;
    }
    
    M0P_AES->CR_f.KEYSIZE = pstcAesCfg->enKeyLen;
    
    //Key cfg
    M0P_AES->KEY0 = pstcAesCfg->pu32Key[0];
    M0P_AES->KEY1 = pstcAesCfg->pu32Key[1];
    M0P_AES->KEY2 = pstcAesCfg->pu32Key[2];
    M0P_AES->KEY3 = pstcAesCfg->pu32Key[3];        
    
    if(AesKey192 == pstcAesCfg->enKeyLen)
    {
            M0P_AES->KEY4 = pstcAesCfg->pu32Key[4];
            M0P_AES->KEY5 = pstcAesCfg->pu32Key[5];    
    }
    
    if(AesKey256 == pstcAesCfg->enKeyLen)
    {
            M0P_AES->KEY4 = pstcAesCfg->pu32Key[4];
            M0P_AES->KEY5 = pstcAesCfg->pu32Key[5];
            M0P_AES->KEY6 = pstcAesCfg->pu32Key[6];
            M0P_AES->KEY7 = pstcAesCfg->pu32Key[7];
    }
        
    //Data cfg
    M0P_AES->DATA0 = pstcAesCfg->pu32Plaintext[0];
    M0P_AES->DATA1 = pstcAesCfg->pu32Plaintext[1];
    M0P_AES->DATA2 = pstcAesCfg->pu32Plaintext[2];
    M0P_AES->DATA3 = pstcAesCfg->pu32Plaintext[3];
                
    M0P_AES->CR_f.MODE = 0;//Encry
    M0P_AES->CR_f.START = 1;
    while(M0P_AES->CR_f.START == 1)
    {
        ;
    }
    pstcAesCfg->pu32Cipher[0] = M0P_AES->DATA0;
    pstcAesCfg->pu32Cipher[1] = M0P_AES->DATA1;
    pstcAesCfg->pu32Cipher[2] = M0P_AES->DATA2;
    pstcAesCfg->pu32Cipher[3] = M0P_AES->DATA3;    
    return Ok;
}


/**
 * \brief   
 *          AES 解密
 *
 * \param   [in]   pstcAesCfg    AES 配置结构体 @ref stc_aes_cfg_t
 *
 * \retval  en_result_t  Ok:  配置成功
 * \retval  en_result_t  ErrorInvalidParameter: 无效参数
 */
en_result_t AES_Decrypt(stc_aes_cfg_t* pstcAesCfg)
{
    if (NULL == pstcAesCfg)
    {
        return ErrorInvalidParameter;
    }

    M0P_AES->CR_f.KEYSIZE = pstcAesCfg->enKeyLen;
	    
    //Key cfg
    M0P_AES->KEY0 = pstcAesCfg->pu32Key[0];
    M0P_AES->KEY1 = pstcAesCfg->pu32Key[1];
    M0P_AES->KEY2 = pstcAesCfg->pu32Key[2];
    M0P_AES->KEY3 = pstcAesCfg->pu32Key[3];

    if(AesKey192 == pstcAesCfg->enKeyLen)
    {
            M0P_AES->KEY4 = pstcAesCfg->pu32Key[4];
            M0P_AES->KEY5 = pstcAesCfg->pu32Key[5];    
    }
    
    if(AesKey256 == pstcAesCfg->enKeyLen)
    {
            M0P_AES->KEY4 = pstcAesCfg->pu32Key[4];
            M0P_AES->KEY5 = pstcAesCfg->pu32Key[5];
            M0P_AES->KEY6 = pstcAesCfg->pu32Key[6];
            M0P_AES->KEY7 = pstcAesCfg->pu32Key[7];
    }        
        
    //Data cfg
    M0P_AES->DATA0 = pstcAesCfg->pu32Cipher[0];
    M0P_AES->DATA1 = pstcAesCfg->pu32Cipher[1];
    M0P_AES->DATA2 = pstcAesCfg->pu32Cipher[2];
    M0P_AES->DATA3 = pstcAesCfg->pu32Cipher[3];
                
    M0P_AES->CR_f.MODE = 1;//UnEncry
    M0P_AES->CR_f.START = 1;
    while(M0P_AES->CR_f.START == 1)
    {
        ;
    }
    pstcAesCfg->pu32Plaintext[0] = M0P_AES->DATA0;
    pstcAesCfg->pu32Plaintext[1] = M0P_AES->DATA1;
    pstcAesCfg->pu32Plaintext[2] = M0P_AES->DATA2;
    pstcAesCfg->pu32Plaintext[3] = M0P_AES->DATA3;
    return Ok;
}

//@} // AesGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
