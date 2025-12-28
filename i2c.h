#ifndef __I2C_H
#define __I2C_H

#include "stm32f10x.h"

// I2C相关函数声明
void I2C_Configuration(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendByte(uint8_t data);
void OLED_Write_Command(uint8_t cmd);
void OLED_Write_Data(uint8_t data);
uint8_t I2C_WaitAck(void);  

#endif
