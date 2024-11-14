#ifndef __HAL_GPIO_H
#define __HAL_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */
#include <stdbool.h>
#include <stdint.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */
typedef enum gpio_pin_names_e
{
    // GPIOA
    PA_0  = 0x00,
    PA_1  = 0x01,
    PA_2  = 0x02,
    PA_3  = 0x03,
    PA_4  = 0x04,
    PA_5  = 0x05,
    PA_6  = 0x06,
    PA_7  = 0x07,
    PA_8  = 0x08,
    PA_9  = 0x09,
    PA_10 = 0x0A,
    PA_11 = 0x0B,
    PA_12 = 0x0C,
    PA_13 = 0x0D,
    PA_14 = 0x0E,
    PA_15 = 0x0F,
    // GPIOB
    PB_0  = 0x10,
    PB_1  = 0x11,
    PB_2  = 0x12,
    PB_3  = 0x13,
    PB_4  = 0x14,
    PB_5  = 0x15,
    PB_6  = 0x16,
    PB_7  = 0x17,
    PB_8  = 0x18,
    PB_9  = 0x19,
    PB_10 = 0x1A,
    PB_11 = 0x1B,
    PB_12 = 0x1C,
    PB_13 = 0x1D,
    PB_14 = 0x1E,
    PB_15 = 0x1F,
    // GPIOC
    PC_0  = 0x20,
    PC_1  = 0x21,
    PC_2  = 0x22,
    PC_3  = 0x23,
    PC_4  = 0x24,
    PC_5  = 0x25,
    PC_6  = 0x26,
    PC_7  = 0x27,
    PC_8  = 0x28,
    PC_9  = 0x29,
    PC_10 = 0x2A,
    PC_11 = 0x2B,
    PC_12 = 0x2C,
    PC_13 = 0x2D,
    PC_14 = 0x2E,
    PC_15 = 0x2F,
    // GPIOD
    PD_0 = 0x30,
    PD_1 = 0x31,
    PD_2 = 0x32,
    PD_3 = 0x33,
    PD_4 = 0x34,
    PD_5 = 0x35,
    PD_6 = 0x36,
    PD_7 = 0x37,
    PD_8 = 0x38,
    PD_9 = 0x39,
    PD_10 = 0x3A,
    PD_11 = 0x3B,
    PD_12 = 0x3C,
    PD_13 = 0x3D,
    PD_14 = 0x3E,
    PD_15 = 0x3F,
    // GPIOE
    PE_0 = 0x40,
    PE_1 = 0x41,
    PE_2 = 0x42,
    PE_3 = 0x43,
    PE_4 = 0x44,
    PE_5 = 0x45,
    PE_6 = 0x46,
    PE_7 = 0x47,
    PE_8 = 0x48,
    PE_9 = 0x49,
    PE_10 = 0x4A,
    PE_11 = 0x4B,
    PE_12 = 0x4C,
    PE_13 = 0x4D,
    PE_14 = 0x4E,
    PE_15 = 0x4F,
    // GPIOF
    PF_0 = 0x50,
    PF_1 = 0x51,
    PF_2 = 0x52,
    PF_3 = 0x53,
    PF_4 = 0x54,
    PF_5 = 0x55,
    PF_6 = 0x56,
    PF_7 = 0x57,
    PF_8 = 0x58,
    PF_9 = 0x59,
    PF_10 = 0x5A,
    PF_11 = 0x5B,
    PF_12 = 0x5C,
    PF_13 = 0x5D,
    PF_14 = 0x5E,
    PF_15 = 0x5F,
    // Not connected
    NC = -1
} hal_gpio_pin_names_t;

/*!
 * GPIO IRQ data context
 */
typedef struct gpio_irq_s
{
    hal_gpio_pin_names_t pin;
    void*                context;
    void                 ( *callback )( void* context );
} hal_gpio_irq_t;

/*!
 * GPIO Pull modes
 */
typedef enum gpio_pull_mode_e
{
    HAL_GPIO_PULL_MODE_NONE = 0,
    HAL_GPIO_PULL_MODE_UP   = 1,
    HAL_GPIO_PULL_MODE_DOWN = 2,
} hal_gpio_pull_mode_t;

/*!
 * GPIO IRQ modes
 */
typedef enum gpio_irq_mode_e
{
    HAL_GPIO_IRQ_MODE_NONE            = 0,
    HAL_GPIO_IRQ_MODE_RISING         = 1,
    HAL_GPIO_IRQ_MODE_FALLING        = 2,
} hal_gpio_irq_mode_t;

/*!
 * GPIO alternate function
 */
typedef enum gpio_af_e
{
    HAL_GPIO_AF_ANALOG = 0,
    HAL_GPIO_AF_DIGITAL_1 = 1,
    HAL_GPIO_AF_DIGITAL_2 = 2,
    HAL_GPIO_AF_DIGITAL_3 = 3,
    HAL_GPIO_AF_DIGITAL_4 = 4,
    HAL_GPIO_AF_DIGITAL_5 = 5,
    HAL_GPIO_AF_DIGITAL_6 = 6,
    HAL_GPIO_AF_DIGITAL_7 = 7,
} hal_gpio_af_t;

typedef enum gpio_alternate_function_e
{
    HAL_GPIO_ALTERNATE_FUNCTION_0 = 0,
} hal_gpio_alternate_function_t;
/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

/*!
 * Initializes given pin as output with given initial value
 *
 * \param [in] pin   MCU pin to be initialized
 * \param [in] value MCU initial pit state
 *
 */
void hal_gpio_init_out(const hal_gpio_pin_names_t pin, hal_gpio_pull_mode_t pull_mode, const uint32_t value);

/*!
 * Initializes given pin as input
 *
 * \param [in]     pin       MCU pin to be initialized
 * \param [in]     pull_mode MCU pin pull mode [HAL_GPIO_PULL_MODE_NONE,
 *                                              HAL_GPIO_PULL_MODE_UP,
 *                                              HAL_GPIO_PULL_MODE_DOWN]
 * \param [in]     irq_mode  MCU IRQ mode [HAL_GPIO_IRQ_MODE_NONE,
 *                                         HAL_GPIO_IRQ_MODE_RISING,
 *                                         HAL_GPIO_IRQ_MODE_FALLING]
 * \param [in/out] irq       Pointer to IRQ data context.
 *                              NULL when HAL_GPIO_IRQ_MODE_NONE
 *                              pin parameter is initialized
 * \note IRQ callback only enable after hal_gpio_irq_attach
 */
void hal_gpio_init_in( const hal_gpio_pin_names_t pin, const hal_gpio_pull_mode_t pull_mode,
                       const hal_gpio_irq_mode_t irq_mode, hal_gpio_irq_t* irq );

/*!
 * Initializes given pin as alternate function
 *
 * \param [in] pin       MCU pin to be initialized
 * \param [in] is_input  MCU pin direction [TRUE, FALSE]
 * \param [in] pull_mode MCU pin pull mode [HAL_GPIO_PULL_MODE_NONE,
 *                                         HAL_GPIO_PULL_MODE_UP,
 *                                         HAL_GPIO_PULL_MODE_DOWN]
 * \param [in] alternate MCU pin alternate function [HAL_GPIO_AF_ANALOG, ...]
 */
void hal_gpio_init_af(const hal_gpio_pin_names_t pin, const bool is_input, const hal_gpio_pull_mode_t pull_mode, const hal_gpio_af_t alternate);

/*!
 * Attaches given callback to the MCU IRQ handler
 *
 * \param [in] irq     Pointer to IRQ data context
 */
void hal_gpio_irq_attach( const hal_gpio_irq_t* irq );

/*!
 * Detattaches callback from the MCU IRQ handler
 *
 * \param [in] irq     Pointer to IRQ data context
 */
void hal_gpio_irq_deatach( const hal_gpio_irq_t* irq );

/*!
 * Enables all GPIO MCU interrupts
 */
void hal_gpio_irq_enable( void );

/*!
 * Disables all GPIO MCU interrupts
 */
void hal_gpio_irq_disable( void );

/*!
 * Sets MCU pin to given value
 *
 * \param [in] pin   MCU pin to be set
 * \param [in] value MCU pin state to be set
 */
void hal_gpio_set_value( const hal_gpio_pin_names_t pin, const uint32_t value );

/*!
 * Gets MCU pin state value
 *
 * \param [in] pin   MCU pin to be read
 *
 * \retval value Current MCU pin state
 */
uint32_t hal_gpio_get_value( const hal_gpio_pin_names_t pin );

/*
 * Get IRQ status
 */
uint32_t hal_gpio_get_irq_status( const hal_gpio_pin_names_t pin );

/*
 * Clears a pending irq on a pin
 *
 * \param [in] pin   pin for which pending state is to be cleared
 */
void hal_gpio_clear_pending_irq( const hal_gpio_pin_names_t pin );

/**
  * @brief  Handle EXTI interrupt request.
  * @param  GPIO_Pin Specifies the port pin connected to corresponding EXTI line.
  * @retval None
  */

void hal_gpio_irq_handler( const hal_gpio_pin_names_t pin );

#ifdef __cplusplus
}
#endif

#endif /* __HAL_GPIO_H */
/* --- EOF ------------------------------------------------------------------ */
