# Project 4: Attitude Estimation

**ECE167 - Sensing and Sensor Technology**  
**University of California, Santa Cruz**  

## 📌 Overview
This project focuses on **attitude estimation**, a critical component in **autonomous navigation systems**. The goal is to solve the **nonlinear differential equations** governing attitude using the **Direction Cosine Matrix (DCM)** and implement **sensor fusion** techniques for improved accuracy.

### 🔹 Objectives:
- Understand **coordinate frames** (Body vs. Inertial)
- Implement **Direction Cosine Matrix (DCM)** transformations
- Convert **DCM to Euler angles** (Yaw, Pitch, Roll)
- Perform **open-loop gyroscope integration**
- Implement **sensor fusion** using **complementary filtering**

## ⚙️ Technologies & Components
- **Microcontroller:** STM32 Nucleo-64
- **IMU Sensor:** BNO055 (Accelerometer, Gyroscope, Magnetometer)
- **Software:** MATLAB for visualization, C for microcontroller programming
- **Algorithms:** DCM integration, Euler angle extraction, Sensor Fusion via Complementary Filtering

## 📂 Project Structure

```plaintext
📦 Lab4_Attitude_Estimation
 ┣ 📜 README.md   # Lab documentation
 ┣ 📂 src         # Source code for DCM, filtering, and integration
 ┣ 📂 matlab   # MATLAB scripts
 ┗ 📂 docs        # Reports, and reference materials
```

## 🚀 Project Sections

### 1️⃣ Coordinate Frames & DCM Transformation
- Understand **body vs. inertial coordinate frames**
- Implement the **DCM transformation matrix**
- Learn how **DCM maps inertial vectors into body coordinates**

### 2️⃣ DCM to Euler Angle Conversion
- Extract **Yaw, Pitch, and Roll** from the DCM
- Implement an **inverse trigonometry function** for Euler angle extraction
- Display **Euler angles on an OLED screen**

### 3️⃣ Open Loop Gyroscope Integration
- Integrate **body-fixed rotation rates** over time to update attitude
- Implement **matrix exponential form** for numerical stability
- Compare **Forward Integration vs. Matrix Exponential methods**

### 4️⃣ Closed Loop Sensor Fusion
- Use **accelerometer & magnetometer** data for attitude correction
- Implement **Proportional-Integral (PI) correction** for drift compensation
- Tune **feedback gains** for optimal attitude estimation

## 🎯 Key Challenges & Solutions
- **DCM Drift:** Used **matrix exponential integration** to maintain orthonormality.
- **Sensor Noise:** Applied **complementary filtering** to smooth sensor readings.
- **Yaw Estimation Issues:** Incorporated **magnetometer corrections** to improve yaw accuracy.

## 🛠 Team
- **Aleida Diaz-Roque**

## 🔗 References
- [Project Git Repository](https://github.com/aleidaroque/ECE167)
- [DCM Attitude Estimation Guide](https://www.vectornav.com/resources/inertial-navigation-primer/dcm)

