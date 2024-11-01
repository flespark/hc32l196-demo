#ifndef __CORTEX_M_LL_H
#define __CORTEX_M_LL_H

#include "hc32l19x.h"
#include "core_cm0plus.h"

static inline uint32_t on_interrupt(void)
{
    return (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk);
}

#endif /* __CORTEX_M_LL_H */
