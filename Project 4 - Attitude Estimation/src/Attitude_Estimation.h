/**
 * 2024 Winter ECE 167 Lab 4
 * Attitude_Estimation.h 
 */

#ifndef ATTITUDE_ESTIMATION_H
#define ATTITUDE_ESTIMATION_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

#define NUM_STEPS 40

// Define the tolerance for floating-point comparisons
#define TOLERANCE 1e-6

/** 
 * @brief Check if two floating-point numbers are almost equal.
 * @param a First number.
 * @param b Second number.
 * @return Integer, 1 if almost equal, 0 if not.
 */
int almost_equal(float a, float b);

/** 
 * @brief Convert Direction Cosine Matrix (DCM) to Euler angles.
 * @param dcm Direction Cosine Matrix.
 * @param roll Pointer to store roll angle.
 * @param pitch Pointer to store pitch angle.
 * @param yaw Pointer to store yaw angle.
 */
void dcm_to_euler(float dcm[3][3], float *roll, float *pitch, float *yaw);

/** 
 * @brief Compute the cross product matrix (skew-symmetric matrix) from a vector.
 * @param w Input vector.
 * @param wx Output skew-symmetric matrix.
 */
void rcross(float w[3], float wx[3][3]);

/** 
 * @brief Perform matrix multiplication.
 * @param A First input matrix.
 * @param B Second input matrix.
 * @param result Output matrix after multiplication.
 */
void matrix_multiply(float A[3][3], float B[3][3], float result[3][3]);

/** 
 * @brief Compute the exponential Rodrigues parameter form of the rotation matrix.
 * @param w Angular velocity vector.
 * @param deltaT Time step.
 * @param R_exp Output rotation matrix.
 */
void Rexp(float w[3], float deltaT, float R_exp[3][3]);

/** 
 * @brief Convert degrees to radians.
 * @param degrees Angle in degrees.
 * @return Angle in radians.
 */
float deg2rad(float degrees);

/** 
 * @brief Print a 3x3 matrix to the console.
 * @param matrix The matrix to be printed.
 */
void printMatrix(float matrix[3][3]);

/** 
 * @brief Create a skew-symmetric matrix from a vector.
 * @param v Input vector.
 * @param result Output skew-symmetric matrix.
 */
void skew_symmetric(float v[3], float result[3][3]);

/** 
 * @brief Integrate the rotation matrix using the open-loop method (Euler integration).
 * @param R Current rotation matrix.
 * @param gyroInput Angular velocity vector.
 * @param deltaTime Time step.
 */
void IntegrateOpenLoop(float R[3][3], float gyroInput[3], float deltaTime);

/** 
 * @brief Perform attitude propagation using forward integration.
 * @param r Roll rate.
 * @param p Pitch rate.
 * @param q Yaw rate.
 * @param Rk Current rotation matrix.
 * @param dT Time step.
 * @param Rk1 Output rotation matrix after propagation.
 */
void attitudePropagation(float r, float p, float q, float Rk[3][3], float dT, float Rk1[3][3]);

/** 
 * @brief Perform forward integration to update the rotation matrix.
 * @param Rk Current rotation matrix.
 * @param Rdot Derivative of the rotation matrix.
 * @param dT Time step.
 * @param Rk1 Output rotation matrix after integration.
 */
void forwardIntegration(float Rk[3][3], float Rdot[3][3], int dT, float Rk1[3][3]);

void integrateClosedLoop(float Rminus[3][3], float Bminus[3], float gyros[3], 
                          float mags[3], float accels[3], float magInertial[3], 
                          float accelInertial[3], float deltaT, float Rplus[3][3], 
                          float Bplus[3]);

/**
 * @brief Normalizes a 3-element vector.
 * @param vector The 3-element vector to be normalized.
 * 
 * This function calculates the Euclidean norm (magnitude) of the input vector and then divides each element
 * of the vector by this norm to normalize it. The normalization is done in-place, modifying the original vector.
 * If the norm is zero (which means the vector is all zeros), the function exits without performing normalization.
 */
void normalizeVector(float vector[3]);

/**
 * @brief Computes the cross product of two 3D vectors.
 * @param a Pointer to the first three-element vector.
 * @param b Pointer to the second three-element vector.
 * @param c Pointer to a three-element vector where the result is stored.
 */
void crossProduct(float *a, float *b, float *c);

#endif // ATTITUDE_ESTIMATION_H
