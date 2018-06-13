#include "delay.h"

/*******************************************
   函数名称：TIMER2_Init
	 功    能：TIMER2初始化
	 
********************************************/
void TIMER6_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	//使能APB2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	
	TIM_TimeBaseStructure.TIM_Prescaler = 72;//TIM2_PRESCALER_VALUE;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 10;//TIM2_PERIOD_TIMING;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseStructure);
	
	/*使能预分频值*/
	TIM_ARRPreloadConfig(TIM6,ENABLE);
}
/*******************************************************
  函数名称：TIMDelay_N10us
  功    能：定时器延时N个10us
  参    数：Times --- N值
  返回值  ：无
********************************************************/
void TIMDelay_N10us(uint16_t Times)
{
	TIM6->CR1 |= TIM_CR1_CEN;             //启动定时器
	while(Times--)
	{
		while((TIM6->SR & TIM_FLAG_Update) == RESET);
		TIM6->SR &= ~TIM_FLAG_Update;       //清除标志
		
	}
	TIM6->CR1 &= ~TIM_CR1_CEN;            //关闭定时器
}
/************************************************************
       函数名称：TIMDelay_Nms
       功    能：定时器延时Nms
       参    数：Times----N值
       返回值  ：无
*************************************************************/
void TIMDelay_Nms(uint16_t Times)
{
	while(Times--)
	{
		TIMDelay_N10us(100);
	}
}

