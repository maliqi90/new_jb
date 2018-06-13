#ifndef _LCDDRV_H_
#define _LCDDRV_H_
#include <stm32f10x.h>

//写命令
#define LCD_CMD  (*(__IO uint16_t  *)  0x60000000)
//写数据
#define LCD_DAT  (*(__IO uint16_t  *)  0x60020000)
//颜色声明
#define LCD_WHITE  0xFFFF  // 白
#define LCD_BLACK  0x0000  // 黑
#define LCD_RED    0xF800  // 红
#define LCD_ORANGE 0xFBE4  // 橙
#define LCD_YELLOW 0xFFE0  // 黄
#define LCD_GREEN  0x07E0  // 绿
#define LCD_BLUE   0x001F  // 蓝
#define LCD_INDIGO 0x051D  // 靛
#define LCD_PURPLE 0x3A39  // 紫
//控制显示方向 0竖屏显示 1横屏显示
#define DISPLAY_ORIENTATION_X 1
//重定义寄存器地址
#define FSMC_BCR1   (*(__IO uint32_t  *)0xA0000000)
#define FSMC_BCR2   (*(__IO uint32_t  *)0xA0000008)
#define FSMC_BCR3   (*(__IO uint32_t  *)0xA0000010)
#define FSMC_BCR4   (*(__IO uint32_t  *)0xA0000018)
//重定义寄存器地址                                               
#define FSMC_BTR1   (*(__IO uint32_t  *)0xA0000004)
#define FSMC_BTR2   (*(__IO uint32_t  *)0xA000000C)
#define FSMC_BTR3   (*(__IO uint32_t  *)0xA0000014)
#define FSMC_BTR4   (*(__IO uint32_t  *)0xA000001C)
//重定义寄存器地址                                           
#define FSMC_BWTR1  (*(__IO uint32_t  *)0xA0000104)
#define FSMC_BWTR2  (*(__IO uint32_t  *)0xA000010C)
#define FSMC_BWTR3  (*(__IO uint32_t  *)0xA0000114)
#define FSMC_BWTR4  (*(__IO uint32_t  *)0xA000011C)
//函数声明
void LCD_Init(void);
void LCD_Clear(u16 color);
void LCD_Point(u16 x,u16 y,u16 color);	
void PWM_Set(uint16_t pwm);

#endif

