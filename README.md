
# Projetos de Temporização com Raspberry Pi Pico W

Este repositório contém dois projetos baseados em temporizadores com o microcontrolador **Raspberry Pi Pico W**. O objetivo é demonstrar como utilizar temporizadores para controlar LEDs e interagir com botões em sistemas embarcados. Os projetos foram desenvolvidos utilizando o **Pico SDK** e simulados na plataforma **Wokwi**.

## Projetos

### 1. **Tarefa 1: Semáforo Periódico com Temporizador (repeating_timer)**

Neste projeto, foi implementado um semáforo utilizando três LEDs (vermelho, amarelo e verde), com o uso de um temporizador repetitivo de 3 segundos (3.000ms). O semáforo alterna entre os estados conforme o fluxo simplificado descrito no fluxograma.

**Componentes necessários:**
- Microcontrolador Raspberry Pi Pico W
- 03 LEDs (vermelho, amarelo, verde)
- 03 resistores de 330Ω

**Funcionamento:**
- O semáforo inicia com o LED vermelho aceso.
- A cada 3 segundos, o semáforo muda de estado, alternando entre os LEDs vermelho, amarelo e verde.
- O código utiliza a função `add_repeating_timer_ms()` do Pico SDK para acionar o temporizador.

**Código:**
```c
// Código do semáforo com temporização de 3 segundos
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#define LED_RED 11
#define LED_YELLOW 12
#define LED_GREEN 13

volatile int state = 0; // Estado do semáforo

// Callback do temporizador
bool repeating_timer_callback(struct repeating_timer *t) {
    // Desliga todos os LEDs antes de alterar o estado
    gpio_put(LED_RED, 0);
    gpio_put(LED_YELLOW, 0);
    gpio_put(LED_GREEN, 0);

    // Alterna entre os estados do semáforo
    if (state == 0) {
        gpio_put(LED_RED, 1);
    } else if (state == 1) {
        gpio_put(LED_YELLOW, 1);
    } else {
        gpio_put(LED_GREEN, 1);
    }

    state = (state + 1) % 3; // Alterna entre 0, 1 e 2
    return true;
}

int main() {
    stdio_init_all();
    
    // Configuração dos pinos dos LEDs
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_init(LED_YELLOW);
    gpio_set_dir(LED_YELLOW, GPIO_OUT);
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);

    struct repeating_timer timer;
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);

    // Loop principal
    while (1) {
        printf("Semáforo funcionando...
");
        sleep_ms(1000);
    }
}
```

**Links úteis:**
- [Simulação no Wokwi](https://wokwi.com/)

---

### 2. **Tarefa 2: Temporizador de Um Disparo (One Shot)**

Este projeto implementa um sistema onde LEDs são acionados com o pressionamento de um botão. A cada 3 segundos, os LEDs são desligados em sequência (azul, vermelho e verde).

**Componentes necessários:**
- Microcontrolador Raspberry Pi Pico W
- 03 LEDs (azul, vermelho e verde)
- 03 resistores de 330Ω
- Botão (Pushbutton)

**Funcionamento:**
- Quando o botão é pressionado, todos os LEDs são acionados.
- A cada 3 segundos, um LED é desligado até que todos fiquem apagados.
- O código usa a função `add_alarm_in_ms()` para acionar o temporizador, controlando o desligamento dos LEDs.

**Código:**
```c
// Código de temporizador de um disparo para LEDs
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

#define LED_BLUE 11
#define LED_RED 12
#define LED_GREEN 13
#define BUTTON 5

volatile bool timer_active = false;

// Callback para desligar o LED verde
int64_t turn_off_green(alarm_id_t id, void *user_data) {
    gpio_put(LED_GREEN, 0);
    timer_active = false; // Permitir novo acionamento do botão
    return 0;
}

// Callback para desligar o LED vermelho e ativar temporizador do verde
int64_t turn_off_red(alarm_id_t id, void *user_data) {
    gpio_put(LED_RED, 0);
    add_alarm_in_ms(3000, turn_off_green, NULL, false);
    return 0;
}

// Callback para desligar o LED azul e ativar temporizador do vermelho
int64_t turn_off_blue(alarm_id_t id, void *user_data) {
    gpio_put(LED_BLUE, 0);
    add_alarm_in_ms(3000, turn_off_red, NULL, false);
    return 0;
}

void button_callback(uint gpio, uint32_t events) {
    if (!timer_active) {
        timer_active = true;
        gpio_put(LED_BLUE, 1);
        gpio_put(LED_RED, 1);
        gpio_put(LED_GREEN, 1);
        add_alarm_in_ms(3000, turn_off_blue, NULL, false);
    }
}

int main() {
    stdio_init_all();
    
    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE, GPIO_OUT);
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_init(BUTTON);
    gpio_set_dir(BUTTON, GPIO_IN);
    gpio_pull_up(BUTTON);
    
    gpio_set_irq_enabled_with_callback(BUTTON, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    
    while (1) {
        sleep_ms(100);
    }
}
```

**Links úteis:**
- [Simulação no Wokwi](https://wokwi.com/)

---

## Como Executar os Projetos

### Pré-requisitos:
1. **Raspberry Pi Pico W** ou ambiente de simulação (recomendado o uso do [Wokwi](https://wokwi.com/)).
2. **Pico SDK** instalado em seu ambiente de desenvolvimento.
3. Compilador **gcc-arm** configurado para o Raspberry Pi Pico.

### Passos:
1. Clone este repositório.
2. Compile o código para o seu Raspberry Pi Pico W ou execute a simulação no Wokwi.
3. Conecte os LEDs aos pinos configurados (11, 12, 13) e o botão ao pino 5 (para o segundo projeto).
4. Carregue o programa no Pico e observe o funcionamento dos temporizadores.

---

## Licença

Distribuído sob a Licença BSD-3-Clause. Veja o arquivo LICENSE para mais informações.
