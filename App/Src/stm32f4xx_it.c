/*!
 * @file        stm32f4xx_it.c
 *
 * @brief       Main Interrupt Service Routines - STM32F411
 */

/* Includes */
#include "stm32f4xx_it.h"

/* Private variables */
extern TIM_HandleTypeDef htim2;
extern volatile uint8_t pwm_duty;
static uint8_t pwm_counter = 0;

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
    while (1);
}

void MemManage_Handler(void)
{
    while (1);
}

void BusFault_Handler(void)
{
    while (1);
}

void UsageFault_Handler(void)
{
    while (1);
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
    HAL_IncTick();
}

/******************************************************************************/
/*                TIM6 Interrupt Handler (Software PWM for Breathing LED)     */
/******************************************************************************/

void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim2);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        pwm_counter++;
        if (pwm_counter >= 100)
            pwm_counter = 0;

        /* PC13 is active low: RESET = LED on, SET = LED off */
        if (pwm_counter < pwm_duty)
            HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_RESET);
        else
            HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_SET);
    }
}
