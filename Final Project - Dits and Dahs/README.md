# Final Project: Dits and Dahs - Morse Code Training Game

**ECE167 - Sensing and Sensor Technology**  
**University of California, Santa Cruz**  
** Winter 2024**

## 📌 Overview
This project, **Dits and Dahs**, is an interactive Morse code training game that helps users improve both their writing and listening skills. The game uses a combination of **flex sensors, capacitive touch sensors, and a homemade capacitive touch sensor** to allow players to transmit Morse code. The game features multiple difficulty levels, an OLED display, and a scoring system to track progress.

### 🔹 Objectives:
- Implement **flex sensors** and **capacitive touch sensors** for Morse code input
- Develop a **state machine-based game** with increasing difficulty
- Display Morse code letters and feedback using an **OLED screen**
- Evaluate sensor effectiveness for transmitting Morse code

## ⚙️ Technologies & Components
- **Microcontroller:** STM32 Nucleo-64
- **Sensors:** Flex sensors, Commercial Capacitive Touch Sensor, Homemade Capacitive Touch Sensor
- **Audio:** Speaker with TPA2005D1 Mono Audio Amplifier
- **Software:** C for microcontroller programming, state machine implementation
- **Hardware:** 555 Timer Circuit, Morse Keyer Prototype, OLED Display

## 📂 Project Structure

```plaintext
📦 Dits_and_Dahs_Final_Project
 ┣ 📜 README.md   # Project documentation
 ┣ 📂 src         # Source code for sensor interfacing, game logic, and display
 ┗ 📂 docs        # Reports, test data, videos, and reference materials
```

## 🚀 Project Sections

### 1️⃣ Sensor Implementation
- **Flex Sensor**: Detects finger bending for Morse code input
- **Capacitive Touch Sensors**: Registers touch duration for dits and dahs
- **555 Timer Circuit**: Processes capacitive touch input

### 2️⃣ Game Logic & State Machines
- **Flex Sensor State Machine**: Interprets finger bends as Morse code
- **Capacitive Touch State Machine**: Decodes touch duration into dits and dahs
- **Game State Machine**: Controls game progression, scoring, and difficulty levels

### 3️⃣ Evaluation & Testing
- **Sensor Comparison**: Evaluates effectiveness of different sensors for Morse code input
- **User Testing**: Assesses usability and accuracy of input methods
- **Performance Metrics**: Measures response time and error rates

## 🎯 Key Challenges & Solutions
- **Flex Sensor Sensitivity**: Adjusted thresholds for reliable detection
- **Capacitive Sensor Noise**: Implemented filtering and tuned capacitance values
- **Game Logic Optimization**: Refined state machine transitions for smoother gameplay

## 📸 Media
<img width="400" alt="Dits_and_Dahs_Setup" src="https://github.com/user-attachments/assets/bcdf54e0-e0eb-438a-b7f1-ef27955a242b" />

<img src="https://github.com/user-attachments/assets/76b64b1e-03b9-44b1-95e3-4daf883b34c6" width="400">

## 🛠 Team
- **Aleida Diaz-Roque**
- **Niki Mobtaker**
- **Jacob Gutierrez**

## 🔗 References
- [Project Git Repository](https://github.com/aleidaroque/ECE167)
- [Morse Code Reference](https://morsecode.world/international/morse2.html)
- [Game in action - Flex Sensor](https://youtu.be/gCa86Q9ShJM)
- [Game in action - Capacative Touch Sensor](https://youtu.be/vxfqrSrVxJI)

