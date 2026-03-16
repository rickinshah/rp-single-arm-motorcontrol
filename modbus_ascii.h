#ifndef MODBUS_ASCII_H
#define MODBUS_ASCII_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    MODBUS_OK = 0,
    MODBUS_ERR_ASCII,
    MODBUS_ERR_LRC,
    MODBUS_ERR_SLAVE,
    MODBUS_ERR_FUNCTION,
    MODBUS_ERR_EXCEPTION,
    MODBUS_ERR_BYTECOUNT,
    MODBUS_ERR_LENGTH,
    MODBUS_ERR_REG

} ModbusStatus;

uint8_t CheckResponse(uint8_t* data, uint16_t data_len, uint8_t expected_slave,
                      uint8_t expected_func, uint16_t reg_quantity, uint16_t* curr_index);
bool    ValidateRegisters(uint8_t* data, uint16_t data_len, uint16_t start_index,
                          uint16_t reg_quantity, const int16_t* match_values);
void    RMCS_SetSpeed(uint8_t slave, uint16_t spd);
void    RMCS_SetPosition(uint8_t slave, int32_t pos);
void    RMCS_SetMode(uint8_t slave, uint16_t mode);
void    WriteSingleRegister(uint8_t slave, uint16_t address, uint16_t data);
void    RequestReadRegisters(uint8_t slave, uint16_t address, uint16_t quantity);
void    ReadUntilMatch(uint8_t slave, uint16_t address, uint16_t reg_quantity,
                       const int16_t* match_values);

#endif
