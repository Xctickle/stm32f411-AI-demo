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
  * @brief  SPI MSP Initialization.
  *         Configure SPI1 GPIO pins and enable clock.
  *         PA5=SCK, PA7=MOSI (AF5), PA6=BUSY(input), PB0=CS, PB1=RST, PB2=DC
  */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (hspi->Instance == SPI1)
    {
        /* Enable SPI1, GPIOA, GPIOB clocks */
        __HAL_RCC_SPI1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();

        /* Configure SPI1 AF pins: SCK(PA5), MOSI(PA7) */
        GPIO_InitStruct.Pin = SPI1_SCK_PIN | SPI1_MOSI_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* Configure BUSY(PA6) as input with pull-up */
        GPIO_InitStruct.Pin = EPD_BUSY_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(EPD_BUSY_PORT, &GPIO_InitStruct);

        /* Configure CS(PB0), RST(PB1), DC(PB2) as GPIO output */
        GPIO_InitStruct.Pin = EPD_CS_PIN | EPD_RST_PIN | EPD_DC_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        HAL_GPIO_Init(EPD_CS_PORT, &GPIO_InitStruct);

        /* Default: CS high (deselected), DC high, RST high */
        HAL_GPIO_WritePin(EPD_CS_PORT, EPD_CS_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(EPD_DC_PORT, EPD_DC_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(EPD_RST_PORT, EPD_RST_PIN, GPIO_PIN_SET);
    }
}

/**
  * @brief  SPI MSP DeInitialization.
  */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI1)
    {
        __HAL_RCC_SPI1_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOA, SPI1_SCK_PIN | SPI1_MOSI_PIN | EPD_BUSY_PIN);
        HAL_GPIO_DeInit(EPD_CS_PORT, EPD_CS_PIN);
        HAL_GPIO_DeInit(EPD_RST_PORT, EPD_RST_PIN);
        HAL_GPIO_DeInit(EPD_DC_PORT, EPD_DC_PIN);
    }
}

/**
  * @brief  DeInitialize the Global MSP.
  */
void HAL_MspDeInit(void)
{
}
