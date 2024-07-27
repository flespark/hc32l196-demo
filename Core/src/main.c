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
#include <base_types.h>
#include <ddl.h>
#include "printf-stdarg.h"
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
static void Clk_RchCfg(void);
static void Clk_XtalCfg(void);
static void App_LedInit(void);
static void App_UserKeyInit(void);
static void UserKeyWait(void);
static void App_RtcCfg(void);
static void LOG_UartInit(void);
static void LOG_UartDmaCfg(void);
static void App_LcdCfg(void);
static void LCD_TimeDisplay(void);
static void LCD_ColonMask(void);


/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
static uint8_t sys_stat = 0;

static const gpio_id lcd_gpio[] = {
    {GpioPortA, GpioPin9},  // COM0
    {GpioPortA, GpioPin10}, // COM1
    {GpioPortA, GpioPin11}, // COM2
    {GpioPortA, GpioPin12}, // COM3

    {GpioPortA, GpioPin8},  // SEG0
    {GpioPortC, GpioPin9},  // SEG1
    {GpioPortC, GpioPin8},  // SEG2
    {GpioPortC, GpioPin7},  // SEG3
    {GpioPortC, GpioPin6},  // SEG4
    {GpioPortB, GpioPin15}, // SEG5
    {GpioPortB, GpioPin14}, // SEG6
    {GpioPortB, GpioPin13}, // SEG7
    {GpioPortB, GpioPin3},  // VLCDH
    {GpioPortB, GpioPin4},  // VLCD3
    {GpioPortB, GpioPin5},  // VLCD2
    {GpioPortB, GpioPin6},  // VLCD1
};

static volatile unsigned int Lcd_Table[10] = {
    Char_0, Char_1, Char_2, Char_3, Char_4, Char_5, Char_6, Char_7, Char_8, Char_9};

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

void Clk_RchCfg(void)
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
void Clk_XtalCfg(void)
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

int main(void)
{
    ///< 配置RCH
    Clk_RchCfg();
    ///< 配置XTAL
    Clk_XtalCfg();
    ///< 配置RTC
    App_RtcCfg();
    LOG_UartInit();
#ifdef LOG_UART_USE_DMA
    LOG_UartDmaCfg();
#endif
    printf("started\n");
    App_LcdCfg();                                          ///< LCD模块配置

    Lcd_ClearDisp();             ///< 清屏
    Lcd_WriteRam(0, 0x0f0f0f0f); ///< 赋值寄存器LCDRAM0
    Lcd_WriteRam(1, 0x0f0f0f0f); ///< 赋值寄存器LCDRAM1

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
    delay1ms(200);
    LCD_TimeDisplay();
    // UserKeyWait(); ///< 等待按键按下后进入休眠模式

    while (1) {
        while (sys_stat == 0) {
            Gpio_SetIO(STK_LED_PORT, STK_LED_PIN); ///< LED点亮
            delay1ms(100);
            Gpio_ClrIO(STK_LED_PORT, STK_LED_PIN); ///< LED关闭
            delay1ms(100);
        }
        ///< 进入低功耗模式——深度休眠(使能唤醒后退出中断执行执行该条语句后再此进入休眠)
        Lpm_GotoDeepSleep(FALSE);
    }
}

///< Port中断服务函数
void PortGPIO_IRQHandler(void)
{
    if (TRUE == Gpio_GetIrqStatus(STK_USER_PORT, STK_USER_PIN)) {
        sys_stat = !sys_stat;
        Gpio_ClearIrq(STK_USER_PORT, STK_USER_PIN);
    } else {
        Error_Handler();
    }
}

///< RTC闹钟和周期中断服务函数
void Rtc_IRQHandler(void)
{
    static uint8_t count = 0;
    if (TRUE == Rtc_GetAlmfItStatus()) {
        Rtc_ClearAlmfItStatus();
    } else if (TRUE == Rtc_GetPridItStatus()) {
        count++;
        if (count & 1) {
            LCD_ColonMask();
        } else {
            LCD_TimeDisplay();
        }
        Rtc_ClearPrdfItStatus();
    } else {
        Error_Handler();
    }
}

void Dmac_IRQHandler(void)
{
#ifdef LOG_UART_USE_DMA
    if (Dma_GetStat(LOG_UART_DMA_CHANNEL) == DmaTransferComplete) {
        Dma_ClrStat(LOG_UART_DMA_CHANNEL);
        printf_dma_done_irq_handle();
    } else {
        Error_Handler();
    }
#endif
}

static void UserKeyWait(void)
{
    while (1) {
        ///< 检测电平(USER按键是否按下(低电平))
        if (FALSE == Gpio_GetInputIO(STK_USER_PORT, STK_USER_PIN)) {
            break;
        } else {
            continue;
        }
    }
}

static void App_UserKeyInit(void)
{
    stc_gpio_cfg_t stcGpioCfg = {0};

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
    ///< 打开并配置按键端口为下降沿中断
    Gpio_EnableIrq(STK_USER_PORT, STK_USER_PIN, GpioIrqFalling);
    EnableNvic(STK_USER_IRQn, IrqLevel3, TRUE);
    ///< GPIO IO USER KEY初始化
    Gpio_Init(STK_USER_PORT, STK_USER_PIN, &stcGpioCfg);
}

static void App_LedInit(void)
{
    stc_gpio_cfg_t stcGpioCfg = {0};

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
    // Rtc_AlmIeCmd(TRUE); // 使能闹钟中断
    RtcCycleIrqType.rtcPrdsel = RtcPrds;
    RtcCycleIrqType.rtcPrds = Rtc05S;
    Rtc_SetCyc(&RtcCycleIrqType); // 使能0.5s周期中断
    EnableNvic(RTC_IRQn, IrqLevel3, TRUE); // 使能RTC中断向量
    Rtc_Cmd(TRUE);                         // 使能RTC开始计数
}

void LOG_UartInit(void)
{
    stc_gpio_cfg_t GpioInitStruct = {0};
    stc_uart_cfg_t  stcCfg = {0};

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    GpioInitStruct.enDir = GpioDirOut;
    GpioInitStruct.enPu = GpioPuEnable;

    Gpio_Init(LOG_TX_Port, LOG_TX_Pin, &GpioInitStruct);
    Gpio_SetAfMode(LOG_TX_Port, LOG_TX_Pin, LOG_UART_AF);

    Sysctrl_SetPeripheralGate(LOG_UART_CLOCK, TRUE);

    ///< UART Init
    stcCfg.enRunMode = UartMskMode1;
    stcCfg.enStopBit = UartMsk1bit;
    stcCfg.enMmdorCk = UartMskDataOrAddr;
    stcCfg.stcBaud.u32Baud = LOG_UART_BAUDRATE;
    stcCfg.stcBaud.enClkDiv = UartMsk8Or16Div;
    stcCfg.stcBaud.u32Pclk = Sysctrl_GetPClkFreq();
    Uart_Init(LOG_UART_SEL, &stcCfg);

#ifdef LOG_UART_USE_DMA
    Uart_ClrStatus(LOG_UART_SEL, UartTC);
    Uart_EnableFunc(LOG_UART_SEL, UartDmaTxFunc);
#endif
}

void LOG_UartDmaCfg(void)
{
    stc_dma_cfg_t stcDmaCfg = {0};

    Sysctrl_SetPeripheralGate(SysctrlPeripheralDma,TRUE);      //使能DMAC外设时钟门控开关

    stcDmaCfg.u32SrcAddress = (uint32_t)(printf_dma_buf);      // 接收数据缓存
    stcDmaCfg.u32DstAddress = (uint32_t)&LOG_UART_SEL->SBUF;   // 发送数据寄存器地址
    stcDmaCfg.enSrcAddrReloadCtl = DmaMskSrcAddrReloadEnable;  // 使能DMA源地址重载
    stcDmaCfg.enSrcBcTcReloadCtl = DmaMskBcTcReloadEnable;     // 使能BC[3:0]和CONFA:TC[15:0]的重载功能
    stcDmaCfg.enDestAddrReloadCtl = DmaMskDstAddrReloadEnable; // 使能DMA目的地址重载
    stcDmaCfg.enTransferMode = DmaMskOneTransfer;              // 一次传输，DMAC传输完成时清除CONFA:ENS位
    stcDmaCfg.enDstAddrMode = DmaMskDstAddrFix;                // 目的地址固定
    stcDmaCfg.enSrcAddrMode = DmaMskSrcAddrInc;                // 源地址自增
    stcDmaCfg.u16BlockSize = LOG_UART_DMA_WIDTH;               // 块传输个数
    stcDmaCfg.u16TransferCnt = 1;                              // 块传输次数，一次传输一个字节
    stcDmaCfg.enMode = DmaMskBlock;                            // 块(Block)传输
    stcDmaCfg.enTransferWidth = DmaMsk8Bit;                    // 8 bit  字节传输
    stcDmaCfg.enRequestNum = LOG_UART_DMA_TRIG;                // DMA硬件触发源位Uart1Tx
    stcDmaCfg.enPriority = DmaMskPriorityFix;                  // DMA 各通道优先级固定 (CH0>CH1)

    Dma_Enable();
    Dma_InitChannel(LOG_UART_DMA_CHANNEL, &stcDmaCfg); // DMA通道1初始化
    Dma_EnableChannelIrq(LOG_UART_DMA_CHANNEL);
    EnableNvic(LOG_UART_DMA_IRQ, LOG_UART_DMA_IRQ_LEVEL, TRUE);
}

void App_LcdCfg(void)
{
    stc_gpio_cfg_t stcGpioCfg = {0};
    stc_lcd_cfg_t LcdInitStruct = {0};
    stc_lcd_segcom_t LcdSegCom = {0};

    ///< 打开GPIO外设时钟门控
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    for (size_t i = 0; i < ARRAY_SZ(lcd_gpio); i++) {
        stcGpioCfg.enDir = GpioDirIn;
        stcGpioCfg.enPu = GpioPuDisable;
        stcGpioCfg.enPd = GpioPdDisable;
        stcGpioCfg.enCtrlMode = GpioAHB;
        Gpio_Init(lcd_gpio[i].port, lcd_gpio[i].pin, &stcGpioCfg);
        Gpio_SetAnalogMode(lcd_gpio[i].port, lcd_gpio[i].pin);
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
}

void LCD_TimeDisplay(void)
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

void LCD_ColonMask(void)
{
    // Mask colon
    M0P_LCD->RAM0 &= ~0x80000;
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
        Gpio_SetIO(STK_LED_PORT, STK_LED_PIN); ///< LED点亮
    }
    /* USER CODE END Error_Handler */
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


