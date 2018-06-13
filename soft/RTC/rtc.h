#ifndef _RTC_H
#define _RTC_H
#define RTCClockSource_LSE   /* �����õ�32.768K ʱ�Ӿ���Դ */
#include "include.h"
extern volatile u8 Time_interr;
//ʱ��ṹ��
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
extern volatile u8 timer_interr_flag; //���ж�ʱ�������

#define LED_ALARM_CTR         GPIOB//GPIOD //�𾯵�GPIO
#define LED_ALARM_PIN         GPIO_Pin_1//GPIO_Pin_6//�𾯵ƿ�������

#define LED_FAULT_CTR         GPIOB//GPIOD //���ϵ�GPIO
#define LED_FAULT_PIN         GPIO_Pin_0//GPIO_Pin_3//���ϵƿ�������

#define LED_ALARM_ON       GPIO_WriteBit(LED_ALARM_CTR,LED_ALARM_PIN,Bit_RESET)
#define LED_ALARM_OFF      GPIO_WriteBit(LED_ALARM_CTR,LED_ALARM_PIN,Bit_SET)

#define LED_FAULT_ON       GPIO_WriteBit(LED_FAULT_CTR,LED_FAULT_PIN,Bit_RESET)
#define LED_FAULT_OFF      GPIO_WriteBit(LED_FAULT_CTR,LED_FAULT_PIN,Bit_SET)

//void RTC_Configuration(void);
char RTC_Init(void); //RTC
void GPIO_Init1(void);
u8 time_num(void);//��ȡʱ��
u8 Is_Leap_Year(u16 year);  //�ж��Ƿ������꺯��

void HEX2ASCII(u8 hex_data,char* arr); //HEXתASCII
void RTC_Dis(void);


#endif

