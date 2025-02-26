#include <Board.h>
#include <timers.h>
#include <stdint.h>
#include <stdio.h>

#include <pwm.h>
#include <ADC.h>
#include "CAPTOUCH.h"

char status;

int main(void)
{

    BOARD_Init();
    CAPTOUCH_Init();
    TIMER_Init();

    while (TRUE)
    {
        status = CAPTOUCH_IsTouched();
        if (status == TRUE)
        {
            printf("\n TOUCHED \n");
        }
        else if (status == FALSE)
        {
            printf("\n NOT TOUCHED \n");
        }
    }

    return 0;
}
