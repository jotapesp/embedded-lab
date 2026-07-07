#include "led.h"
#include "gpioc.h"
#include "stm32f1xx.h"

void led_init() {

    gpioc_init(); // habilita clock do GPIOC
    GPIOC->CRH &= ~(0xF << 20); 
    GPIOC->CRH |= (2 << 20); // seta MODE13 10 (bits 23:20 para o pino 13) saída 2MHz permite acender/apagar
                             // mantém CNF em 00 (limpa) 00 - push-pull (não precisa de resistor externo)
}

void led_on() {
    GPIOC->ODR |= (1  << 13); // acende LED (pino 13)
}

void led_off() {
    GPIOC->ODR &= ~(1  << 13); // apaga LED (limpa pino 13)
}

void led_toggle() {
    GPIOC->ODR ^= (1 << 13); //XOR 1 sempre muda o bit.
}