#include "main.h"
#include "stm32f10x_gpio.h"

// 延时函数
void Delay_ms(uint32_t ms) 
{
    volatile uint32_t i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 7200; j++);
}

// GPIO配置
void GPIO_Config(void) 
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // 按键引脚配置（上拉输入）
    GPIO_InitStructure.GPIO_Pin = KEY1_PIN | KEY2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

// 读取按键1状态
uint8_t Read_Key1(void) 
{
    if (GPIO_ReadInputDataBit(GPIOA, KEY1_PIN) == Bit_RESET) 
	{
        Delay_ms(20);  // 消抖
        if (GPIO_ReadInputDataBit(GPIOA, KEY1_PIN) == Bit_RESET) 
		{
            return KEY_PRESSED;
        }
    }
    return KEY_RELEASED;
}

// 读取按键2状态
uint8_t Read_Key2(void) 
{
    if (GPIO_ReadInputDataBit(GPIOA, KEY2_PIN) == Bit_RESET) 
	{
        Delay_ms(20);
        if (GPIO_ReadInputDataBit(GPIOA, KEY2_PIN) == Bit_RESET) 
		{
            return KEY_PRESSED;
        }
    }
    return KEY_RELEASED;
}