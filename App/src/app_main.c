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
#include <hal_gpio.h>
#include <tiny_printf.h>
#include <hal_lpuart.h>
#include <SEGGER_RTT.h>
#include "main.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/***************************************
 *     LCD初始化定义和变量
 ***************************************/
#define SegA    0x100
#define SegB    0x200
#define SegC    0x400
#define SegD    0x800
#define SegE    0x004
#define SegF    0x001
#define SegG    0x002
#define SegH    0x008
#define DOT     0x008       //LCD_RAM1,LCDRAM0
#define DOT2    0x80000     //LCD_RAM0


#define Char_0  SegA|SegB|SegC|SegD|SegE|SegF
#define Char_1  SegB|SegC
#define Char_2  SegA|SegB|SegD|SegE|SegG
#define Char_3  SegA|SegB|SegC|SegD|SegG
#define Char_4  SegB|SegC|SegF|SegG
#define Char_5  SegA|SegC|SegD|SegF|SegG
#define Char_6  SegA|SegC|SegD|SegE|SegF|SegG
#define Char_7  SegA|SegB|SegC
#define Char_8  SegA|SegB|SegC|SegD|SegE|SegF|SegG
#define Char_9  SegA|SegB|SegC|SegD|SegF|SegG
/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
typedef struct {
    en_gpio_port_t port;
    en_gpio_pin_t pin;
} gpio_id;

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void sys_rch_config(void);
static void sys_xtal_config(void);
static void sys_rtc_config(void);
static void sys_log_init(void);
static void bsp_led_init(void);
static void bsp_key_init(void);
static void app_lcd_config(void);
static void bsp_key_irq_callback(void *unused);


/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
static uint32_t key_count;
static hal_gpio_irq_t bsp_key_irq = {
    .context = NULL,
    .callback = bsp_key_irq_callback,
};

static const hal_gpio_pin_names_t lcd_gpio[] = {
    LCD_COM0_Pin,
    LCD_COM1_Pin,
    LCD_COM2_Pin,
    LCD_COM3_Pin,
    LCD_SEG0_Pin,
    LCD_SEG1_Pin,
    LCD_SEG2_Pin,
    LCD_SEG3_Pin,
    LCD_SEG4_Pin,
    LCD_SEG5_Pin,
    LCD_SEG6_Pin,
    LCD_SEG7_Pin,
    LCD_VLCDH_Pin,
    LCD_VLCD3_Pin,
    LCD_VLCD2_Pin,
    LCD_VLCD1_Pin,
};

static volatile unsigned int Lcd_Table[10] = {
    Char_0, Char_1, Char_2, Char_3, Char_4, Char_5, Char_6, Char_7, Char_8, Char_9};

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

void sys_rch_config(void)
{
    ///< RCH时钟不同频率的切换，需要先将时钟切换到RCL
    Sysctrl_SetRCLTrim(SysctrlRclFreq32768);
    ///< 设置RCL稳定时间
    Sysctrl_SetRCLStableTime(SysctrlRclStableCycle64);
    ///< 使能RCL
    Sysctrl_ClkSourceEnable(SysctrlClkRCL, TRUE);
    ///< 暂停SysTick中断
    ddl_suspend_tick();
    ///< 切换系统时钟到RCL

    Sysctrl_SysClkSwitch(SysctrlClkRCL);
    ///< 加载目标频率的RCH的TRIM值
    Sysctrl_SetRCHTrim(SysctrlRchFreq24MHz);
    ///< 使能RCH
    Sysctrl_ClkSourceEnable(SysctrlClkRCH, TRUE);
    ///< 时钟切换到RCH
    Sysctrl_SysClkSwitch(SysctrlClkRCH);
    ///< HCLK不超过24M：此处设置FLASH读等待周期为0 cycle
    Flash_WaitCycle(FlashWaitCycle1);
    ///< 关闭RCL减少功耗
    Sysctrl_ClkSourceEnable(SysctrlClkRCL, FALSE);
    ///< 更新系统时钟频率
    SystemCoreClockUpdate();
    ///< 重新配置SysTick
	ddl_init_tick();
    ///< 重新使能SysTick中断
    ddl_resume_tick();
}

/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 ** This sample
 **
 ******************************************************************************/
void sys_xtal_config(void)
{
    // conifg XTAL input pin PC13 and PC14
    // input
    M0P_GPIO->PCDIR |= 0xc000;
    // analogy mode
    M0P_GPIO->PCADS |= 0xc000;
    // no pull up
    M0P_GPIO->PCPU &= ~0xc000;
    // no pull down
    M0P_GPIO->PCPD &= ~0xc000;
    // driver current and driver strength
    Sysctrl_XTLDriverCfg(SysctrlXtlAmp1, SysctrlXtalDriver1);
    // waiting stable period
    Sysctrl_SetXTLStableTime(SysctrlXtlStableCycle16384);
    // enable
    Sysctrl_ClkSourceEnable(SysctrlClkXTL, TRUE);
}

void sys_log_init(void)
{
#ifdef LOG_USING_LPUART
    hal_lpuart_init();
#elif defined(LOG_USING_LPUART_DMA)
    hal_lpuart_init();
    hal_lpuart_dma_config();
#elif defined(LOG_USING_RTT)
    SEGGER_RTT_Init();
#endif
}

static void bsp_key_irq_callback(void *unused)
{
    key_count++;
}

static void bsp_key_init(void)
{
    hal_gpio_init_in(BSP_KEY_Pin, BSP_KEY_PP_MODE, BSP_KEY_IRQ_MODE, &bsp_key_irq);
    hal_gpio_irq_attach(&bsp_key_irq);
}

static void bsp_led_init(void)
{
    hal_gpio_init_out(BSP_LED_Pin, BSP_LED_PP_MODE, 0);
}

void sys_rtc_config(void)
{
    stc_rtc_initstruct_t RtcInitStruct = {0};
    stc_rtc_cyccfg_t RtcCycleIrqType = {0};
    Sysctrl_SetPeripheralGate(SysctrlPeripheralRtc, TRUE); // RTC模块时钟打开

    RtcInitStruct.rtcAmpm = RtcPm;                         // 24小时制
    RtcInitStruct.rtcClksrc = RtcClkXtl;                   // 外部低速时钟
    RtcInitStruct.rtcPrdsel.rtcPrdsel = RtcPrdx;           // 周期中断类型PRDX
    RtcInitStruct.rtcPrdsel.rtcPrdx = 1u;                  // 周期中断时间间隔 1秒
    RtcInitStruct.rtcTime.u8Second = 0x55;                 // 配置RTC时间
    RtcInitStruct.rtcTime.u8Minute = 0x01;
    RtcInitStruct.rtcTime.u8Hour = 0x10;
    RtcInitStruct.rtcTime.u8Day = 0x17;
    RtcInitStruct.rtcTime.u8DayOfWeek = 0x04;
    RtcInitStruct.rtcTime.u8Month = 0x04;
    RtcInitStruct.rtcTime.u8Year = 0x24;
    RtcInitStruct.rtcCompen = RtcCompenEnable; // 使能时钟误差补偿
    RtcInitStruct.rtcCompValue = 0;            // 补偿值  根据实际情况进行补偿

    Rtc_Init(&RtcInitStruct);
    RtcCycleIrqType.rtcPrdsel = RtcPrds;
    RtcCycleIrqType.rtcPrds = Rtc05S;
    Rtc_SetCyc(&RtcCycleIrqType); // 使能0.5s周期中断
    EnableNvic(RTC_IRQn, IrqLevel3, TRUE); // 使能RTC中断向量
    Rtc_Cmd(TRUE);                         // 使能RTC开始计数
}


void app_lcd_config(void)
{
    stc_lcd_cfg_t LcdInitStruct = {0};
    stc_lcd_segcom_t LcdSegCom = {0};

    ///< 打开GPIO外设时钟门控
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    for (size_t i = 0; i < ARRAY_SZ(lcd_gpio); i++) {
        hal_gpio_init_af(lcd_gpio[i], false, HAL_GPIO_PULL_MODE_NONE, HAL_GPIO_AF_ANALOG);
    }

    Sysctrl_SetPeripheralGate(SysctrlPeripheralLcd, TRUE); ///< 开启LCD时钟
    LcdSegCom.u32Seg0_31 = 0xffffff00;                            ///< 配置LCD_POEN0寄存器 开启SEG0~SEG7
    LcdSegCom.stc_seg32_51_com0_8_t.seg32_51_com0_8 = 0xffffffff; ///< 初始化LCD_POEN1寄存器 全部关闭输出端口
    LcdSegCom.stc_seg32_51_com0_8_t.segcom_bit.Com0_3 = 0;        ///< 使能COM0~COM3
    LcdSegCom.stc_seg32_51_com0_8_t.segcom_bit.Mux =
        0; ///< Mux=0,Seg32_35=0,BSEL=1表示:选择外部电容工作模式，内部电阻断路
    LcdSegCom.stc_seg32_51_com0_8_t.segcom_bit.Seg32_35 = 0;
    Lcd_SetSegCom(&LcdSegCom); ///< LCD COMSEG端口配置

    LcdInitStruct.LcdBiasSrc = LcdExtCap;   ///< 电容分压模式，需要外部电路配合
    LcdInitStruct.LcdDuty = LcdDuty4;       ///< 1/4duty
    LcdInitStruct.LcdBias = LcdBias3;       ///< 1/3 BIAS
    LcdInitStruct.LcdCpClk = LcdClk2k;      ///< 电压泵时钟频率选择2kHz
    LcdInitStruct.LcdScanClk = LcdClk128hz; ///< LCD扫描频率选择128Hz
    LcdInitStruct.LcdMode = LcdMode0;       ///< 选择模式0
    LcdInitStruct.LcdClkSrc = LcdXTL;       ///< LCD时钟选择RCL
    LcdInitStruct.LcdEn = LcdEnable;        ///< 使能LCD模块
    Lcd_Init(&LcdInitStruct);

    Lcd_ClearDisp();             ///< 清屏
}

void lcd_display_time(void)
{
    stc_rtc_time_t rtc_val;
    if (Rtc_ReadDateTime(&rtc_val) != Ok) {
        Error_Handler();
    }
    // display time number
    M0P_LCD->RAM0 &= 0xffff0000;
    M0P_LCD->RAM0 |= Lcd_Table[rtc_val.u8Hour >> 4];
    M0P_LCD->RAM0 &= 0xffff;
    M0P_LCD->RAM0 |= Lcd_Table[rtc_val.u8Hour & 0x0f] << 16;
    M0P_LCD->RAM1 &= 0xffff0000;
    M0P_LCD->RAM1 |= Lcd_Table[rtc_val.u8Minute >> 4];
    M0P_LCD->RAM1 &= 0xffff;
    M0P_LCD->RAM1 |= Lcd_Table[rtc_val.u8Minute & 0x0f] << 16;
    // display colon
    M0P_LCD->RAM0 |= 0x80000;
}

void lcd_mask_colon(void)
{
    // Mask colon
    M0P_LCD->RAM0 &= ~0x80000;
}

int main(void)
{
    ///< 配置RCH
    sys_rch_config();
    ///< 配置XTAL
    sys_xtal_config();
    ///< 配置RTC
    sys_log_init();
    sys_rtc_config();
    printf("started\n");
    app_lcd_config();                                          ///< LCD模块配置

    ///< LED 端口初始化
    bsp_led_init();
    ///< KEY 端口初始化
    bsp_key_init();

    ///< ===============================================
    ///< ============ 警告,警告,警告!!!=================
    ///< ===============================================
    ///< 本样例程序会进入深度休眠模式，因此以下两行代码起防护作用（防止进入深度
    ///< 休眠后芯片调试功能不能再次使用），
    ///< 在使用本样例时，禁止在没有唤醒机制的情况下删除以下两行代码。
    delay1ms(200);
    lcd_display_time();

    while (1) {
        while ((key_count & 1) == 0) {
            hal_gpio_set_value(BSP_LED_Pin, 1);
            delay1ms(100);
            hal_gpio_set_value(BSP_LED_Pin, 0);
            delay1ms(100);
        }
        // while (!print_isdone()); /* 等待DMA传输完成后进入休眠 */
        ///< 进入低功耗模式——深度休眠(使能唤醒后退出中断执行执行该条语句后再此进入休眠)
        ///< SWD（包括SEGGER RTT）可以将系统从休眠唤醒
        Lpm_GotoDeepSleep(FALSE);
        // delay10us(2); /* 休眠和唤醒稳定性测试 */
        printf("wake\n");
    }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler */
    /* User can add his own implementation to report the HAL error return state */
    while(1)
    {
        hal_gpio_set_value(BSP_LED_Pin, 1);
    }
    /* USER CODE END Error_Handler */
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


