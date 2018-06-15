#ifndef _GPIO_CTRL_H
#define _GPIO_CTRL_H
#include "include.h"
#define KEYCOUNTMAX          20 //��������ʱ��
#define KEYCOUNTFIRE         1000 //�𾯼�����ʱ��
#define KEYCOUNTRESET        2000 // ��λ����ʱ
#define KEYCOUNTSHUTDOWN     2000 //�ػ�
#define KEY_NULL             0 //û�а�������
#define KEY_OK               1 //ȷ�ϼ�
#define KEY_UP               2 //���ϼ�
#define KEY_DOWN             3 //���¼�
#define KEY_SLIENCE          4 //������
#define KEY_FIRE             5 //�𾯼�
#define KEY_MUL              6 //���ϼ�
#define KEY_RESET            6//��λ

#define KEY_SHUTDOWN         7//�ػ�

#define LED_ALARM_CTR         GPIOB//GPIOD //�𾯵�GPIO
#define LED_ALARM_PIN         GPIO_Pin_1 //GPIO_Pin_6//�𾯵ƿ�������

#define LED_FAULT_CTR         GPIOB //���ϵ�GPIO
#define LED_FAULT_PIN         GPIO_Pin_0//���ϵƿ�������

#define LED_SELFTEST_CTR      GPIOC//GPIOD //�Լ�ƿ���GPIO
#define LED_SELFTEST_PIN      GPIO_Pin_5//GPIO_Pin_2//�Լ�ƿ�������

#define LED_POWER_CTR         GPIOC//GPIOA //����ƿ���GPIO
#define LED_POWER_PIN         GPIO_Pin_4//GPIO_Pin_9 //����ƿ�������

#define LED_BAT_CTR           GPIOA//GPIOB//����ƿ���GPIO
#define LED_BAT_PIN           GPIO_Pin_7//GPIO_Pin_11 //����ƿ�������

#define RELAY_CTR             GPIOE //�̵�������GPIO
#define RELAY_PIN             GPIO_Pin_5//�̵�����������

#define AUDIO_SEL_CTR         GPIOE//GPIOC //�����������GPIO
#define AUDIO_SEL_PIN         GPIO_Pin_6//GPIO_Pin_1//���������������

#define AUDIO_CTR             GPIOC//GPIOC //����оƬʹ��
#define AUDIO_CTR_PIN         GPIO_Pin_13//GPIO_Pin_0 //����оƬʹ�ܿ�������

#define KEY_FIRE_CTR          GPIOC//GPIOB //�𾯼�GPIO
#define KEY_FIRE_PIN          GPIO_Pin_2//GPIO_Pin_0//�𾯼���������

#define KEY_TEST_CTR          GPIOC//GPIOA //������GPIO
#define KEY_TEST_PIN          GPIO_Pin_0//GPIO_Pin_7//����������

#define KEY_DOWN_CTR          GPIOC //�¼�GPIO
#define KEY_DOWN_PIN          GPIO_Pin_1//GPIO_Pin_5//�¼�����

#define KEY_UP_CTR            GPIOC//GPIOA //�ϼ�GPIO
#define KEY_UP_PIN            GPIO_Pin_3//GPIO_Pin_6//�ϼ�����

#define KEY_ACK_CTR           GPIOA//GPIOC //ACK��GPIO
#define KEY_ACK_PIN           GPIO_Pin_0//GPIO_Pin_4

#define VBAT_ONOFF_CTR        GPIOC//GPIOB //��ش򿪺͹رտ���
#define VBAT_ONOFF_PIN        GPIO_Pin_8//GPIO_Pin_14

#define GPIO_IN_CTR           GPIOC//GPIOE //GPIO����
#define GPIO_IN_PIN           GPIO_Pin_7//GPIO_Pin_6


#define LED1_STA_CTR          GPIOA //LED1_STA GPIO
#define LED1_STA_PIN          GPIO_Pin_10

#define LED2_STA_CTR          GPIOA //LED2_STA GPIO
#define LED2_STA_PIN          GPIO_Pin_9

#define LED3_STA_CTR          GPIOA //LED3_STA GPIO
#define LED3_STA_PIN          GPIO_Pin_8

#define LED4_STA_CTR          GPIOC //LED4_STA GPIO
#define LED4_STA_PIN          GPIO_Pin_9

#define VBAT_KEY_CTR          GPIOA//GPIOB //���оƬ����KEY
#define VBAT_KEY_PIN          GPIO_Pin_1//GPIO_Pin_11

#define PWR_KEY_CTR           GPIOD //GPRS���ػ�����
#define PWR_KEY_PIN           GPIO_Pin_2

#define VCC_EN_CTR            GPIOC
#define VCC_EN_PIN            GPIO_Pin_8

#define V4_GPRS_CTR           GPIOB
#define V4_GPRS_PIN           GPIO_Pin_10

#define GPRS_STA_CTR          GPIOC
#define GPRS_STA_PIN          GPIO_Pin_12

#define ADC_VOUT_CTR          GPIOA    //�����ѹ�������
#define ADC_VOUT_PIN          GPIO_Pin_15

#define ADC_5V_CTR            GPIOD
/******************************************************************/
/******************************************************************/


#define GPRS_STA_IN           (bool)GPIO_ReadInputDataBit(GPRS_STA_CTR,GPRS_STA_PIN)
	
#define ADC_VOUT_IN            (bool)GPIO_ReadInputDataBit(GPRS_STA_CTR,GPRS_STA_PIN)
/*********************GPRS���Ƶ�Դ����**************************/
#define V4_GPRS_ON    GPIO_WriteBit(V4_GPRS_CTR,V4_GPRS_PIN,Bit_SET)
#define V4_GPRS_OFF   GPIO_WriteBit(V4_GPRS_CTR,V4_GPRS_PIN,Bit_RESET)
/*********************GPRS���Ƶ�Դ����**************************/

/******************KEY****************************/
#define KEY_FIRE_DAT          (bool)GPIO_ReadInputDataBit(KEY_FIRE_CTR,KEY_FIRE_PIN)
#define KEY_TEST_DAT          (bool)GPIO_ReadInputDataBit(KEY_TEST_CTR,KEY_TEST_PIN)
#define KEY_DOWN_DAT          (bool)GPIO_ReadInputDataBit(KEY_DOWN_CTR,KEY_DOWN_PIN)
#define KEY_UP_DAT            (bool)GPIO_ReadInputDataBit(KEY_UP_CTR,KEY_UP_PIN)
#define KEY_ACK_DAT           (bool)GPIO_ReadInputDataBit(KEY_ACK_CTR,KEY_ACK_PIN)
/******************RELAY_CTR********************************/


/*******************************LED_STA*************************************/
#define LED1_STA             (bool)GPIO_ReadInputDataBit(LED1_STA_CTR,LED1_STA_PIN)
#define LED2_STA             (bool)GPIO_ReadInputDataBit(LED2_STA_CTR,LED2_STA_PIN)
#define LED3_STA             (bool)GPIO_ReadInputDataBit(LED3_STA_CTR,LED3_STA_PIN)
#define LED4_STA             (bool)GPIO_ReadInputDataBit(LED4_STA_CTR,LED4_STA_PIN)
/*******************************LED_STA*************************************/
/*******************************GPIO_IN********************************************************/
#define GPIO_IN_STA           (bool)GPIO_ReadInputDataBit(GPIO_IN_CTR,GPIO_IN_PIN)
/*******************************GPIO_IN********************************************************/
#define RELAY_ON    GPIO_WriteBit(RELAY_CTR,RELAY_PIN,Bit_SET)
#define RELAY_OFF   GPIO_WriteBit(RELAY_CTR,RELAY_PIN,Bit_RESET)


/*********************LED_CTR**********************************/
#define LED_ALARM_ON       GPIO_WriteBit(LED_ALARM_CTR,LED_ALARM_PIN,Bit_RESET)
#define LED_ALARM_OFF      GPIO_WriteBit(LED_ALARM_CTR,LED_ALARM_PIN,Bit_SET)

#define LED_FAULT_ON       GPIO_WriteBit(LED_FAULT_CTR,LED_FAULT_PIN,Bit_RESET)
#define LED_FAULT_OFF      GPIO_WriteBit(LED_FAULT_CTR,LED_FAULT_PIN,Bit_SET)

#define LED_SHEILD_ON     GPIO_WriteBit(LED_FAULT_CTR,LED_FAULT_PIN,(BitAction)(1-GPIO_ReadOutputDataBit(LED_FAULT_CTR, LED_FAULT_PIN)))
#define LED_SHEILD_OFF     GPIO_WriteBit(LED_FAULT_CTR,LED_FAULT_PIN,Bit_SET)


#define LED_SELTFTTEST_ON  GPIO_WriteBit(LED_SELFTEST_CTR,LED_SELFTEST_PIN,Bit_RESET)
#define LED_SELTFTTEST_OFF GPIO_WriteBit(LED_SELFTEST_CTR,LED_SELFTEST_PIN,Bit_SET)


#define LED_BAT_ON         GPIO_WriteBit(LED_BAT_CTR,LED_BAT_PIN,Bit_RESET)
#define LED_BAT_OFF        GPIO_WriteBit(LED_BAT_CTR,LED_BAT_PIN,Bit_SET)

#define LED_POWER_ON       GPIO_WriteBit(LED_POWER_CTR,LED_POWER_PIN,Bit_RESET)
#define LED_POWER_OFF      GPIO_WriteBit(LED_POWER_CTR,LED_POWER_PIN,Bit_SET)

/******************AUDIO_SECL_CTR*******************************************************/
#define AUDIO_SECL_ON      GPIO_WriteBit(AUDIO_SEL_CTR,AUDIO_SEL_PIN,Bit_SET)
#define AUDIO_SECL_OFF     GPIO_WriteBit(AUDIO_SEL_CTR,AUDIO_SEL_PIN,Bit_RESET)

/******************AUDIO_CTR*************************************************************************/
#define AUDIO_CTR_ON       GPIO_WriteBit(AUDIO_CTR,AUDIO_CTR_PIN,Bit_RESET)
#define AUDIO_CTR_OFF      GPIO_WriteBit(AUDIO_CTR,AUDIO_CTR_PIN,Bit_SET)
/*****************************************************************************************/
/********************************VBAT_KEY********************************************************/
#define VBAT_KEY_ON        GPIO_WriteBit(VBAT_KEY_CTR,VBAT_KEY_PIN,Bit_SET)
#define VBAT_KEY_OFF       GPIO_WriteBit(VBAT_KEY_CTR,VBAT_KEY_PIN,Bit_RESET)
/********************************VABT_KEY********************************************************/

/*********************************VBAT_ONOFF_CTR*****************************************************************/
#define VBAT_ON            GPIO_WriteBit(VBAT_ONOFF_CTR,VBAT_ONOFF_PIN,Bit_SET)
#define VBAT_OFF           GPIO_WriteBit(VBAT_ONOFF_CTR,VBAT_ONOFF_PIN,Bit_RESET)
/*********************************VBAT_ONOFF_CTR*****************************************************************/
/*********************************PWR_KEY_CTR*******************************************************************************/
#define PWR_KEY_ON        GPIO_WriteBit(PWR_KEY_CTR,PWR_KEY_PIN,Bit_SET)
#define PWR_KEY_OFF       GPIO_WriteBit(PWR_KEY_CTR,PWR_KEY_PIN,Bit_RESET)
/*********************************PWR_KEY_CTR********************************************************************************/
/**********************************�������л�����*********************************************************************************************/
#define VCC_EN_ON         GPIO_WriteBit(VCC_EN_CTR,VCC_EN_PIN,Bit_SET)
#define VCC_EN_OFF        GPIO_WriteBit(VCC_EN_CTR,VCC_EN_PIN,Bit_RESET)
/***********************************�������л�����******************************************************************************/
#define LEDARAM_ON         0 // �𾯵���
#define LEDARAM_OFF        1 //�𾯵���
#define LEDSHIELD_ON       2 //���ε���
#define LEDSHIELD_OFF      3 //���ε���
#define LEDFAULT_ON        4 //���ϵ���
#define LEDFAULT_OFF       5 //���ϵ���
#define LEDBAT_ON          6 //�������
#define LEDBAT_OFF         7 //�������
#define LEDPOWER_ON        8 //�������
#define LEDPOWER_OFF       9 //�������
#define LEDSELF_ON         10 //�Լ����
#define LEDSELF_OFF        11 //�Լ����
extern volatile uint8_t WinKeyValue;
extern void GPIO_Ctrl_Init(void);
extern void Led_Ctrl(uint8_t Type);
extern void TIM5_CONFIG(void);
extern volatile uint8_t key_flag;//�������±��λ
extern void adc_init(void);
extern uint32_t ADC_Check(void);
extern void read_power(void);
extern void power_off(void);
extern volatile uint8_t powernum;
extern void Loop_18V_Curr_EXTI1_Config(void);
extern uint8_t led_temp;
extern void Loop_18V_Volt_EXTI0_Config(void);
#endif

