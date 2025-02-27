# Project 0: Development Environment Setup & Musical Instrument

**ECE167 - Sensing and Sensor Technology**  
**University of California, Santa Cruz**  

## 📌 Overview
Project 0 introduces students to the **STM32 microcontroller** and the **PlatformIO development environment**. The goal is to set up the software toolchain, write a basic **Hello World** program, and create a simple **digital musical instrument** using an **STM32 Nucleo F411RE** microcontroller, a **potentiometer**, and a **speaker**.

### 🔹 Objectives:
- Set up **PlatformIO** and **VSCode** for STM32 development.
- Write and compile a **Hello World** program.
- Read an **analog signal** from a potentiometer.
- Generate **audio tones** using **PWM**.
- Implement basic **software filtering** to smooth the output sound.

## ⚙️ Technologies & Components
- **Microcontroller:** STM32 Nucleo F411RE
- **Audio Output:** Speaker, Audio amplifier
- **Sensors:** Potentiometer
- **Software:** PlatformIO, STM32Cube framework, VSCode
- **Programming:** C language

## 📂 Project Structure

```plaintext
📦 Lab0_Development_Setup
 ┣ 📜 README.md   # Lab documentation
 ┣ 📂 src         # Source code for Hello World and Musical Instrument
 ┣ 📂 datasheets    # Reference materials
 ┗ 📂 docs        # Reports and project assignment
 
```

## 🚀 Project Sections

### 1️⃣ Development Environment Setup
- Install **VSCode** and **PlatformIO**.
- Clone the ECE167 repository.
- Create and compile a **Hello World** program in C.
- Verify the output using a **serial monitor**.

### 2️⃣ Analog-to-Digital Conversion (ADC)
- Connect a **potentiometer** to the microcontroller.
- Read **analog voltage levels** using the ADC module.
- Print the potentiometer values to the serial console.

### 3️⃣ Generating Audio Tones
- Use **PWM (Pulse Width Modulation)** to generate sound.
- Connect a speaker and an amplifier to the STM32.
- Play a tone with a **variable frequency** based on the potentiometer reading.

### 4️⃣ Building a Digital Musical Instrument
- Map **four different tones** to the **I/O Shield buttons**.
- Adjust the **pitch** using the potentiometer.
- Apply **software filtering** to improve sound quality.

## 🎯 Key Challenges & Solutions
- **Noise in Analog Readings:** Applied **software filtering** to smooth data.
- **Scratchy Audio Output:** Used **low-pass filtering** to refine PWM-generated sound.
- **Non-Linear Potentiometer Response:** Applied **polynomial fitting** for a more natural pitch transition.

## 🛠 Team
- **Aleida Diaz-Roque**

## 🔗 References
- [Project Git Repository](https://github.com/aleidaroque/ECE167)
- [STM32 Nucleo Documentation](https://www.st.com/en/evaluation-tools/nucleo-f411re.html)
