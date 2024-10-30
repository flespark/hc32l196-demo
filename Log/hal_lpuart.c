#include "tiny_printf.h"
#include "hal_lpuart.h"
#include "main.h"

char printf_dma_buf[PRINTF_DMA_BUF_SIZE] = {0};

void hal_lpuart_init(void)
{
    stc_gpio_cfg_t GpioInitStruct = {0};
    stc_lpuart_cfg_t  LpuartInitStruct = {0};

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    GpioInitStruct.enDir = GpioDirOut;
    GpioInitStruct.enPu = GpioPuEnable;

    Gpio_Init(LOG_TX_Port, LOG_TX_Pin, &GpioInitStruct);
    Gpio_SetAfMode(LOG_TX_Port, LOG_TX_Pin, LOG_LPUART_AF);

    Sysctrl_SetPeripheralGate(LOG_LPUART_CLOCK, TRUE);

    LpuartInitStruct.enRunMode = LPUartMskMode1;
    LpuartInitStruct.enStopBit = LPUart1bit;
    LpuartInitStruct.enMmdorCk = LPUartDataOrAddr;
    LpuartInitStruct.stcBaud.u32Baud = LOG_LPUART_BAUDRATE;
    LpuartInitStruct.stcBaud.enSclkDiv = LOG_LPUART_CLOCK_DIV;
    LpuartInitStruct.stcBaud.u32Sclk = Sysctrl_GetPClkFreq();
    LPUart_Init(LOG_LPUART_SEL, &LpuartInitStruct);

#ifdef LOG_USING_LPUART_DMA
    LPUart_ClrStatus(LOG_LPUART_SEL, LPUartTC);
    LPUart_EnableFunc(LOG_LPUART_SEL, LPUartDmaTxFunc);
#endif
}

void hal_lpuart_dma_config(void)
{
    stc_dma_cfg_t stcDmaCfg = {0};

    Sysctrl_SetPeripheralGate(SysctrlPeripheralDma, TRUE); // 使能DMAC外设时钟门控开关

    stcDmaCfg.u32SrcAddress       = (uint32_t)(printf_dma_buf);      // 接收数据缓存
    stcDmaCfg.u32DstAddress       = (uint32_t)&LOG_LPUART_SEL->SBUF; // 发送数据寄存器地址
    stcDmaCfg.enSrcAddrReloadCtl  = DmaMskSrcAddrReloadEnable;       // 使能DMA源地址重载
    stcDmaCfg.enSrcBcTcReloadCtl  = DmaMskBcTcReloadEnable;          // 使能BC[3:0]和CONFA:TC[15:0]的重载功能
    stcDmaCfg.enDestAddrReloadCtl = DmaMskDstAddrReloadEnable;       // 使能DMA目的地址重载
    stcDmaCfg.enTransferMode      = DmaMskOneTransfer;    // 一次传输，DMAC传输完成时清除CONFA:ENS位
    stcDmaCfg.enDstAddrMode       = DmaMskDstAddrFix;     // 目的地址固定
    stcDmaCfg.enSrcAddrMode       = DmaMskSrcAddrInc;     // 源地址自增
    stcDmaCfg.u16BlockSize        = LOG_LPUART_DMA_WIDTH; // 块传输个数
    stcDmaCfg.u16TransferCnt      = 1;                    // 块传输次数，一次传输一个字节
    stcDmaCfg.enMode              = DmaMskBlock;          // 块(Block)传输
    stcDmaCfg.enTransferWidth     = DmaMsk8Bit;           // 8 bit  字节传输
    stcDmaCfg.enRequestNum        = LOG_LPUART_DMA_TRIG;  // DMA硬件触发源位Uart1Tx
    stcDmaCfg.enPriority          = DmaMskPriorityFix;    // DMA 各通道优先级固定 (CH0>CH1)

    Dma_Enable();
    Dma_InitChannel(LOG_LPUART_DMA_CHANNEL, &stcDmaCfg); // DMA通道1初始化
    Dma_EnableChannelIrq(LOG_LPUART_DMA_CHANNEL);
    EnableNvic(LOG_LPUART_DMA_IRQ, LOG_LPUART_DMA_IRQ_LEVEL, TRUE);
}