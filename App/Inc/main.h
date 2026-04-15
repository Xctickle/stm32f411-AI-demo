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

/* SPI1 Pin Definition */
#define SPI1_SCK_PIN    GPIO_PIN_5
#define SPI1_SCK_PORT   GPIOA
#define SPI1_MOSI_PIN   GPIO_PIN_7
#define SPI1_MOSI_PORT  GPIOA

/* EPD Control Pin Definition */
#define EPD_CS_PIN      GPIO_PIN_0
#define EPD_CS_PORT     GPIOB
#define EPD_DC_PIN      GPIO_PIN_2
#define EPD_DC_PORT     GPIOB
#define EPD_RST_PIN     GPIO_PIN_1
#define EPD_RST_PORT    GPIOB
#define EPD_BUSY_PIN    GPIO_PIN_6
#define EPD_BUSY_PORT   GPIOA

/* Exported variables */
extern SPI_HandleTypeDef hspi1;

/* Exported functions */
void Error_Handler(void);
void MX_SPI1_Init(void);

#endif /* __MAIN_H */
