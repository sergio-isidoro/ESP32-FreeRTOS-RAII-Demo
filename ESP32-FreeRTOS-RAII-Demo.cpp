#include <Arduino.h>

// RAII wrapper para output GPIO (LED)
class LedRAII {
private:
    const gpio_num_t gpio;

public:
    explicit LedRAII(gpio_num_t pin) : gpio(pin) {
        pinMode(gpio, OUTPUT);
        digitalWrite(gpio, LOW);
    }

    void toggle() const {
        digitalWrite(gpio, !digitalRead(gpio));
    }

    ~LedRAII() {
        digitalWrite(gpio, LOW);
    }
};

// RAII wrapper para input GPIO (Botão)
class ButtonRAII {
private:
    const gpio_num_t gpio;

public:
    explicit ButtonRAII(gpio_num_t pin) : gpio(pin) {
        pinMode(gpio, INPUT_PULLUP); // botão ativo a LOW
    }

    bool isPressed() const {
        return digitalRead(gpio) == LOW;
    }
};

SemaphoreHandle_t mutexLed3;

// Task LED1 — pisca constantemente
void taskLed1(void* pvParameters) {
    auto led = LedRAII(GPIO_NUM_2);
    while (true) {
        led.toggle();
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// Task LED2 — pisca a outro ritmo
void taskLed2(void* pvParameters) {
    auto led = LedRAII(GPIO_NUM_3);
    while (true) {
        led.toggle();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// Task LED3 — só pisca se mutex estiver livre
void taskLed3(void* pvParameters) {
    auto led = LedRAII(GPIO_NUM_4);
    while (true) {
        if (xSemaphoreTake(mutexLed3, pdMS_TO_TICKS(10)) == pdTRUE) {
            led.toggle();
            xSemaphoreGive(mutexLed3);
        }
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

// Task do botão — trava/destrava LED3
void taskButton(void* pvParameters) {
    auto button = ButtonRAII(GPIO_NUM_5);
    bool locked = false;

    while (true) {
        if (button.isPressed()) {
            if (!locked) {
                xSemaphoreTake(mutexLed3, portMAX_DELAY); // bloquear
                locked = true;
                Serial.println("Botão pressionado — LED3 pausado");
            }
        } else {
            if (locked) {
                xSemaphoreGive(mutexLed3); // libertar
                locked = false;
                Serial.println("Botão libertado — LED3 retomado");
            }
        }
        vTaskDelay(pdMS_TO_TICKS(50)); // debounce
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Início das tarefas FreeRTOS com botão a controlar LED3");

    mutexLed3 = xSemaphoreCreateMutex();
    xSemaphoreGive(mutexLed3);

    xTaskCreatePinnedToCore(taskLed1, "LED1", 2048, nullptr, 1, nullptr, 0);
    xTaskCreatePinnedToCore(taskLed2, "LED2", 2048, nullptr, 1, nullptr, 0);
    xTaskCreatePinnedToCore(taskLed3, "LED3", 2048, nullptr, 1, nullptr, 0);
    xTaskCreatePinnedToCore(taskButton, "Button", 2048, nullptr, 1, nullptr, 0);
}

void loop() {
    // Lógica 100% gerida pelas tarefas
}
