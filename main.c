#include <stdint.h>
#include "delay.h"
#include "gpio.h"
#include "i2c.h"
#include "modbus_ascii.h"
#include "mpr121.h"
#include "rmcs_registers.h"
#include "stm32f4xx.h"
#include "uart.h"

int main(void) {
    // uint8_t response[50];
    uint16_t reg = 123;
    SystemInit();
    SysTick_Init();

    UART2_Init();
    I2C1_Init();

    /*
    RMCS_GPIO_Init();

    RMCS_BRAKE_OFF();
    RMCS_ENABLE();
    */

    delay_ms(200);
    /*
    RMCS_SetMode(7, MODE_POS);
    RMCS_SetSpeed(7, 5000);
    while (1) {
        RMCS_SetPosition(7, 133600);
        // ReadUntilMatch(7, REG_LSB_POS_FB, 20000);
        delay_ms(10000);

        // RMCS_SetPosition(7, -120240);
        // ReadUntilMatch(7, REG_LSB_POS_FB, -20000);
        // delay_ms(5000);
    }
    */

    MPR_Write(0x41, 200);
    MPR_Write(0x42, 100);
    // delay_ms(200);
    MPR_Write(0x5E, 0x00);
    MPR_Write(0x5E, 0x0C);

    while (1) {
        reg = MPR_Touch();

        UART_SendHex((reg >> 8) & 0xFF);
        UART_SendHex(reg & 0xFF);

        UART_SendByte('\r');
        UART_SendByte('\n');

        delay_ms(200);
    }
}
