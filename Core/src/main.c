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
 * @file   main.c
 *
 * @brief  Source file for GPIO example
 *
 * @author MADS Team 
 *
 ******************************************************************************/

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "rtc.h"
#include "gpio.h"
#include "lpm.h"
#include "board_stkhc32l19x.h"
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static uint8_t u8bFlag = 0,u8LedFlashCnt=0;
/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
static void App_LedInit(void);
static void App_UserKeyInit(void);
static void App_LowPowerModeGpioSet(void);
static void _UserKeyWait(void);
static void App_RtcCfg(void);

/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 ** This sample
 **
 ******************************************************************************/
int main(void)
{
  ///<外部低速初始化时钟配置(for LCD)
  Sysctrl_XTLDriverCfg(SysctrlXtlAmp1, SysctrlXtalDriver1);
  Sysctrl_SetXTLStableTime(SysctrlXtlStableCycle16384);
  Sysctrl_ClkSourceEnable(SysctrlClkXTL,TRUE);
	// ///< 使能RCL时钟
	// Sysctrl_ClkSourceEnable(SysctrlClkRCL,TRUE); 
	// ///< 配置内部低速时钟频率为32.768kHz
	// Sysctrl_SetRCLTrim(SysctrlRclFreq32768);
  // ///< 配置Demo板上所有不使用的IO,避免端口漏电
	///配置RTC
	App_RtcCfg();
  App_LowPowerModeGpioSet();
  ///< LED 端口初始化
  App_LedInit();
  ///< KEY 端口初始化
  App_UserKeyInit();
    
  ///< ===============================================
  ///< ============ 警告,警告,警告!!!=================
  ///< ===============================================
  ///< 本样例程序会进入深度休眠模式，因此以下两行代码起防护作用（防止进入深度
  ///< 休眠后芯片调试功能不能再次使用），
  ///< 在使用本样例时，禁止在没有唤醒机制的情况下删除以下两行代码。
  delay1ms(2000);
  _UserKeyWait();     ///< 等待按键按下后进入休眠模式
     
  ///< 打开并配置按键端口为下降沿中断
  Gpio_EnableIrq(STK_USER_PORT, STK_USER_PIN, GpioIrqFalling);
  EnableNvic(STK_USER_IRQn, IrqLevel3, TRUE);
        
  while(1)
  {
    if(u8bFlag) {
      u8bFlag = 0;
      u8LedFlashCnt = 10;
      while(u8LedFlashCnt--) {
        Gpio_SetIO(STK_LED_PORT, STK_LED_PIN); ///< LED点亮
        delay1ms(100);
        Gpio_ClrIO(STK_LED_PORT, STK_LED_PIN); ///< LED关闭
        delay1ms(100);
      }
    }
    Lpm_GotoDeepSleep(FALSE);                  ///< 进入低功耗模式——深度休眠(使能唤醒后退出中断执行执行该条语句后再此进入休眠)
  }
}



///< Port中断服务函数
void PortGPIO_IRQHandler(void)
{
  if(TRUE == Gpio_GetIrqStatus(STK_USER_PORT, STK_USER_PIN))
  {  
    Gpio_ClearIrq(STK_USER_PORT, STK_USER_PIN); 
    u8bFlag = 1;    
  }
}    


static void _UserKeyWait(void)
{    
  while(1)
  {
    ///< 检测电平(USER按键是否按下(低电平))
    if(FALSE == Gpio_GetInputIO(STK_USER_PORT, STK_USER_PIN))
    {
      break;
    }
    else
    {
      continue;
    }
  }
}


static void App_LowPowerModeGpioSet(void)
{
  ///< 打开GPIO外设时钟门控
  M0P_SYSCTRL->PERI_CLKEN0_f.GPIO = 1;
  
  ///< swd as gpio
//  Sysctrl_SetFunc(SysctrlSWDUseIOEn, TRUE);  ///< 不追求变态的功耗不建议把SWD设为GPIO口
    
  ///< 配置为数字端口
  M0P_GPIO->PAADS = 0;
  M0P_GPIO->PBADS = 0;
  M0P_GPIO->PCADS = 0;
  M0P_GPIO->PDADS = 0;
  M0P_GPIO->PEADS = 0;
  M0P_GPIO->PFADS = 0;

#if 0               ///< 配置为端口输出,此模式下最小系统功耗0.5uA
  M0P_GPIO->PADIR = 0;
  M0P_GPIO->PBDIR = 0;
  M0P_GPIO->PCDIR = 0;
  M0P_GPIO->PDDIR = 0;
  M0P_GPIO->PEDIR = 0;
  M0P_GPIO->PFDIR = 0;
  ///< 配置IO口为无上下拉输出低
  M0P_GPIO->PAOUT = 0;
  M0P_GPIO->PBOUT = 0;
  M0P_GPIO->PCOUT = 0;
  M0P_GPIO->PDOUT = 0;
  M0P_GPIO->PEOUT = 0;
  M0P_GPIO->PFOUT = 0;
  ///< 配置为输入无上拉
  M0P_GPIO->PAPU = 0;
  M0P_GPIO->PBPU = 0;
  M0P_GPIO->PCPU = 0;
  M0P_GPIO->PDPU = 0;
  M0P_GPIO->PEPU = 0;
  M0P_GPIO->PFPU = 0;
  ///< 配置为输入无上拉
  M0P_GPIO->PAPD = 0;
  M0P_GPIO->PBPD = 0;
  M0P_GPIO->PCPD = 0;
  M0P_GPIO->PDPD = 0;
  M0P_GPIO->PEPD = 0;
  M0P_GPIO->PFPD = 0;
    
#else              ///< 配置为端口输入
  M0P_GPIO->PADIR = 0XFFFF;
  M0P_GPIO->PBDIR = 0XFFFF;
  M0P_GPIO->PCDIR = 0XFFFF;
  M0P_GPIO->PDDIR = 0XFFFF;
  M0P_GPIO->PEDIR = 0XFFFF;
  M0P_GPIO->PFDIR = 0XFFFF;
  #if 0            ///<  IO口配成输入上拉无下拉模式一些芯片功耗71uA,不建议这种配置
  ///< 配置为输入上拉
  M0P_GPIO->PAPU = 0XFFFF;
  M0P_GPIO->PBPU = 0XFFFF;
  M0P_GPIO->PCPU = 0XFFFF;
  M0P_GPIO->PDPU = 0XFFFF;
  M0P_GPIO->PEPU = 0XFFFF;
  M0P_GPIO->PFPU = 0XFFFF;
    
  ///< 输入无下拉
  M0P_GPIO->PAPD = 0;
  M0P_GPIO->PBPD = 0;
  M0P_GPIO->PCPD = 0;
  M0P_GPIO->PDPD = 0;
  M0P_GPIO->PEPD = 0;
  M0P_GPIO->PFPD = 0;
 #else                    ///<  此模式下最小系统功耗0.5uA
   ///< 配置为输入无上拉
  M0P_GPIO->PAPU = 0;
  M0P_GPIO->PBPU = 0;
  M0P_GPIO->PCPU = 0;
  M0P_GPIO->PDPU = 0;
  M0P_GPIO->PEPU = 0;
  M0P_GPIO->PFPU = 0;
  ///< 输入下拉
  M0P_GPIO->PAPD = 0xFFFF;
  M0P_GPIO->PBPD = 0xFFFF;
  M0P_GPIO->PCPD = 0xFFFF;
  M0P_GPIO->PDPD = 0xFFFF;
  M0P_GPIO->PEPD = 0xFFFF;
  M0P_GPIO->PFPD = 0xFFFF;
  #endif  
#endif
}
    
static void App_UserKeyInit(void)
{
  stc_gpio_cfg_t stcGpioCfg;
    
  ///< 打开GPIO外设时钟门控
  Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
    
  ///< 端口方向配置->输入
  stcGpioCfg.enDir = GpioDirIn;
  ///< 端口驱动能力配置->高驱动能力
  stcGpioCfg.enDrv = GpioDrvL;
  ///< 端口上下拉配置->上拉
  stcGpioCfg.enPu = GpioPuEnable;
  stcGpioCfg.enPd = GpioPdDisable;
  ///< 端口开漏输出配置->开漏输出关闭
  stcGpioCfg.enOD = GpioOdDisable;
  ///< 端口输入/输出值寄存器总线控制模式配置->AHB
  stcGpioCfg.enCtrlMode = GpioAHB;
  ///< GPIO IO USER KEY初始化
  Gpio_Init(STK_USER_PORT, STK_USER_PIN, &stcGpioCfg);        

}

static void App_LedInit(void)
{
  stc_gpio_cfg_t stcGpioCfg;
    
  ///< 打开GPIO外设时钟门控
  Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
    
  ///< 端口方向配置->输出(其它参数与以上（输入）配置参数一致)
  stcGpioCfg.enDir = GpioDirOut;
  ////< 端口上下拉配置->无
  stcGpioCfg.enPu = GpioPuDisable;
  stcGpioCfg.enPd = GpioPdDisable;
  ///< GPIO IO LED端口初始化
  Gpio_Init(STK_LED_PORT, STK_LED_PIN, &stcGpioCfg);
  ///< LED关闭
  Gpio_ClrIO(STK_LED_PORT, STK_LED_PIN);
}

void App_RtcCfg(void)
{
    stc_rtc_initstruct_t RtcInitStruct;
    Sysctrl_SetPeripheralGate(SysctrlPeripheralRtc,TRUE);//RTC模块时钟打开
    RtcInitStruct.rtcAmpm = RtcPm;                       //24小时制
    RtcInitStruct.rtcClksrc = RtcClkXtl;                 //外部低速时钟
    RtcInitStruct.rtcPrdsel.rtcPrdsel = RtcPrdx;         //周期中断类型PRDX
    RtcInitStruct.rtcPrdsel.rtcPrdx = 1u;                //周期中断时间间隔 1秒
    RtcInitStruct.rtcTime.u8Second = 0x55;               //配置RTC时间
    RtcInitStruct.rtcTime.u8Minute = 0x01;
    RtcInitStruct.rtcTime.u8Hour   = 0x10;
    RtcInitStruct.rtcTime.u8Day    = 0x17;
    RtcInitStruct.rtcTime.u8DayOfWeek = 0x04;
    RtcInitStruct.rtcTime.u8Month  = 0x04;
    RtcInitStruct.rtcTime.u8Year   = 0x19;
    RtcInitStruct.rtcCompen = RtcCompenEnable;           // 使能时钟误差补偿
    RtcInitStruct.rtcCompValue = 0;                      //补偿值  根据实际情况进行补偿
    Rtc_Init(&RtcInitStruct);
    Rtc_AlmIeCmd(TRUE);                                  //使能闹钟中断
    
    // EnableNvic(RTC_IRQn, IrqLevel3, TRUE);               //使能RTC中断向量
    Rtc_Cmd(TRUE);                                       //使能RTC开始计数
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


