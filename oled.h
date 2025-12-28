#ifndef __OLED_H
#define __OLED_H

#include "stm32f10x.h"

// OLED显存数组，128x64分辨率，分8页，每页128字节
extern uint8_t OLED_GRAM[8][128];

// 字体大小定义
typedef enum 
{
    FONT_SIZE_6x8 = 0,
    FONT_SIZE_8x16,
    FONT_SIZE_16x16
} FontSize;

// 颜色定义
typedef enum 
{
    COLOR_WHITE = 0,
    COLOR_BLACK = 1
} Color;

// OLED初始化
void OLED_Init(void);

// 基本显示函数
void OLED_Clear(void);
void OLED_Refresh(void);
void OLED_SetPos(uint8_t x, uint8_t y);

// 绘图函数
void OLED_DrawPoint(uint8_t x, uint8_t y, Color color);
void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, Color color);
void OLED_DrawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, Color color);
void OLED_Fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, Color color);

// 字符显示函数
void OLED_ShowChar(uint8_t x, uint8_t y, char ch, FontSize size, Color color);
void OLED_ShowString(uint8_t x, uint8_t y, const char *str, FontSize size, Color color);
void OLED_ShowNumber(uint8_t x, uint8_t y, uint32_t num, uint8_t len, FontSize size, Color color);

// 图像显示
void OLED_ShowImage(const uint8_t *image, uint8_t x, uint8_t y, uint8_t width, uint8_t height);

#endif
