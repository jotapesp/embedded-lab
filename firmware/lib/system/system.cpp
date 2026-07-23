#include "system.h"
#include "stm32f1xx.h"

void system_init(){

    RCC->CR &= ~(1 << 16); // limpa os bits (bit 16) do HSE
    RCC->CR |= 1; // seta bit HSI (bit 0) para 1

    while (!(RCC->CR & (1 << 1))) {}; // espera HSIRDY (clock estabilizar - bit 1)

    RCC->CFGR &= ~(3); // limpa os 2 bits do SW e já seta para HSI (00)

    while (RCC->CFGR & (3 << 2) != 0) {}; // espera estabilizar SWS
}