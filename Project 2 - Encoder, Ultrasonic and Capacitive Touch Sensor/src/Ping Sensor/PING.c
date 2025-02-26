/**
   @file PING.c
   @brief Radar sensing module
   @author Aleida Diaz-Roque
*/

#include <stdint.h>
#include <stdio.h>
#include "Board.h"
#include "timers.h"
#include "stm32f4xx_hal.h"
#include "stm32f411xe.h"
#include "stm32f4xx_hal_tim.h"
#include "pwm.h"
#include <math.h>

#include <PING.h>
#include <QEI.h>
#include <ADC.h>

#define ALPHA 0.9
#define OFFSET 100

static int switchTime = 1;
static int startTime = 0;
static int endTime = 0;
static int distance = 0;
static int prevRead = 0;
static int currRead = 0;
static int frequency = 100;

unsigned int PING_GetDistance(void)
{
    int flightTime = PING_GetTimeofFlight();
    // filter out negative values
    if (flightTime > 0)
    {
        distance = 0.0187 * flightTime + 0.9253 -1;
        //printf("        distance: %d\n", distance);
    }
    return distance;
}

unsigned int PING_GetTimeofFlight(void)
{
    //printf("\n diff: %d", endTime - startTime);

    // return raw calculations
    return endTime - startTime;
}

char PING_Init(void)
{
    // init other libraries
    BOARD_Init();
    TIMER_Init();
    PWM_Init();
    ADC_Init();

    // this block initializes the GPIO output pin (PB8, PWM_5 on shield)
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // this block inits the timer generated interrupt
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 83; // divide by 1 prescaler (84-1) = 1 Mhz tick
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 0xFFFF; // MUST CHANGE. number of clock cycles between interrupts
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
    {
        return ERROR;
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
    {
        return ERROR;
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
    {
        return ERROR;
    }
    HAL_TIM_Base_Start_IT(&htim3); // start interrupt

    // this block inits the external pin-generted interrupt on any change of pin PB5 (ENC_B)
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    return SUCCESS;
}

// external interrupt ISR for pin PB5
void EXTI9_5_IRQHandler(void)
{
    // EXTI line interrupt detected
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_5) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5); // clear interrupt flag
                                              // anything that needs to happen when PB5 (ENC_B) changes state
        // read the current state of the pin
        char pinState = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);

        // if pin sent signal then capture the time that the signal left
        if (pinState == 1)
        {
            startTime = TIMERS_GetMicroSeconds();
        }

        // if pin recieved signal then capture the time that the signal returned
        if (pinState == 0)
        {
            endTime = TIMERS_GetMicroSeconds();
        }
    }
}
// TIM3 ISR
void TIM3_IRQHandler(void)
{
    if (__HAL_TIM_GET_IT_SOURCE(&htim3, TIM_IT_UPDATE) != RESET)
    {
        // start the time by sending the signal
        switchTime = 0;

        // load the 10 us
        if (switchTime == 0)
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
            TIM3->ARR = 9;
            switchTime = 1;
        }

        // the 10 us will pass so load the 60 ms
        if (switchTime == 1)
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
            TIM3->ARR = 59999;
            switchTime = 0;
        }

        __HAL_TIM_CLEAR_IT(&htim3, TIM_IT_UPDATE); // clear interrupt flag
    }
}

int main(int argc, char const *argv[])

{
    PING_Init();

    while (TRUE)
    {
        // initializing interrupts because main inside client file
        TIM3_IRQHandler();
        EXTI9_5_IRQHandler();

        // EMA FILTER 
        // desired freq: 100-800
        prevRead = (PING_GetDistance() / 60.0) * 700 ;
        currRead = (PING_GetDistance() / 60.0) * 700 ;

        frequency = ((ALPHA)*currRead) + ((1 - ALPHA) * prevRead);

        // once freq. set the frequency of speaker 
        PWM_SetDutyCycle(PWM_0, 1);
        PWM_SetFrequency(frequency + 100);
        
        // print out data
        printf("time: %d     distance: %d     frequency:  %d\n", PING_GetTimeofFlight(), PING_GetDistance(), frequency);

    }

    return 0;
}
