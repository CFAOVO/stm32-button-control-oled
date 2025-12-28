#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f10x.h"

// 按键定义
#define KEY1_PIN  GPIO_Pin_0
#define KEY1_PORT GPIOA
#define KEY2_PIN  GPIO_Pin_1
#define KEY2_PORT GPIOA

// 按键状态
#define KEY_RELEASED 0
#define KEY_PRESSED  1

// 显示模式
#define DISPLAY_WELCOME      0
#define DISPLAY_KEY1_PRESSED 1
#define DISPLAY_KEY2_PRESSED 2
#define DISPLAY_BOTH_KEYS    3

// 函数声明
void GPIO_Config(void);
void System_Init(void);
void Display_Content(uint8_t mode);
uint8_t Read_Key1(void);
uint8_t Read_Key2(void);
void Delay_ms(uint32_t ms);

#endif
