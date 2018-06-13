#include <string.h>
#include "loop_hw.h"
#include "init.h"
#include "gpio_ctrl.h"

/*
********************************************************************************
*|<-��ʼλ->|<--------------������ַ8λ------------------->|<------������4λ------>|<-P->|<--����PW0~PWn (n<=5)-->..|
*____      __    __    __    __    __    __    __    __    __    __    __    __    __    __    __      __
*    |    |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |__|  |____|  |____......
*    |____|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|  |__|
*       0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29
* ����������ǻ�·����˳��ָʾ
********************************************************************************
*/
#define	LOOP_SHORT_TIME		40     //LOOP_SHORT_TIME*50mS

loop_hw loop_val;

uint8_t loop_short_flag = 0;
/**************************************************************************
 �� �� ��    ��loop_send
 ������������·��0V
 ��ڲ�����channel->��·��
 ���ڲ�������
 ����ֵ      ����
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
 �� �� ��    ��loop_send
 ������������·��5V
 ��ڲ�����channel->��·��
 ���ڲ�������
 ����ֵ      ����
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
 �� �� ��    ��loop_send_24V
 ������������·��24V
 ��ڲ�����channel->��·��
 ���ڲ�������
 ����ֵ      ����
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
 �� �� ��    ��loop_send
 ������������·��0V
 ��ڲ�����channel->��·��
 ���ڲ�������
 ����ֵ      ����
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
 �� �� ��    ��loop_send
 ������������·��5V
 ��ڲ�����channel->��·��
 ���ڲ�������
 ����ֵ      ����
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
 �� �� ��    ��loop_send_24V
 ������������·��24V
 ��ڲ�����channel->��·��
 ���ڲ�������
 ����ֵ      ����
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
 �� �� ��    ��loop_clear_24V
 ������������·ȡ����24V
 ��ڲ�����channel->��·��
 ���ڲ�������
 ����ֵ      ����
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
 �� �� ��    ��loop_power_enable
 ������������·�����Դʹ��
 ��ڲ�����channel->��·��
 ���ڲ�������
 ����ֵ      ����
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
 �� �� ��    ��loop_power_disable
 ������������·�����Դ��ʹ��
 ��ڲ�����channel->��·��
 ���ڲ�������
 ����ֵ      ����
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
 �� �� ��    ��get_parity
 ������������ȡУ��λ
 ��ڲ�������
 ���ڲ�������
 ����ֵ      ��У��λֵ
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
 �� �� ��    ��TIM1_UP_IRQHandler
 ������������ʱ��1�жϳ��򣬷���ͻ��볬ʱ
 ��ڲ�������
 ���ڲ�������
 ����ֵ      ����
**************************************************************************/
void TIM1_UP_IRQHandler(void)				//����ͻ��볬ʱ
{
	if ((TIM1->DIER & (1 << 0)) && (TIM1->SR & (1 << 0)))
	{
		TIM1->SR &= ~(1 << 0);

		if (!(TIM1->CR1 & (1 << 0)))
			return;
			
		if (loop_val.state == STATE_SEND)
		{
			loop_val.send_cnt++;
			
			if (loop_val.send_cnt % 2)						//������24v
			{
				loop_send_24V(loop_val.channel);

				if (loop_val.send_cnt == 27)				//��������ǰ 24V�����淢��5v����
				{
					TIM1->ARR = TIME_2P5MS;					
				}
			}
			else											//ż��������
			{
				if (loop_val.send_cnt < 17)					//���͵�ַ
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
				else if (loop_val.send_cnt < 25)			//���Ϳ���λ
				{
					if (loop_val.cmd & (1 << ((loop_val.send_cnt - 18) / 2)))
						loop_send_5V(loop_val.channel);
					else
						loop_send_0V(loop_val.channel);
				}
				else if (loop_val.send_cnt < 27)			//����У��λ
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
		else if (loop_val.state == STATE_RECEIVE)			//���볬ʱ
		{
			loop_send_24V(loop_val.channel);
			loop_val.state = STATE_END;
			TIM1->CR1 &= ~(1 << 0);
		}
		else if (loop_val.state == STATE_REC_IEV1)			//�����334uS 24V
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
 �� �� ��    ��TIM4_IRQHandler
 ������������ʱ��4�жϳ��򣬻�·2������
 ��ڲ�������
 ���ڲ�������
 ����ֵ      ����
**************************************************************************/
void TIM4_IRQHandler(void)
{	
	static u16 tim1, tim2; 
	u16 tim;
	
	if (TIM4->SR & (1 << 2)) //���벶��2�жϱ�־
	{
		TIM4->SR &= ~(1 << 2);

		if (loop_val.channel == CHANNEL_2 && loop_val.state == STATE_RECEIVE)
		{
			delay_nus(100);
			
			if (loop_val.edge == 0)				//�����½���
			{
				if (LOOP_ANS_CTR->IDR&LOOP_ANS_PIN)
				{
					return;
				}
				else
				{					
					tim1 = TIM4->CCR2;
					loop_val.edge = 1;
					TIM4->CCER &= ~(1 << 5);//�������벶��2����
                    loop_val.edge = 1;
				}
			}
			else								//����������
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

					if (tim < TIME_INVALID)			//���������Ч
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
 �� �� ��    ��TIM3_IRQHandler
 ������������ʱ��1�жϳ��򣬻�·1������
 ��ڲ�������
 ���ڲ�������
 ����ֵ      ����
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
			
			if (loop_val.edge == 0) 			//�����½���
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
			else								//����������
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

					if (tim < TIME_INVALID)			//���������Ч
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

void loopshort_re(void)
{
		uint8_t send_buff[7];
		send_buff[0] = 0;
		send_buff[1] = EVENT_TYPE_LOOPFAULT_RESUME;
		send_buff[2] = 230;
		send_buff[3] = 0xFE;
		send_buff[4] = 0;
		send_buff[5] = 0;
		send_buff[6] = 0;
		Loop_Revice(send_buff);
}
/**************************************************************************
 �� �� ��    ��TIM2_IRQHandler
 ������������ʱ��2�жϳ����ʱ��·ʱ��(50mS�ж�)
 ��ڲ�������
 ���ڲ�������
 ����ֵ      ����
**************************************************************************/
void TIM2_IRQHandler(void)
{	
	static u16 loop1_cnt = 0;
	static u16 loop2_cnt = 0;
	
	if (TIM2->SR & (1 << 0))
	{
		TIM2->SR &= ~(1 << 0);
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
				
				 if(loop_short_flag == 1)
				 {
					 loop_short_flag = 0;
					 loopshort_re();
					 LOOP[1].OptStatFlags.StateBit.ShortReported_flag = 0;

				 }
			}
		}
	}	
}

/**************************************************************************
 �� �� ��    ��EXTI15_10_IRQHandler
 �����������ⲿ�ж�15����·1����
 ��ڲ�������
 ���ڲ�������
 ����ֵ      ����
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
 �� �� ��    ��EXTI1_IRQHandler
 �����������ⲿ�ж�9����·2����
 ��ڲ�������
 ���ڲ�������
 ����ֵ      ����
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
 �� �� ��    ��loop_power_on
 ������������·�ϵ�
 ��ڲ�����channel->��·��
 ���ڲ�������
 ����ֵ      ����
**************************************************************************/
void loop_power_on(u8 channel)
{
//	u32 i, j;
//	#define LOOP_CHARGE_PWM_HIGH 200

	loop_val.sht[channel] = FALSE;
	loop_power_enable(channel);

	// ������������//8ms*10
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
 �� �� ��    ��loop_send
 ������������·����
 ��ڲ�����channel->��·�� addr->�豸��ַcmd->��������
 				   rec_len->�������ճ���
 ���ڲ�������
 ����ֵ      ����
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
	TIM1->CR1 		|= 1;				//������ʱ��
	
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
 �� �� ��    ��loop_answer
 ����������ȡ�ػ�������
 ��ڲ�������	
 ���ڲ�������������
 ����ֵ      �����볤��
**************************************************************************/
u8 loop_answer(u32 pw[5])
{
	if (pw != NULL)
		memcpy(pw, loop_val.pw, 20);
	loop_val.state = STATE_IDLE;
	
	return loop_val.rec_cnt;
}

/**************************************************************************
 �� �� ��    ��allow_loop_send
 ���������������·����
 ��ڲ�������	
 ���ڲ�������
 ����ֵ      ��TURE->���Է���FALSE->��ֹ����
**************************************************************************/
BOOL allow_loop_send(void)
{
	return loop_val.state == STATE_IDLE;
}

/**************************************************************************
 �� �� ��    ��allow_loop_receive
 ���������������·����
 ��ڲ�������	
 ���ڲ�������
 ����ֵ      ��TURE->���Խ���FALSE->��ֹ����
**************************************************************************/
BOOL allow_loop_receive(void)
{
	return loop_val.state == STATE_END;
}

/**************************************************************************
 �� �� ��    ��register_loop_short_callback
 ����������ע���·���ػص�����
 ��ڲ�����channel->��·��call_back->���ش�����
 ���ڲ�������
 ����ֵ      ����
**************************************************************************/
void register_loop_short_callback(void (*call_back)(u8 channel))
{
	loop_val.short_callback = call_back;
}


