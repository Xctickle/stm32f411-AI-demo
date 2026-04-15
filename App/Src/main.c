/*!
 * @file        main.c
 *
 * @brief       Main program body - STM32F411 Template
 *
 * LED blink example using HAL library.
 * Default LED pin: PC13 (Black Pill board)
 */

/* Includes */
#include "main.h"
#include "epd.h"

/* Private variables */
volatile uint32_t tick = 0;
SPI_HandleTypeDef hspi1;
uint8_t Image_BW[5000];  /* Frame buffer: 200 x 200 / 8 = 5000 bytes */

/* Private function prototypes */
void SystemClock_Config(void);
static void GPIO_Init(void);


/**
 * @brief  Main program
 */
int main(void)
{
    /* HAL Library initialization */
    HAL_Init();

    /* Configure system clock (100MHz) */
    SystemClock_Config();

    /* Initialize GPIO */
    GPIO_Init();

    /* Initialize SPI1 */
    MX_SPI1_Init();

    /* Initialize EPD and display "hello AI" */
    EPD_Init();
    Paint_NewImage(Image_BW, EPD_WIDTH, EPD_HEIGHT, ROTATE_0, EPD_WHITE);
    EPD_Clear(EPD_WHITE);
    EPD_ShowString(56, 90, (const uint8_t *)"hello AI", 16, EPD_BLACK);
    EPD_Display(Image_BW);

    while (1)
    {
        /* Toggle LED every 500ms */
        uint32_t now = HAL_GetTick();
        if (now - tick >= 500)
        {
            tick = now;
            HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
        }
    }
}

/**
 * @brief  System Clock Configuration
 *         HSE = 8MHz, SYSCLK = 100MHz
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* Configure voltage scaling */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Configure HSE and PLL */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 200;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /* Configure CPU, AHB and APB buses clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief  GPIO Initialization
 *         Configure LED pin (PC13) as push-pull output
 */
static void GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable GPIO clock */
    LED_GPIO_CLK_ENABLE();

    /* Configure LED pin */
    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);

    /* Turn off LED (PC13 is active low on Black Pill) */
    HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_SET);
}

/**
 * @brief  Error Handler
 */
void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    (void)file;
    (void)line;
}
#endif

/**
 * @brief  SPI1 Initialization
 *         Master mode, TX-only (1-line), 8-bit data, Software NSS
 *         CPOL=Low, CPHA=1Edge, Prescaler=32 (~3.125 MHz)
 */
void MX_SPI1_Init(void)
{
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_1LINE;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 10;

    if (HAL_SPI_Init(&hspi1) != HAL_OK)
    {
        Error_Handler();
    }
}
