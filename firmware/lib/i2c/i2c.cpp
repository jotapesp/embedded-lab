#include "i2c.h"
#include "stm32f1xx.h"

void i2c_init() {

    RCC->APB1ENR |= (1 << 21); // liga clock do I2C
    RCC->APB2ENR |= (1 << 3); // liga clock das portas B (gpiob)
    GPIOB->CRL &= ~(0xFF << 24); // limpa os pinos PB6 (SCL) e PB7 (SDA)
    GPIOB->CRL |= (0xEE << 24); // config ambos PB6 e PB7 para output Open-drain (1110 - cnf 11 (open-drain) mode 10 (vel 2Mhz))
    I2C1->CR2 &= ~0x3F; // limpa os 6 lsb
    I2C1->CR2 |= 8; // marca a freq do APB (8Mhz pois HSI -> acabou se inicializar, sem PLL)
    I2C1->CCR &= ~(0xFFF); // limpa os 11 LSB do CCR
    I2C1->CCR |= 40; // config ccr para 40 (ciclos que precisa gastar do clock principal para atingirmos um sinal de saída de 100kHz - SM do I2C)
    I2C1->TRISE &= ~(0x3F); // limpa os bits do trise, mandento os reserved
    I2C1->TRISE |= 9; // seta apenas os bits do trise para 9 com o OR
    I2C1->CR1 |= 1; // bit PE (bit 0) = 1
}

void i2c_write_register(uint8_t addr, uint8_t reg, uint8_t data) {
    
    I2C1->CR1 |= (1 << 8); // seta bit start = 1
    while (!(I2C1->SR1 & 1)) {}; // espera bit SB = 1
    I2C1->DR = (addr << 1); // preenche apenas os 8 lsb (endereço dispositivo i2c). Note que o DR deve usar apenas atribuiçao, RESERVED bits devem ser 0.
    while (!(I2C1->SR1 & (1 << 1))) {}; // espera bit ADDR = 1
    (void) I2C1->SR1;
    (void) I2C1->SR2;
    I2C1->DR = reg; // preenche apenas os 8 lsb com o endereço do reg.
    while (!(I2C1->SR1 & (1 << 7))) {}; // espera bit TXE = 1
    I2C1->DR = data; // escreve o valor no registrador
    while ((I2C1->SR1 & (33 << 2)) != (33 << 2)) {}; // espera TXE = 1 & BTF = 1
    I2C1->CR1 |= (1 << 9);
}

uint8_t i2c_read_register(uint8_t addr, uint8_t reg) {
    
    uint8_t data;

    I2C1->CR1 |= (1 << 8); // seta bit start = 1
    while (!(I2C1->SR1 & 1)) {}; // espera bit SB = 1
    I2C1->DR = (addr << 1); // preenche apenas os 8 lsb (endereço dispositivo i2c). Note que o DR deve usar apenas atribuiçao, RESERVED bits devem ser 0. LSB = 0 (escrita)
    while (!(I2C1->SR1 & (1 << 1))) {}; // espera bit ADDR = 1
    (void) I2C1->SR1;
    (void) I2C1->SR2; // essas duas leituras limpam o addr que foi setado após leitura do endereço do dispositivo no i2c
    I2C1->DR = reg; // preenche apenas os 8 lsb com o endereço do reg.
    while ((I2C1->SR1 & (33 << 2)) != (33 << 2)) {}; // TXE = 1 & BTF = 1 (espera limpar DR e shift register)
    I2C1->CR1 |= (1 << 8); // RESTART
    while (!(I2C1->SR1 & 1)) {}; // espera bit SB = 1
    I2C1->DR = (addr << 1) + 1; // manda endereço do disp no i2c com bit r/w (lsb) = 1 (leitura)
    while (!(I2C1->SR1 & (1 << 1))) {}; // espera bit ADDR = 1
    I2C1->CR1 &= ~(1 << 10); // ACK = 0
    (void) I2C1->SR1;
    (void) I2C1->SR2; // limpa ADDR
    I2C1->CR1 |= (1 << 9); // seta STOP bit no CR1
    while (!(I2C1->SR1 & (1 << 6))) {}; // espera bit RXNE = 1
    data = I2C1->DR; // le o valor no registrador

    I2C1->CR1 |= (1 << 10); // restaurante ACK para 1 para proximas transferencias

    return data;
}