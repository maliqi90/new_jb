#include "gpio_ctrl.h"
#include "include.h"
volatile uint32_t systickcount = 0;
uint8_t faultled_flag = 0;//���ϵ������ε��л���־λ
volatile uint8_t faultled_on = 0;//���ϵ������־λ
volatile uint16_t key_ok_count = 0; //ȷ�ϼ�������ʱ
volatile uint16_t key_up_count = 0; //�ϼ�������ʱ
volatile uint16_t key_down_count = 0;//�¼�������ʱ
volatile uint16_t key_fire_count = 0;//�𾯼�������ʱ
volatile uint16_t key_selience_count  = 0;//������������ʱ
volatile uint16_t key_shutdown_count = 0;//�ػ�������ʱ
volatile uint16_t key_reset_count = 0;
volatile uint8_t WinKeyValue = 0; //������ֵ
volatile uint8_t key_flag = 0;//�������±��λ
volatile uint8_t powernum = 0;//
volatile uint8_t power_off_flag = 0;//�ػ���־λ
volatile uint8_t power_count = 0;//�ػ������ʱ
volatile static uint8_t power_sw_flag = 0;
extern  uint8_t power_bat_flag;//�����������־λ
/****************************************************
�������ƣ�ADC_Init
��������:ADC���������Ƿ���
****************************************************/
void adc_init(void)
{
    ADC_InitTypeDef  ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
     RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#if 0
    /*ADC����*/
    adc_initstruct.ADC_Mode = ADC_Mode_Independent;
    adc_initstruct.ADC_ScanConvMode = DISABLE;
    adc_initstruct.ADC_ContinuousConvMode = DISABLE;
    adc_initstruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    adc_initstruct.ADC_DataAlign = ADC_DataAlign_Right;
    adc_initstruct.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1,&adc_initstruct);
    
    ADC_RegularChannelConfig(ADC1,ADC_Channel_5,1,ADC_SampleTime_55Cycles5);
    ADC_Cmd(ADC1,ENABLE);
    
    /**����ADC1��У׼�Ĵ���*/
    ADC_ResetCalibration(ADC1);
    
    /*���ADC1����У׼�Ĵ�����״̬��ֱ��У׼�Ĵ����������*/
    while(ADC_GetResetCalibrationStatus(ADC1));
    
    /*��ʼADC1У׼*/
    ADC_StartCalibration(ADC1);
    /*���ADC1У׼�Ľ�����־��ֱ��У׼���*/
    while(ADC_GetCalibrationStatus(ADC1));
    
#endif
  /* ADC1 Configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel14 configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_13Cycles5);

  /* Configure high and low analog watchdog thresholds */
  ADC_AnalogWatchdogThresholdsConfig(ADC1, 4095, 2668);
  /* Configure channel14 as the single analog watchdog guarded channel */
  ADC_AnalogWatchdogSingleChannelConfig(ADC1, ADC_Channel_5);
  /* Enable analog watchdog on one regular channel */
  ADC_AnalogWatchdogCmd(ADC1, ADC_AnalogWatchdog_SingleRegEnable);

  /* Enable AWD interrupt */
  ADC_ITConfig(ADC1, ADC_IT_AWD, ENABLE);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 reset calibration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));

  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    
}

/***********************************************
�������ƣ�ADC_Check
���ܣ�    ��ѹ���
��������

*************************************************/
uint32_t ADC_Check(void)
{
    uint32_t ResultVolt = 0;
    uint8_t i = 0;
    for(i = 0;i < 8;i++)
    {
        ADC_SoftwareStartConvCmd(ADC1,ENABLE);//����ת�����ȴ�ת�����
        while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
        ResultVolt += (uint32_t)ADC_GetConversionValue(ADC1);
    }
    ResultVolt = ResultVolt >> 3;
    ResultVolt = (ResultVolt*3300) >> 12;
    return ResultVolt;
}
void Key_Scan(void);
/*****************************************
    �������ƣ�GPIO_Ctrl_Init
		����    ��������GPIO��ʼ��
******************************************/
void GPIO_Ctrl_Init(void)
{
	uint8_t send_buff[7];
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	 /********************ʹ��GPIOʱ��******************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE);
	
/*************************LED��ʼ��********************************************/
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    //�𾯵ƿ��Ƴ�ʼ��
  GPIO_InitStructure.GPIO_Pin = LED_ALARM_PIN;
  GPIO_Init(LED_ALARM_CTR, &GPIO_InitStructure);
    //���ϵƿ��Ƴ�ʼ��
  GPIO_InitStructure.GPIO_Pin = LED_FAULT_PIN;
  GPIO_Init(LED_FAULT_CTR, &GPIO_InitStructure);
    //�Լ�ƿ��Ƴ�ʼ��
  GPIO_InitStructure.GPIO_Pin = LED_SELFTEST_PIN;
  GPIO_Init(LED_SELFTEST_CTR, &GPIO_InitStructure);
    //����ƿ��Ƴ�ʼ
  GPIO_InitStructure.GPIO_Pin = LED_POWER_PIN;
  GPIO_Init(LED_POWER_CTR, &GPIO_InitStructure);
    //����ƿ��Ƴ�ʼ
  GPIO_InitStructure.GPIO_Pin = LED_BAT_PIN;
  GPIO_Init(LED_BAT_CTR, &GPIO_InitStructure);   
/***********************LED���Ƴ�ʼ��********************************************/   

/***********************KEY��ʼ��*********************************************************/
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  //��������ʼ��
  GPIO_InitStructure.GPIO_Pin = KEY_TEST_PIN;
  GPIO_Init(KEY_TEST_CTR, &GPIO_InitStructure);
  //�ϼ���ʼ��
  GPIO_InitStructure.GPIO_Pin = KEY_UP_PIN;
  GPIO_Init(KEY_UP_CTR, &GPIO_InitStructure);
  //�¼���ʼ��
  GPIO_InitStructure.GPIO_Pin = KEY_DOWN_PIN;
  GPIO_Init(KEY_DOWN_CTR, &GPIO_InitStructure);
  //�𾯼���ʼ��
  GPIO_InitStructure.GPIO_Pin = KEY_FIRE_PIN;
  GPIO_Init(KEY_FIRE_CTR, &GPIO_InitStructure);
  //ACK����ʼ��
  GPIO_InitStructure.GPIO_Pin = KEY_ACK_PIN;
  GPIO_Init(KEY_ACK_CTR, &GPIO_InitStructure);
  
/***********************KEY��ʼ��********************************************/
/***********************GPIOCTR��ʼ��**************************************************/
 //����оƬʹ�����ų�ʼ��
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = AUDIO_CTR_PIN;
  GPIO_Init(AUDIO_CTR, &GPIO_InitStructure);
  //AUDIOѡ�����ų�ʼ��
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = AUDIO_SEL_PIN;
  GPIO_Init(AUDIO_SEL_CTR, &GPIO_InitStructure);
/***********************GPIOCTR��ʼ��**************************************************/

/***************************�̵�����ʼ��*********************************************/
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = RELAY_PIN;
  GPIO_Init(RELAY_CTR, &GPIO_InitStructure);
/***************************�̵�����ʼ��**********************************************/

/****************************GPIOIN************************************************************/
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//  GPIO_InitStructure.GPIO_Pin = GPIO_IN_PIN;
//  GPIO_Init(GPIO_IN_CTR, &GPIO_InitStructure);
/****************************GPIOIN************************************************************/

/*****************************LED_STA*******************************************************************/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Pin = LED1_STA_PIN;
  GPIO_Init(LED1_STA_CTR, &GPIO_InitStructure);
  
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Pin = LED2_STA_PIN;
  GPIO_Init(LED2_STA_CTR, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Pin = LED3_STA_PIN;
  GPIO_Init(LED3_STA_CTR, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Pin = LED4_STA_PIN;
  GPIO_Init(LED4_STA_CTR, &GPIO_InitStructure);
/*****************************LED_STA********************************************************************/

/******************************VBAT_ONOFF******************************************************************************/
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  GPIO_InitStructure.GPIO_Pin = VBAT_ONOFF_PIN;
//  GPIO_Init(VBAT_ONOFF_CTR, &GPIO_InitStructure);
/******************************VBAT_ONOFF******************************************************************************/
/**********************************VBAT_KEY***************************************************************************/
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = VBAT_KEY_PIN;
  GPIO_Init(VBAT_KEY_CTR, &GPIO_InitStructure);
/***********************************VABT_KEY**************************************************************************/

/************************************PWR_KEY****************************************************************************************/
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = PWR_KEY_PIN;
  GPIO_Init(PWR_KEY_CTR, &GPIO_InitStructure);
/************************************PWR_KEY****************************************************************************************/
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  //GPIO_InitStructure.GPIO_Pin = PWR_KEY_PIN;
  //GPIO_Init(PWR_KEY_CTR, &GPIO_InitStructure);
  
  // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
 // GPIO_Init(GPIOB, &GPIO_InitStructure);
	
/****************************GPRS��Դ���Ƴ�ʼ��****************************************/
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = V4_GPRS_PIN;
  GPIO_Init(V4_GPRS_CTR, &GPIO_InitStructure);
/****************************GPRS��Դ���Ƴ�ʼ��****************************************/
/****************************�������л�����****************************************/
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = VCC_EN_PIN;
  GPIO_Init(VCC_EN_CTR, &GPIO_InitStructure);
/****************************�������л�����****************************************/
/******************************GPRS״̬�������ų�ʼ��******************************************************/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPRS_STA_PIN;
  GPIO_Init(GPRS_STA_CTR, &GPIO_InitStructure);
/*******************************GPRS״̬�������ų�ʼ��*****************************************************/


/********************************�����ѹ�������******************************************************/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Pin = ADC_VOUT_PIN;
  GPIO_Init(ADC_VOUT_CTR, &GPIO_InitStructure);
/*********************************�����ѹ�������******************************************************/

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  LED_ALARM_ON;
  LED_FAULT_ON;
  LED_SELTFTTEST_ON;
  LED_BAT_ON;
  LED_POWER_ON;

    TIMDelay_Nms(500);
    TIMDelay_Nms(500);
    key_flag = 0;
    LED_ALARM_OFF;
    LED_FAULT_OFF ;
    LED_SELTFTTEST_OFF;
   // LED_BAT_OFF;
    LED_POWER_OFF;

//�ⲿ�ж�12��ʼ��
	AFIO->EXTICR[3] = 3;//PD12
// EXTI->FTSR |= 1 << 12;

	if((bool)GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12))
	{
		power_bat_flag = 0;
		 Led_Ctrl(LEDPOWER_ON);
		VCC_EN_ON;
	  EXTI->FTSR |= 1 << 12; //�½��ش���		
	}
	else
	{
		 power_bat_flag = 1;
		 send_buff[1] = EVENT_TYPE_POWER_FAULT;
     send_buff[2] = 230;
     send_buff[3] = 0xFE;
     send_buff[4] = 0;
     send_buff[5] = 0;
     send_buff[6] = 0;
     Loop_Revice(send_buff);
		 Led_Ctrl(LEDPOWER_OFF);
		 VCC_EN_OFF;
		 EXTI->RTSR |= 1 << 12; //�����ش���	
	}
	EXTI->IMR  |=  1 << 12; //�����ж�����
	EXTI->EMR  |= 1<<12;

  // NVIC->IP[EXTI15_10_IRQn] = 0x00;
  // NVIC->ISER[EXTI15_10_IRQn >> 0x05] = (u32)0x01 << (EXTI15_10_IRQn & (u8)0x1F);


  // Enable and set EXTI0 Interrupt to the lowest priority 
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}

/*************************************************
�������ƣ�Led_Ctrl
�������ܣ�����LED����/��
**************************************************/
void Led_Ctrl(uint8_t Type)
{
    switch(Type)
    {
        case LEDARAM_ON: //�𾯵Ƶ���
             LED_ALARM_ON;            
            break;
        case LEDARAM_OFF: //�𾯵���
            LED_ALARM_OFF;
            break;
        case LEDSHIELD_ON://���ε���
            faultled_flag = 1;
            systickcount = 0;
            break;
        case LEDSHIELD_OFF://���ε���
            faultled_flag = 0;
            break;
        case LEDFAULT_ON://���ϵ���
            LED_FAULT_ON;
            break;
        case LEDFAULT_OFF://���ϵ���
            LED_FAULT_OFF;
            break;
        case LEDBAT_ON://�������
            LED_BAT_ON;
            break;
        case LEDBAT_OFF://�������
            LED_BAT_OFF;
            break;
        case LEDPOWER_ON://�������
            LED_POWER_ON;
            break;
        case LEDPOWER_OFF://�������
            LED_POWER_OFF;
            break;
        case LEDSELF_ON://�Լ����
            LED_SELTFTTEST_ON;
            break;
        case LEDSELF_OFF://�Լ����
            LED_SELTFTTEST_OFF;
            break;
//        case 
        default :
            break;
    }
}
/*******************************************************************************
 �� �� ��    ��TIM5_CONFIG
 ������������ʱ��5���ã�������ͨ�ļ�ʱ1mS
 ��ڲ�������
 ���ڲ������� 
 ����ֵ	    ����
********************************************************************************/
void TIM5_CONFIG(void)
{	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//ʹ��APB1ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	
	TIM_TimeBaseStructure.TIM_Prescaler = 72;//TIM2_PRESCALER_VALUE;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 1000;//TIM2_PERIOD_TIMING;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);

    
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	/*ʹ��Ԥ��Ƶֵ*/
	TIM_ARRPreloadConfig(TIM5,ENABLE);
    TIM_ClearFlag(TIM5, TIM_FLAG_Update);
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE); 
    TIM_Cmd(TIM5, ENABLE);
}
void TIM5_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
        Key_Scan();
      Rev_Istr();
      Rev2_Istr();
//       if(power_bat_flag == 1)
//			 {
//         read_power();			 
//			 }

//        power_off();
  }

}
/*****************************����ɨ��**************************************************/
void Key_Scan(void)
{
//    bool key1,key2,key3,key4,key5;
//    key1 = KEY_ACK_DAT;
//    key2 = KEY_UP_DAT;
//    key3 = KEY_DOWN_DAT;
//    key4 = KEY_TEST_DAT;
//    key5 = KEY_FIRE_DAT;
//    if((key1&&key2&&key3&&key4&&key5)&&(key_ok_count==0))//&(KEY_DOWN)&(KEY_SLIENCE)&(KEY_FIRE))
//    {
//         
//        return;  //û�а�������ֱ�ӷ���
//    }
    if((!KEY_UP_DAT)&&(!KEY_ACK_DAT))
    {
        key_shutdown_count++;
        return;
    }
    else
    {
         if(key_shutdown_count >= KEYCOUNTSHUTDOWN)
        {
            if(!key_flag)
            {
                key_shutdown_count = 0;
                key_flag = 1;
                WinKeyValue = KEY_SHUTDOWN;//�ж�ȷ�ϼ�����
                return;
            }
        }
        else
        {
          key_shutdown_count = 0;
        }          
    }
    
    if((!KEY_UP_DAT)&&(!KEY_DOWN_DAT))
    {
        key_reset_count++;
        return;
    }
    else 
    {
         if(key_reset_count >= KEYCOUNTRESET)
        {
            if(!key_flag)
            {
                key_reset_count = 0;
                key_flag = 1;
                WinKeyValue = KEY_RESET;//�ж�ȷ�ϼ�����
            }
        }
        else
        {
          key_reset_count = 0;
        }       
    }
    //ȷ��������ȥ
    if(!KEY_ACK_DAT)
    {
        key_ok_count++;

    }
    else
    {
         if(key_ok_count >= KEYCOUNTMAX)
        {
            if(!key_flag)
            {
                key_ok_count = 0;
                key_flag = 1;
                WinKeyValue = KEY_OK;//�ж�ȷ�ϼ�����
            }
        }
        else
        {
          key_ok_count = 0;
        }
    }
    //�ϼ�����ȥ
    if(!KEY_UP_DAT)
    {
       key_up_count++; 

    }
    else
    {
        if(key_up_count >= KEYCOUNTMAX)
        {
            if(!key_flag)
            {
                key_up_count = 0;
                key_flag = 1;
                WinKeyValue = KEY_UP;
            }
        }
        else
        {
            key_up_count = 0;   
        }

    }
    //�¼�����ȥ
    if(!KEY_DOWN_DAT)
    {
        key_down_count++;

    }
    else
    {
         if(key_down_count >= KEYCOUNTMAX)
        {
            if(!key_flag)
            {
                key_down_count = 0;
                key_flag = 1;
                WinKeyValue = KEY_DOWN;
            }
        }
        else
        {
          key_down_count = 0;            
        }

    }
    //�𾯼�����
    if(!KEY_FIRE_DAT)
    {
        key_fire_count++;

    }
    else
    {
        if(key_fire_count >= KEYCOUNTMAX)
        {
            if(!key_flag)
            {
                key_fire_count = 0;
                key_flag = 1;
                WinKeyValue = KEY_FIRE;
            }
        }
        else
        {
          key_fire_count = 0;
        }
    }
    //����������
    if(!KEY_TEST_DAT)
    {
        key_selience_count++;

        
    }
    else
    {
         if(key_selience_count >= KEYCOUNTMAX)
        {
            if(!key_flag)
            {
                key_selience_count = 0;
                key_flag = 1;
                WinKeyValue = KEY_SLIENCE; 
            }
        }
        else
        {
           key_selience_count = 0;          
        }

    }
//    if((!KEY_DOWN_DAT)&&(!KEY_UP_DAT))
//    {
//        key_shutdown_count++;
//    }
//    else
//    {
//        if(key_shutdown_count >= KEYCOUNTFIRE)
//        {
//             VBAT_OFF;
//             power_off_flag = 1;
//        }
//    }
}
/******************************����ɨ��*************************************************/

//
void power_off(void)
{
    if(power_off_flag == 1)
    {
           if(power_count < 50)
           {
             VBAT_KEY_ON;
             power_count++;
           }
           else
           {
               VBAT_KEY_OFF;
               power_count++;
               if(power_count > 100)
               {
                   power_count = 0;
               }
           }
    }
}
/***************************************************************************
�������ƣ�read_power(void)
�������ܣ���ȡ��ص���
*****************************************************************************/
void read_power(void)
{
	  uint8_t send_buff[7];
    uint8_t led1sta = 0,led2sta = 0,led3sta = 0,led4sta = 0;
    led1sta = LED1_STA;
    led2sta = LED2_STA;
    led3sta = LED3_STA;
    led4sta = LED4_STA;
//    uint32_t power_num = 0;
	 if(ADC_VOUT_IN)
	 {		 
			if(led1sta)
			{
				 powernum |= 0x08;
					//LED_BAT_ON;
			}
			else
			{
				 powernum &= 0x07;
				 // LED_BAT_OFF;
			}
			if(led2sta)
			{
					// LED_POWER_ON;
					 powernum |= 0x04;
			}
			else
			{
				 // LED_POWER_OFF;
					 powernum &= 0x0B;
			}
			if(led3sta)
			{
					//LED_SELTFTTEST_ON;
				 powernum |= 0x02; 
			}
			else
			{
				 // LED_SELTFTTEST_OFF;
				 powernum &= 0x0D;
			}
			if(led4sta)
			{
				 // LED_FAULT_ON;
				 powernum |= 0x01; 
			}
			else
			{
					//LED_FAULT_OFF;
				 powernum &= 0x0E;
			}
			  send_buff[1] = EVENT_TYPE_BATPOWER_FAULT_RE;
        send_buff[2] = 230;
        send_buff[3] = 0xFE;
        send_buff[4] = 0;
        send_buff[5] = 0;
        send_buff[6] = 0;
        Loop_Revice(send_buff);	
	}
	 else 
	 {
		 powernum = 0;
		 //�������
        send_buff[1] = EVENT_TYPE_BATPOWER_FAULT;
        send_buff[2] = 230;
        send_buff[3] = 0xFE;
        send_buff[4] = 0;
        send_buff[5] = 0;
        send_buff[6] = 0;
        Loop_Revice(send_buff);			 
	 }

    
    
}


/**
  * @brief  18V��������ж�����
  * @param  None
  * @retval None
  */
void Loop_18V_Curr_EXTI1_Config(void)
{
    EXTI_InitTypeDef   EXTI_InitStructure;
    GPIO_InitTypeDef   GPIO_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure;
  /* Enable GPIOE clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
  
  /* Configure PE.03 pin as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* Enable AFIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

  /* Connect EXTI0 Line to PE.03 pin */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource1);

  /* Configure EXTI1 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI0 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
/**
  *@breif 18V��·�����ж�����
  *
  *
*/
void Loop_18V_Volt_EXTI0_Config(void)
{
    EXTI_InitTypeDef   EXTI_InitStructure;
    GPIO_InitTypeDef   GPIO_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure;
  /* Enable GPIOE clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
  
  /* Configure PE.03 pin as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* Enable AFIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

  /* Connect EXTI0 Line to PE.03 pin */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource0);

  /* Configure EXTI1 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI0 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);	
}
//��������ж�
void EXTI15_10_IRQHandler(void)
{
	uint8_t send_buff[7];
	EXTI->PR = (1<<12);
	//������
	if((bool)GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12))
	{
		power_bat_flag = 0;
		 EXTI->FTSR |= 1 << 12; //�½��ش���		
     VCC_EN_ON;
     send_buff[1] = EVENT_TYPE_POWER_FAULT_RESUME;
     send_buff[2] = 230;
     send_buff[3] = 0xFE;
     send_buff[4] = 0;
     send_buff[5] = 0;
     send_buff[6] = 0;
     Loop_Revice(send_buff);
		Led_Ctrl(LEDPOWER_ON);//�������
	}
	else  //�½���
	{
			power_bat_flag = 1;
		  Led_Ctrl(LEDPOWER_OFF);//�������
			EXTI->RTSR |= 1 << 12; //�����ش���	
      GPIO_WriteBit(VBAT_KEY_CTR,VBAT_KEY_PIN,Bit_SET);
      TIMDelay_Nms(40);
      GPIO_WriteBit(VBAT_KEY_CTR,VBAT_KEY_PIN,Bit_RESET);

      VCC_EN_OFF;	

      send_buff[1] = EVENT_TYPE_POWER_FAULT;
      send_buff[2] = 230;
      send_buff[3] = 0xFE;
      send_buff[4] = 0;
      send_buff[5] = 0;
      send_buff[6] = 0;
      Loop_Revice(send_buff);		
	}
	
	
	
}
