#include <cstdint>

void uart_init();
void uart_send_byte(char c);
void uart_send_string(const char* str);
void uart_send_hex(uint8_t val);