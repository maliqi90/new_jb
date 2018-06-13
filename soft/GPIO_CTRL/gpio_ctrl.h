#ifndef _GPIO_CTRL_H
#define _GPIO_CTRL_H
#include "include.h"
#define KEYCOUNTMAX          20 //按键消抖时间
#define KEYCOUNTFIRE         1000 //火警键消抖时间
#define KEYCOUNTRESET        2000 // 复位键计时
#define KEYCOUNTSHUTDOWN     2000 //关机
#define KEY_NULL             0 //没有按键按下
#define KEY_OK               1 //确认键
#define KEY_UP               2 //向上键
#define KEY_DOWN             3 //向下键
#define KEY_SLIENCE          4 //消音键
#define KEY_FIRE             5 //火警键
#define KEY_MUL              6 //复合键
#define KEY_RESET            6//复位

#define KEY_SHUTDOWN         7//关机

#define LED_ALARM_CTR         GPIOB//GPIOD //火警灯GPIO
#define LED_ALARM_PIN         GPIO_Pin_1 //GPIO_Pin_6//火警灯控制引脚

#define LED_FAULT_CTR         GPIOB //故障灯GPIO
#define LED_FAULT_PIN         GPIO_Pin_0//故障灯控制引脚

#define LED_SELFTEST_CTR      GPIOC//GPIOD //自检灯控制GPIO
#define LED_SELFTEST_PIN      GPIO_Pin_5//GPIO_Pin_2//自检灯控制引脚

#define LED_POWER_CTR         GPIOC//GPIOA //主电灯控制GPIO
#define LED_POWER_PIN         GPIO_Pin_4//GPIO_Pin_9 //主电灯控制引脚

#define LED_BAT_CTR           GPIOA//GPIOB//备电灯控制GPIO
#define LED_BAT_PIN           GPIO_Pin_7//GPIO_Pin_11 //备电灯控制引脚

#define RELAY_CTR             GPIOE //继电器控制GPIO
#define RELAY_PIN             GPIO_Pin_5//继电器控制引脚

#define AUDIO_SEL_CTR         GPIOE//GPIOC //语音输出控制GPIO
#define AUDIO_SEL_PIN         GPIO_Pin_6//GPIO_Pin_1//语音输出控制引脚

#define AUDIO_CTR             GPIOC//GPIOC //功放芯片使能
#define AUDIO_CTR_PIN         GPIO_Pin_13//GPIO_Pin_0 //功放芯片使能控制引脚

#define KEY_FIRE_CTR          GPIOC//GPIOB //火警键GPIO
#define KEY_FIRE_PIN          GPIO_Pin_2//GPIO_Pin_0//火警键控制引脚

#define KEY_TEST_CTR          GPIOC//GPIOA //消音键GPIO
#define KEY_TEST_PIN          GPIO_Pin_0//GPIO_Pin_7//消音键引脚

#define KEY_DOWN_CTR          GPIOC //下键GPIO
#define KEY_DOWN_PIN          GPIO_Pin_1//GPIO_Pin_5//下键引脚

#define KEY_UP_CTR            GPIOC//GPIOA //上键GPIO
#define KEY_UP_PIN            GPIO_Pin_3//GPIO_Pin_6//上键引脚

#define KEY_ACK_CTR           GPIOA//GPIOC //ACK键GPIO
#define KEY_ACK_PIN           GPIO_Pin_0//GPIO_Pin_4

#define VBAT_ONOFF_CTR        GPIOC//GPIOB //电池打开和关闭控制
#define VBAT_ONOFF_PIN        GPIO_Pin_8//GPIO_Pin_14

#define GPIO_IN_CTR           GPIOC//GPIOE //GPIO输入
#define GPIO_IN_PIN           GPIO_Pin_7//GPIO_Pin_6


#define LED1_STA_CTR          GPIOA //LED1_STA GPIO
#define LED1_STA_PIN          GPIO_Pin_10

#define LED2_STA_CTR          GPIOA //LED2_STA GPIO
#define LED2_STA_PIN          GPIO_Pin_9

#define LED3_STA_CTR          GPIOA //LED3_STA GPIO
#define LED3_STA_PIN          GPIO_Pin_8

#define LED4_STA_CTR          GPIOC //LED4_STA GPIO
#define LED4_STA_PIN          GPIO_Pin_9

#define VBAT_KEY_CTR          GPIOA//GPIOB //充电芯片控制KEY
#define VBAT_KEY_PIN          GPIO_Pin_1//GPIO_Pin_11

#define PWR_KEY_CTR           GPIOD //GPRS开关机按键
#define PWR_KEY_PIN           GPIO_Pin_2

#define VCC_EN_CTR            GPIOC
#define VCC_EN_PIN            GPIO_Pin_8

#define V4_GPRS_CTR           GPIOB
#define V4_GPRS_PIN           GPIO_Pin_10

#define GPRS_STA_CTR          GPIOC
#define GPRS_STA_PIN          GPIO_Pin_12

#define ADC_VOUT_CTR          GPIOA    //备电电压检测引脚
#define ADC_VOUT_PIN          GPIO_Pin_15

#define ADC_5V_CTR            GPIOD
/******************************************************************/
/******************************************************************/


#define GPRS_STA_IN           (bool)GPIO_ReadInputDataBit(GPRS_STA_CTR,GPRS_STA_PIN)
	
#define ADC_VOUT_IN            (bool)GPIO_ReadInputDataBit(GPRS_STA_CTR,GPRS_STA_PIN)
/*********************GPRS控制电源引脚**************************/
#define V4_GPRS_ON    GPIO_WriteBit(V4_GPRS_CTR,V4_GPRS_PIN,Bit_SET)
#define V4_GPRS_OFF   GPIO_WriteBit(V4_GPRS_CTR,V4_GPRS_PIN,Bit_RESET)
/*********************GPRS控制电源引脚**************************/

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
/**********************************主备电切换控制*********************************************************************************************/
#define VCC_EN_ON         GPIO_WriteBit(VCC_EN_CTR,VCC_EN_PIN,Bit_SET)
#define VCC_EN_OFF        GPIO_WriteBit(VCC_EN_CTR,VCC_EN_PIN,Bit_RESET)
/***********************************主备电切换控制******************************************************************************/
#define LEDARAM_ON         0 // 火警灯亮
#define LEDARAM_OFF        1 //火警灯灭
#define LEDSHIELD_ON       2 //屏蔽灯亮
#define LEDSHIELD_OFF      3 //屏蔽灯灭
#define LEDFAULT_ON        4 //故障灯亮
#define LEDFAULT_OFF       5 //故障灯灭
#define LEDBAT_ON          6 //备电灯亮
#define LEDBAT_OFF         7 //备电灯灭
#define LEDPOWER_ON        8 //主电灯灭
#define LEDPOWER_OFF       9 //主电灯灭
#define LEDSELF_ON         10 //自检灯亮
#define LEDSELF_OFF        11 //自检灯灭
extern volatile uint8_t WinKeyValue;
extern void GPIO_Ctrl_Init(void);
extern void Led_Ctrl(uint8_t Type);
extern void TIM5_CONFIG(void);
extern volatile uint8_t key_flag;//按键按下标记位
extern void adc_init(void);
extern uint32_t ADC_Check(void);
extern void read_power(void);
extern void power_off(void);
extern volatile uint8_t powernum;
extern void Loop_18V_Curr_EXTI3_Config(void);
#endif

