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
    USART1->CR1 |= (1 << 3); // Transmitter Enable
    USART1->CR1 |= (1 << 13); // USART Enable
}

void uart_send_byte(char c) {

    while (!(USART1->SR & (1 << 7))) {}; // tem que esperar o TXE ir pra 1
    USART1->DR = c;

}

void uart_send_string(const char* str){
    
    const char *ptr = str;
    while (*ptr != '\0'){
        uart_send_byte(*ptr);
        ptr += 1;
    }
}

void uart_send_hex(uint8_t val) {
    const char char_index[] = "0123456789ABCDEF";

    char str[7];
    *str = '0';
    *(str + 1) = 'x';
    // digito hex mais significativo sao os 4 bits bin mais significativos
    // divide por 16 para descartar ultimo digito hex, sobra o mais signficativo
    *(str + 2) = char_index[(val >> 4) & (0x0F)];
    // digito hex menos significativo sao os 4 bits bin menos significativos
    // zera os 4 bits mais altos e sobra os 4 mais baixos, ou seja, o ultimo digito hex
    *(str + 3) = char_index[val & 0x0F];
    *(str + 4) = '\r';
    *(str + 5) = '\n';
    *(str + 6) = '\0';

    uart_send_string(str);
}