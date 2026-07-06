#include "stm32f1xx.h"

int main() {

    RCC->APB2ENR |= (1 << 4); // habilita clock do GPIOC (bit 4 do APB2ENR)
    GPIOC->CRH &= ~(0xF << 20); 
    GPIOC->CRH |= (2 << 20); // seta MODE13 10

    // pisca-pisca para verificar que o STM32 está ligado
    while (1) {
        for (int i = 0; i < 100000; i++) {};
        GPIOC->ODR |= (1  << 13); // acende LED (pino 13)
        for (int i = 0; i < 100000; i++) {};
        GPIOC->ODR &= ~(1 << 13); // apaga LED (limpa pino 13)
    }
}