#include <string.h>
#include "loop_hw.h"
#include "init.h"
#include "gpio_ctrl.h"

/*
********************************************************************************
*|<-起始位->|<--------------器件地址8位------------------->|<------命令字4位------>|<-P->|<--回码PW0~PWn (n<=5)-->..|
*____      __    __    __    __    __    __    __    __    __    __    __    __    __    __    __      __
*    |    |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |__|  |____|  |____......
*    |____|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|
*       0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29
* 上面的数字是回路发码顺序指示
********************************************************************************
*/
#define	LOOP_SHORT_TIME		40     //LOOP_SHORT_TIME*50mS

loop_hw loop_val;


/**************************************************************************
 函 数 名    ：loop_send
 功能描述：回路发0V
 入口参数：channel->回路号
 出口参数：无
 返回值      ：无
**************************************************************************/
void loop_send_0V(u8 channel)
{
	if (channel == 0)
	{
//		GPIOB->BRR  = (1 << 12) | (1 << 13) | (1 << 14);
//		GPIOB->BSRR = 1 << 14; 
	}
	else
	{
//		GPIOB->BRR  = (1 << 5) | (1 << 6) | (1 << 7);
//		GPIOB->BSRR = 1 << 7;
        LOOP_HIGH_CTR->BRR |=LOOP_HIGH_PIN;
        LOOP_ZERO_CTR->BRR |=LOOP_ZERO_PIN;
        LOOP_FIVE_CTR->BRR |=LOOP_FIVE_PIN;
        
        LOOP_ZERO_CTR->BSRR |=LOOP_ZERO_PIN;
        
	}	
}

/**************************************************************************
 函 数 名    ：loop_send
 功能描述：回路发5V
 入口参数：channel->回路号
 出口参数：无
 返回值      ：无
**************************************************************************/
void loop_send_5V(u8 channel)
{
	if (channel == 0)
	{
//		GPIOB->BRR  = (1 << 12) | (1 << 13) | (1 << 14);
//		GPIOB->BSRR = 1 << 12;	
	}
	else
	{
//		GPIOB->BRR  = (1 << 5) | (1 << 6) | (1 << 7);
//		GPIOB->BSRR = 1 << 5;
        LOOP_HIGH_CTR->BRR |=LOOP_HIGH_PIN;
        LOOP_ZERO_CTR->BRR |=LOOP_ZERO_PIN;
        LOOP_FIVE_CTR->BRR |=LOOP_FIVE_PIN;
        
        LOOP_FIVE_CTR->BSRR |=LOOP_FIVE_PIN;
	}
}

/**************************************************************************
 函 数 名    ：loop_send_24V
 功能描述：回路发24V
 入口参数：channel->回路号
 出口参数：无
 返回值      ：无
**************************************************************************/
void loop_send_24V(u8 channel)
{
	if (channel == 0)
	{
//		GPIOB->BRR  = (1 << 12) | (1 << 13) | (1 << 14);
//		GPIOB->BSRR = 1 << 13;
	}
	else
	{
        LOOP_HIGH_CTR->BRR |=LOOP_HIGH_PIN;
        LOOP_ZERO_CTR->BRR |=LOOP_ZERO_PIN;
        LOOP_FIVE_CTR->BRR |=LOOP_FIVE_PIN;
        
        LOOP_HIGH_CTR->BSRR |= LOOP_HIGH_PIN;
//		GPIOB->BRR  = (1 << 5) | (1 << 6) | (1 << 7);
//		GPIOB->BSRR = 1 << 6;
	}
}

#if 0
/**************************************************************************
 函 数 名    ：loop_send
 功能描述：回路发0V
 入口参数：channel->回路号
 出口参数：无
 返回值      ：无
**************************************************************************/
void loop_send_0V(u8 channel)
{
	if (loop_val.sht[channel] == TRUE)
	{		
		if (channel == 0)
		{
			GPIOB->BRR	= (1 << 12) | (1 << 13) | (1 << 14);
		}
		else
		{
			GPIOB->BRR	= (1 << 5) | (1 << 6) | (1 << 7);
		}
	}
	else
	{
		if (channel == 0)
		{
			GPIOB->BRR  = (1 << 12) | (1 << 13) | (1 << 14);
			GPIOB->BSRR = 1 << 14; 
		}
		else
		{
			GPIOB->BRR  = (1 << 5) | (1 << 6) | (1 << 7);
			GPIOB->BSRR = 1 << 7;
		}
	}
}

/**************************************************************************
 函 数 名    ：loop_send
 功能描述：回路发5V
 入口参数：channel->回路号
 出口参数：无
 返回值      ：无
**************************************************************************/
void loop_send_5V(u8 channel)
{
	if (loop_val.sht[channel] == TRUE)
	{		
		if (channel == 0)
		{
			GPIOB->BRR	= (1 << 12) | (1 << 13) | (1 << 14);
		}
		else
		{
			GPIOB->BRR	= (1 << 5) | (1 << 6) | (1 << 7);
		}
	}
	else
	{
		if (channel == 0)
		{
			GPIOB->BRR  = (1 << 12) | (1 << 13) | (1 << 14);
			GPIOB->BSRR = 1 << 12;	
		}
		else
		{
			GPIOB->BRR  = (1 << 5) | (1 << 6) | (1 << 7);
			GPIOB->BSRR = 1 << 5;
		}
	}
}

/**************************************************************************
 函 数 名    ：loop_send_24V
 功能描述：回路发24V
 入口参数：channel->回路号
 出口参数：无
 返回值      ：无
**************************************************************************/
void loop_send_24V(u8 channel)
{
	if (loop_val.sht[channel] == TRUE)
	{		
		if (channel == 0)
		{
			GPIOB->BRR	= (1 << 12) | (1 << 13) | (1 << 14);
		}
		else
		{
			GPIOB->BRR	= (1 << 5) | (1 << 6) | (1 << 7);
		}
	}
	else
	{
		if (channel == 0)
		{
			GPIOB->BRR  = (1 << 12) | (1 << 13) | (1 << 14);
			GPIOB->BSRR = 1 << 13;
		}
		else
		{
			GPIOB->BRR  = (1 << 5) | (1 << 6) | (1 << 7);
			GPIOB->BSRR = 1 << 6;
		}
	}
}
#endif

/**************************************************************************
 函 数 名    ：loop_clear_24V
 功能描述：回路取消发24V
 入口参数：channel->回路号
 出口参数：无
 返回值      ：无
**************************************************************************/
void loop_clear_24V(u8 channel)
{	
	if (channel == 0)
	{
//		GPIOB->BRR  = (1 << 12) | (1 << 13) | (1 << 14);
	}
	else
	{
        LOOP_HIGH_CTR->BRR |=LOOP_HIGH_PIN;
        LOOP_ZERO_CTR->BRR |=LOOP_ZERO_PIN;
        LOOP_FIVE_CTR->BRR |=LOOP_FIVE_PIN;
//		GPIOB->BRR  = (1 << 5) | (1 << 6) | (1 << 7);
	}
}

/**************************************************************************
 函 数 名    ：loop_power_enable
 功能描述：回路输出电源使能
 入口参数：channel->回路号
 出口参数：无
 返回值      ：无
**************************************************************************/
void loop_power_enable(u8 channel)
{
     GPIO_WriteBit(LOOP_PWR_CTR,LOOP_PWR_PIN,Bit_SET);
//	if (channel == CHANNEL_1)
//	{
//		GPIOA->BSRR |=0x01;
//	}
//	else
//	{
////		GPIOA->BRR = 1 << 5;
//	}
}

/**************************************************************************
 函 数 名    ：loop_power_disable
 功能描述：回路输出电源不使能
 入口参数：channel->回路号
 出口参数：无
 返回值      ：无
**************************************************************************/
void loop_power_disable(u8 channel)
{
   GPIO_WriteBit(LOOP_PWR_CTR,LOOP_PWR_PIN,Bit_RESET);
//	if (channel == CHANNEL_1)
//	{
//		GPIOA->BSRR = 1 << 0;
//	}
//	else
//	{
////		GPIOA->BSRR = 1 << 5;
//	}
}

/**************************************************************************
 函 数 名    ：get_parity
 功能描述：获取校验位
 入口参数：无
 出口参数：无
 返回值      ：校验位值
**************************************************************************/
u8 get_parity(void)
{
	u8 i;
	u8 p = 0;
	
	for (i = 0; i < 8; i++)
	{
		if (loop_val.addr& (1 << i))
			p++;
	}

	for (i = 0; i < 4; i++)
	{
		if (loop_val.cmd & (1 << i))
			p++;
	}

	return (p & 1);
}

/**************************************************************************
 函 数 名    ：TIM1_UP_IRQHandler
 功能描述：定时器1中断程序，发码和回码超时
 入口参数：无
 出口参数：无
 返回值      ：无
**************************************************************************/
void TIM1_UP_IRQHandler(void)				//发码和回码超时
{
	if ((TIM1->DIER & (1 << 0)) && (TIM1->SR & (1 << 0)))
	{
		TIM1->SR &= ~(1 << 0);

		if (!(TIM1->CR1 & (1 << 0)))
			return;
			
		if (loop_val.state == STATE_SEND)
		{
			loop_val.send_cnt++;
			
			if (loop_val.send_cnt % 2)						//奇数发24v
			{
				loop_send_24V(loop_val.channel);

				if (loop_val.send_cnt == 27)				//发送收码前 24V，后面发发5v收码
				{
					TIM1->ARR = TIME_2P5MS;					
				}
			}
			else											//偶数发数据
			{
				if (loop_val.send_cnt < 17)					//发送地址
				{
					if (loop_val.send_cnt == 2)
					{
						if (loop_val.addr & (1 << 7))
							loop_send_5V(loop_val.channel);
						else
							loop_send_0V(loop_val.channel);
					}
					else
					{
						if (loop_val.addr & (1 << ((loop_val.send_cnt - 4) / 2)))
							loop_send_5V(loop_val.channel);
						else
							loop_send_0V(loop_val.channel);
					}
				}
				else if (loop_val.send_cnt < 25)			//发送控制位
				{
					if (loop_val.cmd & (1 << ((loop_val.send_cnt - 18) / 2)))
						loop_send_5V(loop_val.channel);
					else
						loop_send_0V(loop_val.channel);
				}
				else if (loop_val.send_cnt < 27)			//发送校验位
				{
					if (get_parity())
						loop_send_5V(loop_val.channel);
					else
						loop_send_0V(loop_val.channel);
				}
				else if (loop_val.send_cnt == 28)
				{
					loop_send_5V(loop_val.channel);
					loop_val.state = STATE_RECEIVE;
				}
			}
		}		
		else if (loop_val.state == STATE_RECEIVE)			//回码超时
		{
			loop_send_24V(loop_val.channel);
			loop_val.state = STATE_END;
			TIM1->CR1 &= ~(1 << 0);
		}
		else if (loop_val.state == STATE_REC_IEV1)			//回码后发334uS 24V
		{
			loop_send_24V(loop_val.channel);
			
			if (loop_val.rec_cnt == loop_val.len)
			{
				loop_val.state = STATE_END;
				TIM1->CR1 &= ~(1 << 0);
			}
			else
			{			
				TIM1->ARR  = TIME_2P5MS;
				loop_val.state = STATE_REC_IEV2;
			}
		}
		else if (loop_val.state == STATE_REC_IEV2)
		{
			loop_send_5V(loop_val.channel);
			loop_val.state = STATE_RECEIVE;
		}
	}
}

/**************************************************************************
 函 数 名    ：TIM4_IRQHandler
 功能描述：定时器4中断程序，回路2回码检测
 入口参数：无
 出口参数：无
 返回值      ：无
**************************************************************************/
void TIM4_IRQHandler(void)
{	
	static u16 tim1, tim2; 
	u16 tim;
	
	if (TIM4->SR & (1 << 2)) //输入捕获2中断标志
	{
		TIM4->SR &= ~(1 << 2);

		if (loop_val.channel == CHANNEL_2 && loop_val.state == STATE_RECEIVE)
		{
			delay_nus(100);
			
			if (loop_val.edge == 0)				//回码下降沿
			{
				if (LOOP_ANS_CTR->IDR&LOOP_ANS_PIN)
				{
					return;
				}
				else
				{					
					tim1 = TIM4->CCR2;
					loop_val.edge = 1;
					TIM4->CCER &= ~(1 << 5);//设置输入捕获2极性
                    loop_val.edge = 1;
				}
			}
			else								//回码上升沿
			{
				if (LOOP_ANS_CTR->IDR & (LOOP_ANS_PIN))
				{
					tim2 = TIM4->CCR2;
					loop_val.edge = 0;
					TIM4->CCER |= 1 << 5;

					if (tim2 > tim1)
						tim = tim2 - tim1;
					else
						tim = 10000 - tim1 + tim2;

					if (tim < TIME_INVALID)			//回码过短无效
					{
						return;
					}
					
					loop_val.pw[loop_val.rec_cnt] = tim;
					loop_val.rec_cnt++;

					loop_val.state = STATE_REC_IEV1;
					TIM1->CNT      = TIME_250US;
					TIM1->ARR      = TIME_334US;
				}
				else
				{
					return;
				}
			}
		}
	}			
}

/**************************************************************************
 函 数 名    ：TIM3_IRQHandler
 功能描述：定时器1中断程序，回路1回码检测
 入口参数：无
 出口参数：无
 返回值      ：无
**************************************************************************/
void TIM3_IRQHandler(void)
{	
	static u16 tim1, tim2;
	u16 tim;
	
	if (TIM3->SR & (1 << 2))
	{
		TIM3->SR &= ~(1 << 2);

		if (loop_val.channel == CHANNEL_1 && loop_val.state == STATE_RECEIVE)
		{
			delay_nus(100);
			
			if (loop_val.edge == 0) 			//回码下降沿
			{				
				if (GPIOA->IDR & (1 << 7))
				{
					tim++;
					return;
				}
				else
				{					
					tim1 = TIM3->CCR2;
					loop_val.edge = 1;
					TIM3->CCER &= ~(1 << 5);
				}
			}
			else								//回码上升沿
			{
				if (GPIOA->IDR & (1 << 7))
				{
					tim2 = TIM3->CCR2;
					loop_val.edge = 0;
					TIM3->CCER |= 1 << 5;

					if (tim2 > tim1)
						tim = tim2 - tim1;
					else
						tim = 10000 - tim1 + tim2;

					if (tim < TIME_INVALID)			//回码过短无效
					{
						tim++;
						return;
					}
					
					loop_val.pw[loop_val.rec_cnt] = tim;	
					loop_val.rec_cnt++;

					loop_val.state = STATE_REC_IEV1;
					TIM1->CNT	   = TIME_250US;
					TIM1->ARR	   = TIME_334US;
				}
				else
				{
					tim++;
					return;
				}
			}
		}
	}			
}


/**************************************************************************
 函 数 名    ：TIM2_IRQHandler
 功能描述：定时器2中断程序计时短路时间(50mS中断)
 入口参数：无
 出口参数：无
 返回值      ：无
**************************************************************************/
void TIM2_IRQHandler(void)
{	
	static u16 loop1_cnt = 0;
	static u16 loop2_cnt = 0;
	
	if (TIM2->SR & (1 << 0))
	{
		TIM2->SR &= ~(1 << 0);

		if (loop_val.sht[CHANNEL_1] == TRUE)
		{
			if (GPIOB->IDR & (1 << 15))
			{
				if (loop1_cnt <= LOOP_SHORT_TIME)
					loop1_cnt++;

				if (loop1_cnt == LOOP_SHORT_TIME)
				{
					if (loop_val.short_callback != NULL)
						loop_val.short_callback(CHANNEL_1);	
						
					loop_power_disable(CHANNEL_1);
				}
			}
			else
			{
				loop1_cnt = 0;
				loop_val.sht[CHANNEL_1] = FALSE;
			}
		}

		if (loop_val.sht[CHANNEL_2] == TRUE)
		{
			if ((LOOP_SHORT_CRT->IDR & (LOOP_SHORT_PIN)) == 0)
			{
				if (loop2_cnt <= LOOP_SHORT_TIME)
					loop2_cnt++;

				if (loop2_cnt == LOOP_SHORT_TIME)
				{
					if (loop_val.short_callback != NULL)
						loop_val.short_callback(CHANNEL_2);	
						
					loop_power_disable(CHANNEL_2);
				}
			}
			else
			{
				loop2_cnt = 0;
				loop_val.sht[CHANNEL_2] = FALSE;
			}
		}
	}	
}

/**************************************************************************
 函 数 名    ：EXTI15_10_IRQHandler
 功能描述：外部中断15，回路1过载
 入口参数：无
 出口参数：无
 返回值      ：无
**************************************************************************/
/*void EXTI15_10_IRQHandler(void)
{
	if (EXTI->PR & (1 << 15))
	{
		EXTI->PR &= 1 << 15;

//		TIM1->CR1 &= ~(1 << 0);

//		loop_clear_24V(CHANNEL_1);
		
		loop_val.sht[CHANNEL_1] = TRUE;
	}						   
}
*/
/**************************************************************************
 函 数 名    ：EXTI1_IRQHandler
 功能描述：外部中断9，回路2过载
 入口参数：无
 出口参数：无
 返回值      ：无
**************************************************************************/
/*void EXTI1_IRQHandler(void)
{
	if (EXTI->PR & (1 << 1))
	{
		EXTI->PR &= 1 << 1;
		
//		TIM1->CR1 &= ~(1 << 0);

//		loop_clear_24V(CHANNEL_2);
		
		loop_val.sht[CHANNEL_2] = TRUE;
	}
}*/

/**************************************************************************
 函 数 名    ：loop_power_on
 功能描述：回路上电
 入口参数：channel->回路号
 出口参数：无
 返回值      ：无
**************************************************************************/
void loop_power_on(u8 channel)
{
//	u32 i, j;
//	#define LOOP_CHARGE_PWM_HIGH 200

	loop_val.sht[channel] = FALSE;
	loop_power_enable(channel);

	// 依次增加脉宽//8ms*10
/*	for (i = 10; i < LOOP_CHARGE_PWM_HIGH; i++)
	{
		for (j = 0; j < LOOP_CHARGE_PWM_HIGH; j++)	 //100*150us = 15s
		{
//			loop_clear_24V(channel);
			delay_nus(LOOP_CHARGE_PWM_HIGH - i);
			loop_send_24V(channel);
			delay_nus(i);
			
			if(loop_val.sht[channel] == TRUE)
			{
//				loop_clear_24V(channel);
				break;
			}
		}

		feed_dog();
	}*/
	
	loop_send_24V(channel);
//	delay_nms(1000);
//	feed_dog();
}

/**************************************************************************
 函 数 名    ：loop_send
 功能描述：回路发码
 入口参数：channel->回路号 addr->设备地址cmd->控制命令
 				   rec_len->期望接收长度
 出口参数：无
 返回值      ：无
**************************************************************************/
void loop_send(u8 channel, u8 addr, u8 cmd, u8 rec_len)
{
	loop_val.channel = channel;
	loop_val.addr	 = addr;
	loop_val.cmd	 = cmd;
	loop_val.len	 = rec_len;

	loop_val.send_cnt= 0;
	loop_val.rec_cnt = 0;
	loop_val.edge    = 0;
	loop_val.state   = STATE_SEND;
	loop_val.pw[0]   = 0;
	loop_val.pw[1]   = 0;
	loop_val.pw[2]   = 0;
	loop_val.pw[3]   = 0;
	loop_val.pw[4]   = 0;

	TIM1->CNT		 = TIME_668US;
//	TIM1->ARR 		 = TIME_668US;
	TIM1->SR		&= ~(1 << 0);
	TIM1->CR1 		|= 1;				//启动定时器
	
	loop_send_0V(loop_val.channel);
	
	TIM1->ARR 		 = TIME_334US;

	if (channel == CHANNEL_1)
	{
//		TIM3->CCER |= 1 << 5;
	}
	else
	{
		TIM4->CCER |= 1 << 5; 
	}	
}

/**************************************************************************
 函 数 名    ：loop_answer
 功能描述：取回回码数据
 入口参数：无	
 出口参数：回码数据
 返回值      ：回码长度
**************************************************************************/
u8 loop_answer(u32 pw[5])
{
	if (pw != NULL)
		memcpy(pw, loop_val.pw, 20);
	loop_val.state = STATE_IDLE;
	
	return loop_val.rec_cnt;
}

/**************************************************************************
 函 数 名    ：allow_loop_send
 功能描述：允许回路发送
 入口参数：无	
 出口参数：无
 返回值      ：TURE->可以发送FALSE->禁止发送
**************************************************************************/
BOOL allow_loop_send(void)
{
	return loop_val.state == STATE_IDLE;
}

/**************************************************************************
 函 数 名    ：allow_loop_receive
 功能描述：允许回路接收
 入口参数：无	
 出口参数：无
 返回值      ：TURE->可以接收FALSE->禁止接收
**************************************************************************/
BOOL allow_loop_receive(void)
{
	return loop_val.state == STATE_END;
}

/**************************************************************************
 函 数 名    ：register_loop_short_callback
 功能描述：注册回路过载回调函数
 入口参数：channel->回路号call_back->过载处理函数
 出口参数：无
 返回值      ：无
**************************************************************************/
void register_loop_short_callback(void (*call_back)(u8 channel))
{
	loop_val.short_callback = call_back;
}


