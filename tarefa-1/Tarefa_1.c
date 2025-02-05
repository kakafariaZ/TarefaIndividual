/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

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
        printf("Semáforo funcionando...\n");
        sleep_ms(1000);
    }
}
