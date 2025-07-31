#include <Arduino.h>

// Simple RAII class to manage an LED on a GPIO pin.
// It sets the pin mode and ensures the LED is off when the object is destroyed.
class LedRAII {
private:
    const gpio_num_t gpio;

public:
    LedRAII(gpio_num_t pin) : gpio(pin) {
        pinMode(gpio, OUTPUT);
        digitalWrite(gpio, LOW); // start with LED off
    }

    void toggle() {
        digitalWrite(gpio, !digitalRead(gpio));
    }

    void on() {
        digitalWrite(gpio, HIGH);
    }

    void off() {
        digitalWrite(gpio, LOW);
    }

    ~LedRAII() {
        digitalWrite(gpio, LOW); // make sure LED is off when object is destroyed
    }
};

// Task to blink LED on GPIO2 every 500ms
void blinkLed1(void* pvParameters) {
    LedRAII led(GPIO_NUM_2);
    while (true) {
        led.toggle();
        vTaskDelay(pdMS_TO_TICKS(500)); // delay 500 milliseconds
    }
}

// Task to blink LED on GPIO3 every 1000ms
void blinkLed2(void* pvParameters) {
    LedRAII led(GPIO_NUM_3);
    while (true) {
        led.toggle();
        vTaskDelay(pdMS_TO_TICKS(1000)); // delay 1 second
    }
}

// Task to blink LED on GPIO4 every 200ms
void blinkLed3(void* pvParameters) {
    LedRAII led(GPIO_NUM_4);
    while (true) {
        led.toggle();
        vTaskDelay(pdMS_TO_TICKS(200)); // delay 200 milliseconds
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Starting RTOS tasks with RAII");

    // Create three independent tasks, one for each LED
    xTaskCreatePinnedToCore(blinkLed1, "LED1", 2048, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(blinkLed2, "LED2", 2048, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(blinkLed3, "LED3", 2048, NULL, 1, NULL, 0);
}

void loop() {
    // Nothing to do here, everything is handled by the RTOS tasks
}
