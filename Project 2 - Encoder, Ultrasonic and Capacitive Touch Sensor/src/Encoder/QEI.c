/**
   @file QEI.c
   @brief Quadrature Encoder sensing module
   @author Aleida Diaz-Roque
*/

#include <Board.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include <stm32f4xx_hal_tim.h>
#include <stdio.h>

#include <QEI.h>
#include <pwm.h>

static int position = 0;
static int prev_pos = 0;
static int state = 0b00;
static int color = 0;

static int r = 0;
static int g = 0;
static int b = 100;

/// RBG converted values to duty
#define C255 0
#define C192 25
#define C125 50
#define C64 75
#define C0 100

void circuilarity()
{
    if (position > 100)
    {
        position = 0;
    }
    else if (position < 0)
    {
        position = 100;
    }
    return;
}

int QEI_GetPosition()
{
    // map the count to 24 (number of colors)
    int tick = 0;
    tick = (position / 100.0) * 23.0;
    return tick;
}

// based on QEI get position
void QEI_COLOR()
{
    switch (color)
    {
    case (0):
        r = C255;
        g = C255;
        b = C0;
        color = QEI_GetPosition();
        break;
    case (1):
        r = C255;
        g = C192;
        b = C0;
        color = QEI_GetPosition();
        break;
    case (2):
        r = C255;
        g = C125;
        b = C0;
        color = QEI_GetPosition();
        break;
    case (3):
        r = C255;
        g = C64;
        b = C0;
        color = QEI_GetPosition();
        break;
    case (4):
        r = C255;
        g = C0;
        b = C0;
        color = QEI_GetPosition();
        break;
    case (5):
        r = C255;
        g = C0;
        b = C64;
        color = QEI_GetPosition();
        break;
    case (6):
        r = C255;
        g = C0;
        b = C125;
        color = QEI_GetPosition();
        break;
    case (7):
        r = C255;
        g = C0;
        b = C192;
        color = QEI_GetPosition();
        break;
    case (8):
        r = C255;
        g = C0;
        b = C255;
        color = QEI_GetPosition();
        break;
    case (9):
        r = C192;
        g = C0;
        b = C255;
        color = QEI_GetPosition();
        break;
    case (10):
        r = C125;
        g = C0;
        b = C255;
        color = QEI_GetPosition();
        break;
    case (11):
        r = C64;
        g = C0;
        b = C255;
        color = QEI_GetPosition();
        break;
    case (12):
        r = C0;
        g = C0;
        b = C255;
        color = QEI_GetPosition();
        break;
    case (13):
        r = C0;
        g = C64;
        b = C255;
        color = QEI_GetPosition();
        break;
    case (14):
        r = C0;
        g = C125;
        b = C255;
        color = QEI_GetPosition();
        break;
    case (15):
        r = C0;
        g = C192;
        b = C255;
        color = QEI_GetPosition();
        break;
    case (16):
        r = C0;
        g = C255;
        b = C255;
        color = QEI_GetPosition();
        break;
    case (17):
        r = C0;
        g = C255;
        b = C192;
        color = QEI_GetPosition();
        break;
    case (18):
        r = C0;
        g = C255;
        b = C125;
        color = QEI_GetPosition();
        break;
    case (19):
        r = C0;
        g = C255;
        b = C64;
        color = QEI_GetPosition();
        break;
    case (20):
        r = C0;
        g = C255;
        b = C0;
        color = QEI_GetPosition();
        break;
    case (21):
        r = C64;
        g = C255;
        b = C0;
        color = QEI_GetPosition();
        break;
    case (22):
        r = C125;
        g = C255;
        b = C0;
        color = QEI_GetPosition();
        break;
    case (23):
        r = C192;
        g = C255;
        b = C0;
        color = QEI_GetPosition();
        break;

    default:
        break;
    }
}

void QEI_IRQ()
{
    char A = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);
    char B = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);

    switch (state)
    {
    case (0b11):
        prev_pos = position;
        // CW = +
        if (A == 0 && B == 1)
        {
            position += 1;
            state = 0b01;
        }
        // CCW = -
        if (A == 1 && B == 0)
        {
            position -= 1;
            state = 0b10;
        }
        circuilarity();
        QEI_COLOR();
        printf("1 Postion: %d       RGB: (%d, %d, %d) \n", position, r, g, b);

        break;

    case (0b01):
        prev_pos = position;
        // CW = +
        if (A == 0 && B == 0)
        {
            position += 1;
            state = 0b00;
        }
        // CCW = -
        if (A == 1 && B == 1)
        {
            position -= 1;
            state = 0b11;
        }
        circuilarity();
        QEI_COLOR();
        if (position != prev_pos)
        {
            printf("1 Postion: %d       RGB: (%d, %d, %d) \n", position, r, b, b);
        }
        break;

    case (0b00):
        prev_pos = position;
        // CW = +
        if (A == 1 && B == 0)
        {

            position += 1;
            state = 0b10;
        }
        // CCW = -
        if (A == 0 && B == 1)
        {
            position -= 1;
            state = 0b01;
        }
        circuilarity();
        QEI_COLOR();
        if (position != prev_pos)
        {
            printf("1 Postion: %d       RGB: (%d, %d, %d) \n", position, r, g, b);
        }
        break;

    case (0b10):
        prev_pos = position;
        // CW = +
        if (A == 1 && B == 1)
        {
            position += 1;
            state = 0b11;
        }
        // CCW = -
        if (A == 0 && B == 0)
        {
            position -= 1;
            state = 0b00;
        }
        circuilarity();
        QEI_COLOR();
        if (position != prev_pos)
        {
            printf("1 Postion: %d       RGB: (%d, %d, %d) \n", position, r, g, b);
        }
        break;

    default:
        break;
    }

    return;
}

void QEI_Init(void)
{
    // Configure GPIO pins : PB4 PB5
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // EXTI interrupt init
    HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    // the rest of the function goes here
    QEI_IRQ();
}

// external interrupt ISR for pin PB5
void EXTI9_5_IRQHandler(void)
{
    // EXTI line interrupt detected
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_5) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5); // clear interrupt flag

        QEI_IRQ();
        // call the color change
    }
}

// external interrupt ISR for pin PB4
void EXTI4_IRQHandler(void)
{
    // EXTI line interrupt detected
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_4) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4); // clear interrupt flag

        QEI_IRQ();
        // call the color change
    }
}

void QEI_ResetPosition()
{
    position = 0;
}

int main(int argc, char const *argv[])

{
    BOARD_Init();
    QEI_Init();
    PWM_Init();

    PWM_SetFrequency(1000);

    while (TRUE)
    {
        printf("Hello\n");

        // initializing interrupts because main inside client file
        EXTI9_5_IRQHandler();
        EXTI4_IRQHandler();

        // setting color hue of each pin
        PWM_SetDutyCycle(PWM_0, r);
        PWM_SetDutyCycle(PWM_1, g);
        PWM_SetDutyCycle(PWM_2, b);
    }
}