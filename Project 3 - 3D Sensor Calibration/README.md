# Project 3: 3D Sensor Calibration

**ECE167 - Sensing and Sensor Technology**  
**University of California, Santa Cruz**  

## 📌 Overview
This project introduces **9-DOF Inertial Measurement Units (IMUs)** and their calibration. The objective is to learn **sensor communication, error characterization, and calibration techniques** for accelerometers, gyroscopes, and magnetometers using **linear algebra methods**.

### 🔹 Objectives:
- Interface with the **BNO055 IMU** using **I2C communication**.
- Understand **bias drift, scale factor errors, and cross-sensitivity** in sensors.
- Perform **2D and 3D ellipsoid calibration** to correct sensor errors.
- Apply **gyroscope bias correction** and **angle integration**.
- Conduct a **tumble test** to validate sensor calibration.

## ⚙️ Technologies & Components
- **Microcontroller:** STM32 Nucleo-64
- **IMU Sensor:** BNO055 (Accelerometer, Gyroscope, Magnetometer)
- **Software:** I2C interfacing, MATLAB for calibration analysis, Least Squares Regression
- **Hardware:** Breadboard, I2C connections, Foamcore jigs for sensor alignment

## 📂 Project Structure

```plaintext
📦 Lab3_3D_Sensor_Calibration
 ┣ 📜 README.md   # Lab documentation
 ┣ 📂 src         # Source code for IMU interfacing and calibration
 ┣ 📂 hardware    # Circuit schematics and sensor connection diagrams
 ┗ 📂 docs        # Reports, calibration data, and MATLAB scripts
```

## 🚀 Project Sections

### 1️⃣ 2D Ellipsoid Calibration (Simulated Data)
- Simulate an **imperfect 2D sensor** with **bias and scale factor errors**.
- Use **Least Squares Regression** to transform an **elliptical dataset** into a **circle**.
- Validate results using MATLAB.

### 2️⃣ 3D Calibration of Accelerometer & Magnetometer
- Extend **2D calibration methods to 3D**.
- Implement **naïve two-point calibration** for the accelerometer and magnetometer.
- Compute **scale factors and bias corrections** for each axis.

### 3️⃣ Gyroscope Bias and Drift Analysis
- Collect **gyroscope readings over time** to estimate **bias drift**.
- Integrate raw **angular velocity** to compute orientation.
- Analyze **error accumulation** and compare drift rates.

### 4️⃣ Tumble Test for Sensor Fusion
- Perform a **real-world tumble test** to validate calibration.
- Use **iterative least squares** to refine sensor corrections.
- Compare **pre-calibration and post-calibration accuracy**.

## 🎯 Key Challenges & Solutions
- **Sensor Noise:** Applied **data averaging** and filtering techniques.
- **Gyroscope Drift:** Implemented **bias correction** and integration techniques.
- **Ellipsoid Fitting in 3D:** Utilized **iterative least squares** for accurate transformation.

## 🛠 Team
- **Aleida Diaz-Roque**

## 🔗 References
- [Project Git Repository](https://github.com/aleidaroque/ECE167)
- [BNO055 IMU Documentation](https://www.bosch-sensortec.com/products/motion-sensors/absolute-orientation-sensors/bno055/)

