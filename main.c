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
    uint16_t reg    = 0;
    uint16_t base   = 0;
    uint8_t* urt    = (uint8_t*) "Value: ";
    uint8_t* basePr = (uint8_t*) "B";
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

    for (int i = 0; i < 12; i++) {
        MPR_Write(0x41 + i * 2, 30);  // touch
        MPR_Write(0x42 + i * 2, 15);  // release
    }
    // Rising
    /*
    MPR_Write(0x2B, 0x01);
    MPR_Write(0x2C, 0x01);
    MPR_Write(0x2D, 0x00);
    MPR_Write(0x2E, 0x01);

    // Falling (keep stable)
    MPR_Write(0x2F, 0x01);
    MPR_Write(0x30, 0x01);
    MPR_Write(0x31, 0xFF);
    MPR_Write(0x32, 0x02);*/
    // delay_ms(200);
    MPR_Write(0x5E, 0x00);
    delay_ms(50);
    MPR_Write(0x5E, 0x0C);

    while (1) {
        reg = MPR_Touch();

        UART_SendArray(urt, 7);
        UART_SendHex((reg >> 8) & 0xFF);
        UART_SendHex(reg & 0xFF);

        UART_SendByte(' ');
        UART_SendByte(' ');

        for (uint8_t i = 0; i < 12; i++) {
            base = MPR_ReadBaseline(i);
            UART_SendByte('B');
            UART_SendHex(i);
            UART_SendByte(':');

            UART_SendHex((base >> 8) & 0xFF);
            UART_SendHex(base & 0xFF);

            UART_SendByte(' ');
        }


        UART_SendByte('\r');
        UART_SendByte('\n');


        delay_ms(200);
    }
}
