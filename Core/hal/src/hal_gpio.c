/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */
#include <stdbool.h> // bool type
#include <stdint.h>  // C99 types

#include "hal_gpio.h"
#include "main.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */
#define GPIO_CALLBACK_NUM 16
#define PIN_PORT(MAP, X)  MAP[PIN_NUM((X) >> 4)]
#define PIN_NUM(X)        ((en_gpio_pin_t)((uint32_t)(X) & 0xF))
#define PIN_IRQ(X)        ((IRQn_Type)((X) >> 4))

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */
static const en_gpio_port_t gpio_port_map[] = {GpioPortA, GpioPortB, GpioPortC, GpioPortD, GpioPortE, GpioPortF};

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/*!
 * GPIO setup data structure
 */
typedef struct hal_gpio_s
{
    hal_gpio_pin_names_t pin;
    uint32_t             mode;
    uint32_t             pull;
    uint32_t             alternate;
} gpio_t;

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

/*!
 * Array holding attached IRQ gpio data context
 */
static hal_gpio_irq_t const *gpio_irq[GPIO_CALLBACK_NUM] = {
    0,
};

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

//
// MCU input pin Handling
//

void hal_gpio_init_in(
    const hal_gpio_pin_names_t pin,
    const hal_gpio_pull_mode_t pull_mode,
    const hal_gpio_irq_mode_t  irq_mode,
    hal_gpio_irq_t            *irq)
{
    stc_gpio_cfg_t stcGpioCfg = {0};

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    stcGpioCfg.enDir = GpioDirIn;
    stcGpioCfg.enOD  = GpioOdDisable;

    switch (pull_mode) {
    case HAL_GPIO_PULL_MODE_UP:
        stcGpioCfg.enPu = GpioPuEnable;
        stcGpioCfg.enPd = GpioPdDisable;
        break;
    case HAL_GPIO_PULL_MODE_DOWN:
        stcGpioCfg.enPu = GpioPuDisable;
        stcGpioCfg.enPd = GpioPdEnable;
        break;
    default:
        stcGpioCfg.enPu = GpioPuDisable;
        stcGpioCfg.enPd = GpioPdDisable;
        break;
    }

    stcGpioCfg.enDrv      = GpioDrvL;
    stcGpioCfg.enCtrlMode = GpioAHB;

    Gpio_Init(PIN_PORT(gpio_port_map, pin), PIN_NUM(pin), &stcGpioCfg);

    if (irq_mode != HAL_GPIO_IRQ_MODE_OFF && irq != NULL) {
        en_gpio_irqtype_t enType;
        switch (irq_mode) {
        case HAL_GPIO_IRQ_MODE_RISING:
            enType = GpioIrqRising;
            break;
        case HAL_GPIO_IRQ_MODE_FALLING:
            enType = GpioIrqFalling;
            break;
        default:
            return;
        }

        Gpio_EnableIrq(PIN_PORT(gpio_port_map, pin), PIN_NUM(pin), enType);
        NVIC_SetPriority(PIN_IRQ(pin), 0);
        NVIC_EnableIRQ(PIN_IRQ(pin));
        irq->pin = pin;
    }
}

void hal_gpio_init_out(const hal_gpio_pin_names_t pin, hal_gpio_pull_mode_t pull_mode, const uint32_t value)
{
    stc_gpio_cfg_t stcGpioCfg = {0};

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    stcGpioCfg.enDir = GpioDirOut;
    switch (pull_mode) {
    case HAL_GPIO_PULL_MODE_UP:
        stcGpioCfg.enPu = GpioPuEnable;
        stcGpioCfg.enPd = GpioPdDisable;
        break;
    case HAL_GPIO_PULL_MODE_DOWN:
        stcGpioCfg.enPu = GpioPuDisable;
        stcGpioCfg.enPd = GpioPdEnable;
        break;
    default:
        stcGpioCfg.enPu = GpioPuDisable;
        stcGpioCfg.enPd = GpioPdDisable;
        break;
    }
    stcGpioCfg.enDrv      = GpioDrvH;
    stcGpioCfg.enCtrlMode = GpioAHB;
    stcGpioCfg.bOutputVal = value ? TRUE : FALSE;

    Gpio_Init(PIN_PORT(gpio_port_map, pin), PIN_NUM(pin), &stcGpioCfg);
}

void hal_gpio_init_af(
    const hal_gpio_pin_names_t pin,
    const bool                 is_input,
    const hal_gpio_pull_mode_t pull_mode,
    const hal_gpio_af_t        alternate)
{
    stc_gpio_cfg_t stcGpioCfg = {0};

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);

    stcGpioCfg.enDir = is_input ? GpioDirIn : GpioDirOut;
    stcGpioCfg.enOD  = GpioOdDisable;
    switch (pull_mode) {
    case HAL_GPIO_PULL_MODE_UP:
        stcGpioCfg.enPu = GpioPuEnable;
        stcGpioCfg.enPd = GpioPdDisable;
        break;
    case HAL_GPIO_PULL_MODE_DOWN:
        stcGpioCfg.enPu = GpioPuDisable;
        stcGpioCfg.enPd = GpioPdEnable;
        break;
    default:
        stcGpioCfg.enPu = GpioPuDisable;
        stcGpioCfg.enPd = GpioPdDisable;
        break;
    }
    stcGpioCfg.enCtrlMode = GpioAHB;

    Gpio_Init(PIN_PORT(gpio_port_map, pin), PIN_NUM(pin), &stcGpioCfg);
    if (alternate == HAL_GPIO_AF_ANALOG) {
        Gpio_SetAnalogMode(PIN_PORT(gpio_port_map, pin), PIN_NUM(pin));
    } else {
        Gpio_SetAfMode(PIN_PORT(gpio_port_map, pin), PIN_NUM(pin), (en_gpio_af_t)alternate);
    }
}

void hal_gpio_irq_attach(const hal_gpio_irq_t *irq)
{
    if ((irq != NULL) && (irq->callback != NULL)) {
        if (gpio_irq[PIN_NUM(irq->pin)] != NULL && gpio_irq[PIN_NUM(irq->pin)] != irq) {
            /* irq_handler already used */
            Error_Handler();
        }
        gpio_irq[PIN_NUM(irq->pin)] = irq;
    }
}

void hal_gpio_irq_deatach(const hal_gpio_irq_t *irq)
{
    if (irq != NULL) {
        gpio_irq[PIN_NUM(irq->pin)] = NULL;
    }
}

void hal_gpio_irq_enable(void)
{
    NVIC_EnableIRQ(PORTA_IRQn);
    NVIC_EnableIRQ(PORTB_IRQn);
    NVIC_EnableIRQ(PORTC_E_IRQn);
    NVIC_EnableIRQ(PORTD_F_IRQn);
}

void hal_gpio_irq_disable(void)
{
    NVIC_DisableIRQ(PORTA_IRQn);
    NVIC_DisableIRQ(PORTB_IRQn);
    NVIC_DisableIRQ(PORTC_E_IRQn);
    NVIC_DisableIRQ(PORTD_F_IRQn);
}

//
// MCU pin state control
//

void hal_gpio_set_value(const hal_gpio_pin_names_t pin, const uint32_t value)
{
    if (value != FALSE) {
        Gpio_SetIO(PIN_PORT(gpio_port_map, pin), PIN_NUM(pin));
    } else {
        Gpio_ClrIO(PIN_PORT(gpio_port_map, pin), PIN_NUM(pin));
    }
}

uint32_t hal_gpio_get_value(const hal_gpio_pin_names_t pin)
{
    return (Gpio_GetInputIO(PIN_PORT(gpio_port_map, pin), PIN_NUM(pin)) != FALSE) ? TRUE : FALSE;
}

uint32_t hal_gpio_get_irq_status(const hal_gpio_pin_names_t pin)
{
    return Gpio_GetIrqStatus(PIN_PORT(gpio_port_map, pin), PIN_NUM(pin));
}

void hal_gpio_clear_pending_irq(const hal_gpio_pin_names_t pin)
{
    Gpio_ClearIrq(PIN_PORT(gpio_port_map, pin), PIN_NUM(pin));
}

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

/**
 * @brief  Handle EXTI interrupt request.
 * @param  GPIO_Pin Specifies the port pin connected to corresponding EXTI line.
 * @retval None
 */

void hal_gpio_irq_handler(const hal_gpio_pin_names_t pin)
{
    uint8_t callback_index = PIN_NUM(pin);

    if ((gpio_irq[callback_index] != NULL) && (gpio_irq[callback_index]->callback != NULL)) {
        gpio_irq[callback_index]->callback(gpio_irq[callback_index]->context);
    }
}

/* --- EOF ------------------------------------------------------------------ */
