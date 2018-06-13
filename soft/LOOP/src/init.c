#include "stm32f10x.h"
//#include "SysTick.h"
#include "init.h"
#include "stm32f10x_exti.h"
#include "gpio_ctrl.h"
/*******************************************************************************
 �� �� ��    ��RCC_CONFIG
 ����������ʹ������ʱ��
 ��ڲ�������
 ���ڲ�������
����ֵ	    ����
********************************************************************************/
void RCC_CONFIG(void)
{
	RCC->APB2ENR = RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN |
				   RCC_APB2ENR_TIM1EN | RCC_APB2ENR_AFIOEN |
                   RCC_APB2ENR_IOPEEN;
				   
	RCC->APB1ENR = RCC_APB1ENR_TIM2EN;


	RCC->AHBENR  = 0;
}
//void Loop_18V_Curr_EXTI1_Config(void)
//{
//    EXTI_InitTypeDef   EXTI_InitStructure;
//    GPIO_InitTypeDef   GPIO_InitStructure;
//    NVIC_InitTypeDef   NVIC_InitStructure;
//  /* Enable GPIOE clock */
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
//  
//  /* Configure PE.03 pin as input floating */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_Init(GPIOE, &GPIO_InitStructure);

//  /* Enable AFIO clock */
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

//  /* Connect EXTI0 Line to PE.01 pin */
//  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource1);

//  /* Configure EXTI1 line */
//  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);

//  /* Enable and set EXTI0 Interrupt to the lowest priority */
//  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);	
//}
/*******************************************************************************
 �� �� ��    ��GPIO_CONFIG
 ����������GPIO ����
 ��ڲ�������
 ���ڲ�������
����ֵ	    ����
********************************************************************************/
void GPIO_CONFIG(void)
{ 
//	GPIOA->CRL = GPIO_CRL_CNF0_0 |
//				 GPIO_CRL_CNF1_0 |
//				 GPIO_CRL_MODE2  | GPIO_CRL_CNF2_1 |
//				 GPIO_CRL_CNF3_0 |
//				 GPIO_CRL_MODE4  |
//				 GPIO_CRL_MODE5  |
//				 GPIO_CRL_CNF6_0 |
//				 GPIO_CRL_CNF7_0;
//				 
//	GPIOA->CRH = GPIO_CRH_CNF8_0  |
//				 GPIO_CRH_MODE9   | GPIO_CRH_CNF9_1  |
//				 GPIO_CRH_CNF10_0 |
//				 GPIO_CRH_CNF11_0 |
//				 GPIO_CRH_MODE12  | GPIO_CRH_CNF12_1;

//	GPIOB->CRL = GPIO_CRL_CNF0_0 |
//				 GPIO_CRL_CNF1_0 |
//			     GPIO_CRL_MODE5  |
//			     GPIO_CRL_MODE6  |
//			     GPIO_CRL_MODE7;
//			     
//	GPIOB->CRH = GPIO_CRH_CNF9_0  |
//				 GPIO_CRH_CNF10_0 |
//				 GPIO_CRH_CNF11_0 |
//			     GPIO_CRH_MODE12  |
//				 GPIO_CRH_MODE13  |
//				 GPIO_CRH_MODE14  |
//				 GPIO_CRH_CNF15_0;

//	GPIOA->ODR |= 1 << 11;
     GPIO_InitTypeDef GPIO_InitStructure;
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE,ENABLE);

     //7.5V��������
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
     GPIO_InitStructure.GPIO_Pin = LOOP_FIVE_PIN;
     GPIO_Init(LOOP_FIVE_CTR, &GPIO_InitStructure);
     
     //18V�������� 
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
     GPIO_InitStructure.GPIO_Pin = LOOP_HIGH_PIN;
     GPIO_Init(LOOP_HIGH_CTR, &GPIO_InitStructure);
     
     //0V��������
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
     GPIO_InitStructure.GPIO_Pin = LOOP_ZERO_PIN;
     GPIO_Init(LOOP_ZERO_CTR, &GPIO_InitStructure);
     
     //PWR_CTR��������
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
     GPIO_InitStructure.GPIO_Pin = LOOP_PWR_PIN;
     GPIO_Init(LOOP_PWR_CTR, &GPIO_InitStructure);
     
     
     //ANS��������
//     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
     GPIO_InitStructure.GPIO_Pin = LOOP_ANS_PIN;
     GPIO_Init(LOOP_ANS_CTR, &GPIO_InitStructure);

}

/*******************************************************************************
 �� �� ��    ��USART2_CONFIG
 ����������USART2�������� (����,19200bps 1λ��ʼλ��1λ����λ��8λ����λ)
 ��ڲ�������
 ���ڲ�������
����ֵ	    ����
********************************************************************************/
void USART2_CONFIG(void)
{
	USART2->CR1 = 0;
	USART2->CR1 = 1 << 6 | 1 << 5 | 1 << 3 | 1 << 2;	
	USART2->BRR = 0x753;  		
	USART2->CR2 = 0;
	USART2->CR3 = 0;
	USART2->CR1 |= 1 << 13;	
	
	NVIC->IP[USART2_IRQn] = 0x30;		//�ж����ȼ�
	NVIC->ISER[USART2_IRQn >> 0x05] = (u32)0x01 << (USART2_IRQn & (u8)0x1F);//���ж�	
}

/*******************************************************************************
 �� �� ��    ��EXTI1_CONFIG
 �����������ⲿ�ж�����(��·2����)
 ��ڲ�������
 ���ڲ������� 
����ֵ	    ����
********************************************************************************/
void EXTI1_CONFIG(void)
{
  EXTI_InitTypeDef EXTI_InitStructure_;
//	AFIO->EXTICR[1] = 1 << 1;

//	EXTI->IMR  |=  1 << 9;
//	EXTI->FTSR |= 1 << 9;

//    NVIC->IP[EXTI9_5_IRQn] = 0x00;
//    NVIC->ISER[EXTI9_5_IRQn >> 0x05] = (u32)0x01 << (EXTI9_5_IRQn & (u8)0x1F);
    
  EXTI_ClearITPendingBit(EXTI_Line1);
  EXTI_InitStructure_.EXTI_Line = EXTI_Line1; 
  EXTI_InitStructure_.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure_.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure_);
    
   NVIC->IP[EXTI1_IRQn] = 0x00;
   NVIC->ISER[EXTI1_IRQn >> 0x05] = (u32)0x01 << (EXTI1_IRQn & (u8)0x1F);
}

/*******************************************************************************
 �� �� ��    ��EXTI15_CONFIG
 �����������ⲿ�ж�����(��·1����)
 ��ڲ�������
 ���ڲ������� 
 ����ֵ	    ����
********************************************************************************/
void EXTI15_CONFIG(void)
{
	AFIO->EXTICR[3] = 1 << 12;

	EXTI->IMR  |=  1 << 15;
	EXTI->FTSR |= 1 << 15;

    NVIC->IP[EXTI15_10_IRQn] = 0x00;
    NVIC->ISER[EXTI15_10_IRQn >> 0x05] = (u32)0x01 << (EXTI15_10_IRQn & (u8)0x1F);
}

/*******************************************************************************
 �� �� ��    ��TIM1_CONFIG
 ������������ʱ��1���ã�������ͻ��볬ʱ
 ��ڲ�������
 ���ڲ������� 
 ����ֵ	    ����
********************************************************************************/
void TIM1_CONFIG(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN|RCC_APB2ENR_AFIOEN;//��ʱ��
    
	TIM1->CR1 = 0;
	
	TIM1->CR1   = (1 << 7) | (1 << 4) | (1 << 2);
	TIM1->CR2   = 0;
	TIM1->SMCR  = 0;
	TIM1->DIER  = (1 << 6) | (1 << 0);
	TIM1->CCER  = 0;
	TIM1->PSC   = SystemCoreClock / 1000000 - 1;  

	NVIC->IP[TIM1_UP_IRQn] = 0x10;
    NVIC->ISER[TIM1_UP_IRQn >> 0x05] = (u32)0x01 << (TIM1_UP_IRQn & (u8)0x1F);
}

/*******************************************************************************
 �� �� ��    ��TIM4_CONFIG
 ������������ʱ��4���ã���������·2���볤��
 ��ڲ�������
 ���ڲ������� 
 ����ֵ	    ����
********************************************************************************/
void TIM4_CONFIG(void)
{
    //AFIO->MAPR = 2<<8; //��ʱ��2��ӳ��
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;//ʹ�ܶ�ʱ��4
	TIM4->CR1 = 0;

	TIM4->ARR = 10000;  //�趨�������Զ���װֵ//�պ�1ms    
	TIM4->PSC = SystemCoreClock / 1000000 - 1;    //Ԥ��Ƶ��,

	TIM4->CCMR1 |= 01 << 8; //CC2ͨ��������Ϊ���룬IC2ӳ����TI2�ϣ�
	TIM4->CCER  |= 3 << 4; //��������/����4������Լ�ʹ��CC2

	TIM4->DIER  |= 1 << 2;   //������²����ж�2				
		   
	TIM4->CR1   |= 1 << 0;    //ʹ�ܶ�ʱ��4
	
	NVIC->IP[TIM4_IRQn] = 0x10;
    NVIC->ISER[TIM4_IRQn >> 0x05] = (u32)0x01 << (TIM4_IRQn & (u8)0x1F);
}

/*******************************************************************************
 �� �� ��    ��TIM3_CONFIG
 ������������ʱ��3���ã���������·1���볤��
 ��ڲ�������
 ���ڲ������� 
 ����ֵ	    ����
********************************************************************************/
void TIM3_CONFIG(void)
{
	TIM3->CR1 = 0;

	TIM3->ARR = 10000;  //�趨�������Զ���װֵ//�պ�1ms    
	TIM3->PSC = SystemCoreClock / 1000000 - 1;    //Ԥ��Ƶ��,

	TIM3->CCMR1 |= 1 << 8;
	TIM3->CCER  |= 3 << 4;

	TIM3->DIER  |= 1 << 2;   //������²����ж�				
		   
	TIM3->CR1   |= 1 << 0;    //ʹ�ܶ�ʱ��2
	
	NVIC->IP[TIM3_IRQn] = 0x10;
    NVIC->ISER[TIM3_IRQn >> 0x05] = (u32)0x01 << (TIM3_IRQn & (u8)0x1F);
}

/*******************************************************************************
 �� �� ��    ��TIM2_CONFIG
 ������������ʱ��2���ã�������ͨ�ļ�ʱ50mS
 ��ڲ�������
 ���ڲ������� 
 ����ֵ	    ����
********************************************************************************/
void TIM2_CONFIG(void)
{	
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->CR1 = 0;
	
	TIM2->CR1   = (1 << 7) | (1 << 4) | (1 << 2);
	TIM2->CR2   = 0;
	TIM2->SMCR  = 0;
	TIM2->DIER  = (1 << 6) | (1 << 0);
	TIM2->CCER  = 0;
	TIM2->PSC   = SystemCoreClock / 1000000 - 1;  //��Ƶ
	TIM2->CNT   = 50000;					
	TIM2->CR1  |= 1 << 0;

	NVIC->IP[TIM2_IRQn] = 0x30;
    NVIC->ISER[TIM2_IRQn >> 0x05] = (u32)0x01 << (TIM2_IRQn & (u8)0x1F);
}

/*******************************************************************************
 �� �� ��    ��CAN1_CONFIG
 ����������can����50KBPS
 ��ڲ�������
 ���ڲ������� 
 ����ֵ	    ����
********************************************************************************/
//void CAN1_CONFIG(void)
//{
//	u32 wait_ack = 0x00000000;

//	/* Exit from sleep mode */
//	CAN1->MCR &= (~(u32)CAN_MCR_SLEEP);

//	/* Request initialisation */
//	CAN1->MCR |= CAN_MCR_INRQ ;

//	/* Wait the acknowledge */
//	while (((CAN1->MSR & CAN_MSR_INAK) != CAN_MSR_INAK) && (wait_ack != 0xFFFF))
//	{
//		wait_ack++;
//	}

//	/* Check acknowledge */
//	if ((CAN1->MSR & CAN_MSR_INAK) == CAN_MSR_INAK)
//	{
//		/* Set the time triggered communication mode */
//		CAN1->MCR &= ~CAN_MCR_TTCM;

//		/* Set the automatic bus-off management */
//		//CAN1->MCR &= ~(u32)CAN_MCR_ABOM;
//		CAN1->MCR |= (u32)CAN_MCR_ABOM;

//		/* Set the automatic wake-up mode */
//		CAN1->MCR &= ~(u32)CAN_MCR_AWUM;

//		/* Set the no automatic retransmission */
//	  	CAN1->MCR &= ~(u32)CAN_MCR_NART;
//	  	//CAN1->MCR |= (u32)CAN_MCR_NART;

//		/* Set the receive FIFO locked mode */
//		CAN1->MCR &= ~(u32)CAN_MCR_RFLM;

//		/* Set the transmit FIFO priority */
//		CAN1->MCR &= ~(u32)CAN_MCR_TXFP;

//		/* Set the bit timing register */
////		CAN1->BTR = 0x3000b;// 500KBPS
//		CAN1->BTR = 0x3e0023;//  50KBPS

//		/* Request leave initialisation */
//		CAN1->MCR &= ~(u32)CAN_MCR_INRQ;

//		/* Wait the acknowledge */
//		wait_ack = 0;

//		while (((CAN1->MSR & CAN_MSR_INAK) == CAN_MSR_INAK) && (wait_ack != 0xFFFF))
//		{
//	 		wait_ack++;
//		}		

//		//�����ж�ʹ��
//		CAN1->IER = 1 << 1;
//		
//		NVIC->IP[USB_LP_CAN1_RX0_IRQn] = 0x30;
//		NVIC->ISER[USB_LP_CAN1_RX0_IRQn >> 0x05] = (u32)0x01 << (USB_LP_CAN1_RX0_IRQn & (u8)0x1F);

////		NVIC->IP[CAN1_SCE_IRQn] = 0x30;
////		NVIC->ISER[CAN1_SCE_IRQn >> 0x05] = (u32)0x01 << (CAN1_SCE_IRQn & (u8)0x1F);

////		NVIC->IP[USB_HP_CAN1_TX_IRQn] = 0x20;
////		NVIC->ISER[USB_HP_CAN1_TX_IRQn >> 0x05] = (u32)0x01 << (USB_HP_CAN1_TX_IRQn & (u8)0x1F);
//	}
//}

/*******************************************************************************
 �� �� ��    ��IWDG_CONFIG
 �����������������Ź���ʼ��
 ��ڲ�������
 ���ڲ�������
 ����ֵ	    ����
********************************************************************************/
void IWDG_CONFIG(void)
{
	IWDG->KR  = 0x5555; 
	IWDG->PR  = 0x04;		//64��Ƶ
	IWDG->RLR = 0x780;		//3s ���Ź��ж�
	IWDG->KR  = 0xCCCC;		//�������Ź�
	IWDG->KR  = 0xAAAA; 	//ι��
}


/****************************************************************************
��������LOOP_CONFIG
������������ʼ����·����
*****************************************************************************/
void LOOP_CONFIG(void)
{
   //RCC_CONFIG();
   GPIO_CONFIG();  
   EXTI1_CONFIG();
   TIM1_CONFIG(); 
   TIM2_CONFIG();
//TIM3_CONFIG();    
   TIM4_CONFIG();  
   Loop_18V_Curr_EXTI1_Config();    
}
//�������л���ʼ��
void Power_Sw_Init(void)
{
    
}
/*******************************************************************************
 �� �� ����	delay_1us
 ����������	��ʱ1us
 ��ڲ�����	��
 ���ڲ�����	�� 
����ֵ	  ����
********************************************************************************/
void delay_1us(void)
{
	u8 i = 7;
	
	while (i--);
}

/*******************************************************************************
 �� �� ����	delay_nus
 ����������	��ʱnus
 ��ڲ�����	t ��ʱus��
 ���ڲ�����	��
 ����ֵ	  ����
********************************************************************************/
void delay_nus(u16 t)
{
	while (t--)
	{
		delay_1us();
	}
}

/*******************************************************************************
 �� �� ����	delay_1ms
 ����������	��ʱ1ms
 ��ڲ�����	��
 ���ڲ�����	��
 ����ֵ	  ����
********************************************************************************/
void delay_1ms(void)
{
	u16 i;
	
	i = 8000;
	
	while (i--);
}

/*******************************************************************************
 �� �� ����	delay_nms
 ����������	��ʱnms
 ��ڲ�����	n:ms��
 ���ڲ�����	��
 ����ֵ	  ����
********************************************************************************/
void delay_nms(u16 n)
{
	while (n--)
		delay_1ms();
}

/**************************************************************************
����ԭ�ͣ�feed_dog
����������ι��
��ڲ�������
���ڲ�������
����ֵ	  ����
**************************************************************************/
void feed_dog(void)
{
	IWDG->KR  = 0xAAAA;
}

