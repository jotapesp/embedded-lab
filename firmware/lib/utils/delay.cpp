#include "delay.h"
#include <stdint.h>
#include "stm32f1xx.h"

void delay(volatile uint32_t count){
    while(count--){
        __NOP();
    };
}