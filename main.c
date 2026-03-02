#include "delay.h"
#include "gpio.h"
#include "modbus_ascii.h"
#include "rmcs_registers.h"
#include "stm32f4xx.h"
#include "uart.h"
#include <stdint.h>

int main(void) {
    uint8_t response[50];
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
        RMCS_SetPosition(7, 133600);
        // ReadUntilMatch(7, REG_LSB_POS_FB, 20000);

        // RMCS_SetPosition(7, -120240);
        // ReadUntilMatch(7, REG_LSB_POS_FB, -20000);
    }
}
