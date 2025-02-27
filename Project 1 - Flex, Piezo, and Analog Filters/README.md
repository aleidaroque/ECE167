# Project 1: Introduction to Resistive Sensors and Analog Filtering

**ECE167 - Sensing and Sensor Technology**  
**University of California, Santa Cruz**  

## 📌 Overview
This project introduces **resistive and piezoelectric sensors** and their integration into digital systems. The goal is to build a **musical instrument** where the flex sensor determines pitch, and the piezo sensor triggers notes. Additionally, we will design, model, and validate **analog filters**.

### 🔹 Objectives:
- Learn how **flex sensors** work and create a regression model.
- Integrate a **piezoelectric sensor** to detect pressure and vibrations.
- Use **analog filters** to condition sensor signals.
- Develop a **musical instrument** using the STM32 microcontroller.

## ⚙️ Technologies & Components
- **Microcontroller:** STM32 Nucleo-64
- **Sensors:** Flex sensor, Piezo sensor
- **Audio:** Speaker, Audio amplifier, Potentiometer
- **Signal Processing:** Low-pass, High-pass, and Band-pass filters
- **Software:** ADC interfacing, filtering, and signal processing in C

## 📂 Project Structure

```plaintext
📦 Lab1_Flex_Piezo_Filters
 ┣ 📜 README.md   # Lab documentation
 ┣ 📂 src         # Source code for sensor interfacing and filtering
 ┗ 📂 docs        # Reports, and reference materials
```

## 🚀 Lab Sections

### 1️⃣ Flex Sensor Implementation
- Assemble and test the **flex sensor**.
- Develop a **regression model** to map sensor bend to resistance.
- Use the STM32 ADC to read the sensor and modulate a speaker tone.

### 2️⃣ Piezoelectric Sensor Integration
- Understand **piezoelectric effect** and voltage spike concerns.
- Implement **voltage snubbing** to protect STM32 input pins.
- Use the piezo sensor as an event trigger for the musical instrument.

### 3️⃣ Musical Instrument Construction
- Combine **flex and piezo sensors** to create a playable instrument.
- Implement **tone generation and duration control**.
- Analyze sensor **noise and response smoothing**.

### 4️⃣ Analog Filtering Analysis
- Design and simulate **low-pass, high-pass, and band-pass filters**.
- Experimentally validate filter performance using real sensor data.
- Compare theoretical and actual filter responses.

## 🎯 Key Challenges & Solutions
- **Flex Sensor Noise:** Applied smoothing techniques for stable readings.
- **Piezo Voltage Spikes:** Implemented protective circuits to prevent damage.
- **Filter Accuracy:** Tuned component values for optimal signal processing.

## 📸 Media
<img src="https://github.com/user-attachments/assets/a401de32-8f6f-472f-8604-b7407373a82d" width="400">




## 🛠 Team
- **Aleida Diaz-Roque**

## 🔗 References
- [Class Project Git Repository](https://github.com/aleidaroque/ECE167)
- [Flex Sensor Documentation](https://learn.sparkfun.com/tutorials/flex-sensor-hookup-guide)

