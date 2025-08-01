# ⚙️ ESP32-C3 LED Multitask Blinker with RAII & FreeRTOS (C++14)

Este é um projeto moderno baseado em **C++14** para ESP32-C3, que utiliza **RAII (Resource Acquisition Is Initialization)** e **FreeRTOS** para fazer três LEDs piscar **simultaneamente**, cada um com um intervalo diferente. Um botão físico permite **pausar e retomar** o piscar de um dos LEDs através de um **mutex**.

- 🔃 3 tarefas concorrentes com FreeRTOS
- 🧠 Gestão de GPIOs via classes RAII em C++
- ⚡ Multitarefa real com `xTaskCreatePinnedToCore`
- 🚀 Sem uso da função `loop()` do Arduino
- 🔘 Botão físico controla mutex do LED3 (pausar/retomar)

> 🧰 Cada LED é controlado por uma task separada e encapsulado numa classe RAII que assegura a configuração e libertação segura dos pinos GPIO. O botão é monitorizado por uma task dedicada que controla um mutex para gerir o comportamento do LED3.

---

## 📋 Funcionalidades

- 🌈 **Três LEDs independentes**:
  - **LED1 (GPIO2)**: pisca a cada 0.5 segundos
  - **LED2 (GPIO3)**: pisca a cada 1 segundo
  - **LED3 (GPIO4)**: pisca a cada 0.2 segundos — controlado por botão
- 🔘 **Botão (GPIO5)**:
  - Pressionado → trava mutex → LED3 para de piscar  
  - Libertado → liberta mutex → LED3 retoma o piscar
- 🧱 **RAII para GPIOs**:
  - `LedRAII` e `ButtonRAII` encapsulam configuração e limpeza automática dos pinos
- ⚙️ **Multitarefa com FreeRTOS**:
  - Cada LED e o botão têm a sua própria tarefa (`xTaskCreatePinnedToCore`)
- ⏱️ **Sem bloqueios com `delay()`**:
  - Usa `vTaskDelay()` para multitarefa cooperativa
- ✅ **Limpeza automática**:
  - LEDs são desligados automaticamente no destrutor (`~LedRAII`)

---

## 🛠️ Requisitos

- Placa **ESP32-C3** (ex: Seeed Studio ESP32-C3)
- 3 **LEDs** + resistores limitadores (220Ω–330Ω)
- 1 **botão** com resistência pull-up (ou usar `INPUT_PULLUP`)
- IDE Arduino ou PlatformIO com suporte para ESP32
- Framework: **Arduino** com suporte a **FreeRTOS**

### 📚 Bibliotecas Utilizadas

- [`Arduino.h`](https://www.arduino.cc/reference/en/) (núcleo base)
- **FreeRTOS** (incluído por defeito no core ESP32 Arduino)

---

## 🧭 Mapa de Pinos

| Dispositivo | GPIO |
|-------------|------|
| LED1        | 2    |
| LED2        | 3    |
| LED3        | 4    |
| Botão       | 5    |

> Para alterar os pinos, modifica as instâncias das classes `LedRAII` e `ButtonRAII` nas tarefas correspondentes.

---

## 🔄 Funcionamento Geral

- Ao iniciar, são criadas 4 tarefas FreeRTOS:
  - 3 tarefas piscam LEDs com intervalos diferentes
  - 1 tarefa lê o botão e trava/liberta o mutex associado ao LED3
- O LED3 para de piscar **imediatamente** ao pressionar o botão, e retoma quando o botão é libertado.
- A função `loop()` está vazia — tudo corre via tarefas FreeRTOS.

---

## 📦 Estrutura do Código

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

## Image

![PINOUT](https://github.com/user-attachments/assets/5ac26256-06c6-40ae-ab29-bd35d11dfe80)
