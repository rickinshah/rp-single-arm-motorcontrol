#ifndef UART_H
#define UART_H

#include <stdint.h>

void UART2_Init(void);
void UART_SendByte(uint8_t data);
void UART_SendArray(uint8_t *buf, uint16_t len);
uint8_t UART_ReadByte(void);
uint16_t UART_ReadAsciiArray(uint8_t *response, uint16_t max_len);

#endif
