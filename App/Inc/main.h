/*!
 * @file        main.h
 *
 * @brief       Header for main.c module - STM32F411 Template
 */

#ifndef __MAIN_H
#define __MAIN_H

/* Includes */
#include "stm32f4xx_hal.h"

/* Exported types */
/* Exported constants */
/* Exported macro */

/* LED Pin Definition (Black Pill: PC13) */
#define LED_PIN         GPIO_PIN_13
#define LED_GPIO_PORT   GPIOC
#define LED_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOC_CLK_ENABLE()

/* Exported functions */
void Error_Handler(void);

#endif /* __MAIN_H */
