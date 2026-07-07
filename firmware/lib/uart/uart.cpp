#include "uart.h"
#include "stm32f1xx.h"

void uart_init() {
    RCC->APB2ENR |= (1 << 14); // ativa clock uart
    RCC->APB2ENR |= (1 << 2); // ativa clock port a
    GPIOA->CRH &= ~(0xFF << 4); // limpar os pinos 9 e 10
    GPIOA->CRH |= (0xA << 4); // PA9 TX - config para 1010 (CNF-MODE)
    GPIOA->CRH |= (0x4 << 8); // PA10 RX - config para 0100 (CNF-MODE)
    USART1->BRR = 833 ; // BRR = f_clock / baud_rate -> 8Mhz (HSI) / 9600 = 833.33
    // não configuramos o tamanho da palavra, paridade e stop bits no CR1 pois usamos padrao 8N1
    USART1->CR1 |= (1 << 4); // Transmitter Enable
    USART1->CR1 |= (1 << 13); // USART Enable
}

void uart_send_byte(char c) {

    while (!(USART1->SR & (1 << 7))) {};
    USART1->DR = c;

}

void uart_send_string(const char* str){
    
    const char *ptr = str;
    while (*ptr != '\0'){
        uart_send_byte(*ptr);
        ptr += 1;
    }
}