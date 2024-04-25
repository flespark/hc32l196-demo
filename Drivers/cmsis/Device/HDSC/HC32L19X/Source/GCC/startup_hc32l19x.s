/**
  ******************************************************************************
    PLACEHOLDER for CMSIS-CORE Startup Device Assembler File
    for the CX32L003 Series for the GNU GCC Compiler Toolchain
  ******************************************************************************
  */

  .syntax unified
  .cpu cortex-m0plus
  .fpu softvfp
  .thumb

.global g_pfnVectors
.global Default_Handler

/* start address for the initialization values of the .data section.
defined in linker script */
.word  _sidata
/* start address for the .data section. defined in linker script */
.word  _sdata
/* end address for the .data section. defined in linker script */
.word  _edata
/* start address for the .bss section. defined in linker script */
.word  _sbss
/* end address for the .bss section. defined in linker script */
.word  _ebss
/* stack used for SystemInit_ExtMemCtl; always internal RAM used */

  .section .text.Reset_Handler
  .weak Reset_Handler
  .type Reset_Handler, %function
Reset_Handler:
  ldr   r0, =_estack
  mov   sp, r0          /* set stack pointer */

/* Copy the data segment initializers from flash to SRAM */
  ldr r0, =_sdata
  ldr r1, =_edata
  ldr r2, =_sidata
  movs r3, #0
  b LoopCopyDataInit

CopyDataInit:
  ldr r4, [r2, r3]
  str r4, [r0, r3]
  adds r3, r3, #4

LoopCopyDataInit:
  adds r4, r0, r3
  cmp r4, r1
  bcc CopyDataInit

/* Zero fill the bss segment. */
  ldr r2, =_sbss
  ldr r4, =_ebss
  movs r3, #0
  b LoopFillZerobss

FillZerobss:
  str  r3, [r2]
  adds r2, r2, #4

LoopFillZerobss:
  cmp r2, r4
  bcc FillZerobss

/* Call the clock system intitialization function.*/
  bl  SystemInit
/* Call the application's entry point.*/
  bl main

LoopForever:
  b LoopForever


.size Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 *
 * @param  None
 * @retval : None
*/
    .section .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
  b Infinite_Loop
  .size Default_Handler, .-Default_Handler
/******************************************************************************
*
* The minimal vector table for a Cortex M0.  Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*
******************************************************************************/
   .section .vectors,"a",%progbits
  .type g_pfnVectors, %object
  .size g_pfnVectors, .-g_pfnVectors


g_pfnVectors:
  .long  _estack
  .long  Reset_Handler
  .long  NMI_Handler
  .long  HardFault_Handler
  .long  0
  .long  0
  .long  0
  .long  0
  .long  0
  .long  0
  .long  0
  .long  SVC_Handler
  .long  0
  .long  0
  .long  PendSV_Handler
  .long  SysTick_Handler

  .long  PORTA_IRQHandler
  .long  PORTB_IRQHandler
  .long  PORTC_E_IRQHandler
  .long  PORTD_F_IRQHandler
  .long  DMAC_IRQHandler
  .long  TIM3_IRQHandler
  .long  UART0_2_IRQHandler
  .long  UART1_3_IRQHandler
  .long  LPUART0_IRQHandler
  .long  LPUART1_IRQHandler
  .long  SPI0_IRQHandler
  .long  SPI1_IRQHandler
  .long  I2C0_IRQHandler
  .long  I2C1_IRQHandler
  .long  TIM0_IRQHandler
  .long  TIM1_IRQHandler
  .long  TIM2_IRQHandler
  .long  LPTIM0_1_IRQHandler
  .long  TIM4_IRQHandler
  .long  TIM5_IRQHandler
  .long  TIM6_IRQHandler
  .long  PCA_IRQHandler
  .long  WDT_IRQHandler
  .long  RTC_IRQHandler
  .long  ADC_DAC_IRQHandler
  .long  PCNT_IRQHandler
  .long  VC0_IRQHandler
  .long  VC1_2_IRQHandler
  .long  LVD_IRQHandler
  .long  LCD_IRQHandler
  .long  FLASH_RAM_IRQHandler
  .long  CLKTRIM_IRQHandler


/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler.
* As they are weak aliases, any function with the same name will override
* this definition.
*
*******************************************************************************/

  .weak  NMI_Handler
  .thumb_set NMI_Handler,Default_Handler

  .weak      HardFault_Handler
  .thumb_set HardFault_Handler,Default_Handler

  .weak      SVC_Handler
  .thumb_set SVC_Handler,Default_Handler

  .weak      PendSV_Handler
  .thumb_set PendSV_Handler,Default_Handler

  .weak      SysTick_Handler
  .thumb_set SysTick_Handler,Default_Handler

  .weak      PORTA_IRQHandler
  .thumb_set PORTA_IRQHandler,Default_Handler

  .weak      PORTB_IRQHandler
  .thumb_set PORTB_IRQHandler,Default_Handler

  .weak      PORTC_E_IRQHandler
  .thumb_set PORTC_E_IRQHandler,Default_Handler

  .weak      PORTD_F_IRQHandler
  .thumb_set PORTD_F_IRQHandler,Default_Handler

  .weak      DMAC_IRQHandler
  .thumb_set DMAC_IRQHandler,Default_Handler

  .weak      TIM3_IRQHandler
  .thumb_set TIM3_IRQHandler,Default_Handler

  .weak      UART0_2_IRQHandler
  .thumb_set UART0_2_IRQHandler,Default_Handler

  .weak      UART1_3_IRQHandler
  .thumb_set UART1_3_IRQHandler,Default_Handler

  .weak      LPUART0_IRQHandler
  .thumb_set LPUART0_IRQHandler,Default_Handler

  .weak      LPUART1_IRQHandler
  .thumb_set LPUART1_IRQHandler,Default_Handler

  .weak      SPI0_IRQHandler
  .thumb_set SPI0_IRQHandler,Default_Handler

  .weak      SPI1_IRQHandler
  .thumb_set SPI1_IRQHandler,Default_Handler

  .weak      I2C0_IRQHandler
  .thumb_set I2C0_IRQHandler,Default_Handler

  .weak      I2C1_IRQHandler
  .thumb_set I2C1_IRQHandler,Default_Handler

  .weak      TIM0_IRQHandler
  .thumb_set TIM0_IRQHandler,Default_Handler

  .weak      TIM1_IRQHandler
  .thumb_set TIM1_IRQHandler,Default_Handler

  .weak      TIM2_IRQHandler
  .thumb_set TIM2_IRQHandler,Default_Handler

  .weak      LPTIM0_1_IRQHandler
  .thumb_set LPTIM0_1_IRQHandler,Default_Handler

  .weak      TIM4_IRQHandler
  .thumb_set TIM4_IRQHandler,Default_Handler

  .weak      TIM5_IRQHandler
  .thumb_set TIM5_IRQHandler,Default_Handler

  .weak      TIM6_IRQHandler
  .thumb_set TIM6_IRQHandler,Default_Handler

  .weak      PCA_IRQHandler
  .thumb_set PCA_IRQHandler,Default_Handler

  .weak      WDT_IRQHandler
  .thumb_set WDT_IRQHandler,Default_Handler

  .weak      RTC_IRQHandler
  .thumb_set RTC_IRQHandler,Default_Handler

  .weak      ADC_DAC_IRQHandler
  .thumb_set ADC_DAC_IRQHandler,Default_Handler

  .weak      PCNT_IRQHandler
  .thumb_set PCNT_IRQHandler,Default_Handler

  .weak      VC0_IRQHandler
  .thumb_set VC0_IRQHandler,Default_Handler

  .weak      VC1_2_IRQHandler
  .thumb_set VC1_2_IRQHandler,Default_Handler

  .weak      LVD_IRQHandler
  .thumb_set LVD_IRQHandler,Default_Handler

  .weak      LCD_IRQHandler
  .thumb_set LCD_IRQHandler,Default_Handler

  .weak      FLASH_RAM_IRQHandler
  .thumb_set FLASH_RAM_IRQHandler,Default_Handler

  .weak      CLKTRIM_IRQHandler
  .thumb_set CLKTRIM_IRQHandler,Default_Handler

