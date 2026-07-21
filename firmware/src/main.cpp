#include "led.h"
#include "uart.h"
#include "delay.h"
#include <stm32f1xx.h>

int main() {

    led_init();
    uart_init();

    uart_send_string("Hello from STM32!\r\n");
    // pisca-pisca para verificar que o STM32 está ligado
    while (1) {
        delay(100000);
        led_toggle();
    }
}