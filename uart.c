#include "uart.h"
#include "stm32f4xx.h"

void UART2_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    GPIOA->MODER |= (2 << (2 * 2)) | (2 << (3 * 2));
    GPIOA->AFR[0] |= (7 << (2 * 4)) | (7 << (3 * 4));

    USART2->BRR = 0x683; /* 9600 baud @ 16MHz APB1*/
    USART2->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

void UART_SendByte(uint8_t data) {
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = data;
}

void UART_SendArray(uint8_t* buf, uint16_t len) {
    uint16_t i;
    for (i = 0; i < len; i++) UART_SendByte(buf[i]);
}

uint8_t UART_ReadByte() {
    while (!(USART2->SR & USART_SR_RXNE));
    return (uint8_t)USART2->DR;
}

uint16_t UART_ReadAsciiArray(uint8_t* response, uint16_t max_len) {
    uint16_t i = 0;
    uint8_t c;
    do {
        c = UART_ReadByte();
    } while(c != ':');

    response[i++] = c;
    do {
        c = UART_ReadByte();
        
        if(i >= max_len) {
            return 0;
        }

        response[i++] = c;
    } while(c != '\n');

    return i;
}
