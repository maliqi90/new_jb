#include "delay.h"

/*******************************************
   �������ƣ�TIMER2_Init
	 ��    �ܣ�TIMER2��ʼ��
	 
********************************************/
void TIMER6_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	//ʹ��APB2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	
	TIM_TimeBaseStructure.TIM_Prescaler = 72;//TIM2_PRESCALER_VALUE;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 10;//TIM2_PERIOD_TIMING;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseStructure);
	
	/*ʹ��Ԥ��Ƶֵ*/
	TIM_ARRPreloadConfig(TIM6,ENABLE);
}
/*******************************************************
  �������ƣ�TIMDelay_N10us
  ��    �ܣ���ʱ����ʱN��10us
  ��    ����Times --- Nֵ
  ����ֵ  ����
********************************************************/
void TIMDelay_N10us(uint16_t Times)
{
	TIM6->CR1 |= TIM_CR1_CEN;             //������ʱ��
	while(Times--)
	{
		while((TIM6->SR & TIM_FLAG_Update) == RESET);
		TIM6->SR &= ~TIM_FLAG_Update;       //�����־
		
	}
	TIM6->CR1 &= ~TIM_CR1_CEN;            //�رն�ʱ��
}
/************************************************************
       �������ƣ�TIMDelay_Nms
       ��    �ܣ���ʱ����ʱNms
       ��    ����Times----Nֵ
       ����ֵ  ����
*************************************************************/
void TIMDelay_Nms(uint16_t Times)
{
	while(Times--)
	{
		TIMDelay_N10us(100);
	}
}

