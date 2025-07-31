# ⚙️ ESP32-C3 LED Multitask Blinker with RAII & FreeRTOS

A modern, C++-based project using **RAII (Resource Acquisition Is Initialization)** and **FreeRTOS** on the **ESP32-C3** to blink three LEDs **simultaneously** with different intervals.

- 🔃 3 concurrent blinking tasks using RTOS
- 🧠 GPIO pin management with C++ RAII pattern
- ⚡ True multitasking using `xTaskCreatePinnedToCore`
- 🚀 Super lightweight — no Arduino `loop()` required

> 🔧 Each LED is controlled in its own RTOS task and uses a dedicated RAII object to manage GPIO pin setup and cleanup safely.

---

## 📋 Features

- 🌈 **Three Independent LEDs**:
  - LED1 (GPIO2): blinks every 0.5s
  - LED2 (GPIO3): blinks every 1s
  - LED3 (GPIO4): blinks every 0.2s
- 🧱 **RAII-Based GPIO Control**: Each LED is wrapped in a C++ class that ensures the pin is properly initialized and reset.
- ⚙️ **RTOS Tasks (FreeRTOS)**: Each blinking routine runs in its own FreeRTOS task pinned to core 0.
- 🔄 **No Delay Blocking**: Uses `vTaskDelay()` for true non-blocking multitasking.
- ✅ **Safe Resource Cleanup**: Destructor ensures each pin is turned off when the task ends or system restarts.

---

## 🛠️ Requirements

- A **Seeed Studio ESP32-C3** board or equivalent
- 3 **LEDs** connected to GPIO2, GPIO3, and GPIO4 with appropriate resistors
- Arduino IDE or PlatformIO with ESP32 board support
- Framework: **Arduino**, with access to FreeRTOS (`xTaskCreate`, `vTaskDelay`, etc.)

### Libraries Used:
- `Arduino.h` (Arduino Core for ESP32)
- Built-in **FreeRTOS** (included by default with ESP32 Arduino Core)

---

## 🔩 Pin Configuration

| LED   | GPIO Pin |
|--------|-----------|
| LED1   | GPIO2     |
| LED2   | GPIO3     |
| LED3   | GPIO4     |

> Feel free to change the pin numbers in the `LedRAII` instantiations inside each task.

---

## 🧪 Behavior Overview

- On boot, the ESP32-C3 starts 3 FreeRTOS tasks.
- Each task blinks its LED at a defined interval.
- Tasks use the RAII class `LedRAII` to safely manage GPIOs.
- The `loop()` function is empty — all execution is handled by the RTOS kernel.

---

## 📦 Project Structure

```cpp
class LedRAII {
public:
    LedRAII(gpio_num_t pin);   // Initializes GPIO
    void toggle();             // Toggles LED state
    void on(), off();          // Controls LED
    ~LedRAII();                // Resets GPIO to LOW
};
```

---

## GIF
# ⚙️ ESP32-C3 LED Multitask Blinker with RAII & FreeRTOS

A modern, C++-based project using **RAII (Resource Acquisition Is Initialization)** and **FreeRTOS** on the **ESP32-C3** to blink three LEDs **simultaneously** with different intervals.

- 🔃 3 concurrent blinking tasks using RTOS
- 🧠 GPIO pin management with C++ RAII pattern
- ⚡ True multitasking using `xTaskCreatePinnedToCore`
- 🚀 Super lightweight — no Arduino `loop()` required

> 🔧 Each LED is controlled in its own RTOS task and uses a dedicated RAII object to manage GPIO pin setup and cleanup safely.

---

## 📋 Features

- 🌈 **Three Independent LEDs**:
  - LED1 (GPIO2): blinks every 0.5s
  - LED2 (GPIO3): blinks every 1s
  - LED3 (GPIO4): blinks every 0.2s
- 🧱 **RAII-Based GPIO Control**: Each LED is wrapped in a C++ class that ensures the pin is properly initialized and reset.
- ⚙️ **RTOS Tasks (FreeRTOS)**: Each blinking routine runs in its own FreeRTOS task pinned to core 0.
- 🔄 **No Delay Blocking**: Uses `vTaskDelay()` for true non-blocking multitasking.
- ✅ **Safe Resource Cleanup**: Destructor ensures each pin is turned off when the task ends or system restarts.

---

## 🛠️ Requirements

- A **Seeed Studio ESP32-C3** board or equivalent
- 3 **LEDs** connected to GPIO2, GPIO3, and GPIO4 with appropriate resistors
- Arduino IDE or PlatformIO with ESP32 board support
- Framework: **Arduino**, with access to FreeRTOS (`xTaskCreate`, `vTaskDelay`, etc.)

### Libraries Used:
- `Arduino.h` (Arduino Core for ESP32)
- Built-in **FreeRTOS** (included by default with ESP32 Arduino Core)

---

## 🔩 Pin Configuration

| LED   | GPIO Pin |
|--------|-----------|
| LED1   | GPIO2     |
| LED2   | GPIO3     |
| LED3   | GPIO4     |

> Feel free to change the pin numbers in the `LedRAII` instantiations inside each task.

---

## 🧪 Behavior Overview

- On boot, the ESP32-C3 starts 3 FreeRTOS tasks.
- Each task blinks its LED at a defined interval.
- Tasks use the RAII class `LedRAII` to safely manage GPIOs.
- The `loop()` function is empty — all execution is handled by the RTOS kernel.

---

## 📦 Project Structure

```cpp
class LedRAII {
public:
    LedRAII(gpio_num_t pin);   // Initializes GPIO
    void toggle();             // Toggles LED state
    void on(), off();          // Controls LED
    ~LedRAII();                // Resets GPIO to LOW
};
```

---

## GIF

[!ESP32-C3](https://github.com/user-attachments/assets/7c674d18-982f-4204-a3c0-f8d9c86c339e)

