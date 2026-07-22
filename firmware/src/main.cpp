#include "led.h"
#include "uart.h"
#include "delay.h"
#include "i2c.h"
#include "mpu6500.h"
#include <stm32f1xx.h>

int main() {

    led_init();
    uart_init();
    MPU6500 mpu_sensor = MPU6500(0x68, i2c_write_register, i2c_read_register);
    bool mpu_check = mpu_sensor.init();
    if (!mpu_check){
        uart_send_string("MPU6500 não encontrado\r\n");
    }

    uart_send_string("UART check...\r\n");
    // pisca-pisca para verificar que o STM32 está ligado
    while (1) {
        delay(100000);
        led_toggle();
    }
}