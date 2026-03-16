#include <stdint.h>
#include "delay.h"
#include "gpio.h"
#include "modbus_ascii.h"
#include "rmcs_registers.h"
#include "stm32f4xx.h"
#include "uart.h"

int main(void) {
    int32_t position;
    int16_t positions[2];
    SystemInit();
    SysTick_Init();

    UART2_Init();
    RMCS_GPIO_Init();

    RMCS_BRAKE_OFF();
    RMCS_ENABLE();

    delay_ms(200);
    RMCS_SetMode(7, MODE_POS);
    RMCS_SetSpeed(7, 5000);
    while (1) {
        position     = 133600;
        positions[0] = (int16_t) (position & 0xFFFF);
        positions[1] = (int16_t) (position >> 16);
        RMCS_SetPosition(7, position);
        ReadUntilMatch(7, REG_LSB_POS_FB, 2, positions);
        delay_ms(200);

        position     = 0;
        positions[0] = (int16_t) (position & 0xFFFF);
        positions[1] = (int16_t) (position >> 16);
        RMCS_SetPosition(7, position);
        ReadUntilMatch(7, REG_LSB_POS_FB, 2, positions);
        delay_ms(200);
    }
}
