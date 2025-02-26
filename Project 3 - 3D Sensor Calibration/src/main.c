#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <timers.h>

#include <Board.h>
#include <BNO055.h>

// Bias obtained from 10s readings
#define X_BIAS -17.5784
#define Y_BIAS -3.3431
#define Z_BIAS 5.8725

#define SCALE 0.0039

void DelayMicros(uint32_t microsec);

#define calib

#ifdef calib
int main(int argc, char const *argv[])
{
    // Initiallizing libraries
    BOARD_Init();
    BNO055_Init();
    TIMER_Init();

    // Initializing vars
    float calibrated_X = 0.0;
    float angle_X = 0.0;

    float calibrated_Y = 0.0;
    float angle_Y = 0.0;

    float calibrated_Z = 0.0;
    float angle_Z = 0.0;

    while (TRUE)
    {

        

        /// CALIBRATING X axis ///////////////////////////////////////////////////

        // calibrated_X = (0.0039) * (90.0/32.67) * (BNO055_ReadGyroX() - X_BIAS);
        //      (0.0039) * (90.0/32.67) = 0.01074
        // calibrated_X = (0.0111) * (BNO055_ReadGyroX() - X_BIAS);
        calibrated_X = (0.0128) * (BNO055_ReadGyroX() - X_BIAS);
        angle_X += calibrated_X * 0.01;

        // printf("X Angle: %f\n", angle_X);

        /// CALIBRATING Y axis ///////////////////////////////////////////////////

        // calibrated_Y = (0.0039) * (90.0/27.86) * (BNO055_ReadGyroY() - Y_BIAS);
        //       (0.0039) * (90.0/27.86) = 0.01259
        // calibrated_Y = (0.0127) * (BNO055_ReadGyroY() - Y_BIAS);
        calibrated_Y = (0.013) * (BNO055_ReadGyroY() - Y_BIAS);
        angle_Y += calibrated_Y * 0.01;

        // printf("Y Angle: %f\n", angle_Y);

        /// CALIBRATING Z axis ///////////////////////////////////////////////////

        // calibrated_Z = (0.0039)* (BNO055_ReadGyroZ() - Z_BIAS);
        //        (0.0039) * (90.0/27) = 0.00956
        //  calibrated_Z = (0.0125) * (BNO055_ReadGyroZ() - Z_BIAS);
        calibrated_Z = (0.0129) * (BNO055_ReadGyroZ() - Z_BIAS);
        angle_Z += calibrated_Z * 0.01;

        printf("Z Angle: %f\n", angle_Z);

        /// Final Axis Calibration

        // printf("(%.2f   %.2f    %.2f)\n", angle_X, angle_Y, angle_Z);

        DelayMicros(10000);
    }

    return 0;
}

#endif
