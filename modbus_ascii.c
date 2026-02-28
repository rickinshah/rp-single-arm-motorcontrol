#include "modbus_ascii.h"
#include "delay.h"
#include "rmcs_registers.h"
#include <stdlib.h>
#include "uart.h"

static void ByteToHex(uint8_t byte, uint8_t* hex) {
    const uint8_t hex_chars[] = "0123456789ABCDEF";
    hex[0]                    = hex_chars[(byte >> 4) & 0x0F];
    hex[1]                    = hex_chars[byte & 0x0F];
}

static uint8_t AsciiToNibble(uint8_t c) {
    if(c >= '0' && c <= '9')
        return (c - '0');
    if(c >= 'A' && c <= 'F')
        return (c - 'A' + 10);
    if(c >= 'a' && c <= 'f')
        return (c - 'a' + 10);
    return 0;
}

static uint8_t AsciiToByte(uint8_t msb_ascii, uint8_t lsb_ascii) {
    uint8_t msb = AsciiToNibble(msb_ascii);
    uint8_t lsb = AsciiToNibble(lsb_ascii);

    return (msb << 4) | lsb;
}

static uint8_t LRC(uint8_t* data, uint16_t length) {
    uint8_t nLRC = 0;

    for (uint32_t i = 0; i < length; i++) nLRC += *data++;

    return (uint8_t) (-nLRC);
}

void WriteSingleRegister(uint8_t slave, uint16_t address, uint16_t data) {
    uint8_t frame[17];
    uint8_t binary_frame[6];
    uint8_t lrc;

    binary_frame[0] = slave;
    binary_frame[1] = 0x06;
    binary_frame[2] = address >> 8;
    binary_frame[3] = address & 0xFF;
    binary_frame[4] = data >> 8;
    binary_frame[5] = data & 0xFF;

    lrc = LRC(binary_frame, 6);

    frame[0] = ':';

    for (uint8_t i = 0; i < 6; i++) {
        ByteToHex(binary_frame[i], &frame[1 + (i * 2)]);
    }
    ByteToHex(lrc, &frame[13]);

    frame[15] = '\r';
    frame[16] = '\n';

    UART_SendArray(frame, 17);
}

void ReadSingleRegister(uint8_t slave, uint16_t address) {
    uint8_t frame[17];
    uint8_t binary_frame[6];
    uint8_t lrc;

    binary_frame[0] = slave;
    binary_frame[1] = 0x03;
    binary_frame[2] = address >> 8;
    binary_frame[3] = address & 0xFF;
    binary_frame[4] = 0x00;
    binary_frame[5] = 0x01;

    lrc = LRC(binary_frame, 6);

    frame[0] = ':';

    for (uint8_t i = 0; i < 6; i++) {
        ByteToHex(binary_frame[i], &frame[1 + (i * 2)]);
    }
    ByteToHex(lrc, &frame[13]);

    frame[15] = '\r';
    frame[16] = '\n';

    UART_SendArray(frame, 17);
}

void RMCS_SetSpeed(uint8_t slave, uint16_t spd) {
    WriteSingleRegister(slave, REG_SPD, spd);
}

void RMCS_SetMode(uint8_t slave, uint16_t mode) {
    WriteSingleRegister(slave, REG_MODE, mode);
}

void RMCS_SetPosition(uint8_t slave, int32_t pos) {
    WriteSingleRegister(slave, REG_LSB_POS, pos & 0xFFFF);
    WriteSingleRegister(slave, REG_MSB_POS, (pos >> 16) & 0xFFFF);
}

void ReadUntilMatch(uint8_t slave, uint16_t address, int16_t value) {
    uint8_t response[50];
    while(1) {
        ReadSingleRegister(slave, address);
        UART_ReadAsciiArray(response);

        int16_t data = (int16_t)(((uint16_t)AsciiToByte(response[7], response[8]) << 8) | AsciiToByte(response[9], response[10]));
        delay_ms(200);

        if (abs(data - value) <= 50) {
            return;
        }
    }
}
