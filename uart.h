#ifndef UART_H
#define UART_H

#include <stdint.h>

void    UART2_Init(void);
void    UART_SendByte(uint8_t data);
void    UART_SendArray(uint8_t* buf, uint16_t len);
uint8_t UART_ReadByte(void);
void    UART_ReadAsciiArray(uint8_t* response);
void    UART_SendHex(uint8_t val);

#endif
