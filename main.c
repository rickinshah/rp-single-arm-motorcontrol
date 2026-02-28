#include "delay.h"
#include "gpio.h"
#include "modbus_ascii.h"
#include "rmcs_registers.h"
#include "stm32f4xx.h"
#include "uart.h"
#include <stdint.h>

int main(void) {
    uint8_t c;
    uint8_t response[50];
    uint8_t i = 0;
    SystemInit();
    SysTick_Init();

    UART2_Init();
    RMCS_GPIO_Init();

    RMCS_BRAKE_OFF();
    RMCS_ENABLE();

    delay_ms(200);
    RMCS_SetMode(7, MODE_POS);
    RMCS_SetSpeed(7, 400);

    while (1) {
        RMCS_SetPosition(7, 20000);
        ReadSingleRegister(7, REG_LSB_POS);
        UART_ReadAsciiArray(response);

        // do {
        //     c = UART_ReadByte();
        // } while(c != ':');
        // rx[i++] = c;
        //
        // do {
        //     c = UART_ReadByte();
        //     rx[i++] = c;
        // } while (c != '\n');

        RMCS_SetPosition(7, -50000);
        delay_ms(5000);
    }
}
