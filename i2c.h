#ifndef I2C_H
#define I2C_H

#include <stdint.h>

void I2C1_Init(void);
void I2C1_Start(void);
void I2C_SendAddress(uint8_t address);
void I2C_WriteByte(uint8_t data);
void I2C1_Stop(void);

#endif
