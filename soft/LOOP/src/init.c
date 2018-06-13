#include "stm32f10x.h"
//#include "SysTick.h"
#include "init.h"
#include "stm32f10x_exti.h"
#include "gpio_ctrl.h"
/*******************************************************************************
 函 数 名    ：RCC_CONFIG
 功能描述：使能外设时钟
 入口参数：无
 出口参数：无
返回值	    ：无
********************************************************************************/
void RCC_CONFIG(void)
{
	RCC->APB2ENR = RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN |
				   RCC_APB2ENR_TIM1EN | RCC_APB2ENR_AFIOEN |
                   RCC_APB2ENR_IOPEEN;
				   
	RCC->APB1ENR = RCC_APB1ENR_TIM2EN;


	RCC->AHBENR  = 0;
}

/*******************************************************************************
 函 数 名    ：GPIO_CONFIG
 功能描述：GPIO 配置
 入口参数：无
 出口参数：无
返回值	    ：无
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

     //7.5V控制引脚
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
     GPIO_InitStructure.GPIO_Pin = LOOP_FIVE_PIN;
     GPIO_Init(LOOP_FIVE_CTR, &GPIO_InitStructure);
     
     //18V控制引脚 
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
     GPIO_InitStructure.GPIO_Pin = LOOP_HIGH_PIN;
     GPIO_Init(LOOP_HIGH_CTR, &GPIO_InitStructure);
     
     //0V控制引脚
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
     GPIO_InitStructure.GPIO_Pin = LOOP_ZERO_PIN;
     GPIO_Init(LOOP_ZERO_CTR, &GPIO_InitStructure);
     
     //PWR_CTR控制引脚
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
     GPIO_InitStructure.GPIO_Pin = LOOP_PWR_PIN;
     GPIO_Init(LOOP_PWR_CTR, &GPIO_InitStructure);
     
     
     //ANS控制引脚
//     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
     GPIO_InitStructure.GPIO_Pin = LOOP_ANS_PIN;
     GPIO_Init(LOOP_ANS_CTR, &GPIO_InitStructure);

}

/*******************************************************************************
 函 数 名    ：USART2_CONFIG
 功能描述：USART2引脚配置 (串口,19200bps 1位开始位，1位结束位，8位数据位)
 入口参数：无
 出口参数：无
返回值	    ：无
********************************************************************************/
void USART2_CONFIG(void)
{
	USART2->CR1 = 0;
	USART2->CR1 = 1 << 6 | 1 << 5 | 1 << 3 | 1 << 2;	
	USART2->BRR = 0x753;  		
	USART2->CR2 = 0;
	USART2->CR3 = 0;
	USART2->CR1 |= 1 << 13;	
	
	NVIC->IP[USART2_IRQn] = 0x30;		//中断优先级
	NVIC->ISER[USART2_IRQn >> 0x05] = (u32)0x01 << (USART2_IRQn & (u8)0x1F);//开中断	
}

/*******************************************************************************
 函 数 名    ：EXTI1_CONFIG
 功能描述：外部中断配置(回路2过载)
 入口参数：无
 出口参数：无 
返回值	    ：无
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
 函 数 名    ：EXTI15_CONFIG
 功能描述：外部中断配置(回路1过载)
 入口参数：无
 出口参数：无 
 返回值	    ：无
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
 函 数 名    ：TIM1_CONFIG
 功能描述：定时器1配置，负责发码和回码超时
 入口参数：无
 出口参数：无 
 返回值	    ：无
********************************************************************************/
void TIM1_CONFIG(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN|RCC_APB2ENR_AFIOEN;//打开时钟
    
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
 函 数 名    ：TIM4_CONFIG
 功能描述：定时器4配置，负责计算回路2回码长度
 入口参数：无
 出口参数：无 
 返回值	    ：无
********************************************************************************/
void TIM4_CONFIG(void)
{
    //AFIO->MAPR = 2<<8; //定时器2重映射
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;//使能定时器4
	TIM4->CR1 = 0;

	TIM4->ARR = 10000;  //设定计数器自动重装值//刚好1ms    
	TIM4->PSC = SystemCoreClock / 1000000 - 1;    //预分频器,

	TIM4->CCMR1 |= 01 << 8; //CC2通道被配置为输入，IC2映射在TI2上；
	TIM4->CCER  |= 3 << 4; //配置输入/捕获4输出极性及使能CC2

	TIM4->DIER  |= 1 << 2;   //允许更新捕获中断2				
		   
	TIM4->CR1   |= 1 << 0;    //使能定时器4
	
	NVIC->IP[TIM4_IRQn] = 0x10;
    NVIC->ISER[TIM4_IRQn >> 0x05] = (u32)0x01 << (TIM4_IRQn & (u8)0x1F);
}

/*******************************************************************************
 函 数 名    ：TIM3_CONFIG
 功能描述：定时器3配置，负责计算回路1回码长度
 入口参数：无
 出口参数：无 
 返回值	    ：无
********************************************************************************/
void TIM3_CONFIG(void)
{
	TIM3->CR1 = 0;

	TIM3->ARR = 10000;  //设定计数器自动重装值//刚好1ms    
	TIM3->PSC = SystemCoreClock / 1000000 - 1;    //预分频器,

	TIM3->CCMR1 |= 1 << 8;
	TIM3->CCER  |= 3 << 4;

	TIM3->DIER  |= 1 << 2;   //允许更新捕获中断				
		   
	TIM3->CR1   |= 1 << 0;    //使能定时器2
	
	NVIC->IP[TIM3_IRQn] = 0x10;
    NVIC->ISER[TIM3_IRQn >> 0x05] = (u32)0x01 << (TIM3_IRQn & (u8)0x1F);
}

/*******************************************************************************
 函 数 名    ：TIM2_CONFIG
 功能描述：定时器2配置，负责普通的计时50mS
 入口参数：无
 出口参数：无 
 返回值	    ：无
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
	TIM2->PSC   = SystemCoreClock / 1000000 - 1;  //分频
	TIM2->CNT   = 50000;					
	TIM2->CR1  |= 1 << 0;

	NVIC->IP[TIM2_IRQn] = 0x30;
    NVIC->ISER[TIM2_IRQn >> 0x05] = (u32)0x01 << (TIM2_IRQn & (u8)0x1F);
}

/*******************************************************************************
 函 数 名    ：CAN1_CONFIG
 功能描述：can配置50KBPS
 入口参数：无
 出口参数：无 
 返回值	    ：无
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

//		//接收中断使能
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
 函 数 名    ：IWDG_CONFIG
 功能描述：独立看门狗初始化
 入口参数：无
 出口参数：无
 返回值	    ：无
********************************************************************************/
void IWDG_CONFIG(void)
{
	IWDG->KR  = 0x5555; 
	IWDG->PR  = 0x04;		//64分频
	IWDG->RLR = 0x780;		//3s 看门狗中断
	IWDG->KR  = 0xCCCC;		//启动看门狗
	IWDG->KR  = 0xAAAA; 	//喂狗
}


/****************************************************************************
函数名：LOOP_CONFIG
功能描述：初始化回路配置
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
  // Loop_18V_Curr_EXTI3_Config();    
}
//主备电切换初始化
void Power_Sw_Init(void)
{
    
}
/*******************************************************************************
 函 数 名：	delay_1us
 功能描述：	延时1us
 入口参数：	无
 出口参数：	无 
返回值	  ：无
********************************************************************************/
void delay_1us(void)
{
	u8 i = 7;
	
	while (i--);
}

/*******************************************************************************
 函 数 名：	delay_nus
 功能描述：	延时nus
 入口参数：	t 延时us数
 出口参数：	无
 返回值	  ：无
********************************************************************************/
void delay_nus(u16 t)
{
	while (t--)
	{
		delay_1us();
	}
}

/*******************************************************************************
 函 数 名：	delay_1ms
 功能描述：	延时1ms
 入口参数：	无
 出口参数：	无
 返回值	  ：无
********************************************************************************/
void delay_1ms(void)
{
	u16 i;
	
	i = 8000;
	
	while (i--);
}

/*******************************************************************************
 函 数 名：	delay_nms
 功能描述：	延时nms
 入口参数：	n:ms数
 出口参数：	无
 返回值	  ：无
********************************************************************************/
void delay_nms(u16 n)
{
	while (n--)
		delay_1ms();
}

/**************************************************************************
函数原型：feed_dog
功能描述：喂狗
入口参数：无
出口参数：无
返回值	  ：无
**************************************************************************/
void feed_dog(void)
{
	IWDG->KR  = 0xAAAA;
}

