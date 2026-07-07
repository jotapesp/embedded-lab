#include "led.h"
#include "uart.h"
#include "stm32f1xx.h"

int main() {

    led_init();
    uart_init();

    uart_send_string("Hello from STM32!\r\n");
    // pisca-pisca para verificar que o STM32 está ligado
    while (1) {
        for (int i = 0; i < 100000; i++) {};
        led_toggle();
        for (int i = 0; i < 100000; i++) {};
    }
}