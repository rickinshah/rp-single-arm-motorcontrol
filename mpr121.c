#include "mpr121.h"
#include "stm32f4xx.h"
#include "i2c.h"

void MPR_Write(uint8_t reg, uint8_t data){
    I2C1_Start();
    
    I2C_SendAddress(0x5A << 1);
    
    I2C_WriteByte(reg);
    I2C_WriteByte(data);
    
    while(!(I2C1->SR1 & I2C_SR1_BTF));
    
    I2C1_Stop();
}

uint8_t MPR_Read(uint8_t reg) {
    uint8_t data;
    I2C1_Start();
    I2C_SendAddress(0x5A << 1);
    I2C_WriteByte(reg);
    
    I2C1_Start();
    I2C_SendAddress((0x5A << 1) | 1);
    
    I2C1->CR1 &= ~I2C_CR1_ACK;

    while(!(I2C1->SR1 & I2C_SR1_RXNE));
    data = I2C1->DR;

    I2C1_Stop();

    return data;
}

uint16_t MPR_Touch() {
    uint8_t lsb = MPR_Read(0x00);
    uint8_t msb = MPR_Read(0x01);
    
    return ((uint16_t)(msb << 8) | lsb);
}
