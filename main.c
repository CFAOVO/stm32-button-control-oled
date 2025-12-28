#include "main.h"
#include "oled.h"
#include "i2c.h"

// 全局变量
uint8_t current_display = DISPLAY_WELCOME;
uint8_t key1_state = KEY_RELEASED;
uint8_t key2_state = KEY_RELEASED;

// 系统初始化
void System_Init(void) 
{
    // 初始化系统时钟
    SystemInit();
    
    // 配置GPIO
    GPIO_Config();
    
    // 配置I2C
    I2C_Configuration();
    
    // 初始化OLED
    OLED_Init();
}

// 显示内容函数
void Display_Content(uint8_t mode) 
{
    OLED_Clear();
    
    switch(mode) 
	{
        case DISPLAY_WELCOME:
            OLED_ShowString(0, 0, "Welcome!", FONT_SIZE_8x16, COLOR_WHITE);
            OLED_ShowString(0, 16, "STM32F103C8", FONT_SIZE_8x16, COLOR_WHITE);
            OLED_ShowString(0, 32, "Press Key1/2", FONT_SIZE_8x16, COLOR_WHITE);
            break;
            
        case DISPLAY_KEY1_PRESSED:
            OLED_ShowString(0, 0, "Key1 Pressed!", FONT_SIZE_8x16, COLOR_WHITE);
            OLED_ShowString(0, 16, "Mode 1 Active", FONT_SIZE_8x16, COLOR_WHITE);
            OLED_ShowString(0, 32, "LED Display", FONT_SIZE_8x16, COLOR_WHITE);
            break;
            
        case DISPLAY_KEY2_PRESSED:
            OLED_ShowString(0, 0, "Key2 Pressed!", FONT_SIZE_8x16, COLOR_WHITE);
            OLED_ShowString(0, 16, "Mode 2 Active", FONT_SIZE_8x16, COLOR_WHITE);
            OLED_ShowString(0, 32, "Text Changed", FONT_SIZE_8x16, COLOR_WHITE);
            break;
            
        case DISPLAY_BOTH_KEYS:
            OLED_ShowString(0, 0, "Both Keys!", FONT_SIZE_8x16, COLOR_WHITE);
            OLED_ShowString(0, 16, "Special Mode", FONT_SIZE_8x16, COLOR_WHITE);
            OLED_ShowString(0, 32, "Config Menu", FONT_SIZE_8x16, COLOR_WHITE);
            break;
            
        default:
            break;
    }
    OLED_Refresh();
}

int main(void) 
{
    // 系统初始化
    System_Init();
    
    // 初始显示欢迎信息
    Display_Content(DISPLAY_WELCOME);
    
    while(1) 
	{
        uint8_t key1_now = Read_Key1();
        uint8_t key2_now = Read_Key2();
        
        // 按键1按下
        if(key1_now == KEY_PRESSED && key1_state == KEY_RELEASED) 
		{
            key1_state = KEY_PRESSED;
            if(key2_state == KEY_PRESSED) 
			{
                current_display = DISPLAY_BOTH_KEYS;
            } else 
			{
                current_display = DISPLAY_KEY1_PRESSED;
            }
            Display_Content(current_display);
        }
        else if(key1_now == KEY_RELEASED && key1_state == KEY_PRESSED) 
		{
            key1_state = KEY_RELEASED;
            if(key2_state == KEY_RELEASED) 
			{
                current_display = DISPLAY_WELCOME;
                Display_Content(current_display);
            }
        }
        
        // 按键2按下
        if(key2_now == KEY_PRESSED && key2_state == KEY_RELEASED) 
		{
            key2_state = KEY_PRESSED;
            if(key1_state == KEY_PRESSED) 
			{
                current_display = DISPLAY_BOTH_KEYS;
            } else 
			{
                current_display = DISPLAY_KEY2_PRESSED;
            }
            Display_Content(current_display);
        }
        else if(key2_now == KEY_RELEASED && key2_state == KEY_PRESSED) 
		{
            key2_state = KEY_RELEASED;
            if(key1_state == KEY_RELEASED) 
			{
                current_display = DISPLAY_WELCOME;
                Display_Content(current_display);
            }
        }
        
        Delay_ms(10);  // 减少CPU占用
    }
}
