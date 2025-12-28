基于STM32F103C8的按键控制系统，通过两个独立按键控制OLED显示屏显示不同的文字信息


模式
第1行	        
第2行	           
第3行
欢迎界面	 
Welcome!	      
STM32F103C8	     
Press Key1/2
按键1按下	 
Key1 Pressed!
Mode 1 Active	   
LED Display
按键2按下	 
Key2 Pressed!	
Mode 2 Active	   
Text Changed
两键同时	 
Both Keys!	    
Special Mode	   
Config Menu


显示模块 I2C接口的OLED   I2C地址0x78  驱动芯片 SSD1306


模式采用了  轮询检测 状态机模式 模块化设计


文件结构

project/

├── main.c                     # 主程序文件

├── main.h                     # 主头文件（宏定义、函数声明）

├── gpio.c                     # GPIO配置和按键读取

├── i2c.c / i2c.h              # I2C总线驱动

├── oled.c / oled.h            # OLED显示驱动

├── oled_font.c / oled_font.h  # 字库文件

└── system_stm32f10x.c         # 系统时钟配置



调试手段
LED指示 可用板载LED指示程序运行状态
串口调试 预留USART接口用于调试信息输出
断点调试 Keil调试器支持硬件断点


测试流程
GPIO测试 → 按键输入是否正常
I2C测试 → OLED通信是否正常
显示测试 → 文字显示是否正确
功能测试 → 按键控制逻辑是否正确
