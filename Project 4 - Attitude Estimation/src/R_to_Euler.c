/*
 * File:   R_to_Euler.c
 * Author: Aleida Diaz-Roque
 *
 * Created on Feb 27, 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "BOARD.h"

#define MATRIX_SIZE 3
#define M_PI 3.14

// #define R_ZYX
#define R_XYZ

/**
 * function takes in a rotation matrix and outputs euler angless
 */
void R_to_E(float rMatrix[3][3], float result[3]);

void R_to_E(float rMatrix[3][3], float result[3])
{
// R = R_z * R_y * R_x
#ifdef R_ZYX
    float R31 = rMatrix[2][0];
    float R32 = rMatrix[2][1];
    float R33 = rMatrix[2][2];
    // Check if R31 is approximately 1 or -1
    if (fabsf(R31 - 1.0f) < 1e-6 || fabsf(R31 + 1.0f) < 1e-6)
    {
        result[0] = -asinf(R31);
        // result[1] = (fabsf(R31 - 1.0f) < 1e-6) ? atan2f(rMatrix[0][1], rMatrix[0][2]) : -atan2f(-rMatrix[0][1], -rMatrix[0][2]);
        if (fabsf(R31 - 1.0f) < 1e-6)
        {
            result[1] = atan2f(rMatrix[0][1], rMatrix[0][2]);
        }
        else
        {
            result[1] = -atan2f(-rMatrix[0][1], -rMatrix[0][2]);
        }
        result[2] = atan2f(R32 * cosf(result[0]), R33 * cosf(result[0]));
    }
    else
    {
        result[1] = 0.0f; // Set phi to any value, it will be overwritten later
        if (fabsf(R31 + 1.0f) < 1e-6)
        {
            result[0] = M_PI / 2.0f;
            result[2] = result[1] + atan2f(rMatrix[1][0], rMatrix[0][0]);
        }
        else
        {
            result[0] = -M_PI / 2.0f;
            result[2] = -(result[1]) + atan2f(-rMatrix[1][0], -rMatrix[0][0]);
        }
    }

#endif

// R = R_x * R_y * R_z
#ifdef R_XYZ

    float R13 = rMatrix[0][2];
    float R23 = rMatrix[1][2];
    float R33 = rMatrix[2][2];
    float R12 = rMatrix[0][1];
    float R11 = rMatrix[0][0];


    float pitch = -asinf(R13) * 180.0 / M_PI;        // pitch in degrees
    float roll = atan2f(R23, R33) * 180.0 / M_PI;   // yaw in degrees
    float yaw = atan2f(R12, R11) * 180.0 / M_PI;    // roll in degrees

    // insert the angles into the matrix
    result[0] = yaw;
    result[1] = pitch;
    result[2] = roll;

#endif
}

int main()
{
    BOARD_Init();

    float euler_angles[3]; // array holding Euler angles

    // theta: 0 | phi: pi/2 | psi: pi
    float r_matrix[3][3] = {
        {-1.0, 0.0, 0.0},
        {0.0, 0.0, 1.0},
        {0.0, 1.0, 0.0}};

    // print the matrix
    printf("Rotation matrix: \n");
    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            printf("%0.1f ", r_matrix[i][j]);
        }
        printf("\n");
    }

    // test for R_xyz
    R_to_E(r_matrix, euler_angles);

    printf("Angles: %0.1f %0.1f %0.1f\n", euler_angles[0], euler_angles[1], euler_angles[2]);

    BOARD_End();
    return (EXIT_SUCCESS);
}
