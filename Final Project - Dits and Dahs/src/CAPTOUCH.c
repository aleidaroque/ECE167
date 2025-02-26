#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <timers.h>

#include <ADC.h>
#include <pwm.h>
#include <buttons.h>
#include <QEI.h>
#include <PING.h>
#include <CAPTOUCH.h>
#include <Board.h>

static int prev = 0;
static int cur = 0;
static int ave = 0;
static int total = 0;
static int ave_list[6] = {};
static int index = 0;
static int diff = 0;
//static char touch_status;

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

    TIMER_Init();
    prev = 0;

    // the rest of the function goes here
}

// external interrupt ISR for rising edge of pin PB5
void EXTI9_5_IRQHandler(void)
{
    // EXTI line interrupt detected
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_5) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5); // clear interrupt flag
    }
        // anything that needs to happen on rising edge of PB5 (ENC_B)
        // essesntially a moving average filter, get new value->get rid of current index's
        // item->add new item->take new average->go to IsTouch->
        // next index and now new ave is prev ave

        prev = ave;                     // setting previous average to prev
        cur = TIMERS_GetMicroSeconds(); // get current time of change
        total -= ave_list[index];       // subtract old value from list
        ave_list[index] = cur;          // add new current time into index's spot
        total += cur;                   // add the cur time to your whole average
        ave = (total / 5);              // actual average equation
        index = ((index + 1) % 5);      // using modular to not have to reset index

}
char CAPTOUCH_IsTouched(void)
{

    diff = (ave - prev); // difference between averages with old item and new item (singular)
    //printf("difference %d \n", diff);
    if (diff > 8500)
    { // untouched: period = 625 us, touched: period = 3 ms.
        return TRUE;
    }
    else
    {
        //printf("flase");
        return FALSE;
    }

}