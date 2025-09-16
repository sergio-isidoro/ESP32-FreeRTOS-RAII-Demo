# ESP32-C3 LED Multitask Blinker with RAII & FreeRTOS (C++14)

This is a modern C++14-based project for the ESP32-C3 that uses RAII (Resource Acquisition Is Initialization) and FreeRTOS to blink three LEDs simultaneously, each at a different interval. A physical button allows you to pause and resume the blinking of one of the LEDs using a mutex.

- 🔃 3 concurrent tasks with FreeRTOS
- 🧠 GPIO management via RAII classes in C++
- ⚡ True multitasking with `xTaskCreatePinnedToCore`
- 🚀 No use of the Arduino `loop()` function
- 🔘 Physical button controls LED3's mutex (pause/resume)

> 🧰 Each LED is controlled by a separate task and encapsulated in a RAII class that ensures safe setting and releasing of the GPIO pins. The button is monitored by a dedicated task that controls a mutex to manage LED3's behavior.

---

## 📋 Features

- 🌈 **Three independent LEDs**:
- **LED1 (GPIO2)**: blinks every 0.5 seconds
- **LED2 (GPIO3)**: blinks every 1 second
- **LED3 (GPIO4)**: blinks every 0.2 seconds — controlled by a button
- 🔘 **Button (GPIO5)**:
- Pressed → locks mutex → LED3 stops blinking
- Released → releases mutex → LED3 resumes blinking
- 🧱 **RAII for GPIOs**:
- `LedRAII` and `ButtonRAII` encapsulate automatic pin configuration and clearing
- ⚙️ **Multitasking with FreeRTOS**:
- Each LED and button has its own task (`xTaskCreatePinnedToCore`)
- ⏱️ **No crashes with `delay()`**:
- Uses `vTaskDelay()` for cooperative multitasking
- ✅ **Automatic cleanup**:
- LEDs are automatically turned off in the destructor (`~LedRAII`)

---

## 🛠️ Requirements

- **ESP32-C3** board (e.g., Seeed Studio ESP32-C3)
- 3 **LEDs** + limiting resistors (220Ω–330Ω)
- 1 **button** with pull-up resistor (or use `INPUT_PULLUP`)
- Arduino or PlatformIO IDE with ESP32 support
- Framework: **Arduino** with **FreeRTOS** support

### 📚 Libraries Used

- [`Arduino.h`](https://www.arduino.cc/reference/en/) (base core)
- **FreeRTOS** (included by default in the ESP32 Arduino core)

---

## 🧭 Pin Map

| Device | GPIO |
|-------------|------|
| LED1 | 2 |
| LED2 | 3 |
| LED3 | 4 |
| Button | 5 |

> To change the pins, you modify the instances of the `LedRAII` and `ButtonRAII` classes in the corresponding tasks.

---

## 🔄 General Operation

- At startup, 4 FreeRTOS tasks are created:
- 3 tasks blink LEDs at different intervals
- 1 task reads the button and locks/releases the mutex associated with LED3
- LED3 stops blinking **immediately** when the button is pressed, and resumes blinking when the button is released.
- The `loop()` function is empty — everything runs via FreeRTOS tasks.

--
## 📦 Code Structure

```cpp
class LedRAII {
public:
    explicit LedRAII(gpio_num_t pin); // Configura o pino como OUTPUT
    void toggle() const;              // Alterna o estado do LED
    ~LedRAII();                       // Desliga o LED automaticamente
};

class ButtonRAII {
public:
    explicit ButtonRAII(gpio_num_t pin); // Configura o pino como INPUT_PULLUP
    bool isPressed() const;              // Retorna true se o botão estiver pressionado
};
```

---

## 📽️ Vídeo

<div align="center">

https://github.com/user-attachments/assets/fc7a1111-801a-4339-8f46-50f2639413fc

</div>

---

## 🖼️ Image

![PINOUT](https://github.com/user-attachments/assets/5ac26256-06c6-40ae-ab29-bd35d11dfe80)
