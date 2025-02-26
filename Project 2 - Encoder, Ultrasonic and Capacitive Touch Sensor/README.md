# Project 2: Encoder, Ultrasonic, and Capacitive Touch Sensors

**ECE167 - Sensing and Sensor Technology**  
**University of California, Santa Cruz**  

## 📌 Overview
This project introduces three different types of sensors: **quadrature encoders, ultrasonic sensors, and capacitive touch sensors**. The goal is to implement non-blocking modular code to interface with these sensors and use them for real-time measurement and interaction.

### 🔹 Objectives:
- Implement a **Quadrature Encoder Interface (QEI)** to measure rotation.
- Calibrate an **ultrasonic distance sensor** using **Least Squares Regression**.
- Develop a **capacitive touch sensor** and explore different sensing techniques.
- Write **modular, non-blocking code** to integrate each sensor efficiently.

## ⚙️ Technologies & Components
- **Microcontroller:** STM32 Nucleo-64
- **Sensors:** Rotary encoder, Ultrasonic distance sensor, Capacitive touch sensor
- **Software:** Interrupt-driven sensor interfaces, Least Squares calibration, GPIO handling
- **Hardware:** Breadboard circuits, LM555 timer chip, Resistors & capacitors

## 📂 Project Structure

```plaintext
📦 Lab2_Encoder_Ultrasonic_Capacitive
 ┣ 📜 README.md   # Lab documentation
 ┣ 📂 src         # Source code for sensor interfacing and calibration
 ┣ 📂 hardware    # Circuit schematics and component specifications
 ┗ 📂 docs        # Reports, calibration data, and reference materials 
```

## 🚀 Lab Sections

### 1️⃣ Quadrature Encoder Interface (QEI)
- Understand how **quadrature encoders** work.
- Implement **a state machine** to track encoder movement.
- Use **external interrupts** to detect signal changes in real-time.
- Map encoder rotation to **RGB LED color changes**.

### 2️⃣ Ultrasonic Distance Sensor
- Implement **time-of-flight measurement** to determine distance.
- Use **interrupt-driven measurement** to avoid blocking execution.
- Apply **Least Squares Regression** to calibrate the sensor.
- Output accurate **distance readings** via serial communication.

### 3️⃣ Capacitive Touch Sensor
- Implement **RC time constant-based touch detection**.
- Explore **capacitive bridges and differential amplifiers** for noise reduction.
- Build a **555-timer-based relaxation oscillator** for frequency-based touch sensing.
- Develop **software filtering** for improved touch detection.

## 🎯 Key Challenges & Solutions
- **Missed Encoder Steps:** Optimized interrupt handling for real-time tracking.
- **Ultrasonic Sensor Calibration:** Applied **Least Squares Regression** for accurate distance measurement.
- **Capacitive Sensor Noise:** Implemented **differential amplifiers and software filtering**.

## 📸 Media
*(Include images of the encoder, ultrasonic sensor setup, and capacitive touch circuits if available.)*

## 🛠 Team
- **Aleida Diaz-Roque**

## 🔗 References
- [Project Git Repository](https://github.com/aleidaroque/ECE167)
- [Quadrature Encoder Documentation](https://www.sparkfun.com/products/retired/retired_parts/encoder)

