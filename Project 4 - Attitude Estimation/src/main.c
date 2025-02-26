#include <stdio.h>
#include <math.h>
#include <string.h>

#include "Board.h"
#include "Attitude_Estimation.h"
#include "BNO055.h"

// #define PART_2
 // #define PART3_1
 #define PART3_2
// #define PART_4
// #define PART_6

int main()
{

    // Initialize the board
    BOARD_Init();
    /*
     * --------------------------------------------------------------
     * PART 2: Convert Direction Cosine Matrix (DCM) to Euler angles
     * --------------------------------------------------------------
     */

#ifdef PART_2
    //  Normal Case ------------------
    float dcm[3][3] = {
        {0.908, 0.418, -0.0396},
        {-0.330, 0.770, 0.547},
        {0.259, -0.483, 0.837}};

    float roll, pitch, yaw;

    // Convert DCM to Euler angles
    dcm_to_euler(dcm, &roll, &pitch, &yaw);

    // Convert radians to degrees for display
    roll *= 180 / M_PI;
    pitch *= 180 / M_PI;
    yaw *= 180 / M_PI;

    // Print the results

    printf("\n\nDCM Matrix: \n");
    printMatrix(dcm);

    printf("\n\n Euler Angles: \n");
    printf("Roll: %.2f degrees\n", roll);
    printf("Pitch: %.2f degrees\n", pitch);
    printf("Yaw: %.2f degrees\n\n", yaw);

    //  GIMBAL LOCK ------------------

    float dcm_gl[3][3] = {

        {0, sqrt(2) / 2, -sqrt(2) / 2},
        {0, sqrt(2) / 2, sqrt(2) / 2},
        {1, 0, 0}};

    float roll_gl, pitch_gl, yaw_gl;

    // Convert DCM to Euler angles
    dcm_to_euler(dcm_gl, &roll_gl, &pitch_gl, &yaw_gl);

    // Convert radians to degrees for display
    roll *= 180 / M_PI;
    pitch *= 180 / M_PI;
    yaw *= 180 / M_PI;

    // Print the results

    printf("\n\nDCM Matrix: \n");
    printMatrix(dcm_gl);

    printf("\n\n Euler Angles: \n");
    printf("Roll: %.2f degrees\n", roll_gl);
    printf("Pitch: %.2f degrees\n", pitch_gl);
    printf("Yaw: %.2f degrees\n\n", yaw_gl);

#endif

    /*
     * --------------------------------------------------------------
     * PART 3.1: Attitude propagation using matrix exponential
     * --------------------------------------------------------------
     */

#ifdef PART3_1
    float deltaTime = 1.0; // Time step in seconds
    float p = deg2rad(10); // 10 degrees per second roll rate
    float q = deg2rad(0);
    float r = deg2rad(0);
    float gyroInput[3] = {p, q, r};

    float R[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}; // Initial rotation matrix

    // Iterate and update the rotation matrix using matrix exponential
    for (int i = 0; i <= 40; i++)
    {
        printf("Step %d:\n", i);

        float R_exp[3][3];
        Rexp(gyroInput, deltaTime, R_exp);

        float R_new[3][3];
        matrix_multiply(R_exp, R, R_new); // R = Rexp * R
        memcpy(R, R_new, sizeof(R_new));

        // Print the updated rotation matrix
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                printf("%.6f ", R[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
#endif
    /////////////////////////////////////////////////////////////////

    /*
     * --------------------------------------------------------------
     * PART 3.2: Attitude propagation using Forward_Integration and attitude_propagation
     * --------------------------------------------------------------
     */

#ifdef PART3_2
    float deltaTime = 1.0; // Time step in seconds
    float p = deg2rad(10); // 10 degrees per second roll rate
    float q = deg2rad(0);
    float r = deg2rad(0);

    float R[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}; // Initial rotation matrix

    // Iterate and update the rotation matrix using Forward_Integration and attitude_propagation
    for (int i = 0; i <= 40; i++)
    {
        printf("Step %d:\n", i);

        float R_new[3][3];
        attitudePropagation(r, p, q, R, deltaTime, R_new); // Update the rotation matrix
        memcpy(R, R_new, sizeof(R_new));

        // Print the updated rotation matrix
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                printf("%.6f ", R[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
#endif

    /////////////////////////////////////////////////////////////////

    /*
     * --------------------------------------------------------------
     * PART 4: Open Loop Gyroscope Integration
     * --------------------------------------------------------------
     */

#ifdef PART_4

    BNO055_Init();
    // from 10 sec data
    float z_bias = 8.216418;
    float y_bias = -24.494403;
    float x_bias = -14.518657;

    float p; // x or phi
    float q; // y or theta
    float r; // z or psi
    float deltaT = 0.1;

    float Rk[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}; // identity matrix
    float Rk1[3][3];
    float pqr[3];
    while (TRUE)
    {
        p = (((BNO055_ReadGyroX() - x_bias) * (M_PI / 180)) / 131);
        q = (((BNO055_ReadGyroY() - y_bias) * (M_PI / 180)) / 131);
        r = (((BNO055_ReadGyroZ() - z_bias) * (M_PI / 180)) / 131);
        pqr[0] = p;
        pqr[1] = q;
        pqr[2] = r;
        Rexp(pqr, deltaT, Rk);
        float roll, pitch, yaw;

        // Convert DCM to Euler angles
        dcm_to_euler(Rk, &roll, &pitch, &yaw);

        memcpy(Rk, Rk1, sizeof(Rk));
        printf("\n\n Euler Angles: \n");
        printf("Roll: %.2f degrees\n", roll);
        printf("Pitch: %.2f degrees\n", pitch);
        printf("Yaw: %.2f degrees\n\n", yaw);
    }

#endif

    /////////////////////////////////////////////////////////////////

    /*
     * --------------------------------------------------------------
     * PART 6: Misalignment of Accelerometer and Magnetometer
     * --------------------------------------------------------------
     */

#ifdef PART_6

    //  Misalignment Matriz]x ------------------

    float mis_mat[3][3] = {{0.0169, -0.0320, -0.0010}, {0.0392, 0.0152, -0.0138}, {-0.0047, 0.0269, -0.0021}};

    float roll, pitch, yaw;

    // Convert DCM to Euler angles
    dcm_to_euler(mis_mat, &roll, &pitch, &yaw);

    // Convert radians to degrees for display
    roll *= 180 / M_PI;
    pitch *= 180 / M_PI;
    yaw *= 180 / M_PI;

    // Print the results

    printf("\n\nDCM Matrix: \n");
    printMatrix(mis_mat);

    printf("\n\n Euler Angles: \n");
    printf("Roll: %.2f degrees\n", roll);
    printf("Pitch: %.2f degrees\n", pitch);
    printf("Yaw: %.2f degrees\n\n", yaw);

#endif

    // End the board session
    BOARD_End();
    return 0;
}