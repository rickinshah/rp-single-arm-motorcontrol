#ifndef MODBUS_ASCII_H
#define MODBUS_ASCII_H

#include <stdint.h>

void RMCS_SetSpeed(uint8_t slave, uint16_t spd);
void RMCS_SetPosition(uint8_t slave, int32_t pos);
void RMCS_SetMode(uint8_t slave, uint16_t mode);
void WriteSingleRegister(uint8_t slave, uint16_t address, uint16_t data);
void ReadSingleRegister(uint8_t slave, uint16_t address);
void ReadUntilMatch(uint8_t slave, uint16_t address, int16_t value);

#endif
