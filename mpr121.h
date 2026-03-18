#ifndef MPR121_H
#define MPR121_H

#include <stdint.h>

void     MPR_Write(uint8_t reg, uint8_t data);
uint8_t  MPR_Read(uint8_t reg);
uint16_t MPR_Touch(void);
uint16_t MPR_ReadBaseline(uint8_t electrode);

#endif
