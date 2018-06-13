#ifndef _LCDDRV_H_
#define _LCDDRV_H_
#include <stm32f10x.h>

//д����
#define LCD_CMD  (*(__IO uint16_t  *)  0x60000000)
//д����
#define LCD_DAT  (*(__IO uint16_t  *)  0x60020000)
//��ɫ����
#define LCD_WHITE  0xFFFF  // ��
#define LCD_BLACK  0x0000  // ��
#define LCD_RED    0xF800  // ��
#define LCD_ORANGE 0xFBE4  // ��
#define LCD_YELLOW 0xFFE0  // ��
#define LCD_GREEN  0x07E0  // ��
#define LCD_BLUE   0x001F  // ��
#define LCD_INDIGO 0x051D  // ��
#define LCD_PURPLE 0x3A39  // ��
//������ʾ���� 0������ʾ 1������ʾ
#define DISPLAY_ORIENTATION_X 1
//�ض���Ĵ�����ַ
#define FSMC_BCR1   (*(__IO uint32_t  *)0xA0000000)
#define FSMC_BCR2   (*(__IO uint32_t  *)0xA0000008)
#define FSMC_BCR3   (*(__IO uint32_t  *)0xA0000010)
#define FSMC_BCR4   (*(__IO uint32_t  *)0xA0000018)
//�ض���Ĵ�����ַ                                               
#define FSMC_BTR1   (*(__IO uint32_t  *)0xA0000004)
#define FSMC_BTR2   (*(__IO uint32_t  *)0xA000000C)
#define FSMC_BTR3   (*(__IO uint32_t  *)0xA0000014)
#define FSMC_BTR4   (*(__IO uint32_t  *)0xA000001C)
//�ض���Ĵ�����ַ                                           
#define FSMC_BWTR1  (*(__IO uint32_t  *)0xA0000104)
#define FSMC_BWTR2  (*(__IO uint32_t  *)0xA000010C)
#define FSMC_BWTR3  (*(__IO uint32_t  *)0xA0000114)
#define FSMC_BWTR4  (*(__IO uint32_t  *)0xA000011C)
//��������
void LCD_Init(void);
void LCD_Clear(u16 color);
void LCD_Point(u16 x,u16 y,u16 color);	
void PWM_Set(uint16_t pwm);

#endif

