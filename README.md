# 🚗 Car Black Box System | Embedded Systems Project

## 📌 Project Summary

Designed and developed a **Car Black Box System** using **Embedded C** on the **PIC18F4580 microcontroller** to log real-time vehicle data such as speed, events, and timestamps for accident analysis and diagnostics.
The system mimics an aircraft black box by enabling **event tracking, data logging, and post-event analysis**.

---

## 🎯 Key Highlights

* Implemented **real-time data logging system** using RTC and EEPROM
* Designed **circular buffer** to store only latest logs in limited memory
* Interfaced multiple peripherals ensuring synchronized operation
* Achieved reliable **non-volatile data storage** using internal EEPROM
* Developed **menu-driven interface** using LCD and keypad

---

## 🛠️ Technical Skills Demonstrated

* **Languages:** Embedded C
* **Microcontroller:** PIC18F4580
* **Protocols:** I2C, UART
* **Peripherals:** ADC, RTC (DS1307), EEPROM, LCD, Matrix Keypad
* **Concepts:** Real-Time Systems, Memory Management, Circular Buffer

---

## ⚙️ System Architecture

* ADC captures simulated speed input
* RTC provides timestamp via I2C communication
* EEPROM stores event logs using circular buffer logic
* LCD displays real-time data and logs
* Keypad enables user navigation
* UART used for debugging and communication

---

## 🚀 How It Works

1. System initializes all peripherals (ADC, I2C, UART, LCD)
2. RTC provides real-time timestamp
3. Sensor inputs (speed/events) are captured
4. Data is stored in EEPROM using circular buffer
5. User can view logs via keypad navigation on LCD

---

## 📊 Impact / Outcome

* Optimized memory usage by storing only recent logs
* Ensured accurate and reliable event recording
* Built a scalable system adaptable for automotive applications

---

## ⚡ Challenges & Solutions

* **Challenge:** Limited EEPROM memory
  → **Solution:** Implemented circular buffer for efficient storage

* **Challenge:** Synchronization of multiple peripherals
  → **Solution:** Designed modular drivers for each interface

* **Challenge:** Debugging without OS
  → **Solution:** Used UART-based debugging techniques

---

## 📂 Project Structure

```
main.c
adc.c / adc.h
uart.c / uart.h
i2c.c / i2c.h
ds1307.c / ds1307.h
internal_eeprom.c / internal_eeprom.h
clcd.c / clcd.h
matrix_keypad.c / matrix_keypad.h
black_box.c / black_box.h
Makefile
```

---

## 🚀 Future Scope

* Integration with CAN protocol for automotive communication
* IoT-based remote monitoring using ESP32
* SD card support for large-scale data logging
* Crash detection using sensors

---

## 👨‍💻 Author

**Omkar Ashok Sawant**  
Embedded Systems Engineer

---

## ⭐ Conclusion

This project demonstrates strong fundamentals in **embedded systems design, real-time data logging, and peripheral interfacing**, making it highly relevant for automotive and IoT applications.
