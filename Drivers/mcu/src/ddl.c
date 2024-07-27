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
 * @file   ddl.c
 *
 * @brief  Source file for DDL functions
 *
 * @author MADS Team
 *
 ******************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "ddl.h"

/**
 ******************************************************************************
 ** \addtogroup DDL Common Functions
 ******************************************************************************/
//@{

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/******************************************************************************/
/* Local variable definitions ('static')                                      */
/******************************************************************************/

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/

/******************************************************************************/
/* Function implementation - global ('extern') and local ('static')           */
/******************************************************************************/
#ifndef __DDL_C
#define __DDL_C
//#define __CC_ARM
#endif /* __DDL_C */

#define DDL_TICK_FREQ_DEFAULT 1000
#define DDL_TICK_IRQ_PRIORITY ((1 <<__NVIC_PRIO_BITS) - 1)

__IO uint32_t uwTick;

uint32_t Log2(uint32_t u32Val)
{
    uint32_t u32V1 = 0;

    if(0u == u32Val)
    {
        return 0;
    }

    while(u32Val > 1u)
    {
        u32V1++;
        u32Val /=2;
    }

    return u32V1;
}


/**
 *******************************************************************************
 ** \brief Memory clear function for DDL_ZERO_STRUCT()
 ******************************************************************************/
void ddl_memclr(void *pu8Address, uint32_t u32Count)
{
    uint8_t *pu8Addr = (uint8_t *)pu8Address;

    if(NULL == pu8Addr)
    {
        return;
    }

    while (u32Count--)
    {
        *pu8Addr++ = 0;
    }
}

void ddl_init_tick(void)
{
    SysTick_Config(SystemCoreClock / DDL_TICK_FREQ_DEFAULT);
    NVIC_SetPriority(SysTick_IRQn, DDL_TICK_IRQ_PRIORITY);

}

uint32_t ddl_get_tick(void)
{
    return uwTick;
}

void ddl_suspend_tick(void)
{
    /* Disable SysTick Interrupt */
    CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);
}

void ddl_resume_tick(void)
{
    /* Enable SysTick Interrupt */
    SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);
}


/**
 * \brief   delay1ms
 *          delay approximately 1ms.
 * \param   [in]  u32Cnt
 * \retval  void
 */
void delay1ms(uint32_t u32Cnt)
{
    uint32_t tickstart = ddl_get_tick();
    uint32_t wait = u32Cnt;

    wait++;

    while ((ddl_get_tick() - tickstart) < wait);
}

/**
 * \brief   delay100us
 *          delay approximately 100us.
 * \param   [in]  u32Cnt
 * \retval  void
 */
void delay100us(uint32_t u32Cnt)
{
    delay1ms(u32Cnt * 100);
}

/**
 * \brief   delay10us
 *          delay approximately 10us.
 * \param   [in]  u32Cnt
 * \retval  void
 */
void delay10us(uint32_t u32Cnt)
{
    delay1us(u32Cnt * 10);
}

/**
 * \brief   delay1us
 *          delay approximately 1us.
 * \param   [in]  u32Cnt
 * \retval  void
 */
void delay1us(uint32_t u32Cnt)
{
    uint32_t last, now, reload, ticks, sub = 0;
    last = SysTick->VAL;
    reload = SysTick->LOAD;
    ticks = SystemCoreClock / 1000000UL * u32Cnt;

    while (ticks > sub)
    {
        ticks -= sub;
        now = SysTick->VAL;
        sub = last >= now ? last - now : reload + last - now;
        last = now;
    }
}

/**
 * \brief   set register bit
 *
 * \param   [in]  addr
 * \param   [in]  offset
 * \retval  void
 */
void SetBit(uint32_t addr, uint32_t offset, boolean_t bFlag)
{
    if(TRUE == bFlag)
    {
        *((volatile uint32_t *)(addr)) |= ((1UL)<<(offset));
    }
    else
    {
        *((volatile uint32_t *)(addr)) &= (~(1UL<<(offset)));
    }


}


/**
 * \brief   get register bit
 *
 * \param   [in]  addr
 * \param   [in]  offset
 * \retval  void
 */
boolean_t GetBit(uint32_t addr, uint32_t offset)
{
    return ((((*((volatile uint32_t *)(addr))) >> (offset)) & 1u) > 0) ? TRUE : FALSE;
}


void SysTick_IRQHandler(void)
{
    NVIC_ClearPendingIRQ(SysTick_IRQn);
    uwTick++;
}

//@} // DDL Functions

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/