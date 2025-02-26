#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Attitude_Estimation.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif


// Define the tolerance for floating-point comparisons
#define TOLERANCE 1e-6

/** 
 * @brief Check if two floating-point numbers are almost equal.
 * @param a First number.
 * @param b Second number.
 * @return Integer, 1 if almost equal, 0 if not.
 */
int almost_equal(float a, float b) {
    return fabs(a - b) < TOLERANCE;
}


/** 
 * @brief Convert Direction Cosine Matrix (DCM) to Euler angles.
 * @param dcm Direction Cosine Matrix.
 * @param roll Pointer to store roll angle.
 * @param pitch Pointer to store pitch angle.
 * @param yaw Pointer to store yaw angle.
 */
void dcm_to_euler(float dcm[3][3], float *roll, float *pitch, float *yaw) {
    // Check for gimbal lock (pitch angle is ±90 degrees)
    if (almost_equal(dcm[2][0], -1.0)) {
        // Pitch is -90 degrees
        *pitch = -M_PI_2;
        *roll = 0; // Set roll to zero in gimbal lock
        *yaw = atan2(-dcm[0][1], dcm[0][2]);
    } else if (almost_equal(dcm[2][0], 1.0)) {
        // Pitch is 90 degrees
        *pitch = M_PI_2;
        *roll = 0; // Set roll to zero in gimbal lock
        *yaw = atan2(dcm[0][1], -dcm[0][2]);
    } else {
        // Normal case (no gimbal lock)
        *pitch = asin(-dcm[2][0]);
        *roll = atan2(dcm[2][1], dcm[2][2]);
        *yaw = atan2(dcm[1][0], dcm[0][0]);
    }
}
/** 
 * @brief Compute the cross product matrix (skew-symmetric matrix) from a vector.
 * @param w Input vector.
 * @param wx Output skew-symmetric matrix.
 */
void rcross(float w[3], float wx[3][3]) {
    wx[0][0] = 0;
    wx[0][1] = -w[2];
    wx[0][2] = w[1];
    wx[1][0] = w[2];
    wx[1][1] = 0;
    wx[1][2] = -w[0];
    wx[2][0] = -w[1];
    wx[2][1] = w[0];
    wx[2][2] = 0;
}


/** 
 * @brief Compute the exponential Rodrigues parameter form of the rotation matrix.
 * @param w Angular velocity vector.
 * @param deltaT Time step.
 * @param R_exp Output rotation matrix.
 */
void Rexp(float w[3], float deltaT, float R_exp[3][3]) {
    float wnorm = sqrt(w[0] * w[0] + w[1] * w[1] + w[2] * w[2]);
    float wx[3][3];
    rcross(w, wx);

    float sincW, oneMinusCosW;
    if (wnorm < 0.2) {
        sincW = deltaT - (deltaT * deltaT * deltaT * wnorm * wnorm) / 6.0 + (deltaT * deltaT * deltaT * deltaT * deltaT * wnorm * wnorm * wnorm * wnorm) / 120.0;
        oneMinusCosW = (deltaT * deltaT) / 2.0 - (deltaT * deltaT * deltaT * deltaT * wnorm * wnorm) / 24.0 + (deltaT * deltaT * deltaT * deltaT * deltaT * deltaT * wnorm * wnorm * wnorm * wnorm) / 720.0;
    } else {
        sincW = sin(wnorm * deltaT) / wnorm;
        oneMinusCosW = (1.0 - cos(wnorm * deltaT)) / (wnorm * wnorm);
    }

    float wx2[3][3]; // wx * wx
    matrix_multiply(wx, wx, wx2);

    // R_exp = I - sincW * wx + oneMinusCosW * wx * wx;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            R_exp[i][j] = (i == j ? 1.0 : 0.0) - sincW * wx[i][j] + oneMinusCosW * wx2[i][j];
        }
    }
}

/** 
 * @brief Convert degrees to radians.
 * @param degrees Angle in degrees.
 * @return Angle in radians.
 */
float deg2rad(float degrees) {
    return degrees * M_PI / 180.0;
}

/** 
 * @brief Perform matrix multiplication.
 * @param A First input matrix.
 * @param B Second input matrix.
 * @param result Output matrix after multiplication.
 */
void matrix_multiply(float A[3][3], float B[3][3], float result[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result[i][j] = 0;
            for (int k = 0; k < 3; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

/** 
 * @brief Print a 3x3 matrix to the console.
 * @param matrix The matrix to be printed.
 */
void printMatrix(float matrix[3][3]) {
    int i, j;
    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            printf("%.6f ", matrix[i][j]);
        }
        printf("\n"); // Move to the next line after printing each row
    }
}

/** 
 * @brief Create a skew-symmetric matrix from a vector.
 * @param v Input vector.
 * @param result Output skew-symmetric matrix.
 */
void skew_symmetric(float v[3], float result[3][3]) {
    result[0][0] = 0;
    result[0][1] = -v[2];
    result[0][2] = v[1];
    result[1][0] = v[2];
    result[1][1] = 0;
    result[1][2] = -v[0];
    result[2][0] = -v[1];
    result[2][1] = v[0];
    result[2][2] = 0;
}

/** 
 * @brief Integrate the rotation matrix using the open-loop method (Euler integration).
 * @param R Current rotation matrix.
 * @param gyroInput Angular velocity vector.
 * @param deltaTime Time step.
 */
void IntegrateOpenLoop(float R[3][3], float gyroInput[3], float deltaTime) {
    float omega_skew[3][3];
    skew_symmetric(gyroInput, omega_skew);

    // Scale the skew-symmetric matrix by the time step
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            omega_skew[i][j] *= deltaTime;
        }
    }

    // Compute the change in rotation matrix
    float deltaR[3][3];
    matrix_multiply(omega_skew, R, deltaR);

    // Update the original rotation matrix
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            R[i][j] += deltaR[i][j];
        }
    }
}

/** 
 * @brief Perform attitude propagation using forward integration.
 * @param r Roll rate.
 * @param p Pitch rate.
 * @param q Yaw rate.
 * @param Rk Current rotation matrix.
 * @param dT Time step.
 * @param Rk1 Output rotation matrix after propagation.
 */
void attitudePropagation(float r, float p, float q, float Rk[3][3], float dT, float Rk1[3][3]) {
    // Create the skew-symmetric matrix from the angular rates (r, p, q)
    float omegaCross[3][3] = {
        {0, -r, q},
        {r, 0, -p},
        {-q, p, 0}
    };

    // Initialize the matrix to store the derivative of the rotation matrix
    float Rdot[3][3];

    // Multiply the skew-symmetric matrix by the current rotation matrix to get the derivative
    matrix_multiply(omegaCross, Rk, Rdot);

    // Perform forward integration to get the rotation matrix at the next time step
    forwardIntegration(Rk, Rdot, dT, Rk1);
}

/** 
 * @brief Perform forward integration to update the rotation matrix.
 * @param Rk Current rotation matrix.
 * @param Rdot Derivative of the rotation matrix.
 * @param dT Time step.
 * @param Rk1 Output rotation matrix after integration.
 */
void forwardIntegration(float Rk[3][3], float Rdot[3][3], int dT, float Rk1[3][3]) {
    // Loop through each element of the matrix
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            // Update the rotation matrix using Euler's method for integration
            // Rk1 = Rk + Rdot * dT
            Rk1[i][j] = Rk[i][j] - Rdot[i][j] * dT; // minus because omegaX is negative
        }
    }
}

/**
 * @brief Implements the full complementary filter for attitude estimation using gyroscope, accelerometer, and magnetometer data.
 * @param Rminus The previous attitude direction cosine matrix (DCM).
 * @param Bminus The previous gyroscope bias estimate.
 * @param gyros The body-fixed rotation rates in rad/s.
 * @param mags The magnetometer readings in body coordinates.
 * @param accels The accelerometer readings in body coordinates.
 * @param magInertial The known inertial reference magnetic field vector.
 * @param accelInertial The known inertial reference gravity field vector.
 * @param deltaT The time step in seconds between the current and previous estimates.
 * @param Rplus The updated attitude DCM after processing the current sensor data.
 * @param Bplus The updated gyroscope bias estimate.
 * 
 * This function integrates the gyroscope readings, applying feedback corrections based on the accelerometer and magnetometer readings.
 * The corrections are scaled by proportional (Kp) and integral (Ki) gains for both the accelerometer and magnetometer.
 * The function updates both the attitude estimate (Rplus) and the gyroscope bias estimate (Bplus).
 */
void integrateClosedLoop(float Rminus[3][3], float Bminus[3], float gyros[3], 
                          float mags[3], float accels[3], float magInertial[3], 
                          float accelInertial[3], float deltaT, float Rplus[3][3], 
                          float Bplus[3]) {
                            
    float Kp_a = 10.0f; // Proportional gain for accelerometer
    float Ki_a = Kp_a / 10.0f; // Integral gain for accelerometer
    float Kp_m = 10.0f; // Proportional gain for magnetometer
    float Ki_m = Kp_m / 10.0f; // Integral gain for magnetometer

    // Normalize accelerometer and magnetometer readings
    normalizeVector(accels);
    normalizeVector(mags);

    // Normalize inertial reference vectors
    normalizeVector(magInertial);
    normalizeVector(accelInertial);

    // Calculate gyro input with bias
    float gyroInputWithBias[3];
    for (int i = 0; i < 3; i++) {
        gyroInputWithBias[i] = gyros[i] - Bminus[i];
    }

    // Calculate accelerometer correction in body frame
    float wmeas_a[3];
    float accel_corr[3][3];
    matrix_multiply(Rminus, accelInertial, accel_corr);
    crossProduct(accels, accel_corr, wmeas_a);

    // Calculate magnetometer correction in body frame
    float wmeas_m[3];
    float mag_corr[3][3];
    matrix_multiply(Rminus, magInertial, mag_corr);
    crossProduct(mags, mag_corr, wmeas_m);

   // Calculate gyro input with feedback
    float gyroInputWithFeedback[3];
    for (int i = 0; i < 3; i++) {
        gyroInputWithFeedback[i] = gyroInputWithBias[i] + Kp_a * wmeas_a[i] + Kp_m * wmeas_m[i];
    }

    // Calculate bias derivative
    float bdot[3];
    for (int i = 0; i < 3; i++) {
        bdot[i] = -Ki_a * wmeas_a[i] - Ki_m * wmeas_m[i];
    }

    // Integrate to get new rotation matrix using Rexp
    float R_exp[3][3];
    Rexp(gyroInputWithFeedback, deltaT, R_exp);
    matrix_multiply(R_exp, Rminus, Rplus); // Rplus = R_exp * Rminus

    // Update bias
    for (int i = 0; i < 3; i++) {
        Bplus[i] = Bminus[i] + bdot[i] * deltaT;
    }
}

/**
 * @brief Normalizes a 3-element vector.
 * @param vector The 3-element vector to be normalized.
 * 
 * This function calculates the Euclidean norm (magnitude) of the input vector and then divides each element
 * of the vector by this norm to normalize it. The normalization is done in-place, modifying the original vector.
 * If the norm is zero (which means the vector is all zeros), the function exits without performing normalization.
 */
void normalizeVector(float vector[3]) {
    float norm = sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
    
    // Check for zero norm
    if (norm <= 1e-6) { // Use a small threshold to account for floating point precision
        return; // Do not perform normalization if the vector is zero or close to zero
    }
    
    // Normalize the vector in-place
    vector[0] /= norm;
    vector[1] /= norm;
    vector[2] /= norm;
}



/**
 * @brief Computes the cross product of two 3D vectors.
 * @param a Pointer to the first three-element vector.
 * @param b Pointer to the second three-element vector.
 * @param c Pointer to a three-element vector where the result is stored.
 */
void crossProduct(float *a, float *b, float *c) {
    c[0] = a[1] * b[2] - a[2] * b[1];
    c[1] = a[2] * b[0] - a[0] * b[2];
    c[2] = a[0] * b[1] - a[1] * b[0];
}
