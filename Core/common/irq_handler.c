#include "hal_gpio.h"
#include "tiny_printf.h"
#include "main.h"

void BSP_KEY_IRQ_Handler(void)
{
    if (hal_gpio_get_irq_status(BSP_KEY_Pin)) {
        hal_gpio_irq_handler(BSP_KEY_Pin);
        hal_gpio_clear_pending_irq(BSP_KEY_Pin);
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
            lcd_mask_colon();
        } else {
            lcd_display_time();
        }
        Rtc_ClearPrdfItStatus();
    } else {
        Error_Handler();
    }
}

void Dmac_IRQHandler(void)
{
#ifdef LOG_USING_LPUART_DMA
    if (Dma_GetStat(LOG_LPUART_DMA_CHANNEL) == DmaTransferComplete) {
        Dma_ClrStat(LOG_LPUART_DMA_CHANNEL);
        printf_dma_done_irq_handle();
    } else {
        Error_Handler();
    }
#endif
}
