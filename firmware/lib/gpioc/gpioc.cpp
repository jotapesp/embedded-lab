#include "gpioc.h"
#include "stm32f1xx.h"

void gpioc_init() {

    RCC->APB2ENR |= (1 << 4); // habilita clock do GPIOC (bit 4 do APB2ENR)
}