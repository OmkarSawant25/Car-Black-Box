# 🚗 Car Black Box System (Embedded C Project)

## 📌 Overview

The **Car Black Box System** is an embedded system designed to log critical vehicle data such as speed, time, and events.
It works similar to an aircraft black box, helping in **event tracking and accident analysis**.

This project is implemented using **Embedded C** and integrates multiple peripherals for real-time data logging and display.

---

## ⚙️ Features

* 📊 Real-time data logging (speed, events, timestamps)
* ⏱️ RTC-based time tracking using DS1307
* 💾 EEPROM storage for persistent data
* 🔁 Circular buffer for efficient memory usage
* 🖥️ LCD display for live monitoring
* 🔑 Matrix keypad for user interaction
* 📡 UART communication support

---

## 🧠 Key Concepts Used

* Embedded C programming
* Peripheral interfacing (I2C, UART, ADC)
* Real-time systems
* Memory management (EEPROM)
* Circular buffer implementation

---

## 🛠️ Hardware Components

* Microcontroller (PIC18F4580)
* RTC Module (DS1307)
* EEPROM (Internal)
* 16x2 LCD Display
* Matrix Keypad
* ADC (for speed input simulation)
* UART Interface

---

## 📂 Project Structure

```
.
├── main.c
├── adc.c / adc.h
├── uart.c / uart.h
├── i2c.c / i2c.h
├── ds1307.c / ds1307.h
├── internal_eeprom.c / internal_eeprom.h
├── clcd.c / clcd.h
├── matrix_keypad.c / matrix_keypad.h
├── black_box.c / black_box.h
├── Makefile
```

---

## 🚀 How It Works

1. System initializes all peripherals (ADC, I2C, UART, LCD)
2. RTC provides real-time timestamp
3. Sensor inputs (speed/events) are captured
4. Data is stored in EEPROM using a circular buffer
5. User can view logs via keypad navigation on LCD

---

## ⚡ Challenges & Learnings

* Efficient EEPROM memory management using circular buffer
* Synchronizing multiple peripherals in real-time
* Handling I2C communication with RTC module
* Debugging embedded systems without OS support

---

## 📈 Future Improvements

* CAN protocol integration for automotive communication
* SD card logging for large data storage
* IoT integration for remote monitoring
* Crash detection using sensors

---

## 👨‍💻 Author

**Omkar Ashok Sawant**
Embedded Systems Engineer

---

## ⭐ Conclusion

This project demonstrates strong fundamentals in **embedded systems design, real-time data logging, and peripheral interfacing**, making it highly relevant for automotive and IoT applications.

---
