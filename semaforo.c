#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

// Definição dos pinos do LED RGB (BitDogLab)
#define LED_RED 11
#define LED_YELLOW 12
#define LED_GREEN 13

// Tempo de troca de sinal (3s = 3000ms)
#define SEMAPHORE_DELAY 3000
// Tempo de impressão no terminal (1s = 1000ms)
#define SERIAL_PRINT_DELAY 1000

// Estados do semáforo
typedef enum { RED, YELLOW, GREEN } semaphore_state_t;
semaphore_state_t state = RED;

// Callback do temporizador para alternar o semáforo
bool repeating_timer_callback(struct repeating_timer *t) {
    // Alternar estado do semáforo
    switch (state) {
        case RED:
            gpio_put(LED_RED, 0);
            gpio_put(LED_YELLOW, 1);
            state = YELLOW;
            break;
        case YELLOW:
            gpio_put(LED_YELLOW, 0);
            gpio_put(LED_GREEN, 1);
            state = GREEN;
            break;
        case GREEN:
            gpio_put(LED_GREEN, 0);
            gpio_put(LED_RED, 1);
            state = RED;
            break;
    }
    return true;  // Mantém o temporizador ativo
}


int main() {
    //stdio_init_all();

    // Inicializa os pinos do LED RGB como saída
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_init(LED_YELLOW);
    gpio_set_dir(LED_YELLOW, GPIO_OUT);
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);

    // Inicializa o semáforo no vermelho
    gpio_put(LED_RED, 1);
    gpio_put(LED_YELLOW, 0);
    gpio_put(LED_GREEN, 0);

    // Configura o temporizador para trocar o semáforo a cada 3s
    struct repeating_timer timer;
    add_repeating_timer_ms(-SEMAPHORE_DELAY, repeating_timer_callback, NULL, &timer);

    // Loop principal - imprime mensagem a cada 1s
    while (true) {
        printf("Semáforo atual: %s\n", state == RED ? "Vermelho" : state == YELLOW ? "Amarelo" : "Verde");
        sleep_ms(SERIAL_PRINT_DELAY);
    }

    return 0;
}
