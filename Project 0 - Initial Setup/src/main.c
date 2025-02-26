#include <stdio.h>
#include <stdlib.h>
#include <Board.h>
#include <math.h>
#include <string.h>

#include <ADC.h>
#include <pwm.h>
#include <buttons.h>

// #define HELLO_WORLD
// #define ADC_POT
// #define TONE
// #define ADC_TONE
// #define BUTTONS
 #define OFFSET

// #define HBD


// Common use
#define DEL300 300
#define DEL100 100
#define DEL5 5


#define FREQ300 300
#define FREQ500 500
#define FREQ700 700
#define FREQ900 900

int currRead = 0;
int prevRead = 0;
int frequency = 0;

int main(void)
{
    BOARD_Init();
    PWM_Init();
    ADC_Init();
    BUTTONS_Init();

#ifdef HELLO_WORLD
    while (TRUE)
    {
        printf("Hello World\r\n");
    }
#endif

#ifdef ADC_POT
    while (TRUE)
    {
        printf("A/D reading from the potentiometer: %ld \n", ADC_Read(POT));
        HAL_Delay(DEL300);
    }
#endif

#ifdef TONE
    PWM_Init();
    PWM_SetDutyCycle(PWM_0, 90);
    PWM_SetFrequency(FREQ500); // lower = deeper
    while (TRUE)
        ;
#endif

#ifdef ADC_TONE

    while (TRUE)
    {

        // Using EMA Filter
        // two readings of potentiometer: currRead, prevRead
        // constant a
        // y[i] = a * x[i] + (1-a) * y[i-1]
        // output = currADC

        prevRead = ADC_Read(POT);
        currRead = ADC_Read(POT);

        frequency = ((ALPHA) * currRead) + ((1 - ALPHA) * prevRead);

        HAL_Delay(DEL5); // ensures a clearer reading

        PWM_SetDutyCycle(PWM_0, 1);
        PWM_SetFrequency(frequency);
    }

#endif

#ifdef BUTTONS
    ADC_Init();
    PWM_Init();
    BUTTONS_Init();

    uint8_t buttons = 0;
    while (TRUE)
    {
        buttons = buttons_state();

        // sanity print check
        printf("%d %d %d %d\r\n", (buttons >> 3) & 0x1, (buttons >> 2) & 0x1, (buttons >> 1) & 0x1, buttons & 0x1);
        HAL_Delay(DEL100);

        PWM_SetDutyCycle(PWM_0, 0);

        if (((buttons >> 3) & 0x1) == 0)
        {
            PWM_SetDutyCycle(PWM_0, 50);
            PWM_SetFrequency(FREQ300);
        }
        else if (((buttons >> 2) & 0x1) == 0)
        {
            PWM_SetDutyCycle(PWM_0, 50);
            PWM_SetFrequency(FREQ500);
        }
        else if (((buttons >> 1) & 0x1) == 0)
        {
            PWM_SetDutyCycle(PWM_0, 50);
            PWM_SetFrequency(FREQ700);
        }
        else if (((buttons >> 0) & 0x1) == 0)
        {
            PWM_SetDutyCycle(PWM_0, 50);
            PWM_SetFrequency(FREQ900);
        }
        else
        {
            // if no button is pressed reset to 0
            PWM_SetDutyCycle(PWM_0, 0);
        }
    }

#endif

#ifdef OFFSET

    uint8_t buttons = 0;
    while (TRUE)
    {

        // printing buttons
        buttons = buttons_state();
        printf("%d %d %d %d\r\n", (buttons >> 3) & 0x1, (buttons >> 2) & 0x1, (buttons >> 1) & 0x1, buttons & 0x1);
        HAL_Delay(DEL5);

        // start with no sound
        PWM_SetDutyCycle(PWM_0, 0);

        prevRead = 0;
        currRead = 0;

        if (((buttons >> 3) & 0x1) == 0)
        {

            prevRead = (ADC_Read(POT) / 4095.0) * 200;
            currRead = (ADC_Read(POT) / 4095.0) * 200;

            frequency = ((0.5) * currRead) + ((1 - 0.5) * prevRead);

            PWM_SetDutyCycle(PWM_0, 1);
            HAL_Delay(DEL5);
            PWM_SetFrequency(300 - frequency);
        }
        else if (((buttons >> 2) & 0x1) == 0)
        {
            prevRead = (ADC_Read(POT) / 4095.0) * 200;
            currRead = (ADC_Read(POT) / 4095.0) * 200;

            frequency = ((0.5) * currRead) + ((1 - 0.5) * prevRead);

            PWM_SetDutyCycle(PWM_0, 1);
            HAL_Delay(DEL5);
            PWM_SetFrequency(500 - frequency);
        }
        else if (((buttons >> 1) & 0x1) == 0)
        {
            prevRead = (ADC_Read(POT) / 4095.0) * 200;
            currRead = (ADC_Read(POT) / 4095.0) * 200;

            frequency = ((0.5) * currRead) + ((1 - 0.5) * prevRead);

            PWM_SetDutyCycle(PWM_0, 1);
            HAL_Delay(DEL5);
            PWM_SetFrequency(700 - frequency);
        }
        else if (((buttons >> 0) & 0x1) == 0)
        {
            prevRead = (ADC_Read(POT) / 4095.0) * 200;
            currRead = (ADC_Read(POT) / 4095.0) * 200;

            frequency = ((0.5) * currRead) + ((1 - 0.5) * prevRead);

            PWM_SetDutyCycle(PWM_0, 1);
            HAL_Delay(DEL5);
            PWM_SetFrequency(900 - frequency);
        }
        else
        {
            PWM_SetDutyCycle(PWM_0, 0);
        }
    }

#endif

#ifdef HBD

    // G – A – G – C – B – G – G – A – G – D – C – G – G – G – E – C – C – B – A – F – F – E – C – D – C.

    // C = 261
    // D = 293
    // E = 329
    // F = 349
    // G = 392
    // A = 440
    // B = 493

    PWM_Init();


    HAL_Delay(3000);
    PWM_SetDutyCycle(PWM_0, 1);

    PWM_SetFrequency(261);
    HAL_Delay(300);

    PWM_SetFrequency(293);
    HAL_Delay(300);

    PWM_SetFrequency(329);
    HAL_Delay(300);

    PWM_SetFrequency(349);
    HAL_Delay(300);

    PWM_SetFrequency(392);
    HAL_Delay(300);

    PWM_SetFrequency(440);
    HAL_Delay(300);

    PWM_SetFrequency(493);
    HAL_Delay(300);

    PWM_SetFrequency(523);
    HAL_Delay(900);


    while (TRUE)
    {
        PWM_SetDutyCycle(PWM_0, 1);
        // G

        PWM_SetFrequency(392);
        HAL_Delay(300);

        PWM_SetDutyCycle(PWM_0, 0);
        HAL_Delay(50);
        PWM_SetDutyCycle(PWM_0, 1);

        // G
        PWM_SetFrequency(392);
        HAL_Delay(300);
        PWM_SetDutyCycle(PWM_0, 0);
        HAL_Delay(50);
        PWM_SetDutyCycle(PWM_0, 1);

        // A
        PWM_SetFrequency(440);
        HAL_Delay(400);
        PWM_SetDutyCycle(PWM_0, 0);
        HAL_Delay(50);
        PWM_SetDutyCycle(PWM_0, 1);

        // G
        PWM_SetFrequency(392);
        HAL_Delay(400);
        PWM_SetDutyCycle(PWM_0, 0);
        HAL_Delay(50);
        PWM_SetDutyCycle(PWM_0, 1);

        // C
        PWM_SetFrequency(523);
        HAL_Delay(300);
        PWM_SetDutyCycle(PWM_0, 0);
        HAL_Delay(50);
        PWM_SetDutyCycle(PWM_0, 1);

        // B
        PWM_SetFrequency(493);
        HAL_Delay(900);

        PWM_SetDutyCycle(PWM_0, 0);
        HAL_Delay(50);
        PWM_SetDutyCycle(PWM_0, 1);

        // G
        PWM_SetFrequency(392);
        HAL_Delay(300);
        PWM_SetDutyCycle(PWM_0, 0);
        HAL_Delay(50);
        PWM_SetDutyCycle(PWM_0, 1);

        // G
        PWM_SetFrequency(392);
        HAL_Delay(300);
        PWM_SetDutyCycle(PWM_0, 0);
        HAL_Delay(50);
        PWM_SetDutyCycle(PWM_0, 1);

        // A
        PWM_SetFrequency(440);
        HAL_Delay(300);
        PWM_SetDutyCycle(PWM_0, 0);
        HAL_Delay(50);
        PWM_SetDutyCycle(PWM_0, 1);

        // G
        PWM_SetFrequency(392);
        HAL_Delay(300);
        PWM_SetDutyCycle(PWM_0, 0);
        HAL_Delay(50);
        PWM_SetDutyCycle(PWM_0, 1);

        // D
        PWM_SetFrequency(587);
        HAL_Delay(300);
        PWM_SetDutyCycle(PWM_0, 0);
        HAL_Delay(50);
        PWM_SetDutyCycle(PWM_0, 1);

        // C
        PWM_SetFrequency(523);
        HAL_Delay(900);
        PWM_SetDutyCycle(PWM_0, 0);
        HAL_Delay(50);
        PWM_SetDutyCycle(PWM_0, 1);

        // G
        PWM_SetFrequency(392);
        HAL_Delay(300);
        PWM_SetDutyCycle(PWM_0, 0);
        HAL_Delay(50);
        PWM_SetDutyCycle(PWM_0, 1);

        // G
        PWM_SetFrequency(392);
        HAL_Delay(300);
        PWM_SetDutyCycle(PWM_0, 0);
        HAL_Delay(50);
        PWM_SetDutyCycle(PWM_0, 1);

        // E
        PWM_SetFrequency(659);
        HAL_Delay(300);
        PWM_SetDutyCycle(PWM_0, 0);
        HAL_Delay(50);
        PWM_SetDutyCycle(PWM_0, 1);

        // D
        PWM_SetFrequency(587);
        HAL_Delay(300);
        PWM_SetDutyCycle(PWM_0, 0);
        HAL_Delay(50);
        PWM_SetDutyCycle(PWM_0, 1);

        // C
        PWM_SetFrequency(523);
        HAL_Delay(300);
        PWM_SetDutyCycle(PWM_0, 0);
        HAL_Delay(50);
        PWM_SetDutyCycle(PWM_0, 1);

        // B
        PWM_SetFrequency(493);
        HAL_Delay(300);
        PWM_SetDutyCycle(PWM_0, 0);
        HAL_Delay(50);
        PWM_SetDutyCycle(PWM_0, 1);

        // A
        PWM_SetFrequency(440);
        HAL_Delay(900);
        PWM_SetDutyCycle(PWM_0, 0);
        HAL_Delay(50);
        PWM_SetDutyCycle(PWM_0, 1);

        // F
        PWM_SetFrequency(698);
        HAL_Delay(300);
        PWM_SetDutyCycle(PWM_0, 0);
        HAL_Delay(50);
        PWM_SetDutyCycle(PWM_0, 1);

        // F
        PWM_SetFrequency(698);
        HAL_Delay(300);
        PWM_SetDutyCycle(PWM_0, 0);
        HAL_Delay(50);
        PWM_SetDutyCycle(PWM_0, 1);

        // E
        PWM_SetFrequency(659);
        HAL_Delay(300);
        PWM_SetDutyCycle(PWM_0, 0);
        HAL_Delay(50);
        PWM_SetDutyCycle(PWM_0, 1);

        // C
        PWM_SetFrequency(523);
        HAL_Delay(300);
        PWM_SetDutyCycle(PWM_0, 0);
        HAL_Delay(50);
        PWM_SetDutyCycle(PWM_0, 1);

        // D
        PWM_SetFrequency(587);
        HAL_Delay(300);
        PWM_SetDutyCycle(PWM_0, 0);
        HAL_Delay(50);
        PWM_SetDutyCycle(PWM_0, 1);

        // C
        PWM_SetFrequency(523);
        HAL_Delay(300);
        PWM_SetDutyCycle(PWM_0, 0);
        HAL_Delay(50);
        PWM_SetDutyCycle(PWM_0, 1);

        PWM_SetDutyCycle(PWM_0, 0);
        HAL_Delay(900);
    }

#endif

}

/*
{

    PWM_Init();

    if (strcmp(note, 'C'))
    {
        PWM_SetFrequency(261); // lower = deeper
        HAL_Delay(200);
    }
    else if (strcmp(note[0], 'D') == 0)
    {
        printf("note playing: %s\n", note[0]);
        PWM_SetFrequency(293); // lower = deeper
        HAL_Delay(200);
    }
    else if (strcmp(note[0], 'E'))
    {
        prtinf("note playing: %s\n", note[0]);
        PWM_SetFrequency(329); // lower = deeper
        HAL_Delay(200);
    }
    else if (strcmp(note[0], 'F'))
    {
        prtinf("note playing: %s\n", note[0]);
        PWM_SetFrequency(349); // lower = deeper
        HAL_Delay(200);
    }
    else if (strcmp(note[0], 'G'))
    {
        prtinf("note playing: %s\n", note[0]);
        PWM_SetFrequency(392); // lower = deeper
        HAL_Delay(200);
    }
    else if (strcmp(note[0], 'A'))
    {
        prtinf("note playing: %s\n", note[0]);
        PWM_SetFrequency(440); // lower = deeper
        HAL_Delay(200);
    }
    else if (strcmp(note[0], 'B'))
    {
        prtinf("note playing: %s\n", note[0]);
        PWM_SetFrequency(493); // lower = deeper
        HAL_Delay(200);
    }
}
*/