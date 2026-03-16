#include "modbus_ascii.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

uint8_t CheckResponse(uint8_t* data, uint16_t data_len, uint8_t expected_slave,
                      uint8_t expected_func, uint16_t reg_quantity, uint16_t* out_index) {
    uint16_t data_bytes = reg_quantity * 2;
    *out_index          = 0;

    if (data_len < (3 + data_bytes + 1))
        return MODBUS_ERR_LENGTH;

    if (!validateLRC(data, data_len))
        return MODBUS_ERR_LRC;

    if (data[(*out_index)++] != expected_slave)
        return MODBUS_ERR_SLAVE;

    if (data[*out_index] != expected_func) {
        if (data[*out_index] == (0x80 | expected_func))
            return MODBUS_ERR_EXCEPTION;
        else
            return MODBUS_ERR_FUNCTION;
    }
    (*out_index)++;

    if (data_bytes != data[(*out_index)++])
        return MODBUS_ERR_BYTECOUNT;

    return MODBUS_OK;
}

bool ValidateRegisters(uint8_t* data, uint16_t data_len, uint16_t start_index,
                       uint16_t reg_quantity, const int16_t* match_values) {
    uint16_t curr_index = start_index;

    if (data_len < start_index + (reg_quantity * 2))
        return false;

    for (int i = 0; i < reg_quantity; i++) {
        int16_t curr_reg = (int16_t) BytesToDouble(data[curr_index], data[curr_index + 1]);
        int32_t diff     = (int32_t) curr_reg - match_values[i];
        if (abs(diff) > 20) {
            return false;
        }
        curr_index += 2;
    }

    return true;
}

void ReadUntilMatch(uint8_t slave, uint16_t address, uint16_t reg_quantity,
                    const int16_t* match_values) {
    uint8_t  ascii_response[64];
    uint16_t ascii_len;
    uint8_t  byte_response[32];
    uint16_t byte_len;
    uint8_t  status;
    uint16_t index;

    while (1) {
        RequestReadRegisters(slave, address, reg_quantity);
        ascii_len = UART_ReadAsciiArray(ascii_response, 64);

        if (!ModbusAsciiToBytes(ascii_response, ascii_len, 32, byte_response, &byte_len))
            continue;

        status = CheckResponse(byte_response, byte_len, slave, 0x03, reg_quantity, &index);

        if (status != MODBUS_OK)
            continue;

        if (ValidateRegisters(byte_response, byte_len, index, reg_quantity, match_values))
            break;
    }
}
