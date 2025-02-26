#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <timers.h>
#include <stdbool.h>

#include <Board.h>
#include <pwm.h>
#include <ADC.h>

// Different iteration of components
// #define FLEX_ANGLE
// #define PIEZO
// #define FREQ_MAP

#define FLEX_PIEZO_TONE // FINAL VERSION

int frequency = 0;

// TO USE IN DELAY
#define DEL5 5
#define DEL100 100
#define DEL300 300
#define DEL800 800

/////////////////////// CONTROLS //////////////////////////

// PLAY TIME CONTROLS
#define PLAY_TIME 5
#define SENS 50

// MAPPING CONTROLS
// Change values for various ranges
#define MAX_ADC 2700.0
#define MIN_ADC 1000.0
#define ADC_RANGE (MAX_ADC - MIN_ADC)

#define MAX_R 92000.0
#define MIN_R 27000.0
#define R_RANGE (MAX_R - MIN_R)

#define MAX_ANG 150.0
#define MIN_ANG 10.0
#define ANG_RANGE (MAX_ANG - MIN_ANG)

#define MAX_FREQ 600.0
#define MIN_FREQ 100.0
#define FREQ_RANGE (MAX_FREQ - MIN_FREQ)

//_________________________________________________________

//////////// Supporting Functions /////////////////////////

int mapFlexToFreq(int Flex_reading)
{
    // 1. Map the ADC values to the resistance
    // ADC range [1100, 2100], difference: 1000
    // measured resistance range [27k ohms - 92k ohms], difference: 65k ohms
    int R_offset = ((Flex_reading - MIN_ADC) / ADC_RANGE) * R_RANGE;
    int ADC_to_R = MIN_R + R_offset;

    // 2. Map the Resistance values to the corresponding angle
    // measured resistance range [27k ohms - 92k ohms], difference: 65k ohms
    // Angle range [30, 150], difference: 120

    // Polynomial trend line for mapping of resistance to angle
    // y = -2E-08x^2 + 0.0035x - 42.53
    int ANG_offset = ((-2E-08 * pow(ADC_to_R, 2)) + (0.0035 * ADC_to_R) - 42.53);
    int R_to_ANG = ANG_offset - MIN_ANG;

    // 3. Map the angle to the frequency
    // Freq range [150, 500], difference: 350
    // Angle range [30, 150], difference: 120
    int FREQ_offset = (R_to_ANG / ANG_RANGE) * FREQ_RANGE;
    int ANG_to_FREQ = MIN_FREQ + FREQ_offset;

    // RETURN the mapped value
    return (ANG_to_FREQ);
}

bool playTone(int play_time)
{
    int time = 0;

    while (TRUE)
    {
        // continuously take many reading with a delay to counteract bleeding
        int read = ADC_Read(ADC_0);

        for (int i = 0; i < 75; i++)
        {
            read = ADC_Read(ADC_0);
            HAL_Delay(1);
        }

        // Do a check on every 76 read to see if the value changed big
        if (read > 200)
        {
            // exit out of the function becuase NEW tone has to be selected and played
            return TRUE;
        }
        else
        {
            // if the value didnt cange big then play the CURRENT tone
            // add to the time counter
            time++;
        }

        // Check if tone played long enough
        if (time > play_time)
        {
            // Tone finished playing
            return FALSE;
        }
    }
}

//_________________________________________________________

int main(void)
{
    BOARD_Init();
    PWM_Init();
    ADC_Init();

#ifdef FLEX_ANGLE

    // FUNCTION:
    // Reads the ADC output when connected to the Flex sensor
    // attempt at mapping to frequency using ema filter, no scaling

    int currRead = 0;
    int prevRead = 0;

    while (TRUE)
    {
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                int ADCflex = ADC_Read(ADC_0);

                printf(" FLEX data: %d  \n", ADCflex);
                HAL_Delay(300);

                prevRead = ADC_Read(ADC_1);
                currRead = ADC_Read(ADC_1);

                frequency = ((ALPHA)*currRead) + ((1 - ALPHA) * prevRead);

                HAL_Delay(DEL5); // ensures a clearer reading

                PWM_SetDutyCycle(PWM_0, 1);
                PWM_SetFrequency(frequency);
            }
        }
    }
    //

#endif

#ifdef PIEZO

    while (TRUE)
    {

        // start with no sound
        PWM_SetDutyCycle(PWM_0, 0);

        int ADCpiezo = ADC_Read(ADC_0);
        printf("Reading from the piezo:  %d  \n", ADCpiezo);

        // if the reading is above the treshold then activate the tone
        if (ADCpiezo > 100)
        {
            // turn on tone by setting dutty cycle
            PWM_SetDutyCycle(PWM_0, 1);

            // add a delay to the tone
            playTone(PLAY_TIME);

            // turn off the tone
            PWM_SetDutyCycle(PWM_0, 0);
        }
        HAL_Delay(DEL100);
    }

#endif

#ifdef FREQ_MAP
    while (TRUE)
    {

        frequency = mapFlexToFreq(ADC_Read(ADC_1));
        printf("\n\n Frequency: %d\n\n", frequency);
        PWM_SetDutyCycle(PWM_0, 1);
        PWM_SetFrequency(frequency);
        HAL_Delay(100);
    }

#endif

// FINAL VERSION
#ifdef FLEX_PIEZO_TONE

    while (TRUE)
    {

        // Start with no sound
        PWM_SetDutyCycle(PWM_0, 0);

        // Take a sample of Piezo
        int ADCpiezo = ADC_Read(ADC_0);

        // if the reading is above the treshold then play tone
        if (ADCpiezo > 100)
        {
            // PLAYING TONE ------------------------------------------------------
            // obtain the frequency to be played and
            frequency = mapFlexToFreq(ADC_Read(ADC_1));
            PWM_SetFrequency(frequency);

            // turn on the speaker
            PWM_SetDutyCycle(PWM_0, 1);

            // While tone starts playing
            while (TRUE)
            {
                PWM_SetDutyCycle(PWM_0, 1);

                // continiously check to see if the piezo was pressed again while playing
                if (playTone(PLAY_TIME))
                {
                    // return TRUE means that it was pressed while the freq. of the tone was already chosen

                    // must pick the new frequency for the tone
                    frequency = mapFlexToFreq(ADC_Read(ADC_1));
                    PWM_SetFrequency(frequency);

                    // make sure the tone is still on
                    PWM_SetDutyCycle(PWM_0, 1);
                    HAL_Delay(1);
                }
                else
                {
                    // return FALSE means it finished playing a single tone without interruptions
                    // so break from tone playing loop
                    break;
                }
            }

            // once out of tone playing loop, turn off the tone
            PWM_SetDutyCycle(PWM_0, 0);
        }
        else
        {
            // the reading for the piezo was not above the threshold

            // to counteract bleeding as well as sensitivity issues with readings:
            // tke the average of 5 piezo samples and use that value to determine a press
            int avg = 0;
            for (int a = 0; a < 5; a++)
            {
                HAL_Delay(1);
                avg += ADC_Read(ADC_0);
            }
            avg = avg / 5;

            // if the average piezo value is greater than the desired sensitivity value
            if (avg > SENS)
            {
                // Same as PLAYING TONE ----------------------------------------------
                frequency = mapFlexToFreq(ADC_Read(ADC_1));
                PWM_SetFrequency(frequency);
                PWM_SetDutyCycle(PWM_0, 1);
                while (TRUE)
                {
                    PWM_SetDutyCycle(PWM_0, 1);
                    if (playTone(PLAY_TIME))
                    {
                        frequency = mapFlexToFreq(ADC_Read(ADC_1));
                        PWM_SetFrequency(frequency);
                        PWM_SetDutyCycle(PWM_0, 1);
                        HAL_Delay(1);
                    }
                    else
                    {
                        break;
                    }
                }
                PWM_SetDutyCycle(PWM_0, 0);
            }
        }

        // delay the iteration bewteen each run
        HAL_Delay(DEL100);
    }

#endif

    return 0;
}
