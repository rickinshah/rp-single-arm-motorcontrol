#include "modbus_ascii.h"
#include <stdbool.h>
#include <stdint.h>
#include "rmcs_registers.h"
#include "uart.h"
#include "utils.h"

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

void RequestReadRegisters(uint8_t slave, uint16_t address, uint16_t quantity) {
    uint8_t frame[17];
    uint8_t binary_frame[6];
    uint8_t lrc;

    binary_frame[0] = slave;
    binary_frame[1] = 0x03;
    binary_frame[2] = address >> 8;
    binary_frame[3] = address & 0xFF;
    binary_frame[4] = quantity >> 8;
    binary_frame[5] = quantity & 0xFF;

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

// TODO: check the response like slave, address, function, lrc, registers and return code based on
// that itself uint8_t checkResponse(uint8_t expected_slave, uint16_t expected_address, uint16_t
// reg_quantity, int16_t *match_values) {
//     if(!)
//
// }

void ReadUntilMatch(uint8_t slave, uint16_t address, uint16_t reg_quantity,
                    const int16_t* match_values) {
    uint8_t  ascii_response[64];
    uint16_t ascii_len;
    uint8_t  byte_response[32];
    uint16_t byte_len;
    uint8_t  exception_code;

    while (1) {
        RequestReadRegisters(slave, address, reg_quantity);
        ascii_len = UART_ReadAsciiArray(ascii_response, 64);

        if (!ModbusAsciiToBytes(ascii_response, ascii_len, 32, byte_response, &byte_len))
            continue;

        if (!validateLRC(byte_response, byte_len))
            continue;

        if (byte_response[0] != slave)
            continue;
        if (byte_response[1] != 0x03)
            continue;
        if (byte_response[1] & 0x80) {
            exception_code = byte_response[2];
            break;
        }


        // TODO: check response

        // TODO: check each register value with *match_values
        //         if (abs(data - value) <= 50) {
        //             return;
        //         }
    }
}
