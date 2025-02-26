#include <Board.h>
#include <timers.h>
#include <stdint.h>

#include <pwm.h>
#include <ADC.h>

static int prevT = 0;
static int currT = 0;
static int avgT = 0;
static int LIST[5] = {};
static int ind = 0;
static int total = 0;
static int change = 0;

#define MAX 700
// boundary of touch no touch


void CAPTOUCH_Init(void)
{
    // Configure GPIO pin PB5
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // EXTI interrupt init
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    // the rest of the function goes here
    TIMER_Init();
    prevT = 0;
}

void EXTI9_5_IRQHandler(void)
{
    // EXTI line interrupt detected
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_5) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5); // clear interrupt flag

        // anything that needs to happen on rising edge of PB5 (ENC_B)
    }
    // first set the previous time measured to the average time
    prevT = avgT;

    // subtract the last time placed in the array from the total
    total -= LIST[ind];

    // read in the current time
    currT = TIMERS_GetMicroSeconds();

    // add the new reading into the array
    LIST[ind] = currT;

    // calculate the new total
    total += currT;

    // move array cursor
    ind = (ind + 1) % 5;

    // compute the average of the last five times 
    avgT = total / 5;
}

char CAPTOUCH_IsTouched(void)
{
    change = avgT - prevT;
    if (change >= MAX)
    {
        return TRUE;
    }
    return FALSE;
}