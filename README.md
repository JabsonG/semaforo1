# Semáforo com Raspberry Pi Pico W

### Autor: Jabson Gama Santana Júnior

## Descrição
Este projeto implementa um semáforo utilizando um Raspberry Pi Pico W e LEDs RGB, seguindo uma temporização de 3 segundos para cada mudança de estado.

## Componentes Utilizados
- Raspberry Pi Pico W
- 3 LEDs (Vermelho, Amarelo e Verde)
- 3 Resistores de 330Ω

## Funcionamento
- O semáforo inicia com o LED vermelho aceso.
- Após 3 segundos, muda para amarelo.
- Depois de mais 3 segundos, muda para verde.
- O ciclo se repete continuamente.
- Além disso, uma mensagem informando o estado atual do semáforo é impressa no terminal a cada 1 segundo.

## Código
```c
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
```

## Como Usar
1. Conecte os LEDs e resistores ao Raspberry Pi Pico W seguindo o esquema de ligação.
2. Compile e carregue o código na placa.
3. Observe os LEDs alternando entre vermelho, amarelo e verde a cada 3 segundos.
4. Abra o monitor serial para visualizar as mensagens do estado atual do semáforo.


