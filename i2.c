#include "i2c.h"

// I2C初始化
void I2C_Configuration(void) 
{
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;
    
    // GPIOBI2C1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    
    // 配置PB6 PB7复用开漏
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // I2C1配置
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 400000;  
    
    I2C_Init(I2C1, &I2C_InitStructure);
    I2C_Cmd(I2C1, ENABLE);
}

// I2C起始信号
void I2C_Start(void) 
{
    I2C1->CR1 |= I2C_CR1_START;
    while(!(I2C1->SR1 & I2C_SR1_SB));
}

// I2C停止信号
void I2C_Stop(void) 
{
    I2C1->CR1 |= I2C_CR1_STOP;
    while(I2C1->CR1 & I2C_CR1_STOP);
}

// I2C发送一个字节
void I2C_SendByte(uint8_t data) 
{
    I2C1->DR = data;
    while(!(I2C1->SR1 & I2C_SR1_TXE));
}

// I2C等待应答
uint8_t I2C_WaitAck(void) 
{
    uint32_t time = 0;
    while(!(I2C1->SR1 & I2C_SR1_ADDR)) 
	{
        time++;
        if(time > 10000) return 1;  // 超时
    }
    (void)I2C1->SR2;  // 读取SR2清除ADDR标志
    return 0;
}

// 向OLED写命令
void OLED_Write_Command(uint8_t cmd) 
{
    I2C_Start();
    I2C_SendByte(0x78);  // OLED地址
    I2C_WaitAck();
    I2C_SendByte(0x00);  // 写命令
    I2C_WaitAck();
    I2C_SendByte(cmd);
    I2C_WaitAck();
    I2C_Stop();
}

// 向OLED写数据
void OLED_Write_Data(uint8_t data) 
{
    I2C_Start();
    I2C_SendByte(0x78);  // OLED地址
    I2C_WaitAck();
    I2C_SendByte(0x40);  // 写数据
    I2C_WaitAck();
    I2C_SendByte(data);
    I2C_WaitAck();
    I2C_Stop();
}
