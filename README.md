# 📊 **Contador de LED 5x5 com Pi Pico W**

About
Projeto utilizando as funções de botões, controle de LED, controle do painel de led WS2812, rotinas de interrupção, comunicação serial da RP2020 com UART e I2C e controle do display OLED SSD 1306, para a residência de Sistemas Embarcados TIC 37 - Embarcatech

---

## 🔎 **Objetivos**

O objetivo principal é consolidar os conceitos sobre o uso de interfaces de comunicação serial no RP2040 e explorar as funcionalidades da placa de desenvolvimento BitDogLab

---

## 🎥 **Demonstração**

[Ver Vídeo do Projeto]()

---

## 🛠️ **Tecnologias Utilizadas**

- **Linguagem de Programação:** C / CMake
- **Placas Microcontroladoras:**
  - BitDogLab
  - Pico W
---

## 📖 **Como Utilizar**

- **Apertar o botão A**: Alternar o estado do LED RGB Verde (ligado/desligado). Uma mensagem informativa sobre o estado do LED deve ser exibida no display SSD1306 e Um texto descritivo sobre a operação deve ser enviado ao Serial Monitor.
- **Apertar o botão B**: Alternar o estado do LED RGB Azul (ligado/desligado). Uma mensagem informativa sobre o estado do LED deve ser exibida no display SSD1306 e Um texto descritivo sobre a operação deve ser enviado ao Serial Monitor.
- **Serial Monitor**: Qualquer letra, maiúscula ou minúscula que for enviada através do Serial Monitor, devera ser exibida no display SSD1306.
- **Serial Monitor**: Qualquer número de 0 a 9 enviado através do Serial Monitor, devera ser exibido no painel de LED 5x5 WS2812.

---

## 📊 **Funcionalidades Demonstradas**

- Manipulação do painel LED 5x5 WS2812
- Utilização de botões.
- Efeitos com LEDs RGB.
- Rotinas de interrupção(IRQ)
- Manipulação do Display OLED SSD1306
- Comunicação Serial através do Serial Monitor via UART.
- Utilização do protocolo I2C.

---


