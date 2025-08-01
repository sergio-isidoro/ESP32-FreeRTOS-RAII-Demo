# ⚙️ ESP32-C3 LED Multitask Blinker with RAII & FreeRTOS

A modern, C++-based project using **RAII (Resource Acquisition Is Initialization)** and **FreeRTOS** on the **ESP32-C3** to blink three LEDs **simultaneously** with different intervals — and a button that **pauses and resumes** the blinking of one LED via a mutex.

- 🔃 3 concurrent blinking tasks using RTOS
- 🧠 GPIO pin management with C++ RAII pattern
- ⚡ True multitasking using `xTaskCreatePinnedToCore`
- 🚀 Super lightweight — no Arduino `loop()` required
- 🔘 Button controls LED3 blinking by locking/unlocking a mutex

> 🔧 Each LED is controlled in its own RTOS task and uses a dedicated RAII object to manage GPIO pin setup and cleanup safely. The button task monitors a GPIO input and controls a mutex that gates LED3's blinking.

---

## 📋 Features

- 🌈 **Three Independent LEDs**:
  - LED1 (GPIO2): blinks every 0.5s
  - LED2 (GPIO3): blinks every 1s
  - LED3 (GPIO4): blinks every 0.2s — can be paused/resumed with a button
- 🔘 **Button (GPIO5)**: when pressed, stops LED3 blinking by locking a mutex; releasing the button resumes blinking.
- 🧱 **RAII-Based GPIO Control**: Each LED and the button are wrapped in C++ classes ensuring proper initialization and cleanup.
- ⚙️ **RTOS Tasks (FreeRTOS)**: Each blinking routine and the button monitoring run in their own FreeRTOS tasks pinned to core 0.
- 🔄 **No Delay Blocking**: Uses `vTaskDelay()` for non-blocking multitasking.
- ✅ **Safe Resource Cleanup**: Destructors ensure pins are reset when objects go out of scope.

---

## 🛠️ Requirements

- A **Seeed Studio ESP32-C3** board or equivalent
- 3 **LEDs** connected to GPIO2, GPIO3, and GPIO4 with appropriate resistors
- 1 **button** connected to GPIO5 with pull-up resistor (or use internal pull-up)
- Arduino IDE or PlatformIO with ESP32 board support
- Framework: **Arduino**, with FreeRTOS (`xTaskCreate`, `vTaskDelay`, etc.)

### Libraries Used:
- `Arduino.h` (Arduino Core for ESP32)
- Built-in **FreeRTOS** (included by default with ESP32 Arduino Core)

---

## 🔩 Pin Configuration

| Device | GPIO Pin |
|--------|----------|
| LED1   | GPIO2    |
| LED2   | GPIO3    |
| LED3   | GPIO4    |
| Button | GPIO5    |

> Pins can be changed by editing the `LedRAII` and `ButtonRAII` class instantiations in the tasks.

---

## 🧪 Behavior Overview

- On boot, the ESP32-C3 starts 4 FreeRTOS tasks:
  - 3 tasks blink LEDs at their own intervals.
  - 1 task monitors the button state and locks/unlocks a mutex to pause/resume LED3 blinking.
- The `loop()` function is empty — all execution is handled by the RTOS kernel.
- LED3 stops blinking immediately when the button is pressed, and resumes when released.

---

## 📦 Project Structure

```cpp
class LedRAII {
public:
    LedRAII(gpio_num_t pin);   // Initializes GPIO pin as output
    void toggle();             // Toggles LED state
    ~LedRAII();                // Turns LED off on destruction
};

class ButtonRAII {
public:
    ButtonRAII(gpio_num_t pin);  // Initializes GPIO pin as input with pull-up
    bool isPressed() const;      // Reads button state (active low)
};
```

---

## Video

https://github.com/user-attachments/assets/024e8173-2d12-4639-b704-28607c7e88dc
