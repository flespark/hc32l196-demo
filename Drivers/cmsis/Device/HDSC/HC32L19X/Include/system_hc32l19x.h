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
 * @file   system_hc32l19x.h
 *
 * @brief  Header file for SYSTEM functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/

#ifndef __SYSTEM_HC32L19X_H__
#define __SYSTEM_HC32L19X_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/* Global pre-processor symbols/macros ('define')                             */
/******************************************************************************/
#define HWWD_DISABLE        (1)

#define HC32L19xPxxx           //100PIN
//#define HC32L19xMxxx           //80PIN
//#define HC32L19xKxxx           //64PIN
//#define HC32L19xJxxx           //48PIN
//#define HC32L19xFxxx           //32PIN

/**
 ******************************************************************************
 ** \brief Clock Setup macro definition
 **
 ** - 0: CLOCK_SETTING_NONE  - User provides own clock setting in application
 ** - 1: CLOCK_SETTING_CMSIS - 
 ******************************************************************************/
#define CLOCK_SETTING_NONE  0u
#define CLOCK_SETTING_CMSIS 1u


/******************************************************************************/
/*                                                                            */
/*                      START OF USER SETTINGS HERE                           */
/*                      ===========================                           */
/*                                                                            */
/*                 All lines with '<<<' can be set by user.                   */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/* Global function prototypes ('extern', definition in C source)              */
/******************************************************************************/


extern uint32_t SystemCoreClock;          // System Clock Frequency (Core Clock)
extern void SystemInit (void);            // Initialize the system
extern void SystemCoreClockUpdate (void); // Update SystemCoreClock variable

#ifdef __cplusplus
}
#endif

#endif /* __SYSTEM_HC32L19X _H__ */







