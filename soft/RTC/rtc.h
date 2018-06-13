#ifndef _RTC_H
#define _RTC_H
#define RTCClockSource_LSE   /* 用外置的32.768K 时钟晶振源 */
#include "include.h"
extern volatile u8 Time_interr;
//时间结构体
typedef struct
{
	int   year;
	char  month;
	char  date;
	
	u8 hour;
	u8 min;
	u8 sec;	
}tm;


extern tm Timer_value;
extern volatile u8 timer_interr_flag; //秒中断时间计数器

#define LED_ALARM_CTR         GPIOB//GPIOD //火警灯GPIO
#define LED_ALARM_PIN         GPIO_Pin_1//GPIO_Pin_6//火警灯控制引脚

#define LED_FAULT_CTR         GPIOB//GPIOD //故障灯GPIO
#define LED_FAULT_PIN         GPIO_Pin_0//GPIO_Pin_3//故障灯控制引脚

#define LED_ALARM_ON       GPIO_WriteBit(LED_ALARM_CTR,LED_ALARM_PIN,Bit_RESET)
#define LED_ALARM_OFF      GPIO_WriteBit(LED_ALARM_CTR,LED_ALARM_PIN,Bit_SET)

#define LED_FAULT_ON       GPIO_WriteBit(LED_FAULT_CTR,LED_FAULT_PIN,Bit_RESET)
#define LED_FAULT_OFF      GPIO_WriteBit(LED_FAULT_CTR,LED_FAULT_PIN,Bit_SET)

//void RTC_Configuration(void);
char RTC_Init(void); //RTC
void GPIO_Init1(void);
u8 time_num(void);//读取时间
u8 Is_Leap_Year(u16 year);  //判断是否是闰年函数

void HEX2ASCII(u8 hex_data,char* arr); //HEX转ASCII
void RTC_Dis(void);


#endif

