#include "led.h"
#include "uart.h"
#include "delay.h"
#include "i2c.h"
#include "mpu6500.h"
#include "system.h"
#include <stm32f1xx.h>

int main() {

    system_init();
    led_init();
    uart_init();
    i2c_init();
    // bloco para avisar que o bloco de init funcionou corretamente
    led_toggle();
    delay(1000000);
    led_toggle();
    delay(2000000);

    // fim do bloco de aviso luminoso

    MPU6500 mpu_sensor = MPU6500(0x68, i2c_write_register, i2c_read_register);
    bool mpu_check = mpu_sensor.init();
    uart_send_hex(mpu_sensor._last_read_val);
    if (!mpu_check){
        uart_send_string("MPU6500 não encontrado\r\n");
    } else {
        uart_send_string("MPU6500 detectado\r\n");
    }

    uart_send_string("UART check...\r\n");
    // pisca-pisca para verificar que o STM32 está ligado
    while (1) {
        delay(100000);
        led_toggle();
    }
}