#include "i2c.h"
#include "stm32f4xx.h"

void I2C1_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_APB1ENR_I2C1EN;
    
    GPIOB->MODER |= (2 << (6*2)) | (2 << (7*2));

    GPIOB->OTYPER |= (1 <<6) | (1 << 7);
    GPIOB->OSPEEDR |= (3 << (6*2)) | (3 << (7*2));
    GPIOB->PUPDR |= (1 << (6*2)) | (1 << (7*2));
    GPIOB->AFR[0] |= (4 << (6*4)) | (4 << (7*4));
    
    // I2C
    I2C1->CR1 &= ~I2C_CR1_PE;
    I2C1->CR2 = 16;
    I2C1->CCR = 80; // 100 KHz
    I2C1->TRISE = 17;
    
    I2C1->CR1 |= I2C_CR1_PE;
}

void I2C1_Start(void) {
    I2C1->CR1 |= I2C_CR1_START;
    while(!(I2C1->SR1 & I2C_SR1_SB));
}

void I2C_SendAddress(uint8_t address) {
    volatile uint32_t temp;
    I2C1->DR = address;
    while(!(I2C1->SR1 & I2C_SR1_ADDR));
    temp = I2C1->SR2;
    (void)temp;
}

void I2C_WriteByte(uint8_t data) {
    while(!(I2C1->SR1 & I2C_SR1_TXE));
    I2C1->DR = data;
}

void I2C1_Stop(void) {
    I2C1->CR1 |= I2C_CR1_STOP;
}

