#include "oled.h"
#include "oled_font.h"
#include "i2c.h"
#include <string.h>

// OLED显存
uint8_t OLED_GRAM[8][128] = {0};

// OLED初始化命令序列
static const uint8_t OLED_INIT_CMD[] = 
{
    0xAE, // 关闭显示
    
    0xD5, // 设置显示时钟分频比/振荡器频率
    0x80, // 设置分频比
    
    0xA8, // 设置多路复用率
    0x3F, // 64行
    
    0xD3, // 设置显示偏移
    0x00, // 不偏移
    
    0x40, // 设置显示开始行
    
    0x8D, // 电荷泵设置
    0x14, // 开启电荷泵
    
    0x20, // 设置内存地址模式
    0x00, // 水平地址模式
    
    0xA1, // 设置段重映射（列127映射到SEG0）
    
    0xC8, // 设置COM扫描方向（从COM63扫描到COM0）
    
    0xDA, // 设置COM硬件配置
    0x12, // 使用alternative COM配置
    
    0x81, // 设置对比度控制
    0xCF, // 对比度值
    
    0xD9, // 设置预充电周期
    0xF1, // 预充电周期
    
    0xDB, // 设置VCOMH取消选择级别
    0x40, // VCOMH = ~0.77 * VCC
    
    0xA4, // 设置整个显示开启/关闭
    0xA6, // 设置正常显示（不反白）
    
    0xAF  // 开启OLED显示
};

// I2C写命令
void OLED_WriteCmd(uint8_t cmd) 
{
    I2C_Start();
    I2C_SendByte(0x78);  // OLED地址，写模式
    I2C_WaitAck();
    I2C_SendByte(0x00);  // 写命令
    I2C_WaitAck();
    I2C_SendByte(cmd);
    I2C_WaitAck();
    I2C_Stop();
}

// I2C写数据
void OLED_WriteData(uint8_t data) 
{
    I2C_Start();
    I2C_SendByte(0x78);  // OLED地址，写模式
    I2C_WaitAck();
    I2C_SendByte(0x40);  // 写数据
    I2C_WaitAck();
    I2C_SendByte(data);
    I2C_WaitAck();
    I2C_Stop();
}

// 设置显示位置
void OLED_SetPos(uint8_t x, uint8_t y) 
{
    if (x > 127 || y > 7) return;
    
    OLED_WriteCmd(0xB0 + y);                     // 设置页地址
    OLED_WriteCmd(((x & 0xF0) >> 4) | 0x10);    // 设置列地址高4位
    OLED_WriteCmd(x & 0x0F);                    // 设置列地址低4位
}

// OLED初始化
void OLED_Init(void) 
{
    uint8_t i;
    
    // 延时等待OLED上电稳定
    for(i = 0; i < 100; i++);
    
    // 发送初始化命令序列
    for(i = 0; i < sizeof(OLED_INIT_CMD); i++) 
	{
        OLED_WriteCmd(OLED_INIT_CMD[i]);
    }
    
    // 清屏
    OLED_Clear();
    
    // 刷新显示
    OLED_Refresh();
}

// 清屏
void OLED_Clear(void) 
{
    uint8_t i, j;
    for(i = 0; i < 8; i++) 
	{
        for(j = 0; j < 128; j++) 
		{
            OLED_GRAM[i][j] = 0x00;
        }
    }
}

// 刷新显示（将显存内容发送到OLED）
void OLED_Refresh(void) 
{
    uint8_t i, j;
    for(i = 0; i < 8; i++) 
	{
        OLED_SetPos(0, i);
        for(j = 0; j < 128; j++) 
		{
            OLED_WriteData(OLED_GRAM[i][j]);
        }
    }
}

// 画点
void OLED_DrawPoint(uint8_t x, uint8_t y, Color color) 
{
    uint8_t page, bit;
    
    if(x >= 128 || y >= 64) return;
    
    page = y / 8;      // 计算页
    bit = y % 8;       // 计算位
    
    if(color == COLOR_WHITE) 
	{
        OLED_GRAM[page][x] |= (1 << bit);
    } else 
	{
        OLED_GRAM[page][x] &= ~(1 << bit);
    }
}

// 画线（Bresenham算法）
void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, Color color) 
	{
    int16_t dx, dy, sx, sy, err, e2;
    
    dx = (x2 > x1) ? (x2 - x1) : (x1 - x2);
    dy = (y2 > y1) ? (y2 - y1) : (y1 - y2);
    sx = (x2 > x1) ? 1 : -1;
    sy = (y2 > y1) ? 1 : -1;
    err = dx - dy;
    
    while(1) 
	{
        OLED_DrawPoint(x1, y1, color);
        if(x1 == x2 && y1 == y2) break;
        
        e2 = err * 2;
        if(e2 > -dy) 
		{
            err -= dy;
            x1 += sx;
        }
        if(e2 < dx) 
		{
            err += dx;
            y1 += sy;
        }
    }
}

// 画矩形
void OLED_DrawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, Color color) 
{
    OLED_DrawLine(x1, y1, x2, y1, color);  // 上边
    OLED_DrawLine(x1, y2, x2, y2, color);  // 下边
    OLED_DrawLine(x1, y1, x1, y2, color);  // 左边
    OLED_DrawLine(x2, y1, x2, y2, color);  // 右边
}

// 填充区域
void OLED_Fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, Color color) 
{
    uint8_t x, y;
    
    if(x1 > x2) { x = x1; x1 = x2; x2 = x; }
    if(y1 > y2) { y = y1; y1 = y2; y2 = y; }
    
    for(y = y1; y <= y2; y++) 
	{
        for(x = x1; x <= x2; x++) 
		{
            OLED_DrawPoint(x, y, color);
        }
    }
}

// 显示一个字符
void OLED_ShowChar(uint8_t x, uint8_t y, char ch, FontSize size, Color color) 
{
    uint8_t i, j, temp;
    const uint8_t *font_ptr = NULL;
    uint8_t font_width = 0, font_height = 0;
    
    // 选择字体
    switch(size) 
	{
        case FONT_SIZE_6x8:
            if(ch < 32 || ch > 127) return;
            font_ptr = &Font6x8[(ch - 32) * 6];
            font_width = 6;
            font_height = 8;
            break;
            
        case FONT_SIZE_8x16:
            if(ch < 32 || ch > 127) return;
            font_ptr = &Font8x16[(ch - 32) * 16];
            font_width = 8;
            font_height = 16;
            break;
            
        default:
            return;
    }
    
    // 显示字符
    for(i = 0; i < font_width; i++) 
	{
        temp = font_ptr[i];
        for(j = 0; j < font_height; j++) 
		{
            if(temp & 0x01) 
			{
                OLED_DrawPoint(x + i, y + j, color);
            } else if(color == COLOR_BLACK) 
			{
                // 如果需要黑色，则绘制黑点
                OLED_DrawPoint(x + i, y + j, COLOR_BLACK);
            }
            temp >>= 1;
        }
    }
}

// 显示字符串
void OLED_ShowString(uint8_t x, uint8_t y, const char *str, FontSize size, Color color) 
{
    uint8_t font_width = (size == FONT_SIZE_6x8) ? 6 : 8;
    
    while(*str) 
	{
        OLED_ShowChar(x, y, *str, size, color);
        x += font_width;
        
        // 换行处理
        if(x > 128 - font_width) 
		{
            x = 0;
            y += (size == FONT_SIZE_6x8) ? 8 : 16;
        }
        
        str++;
    }
}

// 显示数字 - 修改后的版本（不调用OLED_Pow）
void OLED_ShowNumber(uint8_t x, uint8_t y, uint32_t num, uint8_t len, FontSize size, Color color) 
{
    uint8_t i, temp;
    uint8_t font_width = (size == FONT_SIZE_6x8) ? 6 : 8;
    uint32_t divisor = 1;
    
    // 计算10的(len-1)次幂
    for(i = 1; i < len; i++) 
	{
        divisor *= 10;
    }
    
    // 显示数字
    for(i = 0; i < len; i++) 
	{
        temp = (num / divisor) % 10;
        OLED_ShowChar(x + i * font_width, y, temp + '0', size, color);
        divisor /= 10;
    }
}

// 显示图像 - 修改后的版本（删除未使用的变量）
void OLED_ShowImage(const uint8_t *image, uint8_t x, uint8_t y, uint8_t width, uint8_t height) 
{
    uint8_t i, j;
    
    for(j = 0; j < height; j++) 
	{
        for(i = 0; i < width; i++) 
		{
            if(image[j * width + i]) 
			{
                OLED_DrawPoint(x + i, y + j, COLOR_WHITE);
            }
        }
    }
}
