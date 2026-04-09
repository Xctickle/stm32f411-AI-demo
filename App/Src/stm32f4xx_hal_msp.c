/*!
 * @file        stm32f4xx_hal_msp.c
 *
 * @brief       HAL MSP Initialization and De-Initialization callbacks
 *
 * This file provides the default callback implementations for HAL peripheral
 * initialization. Override as needed for specific hardware configuration.
 */

/* Includes */
#include "main.h"

/**
  * @brief  Initialize the Global MSP.
  */
void HAL_MspInit(void)
{
    /* NOTE: This function is called automatically by HAL_Init() */
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
}

/**
  * @brief  DeInitialize the Global MSP.
  */
void HAL_MspDeInit(void)
{
}
