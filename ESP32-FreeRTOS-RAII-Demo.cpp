#include <Arduino.h>

// RAII wrapper for output GPIO (LED)
class LedRAII {
private:
    const gpio_num_t gpio;

public:
    LedRAII(gpio_num_t pin) : gpio(pin) {
        pinMode(gpio, OUTPUT);
        digitalWrite(gpio, LOW);
    }

    void toggle() {
        digitalWrite(gpio, !digitalRead(gpio));
    }

    ~LedRAII() {
        digitalWrite(gpio, LOW);
    }
};

// RAII wrapper for input GPIO (Button)
class ButtonRAII {
private:
    const gpio_num_t gpio;

public:
    ButtonRAII(gpio_num_t pin) : gpio(pin) {
        pinMode(gpio, INPUT_PULLUP); // Button is active-low
    }

    bool isPressed() const {
        return digitalRead(gpio) == LOW;
    }
};

// Global mutex to control LED3
SemaphoreHandle_t mutexLed3;

// Task for LED1 — always blinking
void taskLed1(void* pvParameters) {
    LedRAII led(GPIO_NUM_2);
    while (true) {
        led.toggle();
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// Task for LED2 — always blinking
void taskLed2(void* pvParameters) {
    LedRAII led(GPIO_NUM_3);
    while (true) {
        led.toggle();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// Task for LED3 — blinking only if mutex is available
void taskLed3(void* pvParameters) {
    LedRAII led(GPIO_NUM_4);
    while (true) {
        if (xSemaphoreTake(mutexLed3, pdMS_TO_TICKS(10)) == pdTRUE) {
            led.toggle();
            xSemaphoreGive(mutexLed3);
        }
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

// Button task — locks/unlocks mutex for LED3
void taskButton(void* pvParameters) {
    ButtonRAII button(GPIO_NUM_5);
    bool locked = false;

    while (true) {
        if (button.isPressed()) {
            if (!locked) {
                xSemaphoreTake(mutexLed3, portMAX_DELAY); // lock
                locked = true;
                Serial.println("Button pressed — LED3 paused");
            }
        } else {
            if (locked) {
                xSemaphoreGive(mutexLed3); // unlock
                locked = false;
                Serial.println("Button released — LED3 resumed");
            }
        }
        vTaskDelay(pdMS_TO_TICKS(50)); // debounce
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Starting FreeRTOS tasks with button controlling LED3");

    // Create and release the mutex
    mutexLed3 = xSemaphoreCreateMutex();
    xSemaphoreGive(mutexLed3);

    // Launch tasks
    xTaskCreatePinnedToCore(taskLed1, "LED1", 2048, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(taskLed2, "LED2", 2048, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(taskLed3, "LED3", 2048, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(taskButton, "Button", 2048, NULL, 1, NULL, 0);
}

void loop() {
    // Empty — all logic handled by FreeRTOS tasks
}
