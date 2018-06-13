#include "include.h"
void PWM_Init(void);
void PWM_Set(uint16_t pwm);

void LCD_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	 //开启时钟
	RCC->APB2ENR |= (1<<0)|(1<<5)|(1<<6)|(1<<3);
	RCC->AHBENR  |= (1<<8);
	PWM_Init();

	//配置IO
	// IO模式设置
//	GPIOD->CRH &= ~(0XF<<16);
//	GPIOD->CRH |= (0X3<<16);	  // Pd12 BL背光 普通IO输出
        //新板子
//	GPIOB->CRH &= 0X0FFFFFFF;
//	GPIOB->CRH |= 0X30000000;	  // Pb15 BL背光 普通IO输出
	
/*	GPIOD->CRL &= 0xFFFFFF00;
	GPIOD->CRL |= 0x000000BB;  //PD1 2复用推挽
  GPIOD->CRH &= 0x00FFF000;
	GPIOD->CRH |= 0xBB000BBB;  //PD8 9 10 14 15复用推挽*/
//PORTD复用推挽输出    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15;                 // //PORTD??????    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;          //??????     
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(GPIOD, &GPIO_InitStructure);  
		
/*	GPIOE->CRL &= 0x0FFFFFF0;
	GPIOE->CRL |= 0xB0000003;	 //PE7 复用推挽 PE1	推挽
	GPIOE->CRH &= 0x00000000;
	GPIOE->CRH |= 0xBBBBBBBB;   // FSMC_D[15:0]  PE8~15 复用推挽*/
	  //PORTE复用推挽输出    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;              // //PORTD??????    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;          //??????     
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(GPIOE, &GPIO_InitStructure);
   //液晶屏复位引脚配置
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
  //命令数据线 PD11  a16
//	GPIOD->CRH &= ~(0xf<<12);  
//	GPIOD->CRH |= (0xb<<12);
	  //       
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_11;    // //PORTD??????    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;          //??????     
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	//配置先清零  在进行初始化
	FSMC_BCR1  =0X00000000;
	FSMC_BTR1  =0X00000000;
	FSMC_BWTR1 =0X00000000;
	
	FSMC_BCR1 |= 1<<12;
	FSMC_BCR1 |= 1<<14;
	FSMC_BCR1 |= 1<<4;
	// FSMC_BTR4[29:28]         // 默认00 使用模式A
  FSMC_BTR1  |= 1<<8;         // 数据建立时间DATAST RD-0
  FSMC_BTR1  |= 0x6<<0;       // 地址建立时间ADDSET RD-1
  //FSMC_BTR1  |= 0x04<<20;     //分频
  // FSMC_BWTR4[29:28]        // 默认00 使用模式A
	FSMC_BWTR1 |= 1<<8;         // 数据建立时间DATAST WR-0
	FSMC_BWTR1 |= 1<<0;         // 地址建立时间ADDSET WR-
	FSMC_BCR1  |= 1<<0;         // 使能
	//FSMC_BWTR1 |= 0x04<<20;
	//屏幕复位
	GPIOD->BSRR |= (1<<13);
	TIMDelay_Nms(1);
	GPIOD->BRR |=(1<<13);
	TIMDelay_Nms(10);
	GPIOD->BSRR |= (1<<13);
	TIMDelay_Nms(120);

     LCD_CMD=0xCF;      
		LCD_DAT=0x00; 
		LCD_DAT=0xC1; 
		LCD_DAT=0X30; 
		
	LCD_CMD=0xED;  
		LCD_DAT=0x64; 
		LCD_DAT=0x03; 
		LCD_DAT=0X12; 
		LCD_DAT=0X81;
		
	LCD_CMD=0xE8;  
		LCD_DAT=0x85; 
		LCD_DAT=0x10; 
		LCD_DAT=0x7A; 
		
	LCD_CMD=0xCB; 
		LCD_DAT=0x39; 
		LCD_DAT=0x2C; 
		LCD_DAT=0x00; 
		LCD_DAT=0x34; 
		LCD_DAT=0x02;
		
	LCD_CMD=0xF7;  
		LCD_DAT=0x20;
		
	LCD_CMD=0xEA;  
		LCD_DAT=0x00; 
		LCD_DAT=0x00;
		
	LCD_CMD=0xC0;    
		LCD_DAT=0x1B; 
		
		
	LCD_CMD=0xC1;    
		LCD_DAT=0x01;
		
		
	LCD_CMD=0xC5;  
		LCD_DAT=0x30;
		LCD_DAT=0x30;
		
		
	LCD_CMD=0xC7;  
		LCD_DAT=0XB7; 
		
		
	LCD_CMD=0x36;
		LCD_DAT=0x48; 
		
		
	LCD_CMD=0x3A;   
		LCD_DAT=0x55; 
		
	LCD_CMD=0xB1;   
		LCD_DAT=0x00;   
		LCD_DAT=0x1A; 
		
		
	LCD_CMD=0xB6;
		LCD_DAT=0x0A; 
		LCD_DAT=0xA2; 
		
	LCD_CMD=0xF2; 
		LCD_DAT=0x00; 
		
	LCD_CMD=0x26; 
		LCD_DAT=0x01; 
		
	LCD_CMD=0xE0;  
		LCD_DAT=0x0F; 
		LCD_DAT=0x2A; 
		LCD_DAT=0x28; 
		LCD_DAT=0x08; 
		LCD_DAT=0x0E; 
		LCD_DAT=0x08; 
		LCD_DAT=0x54; 
		LCD_DAT=0XA9; 
	  LCD_DAT=0x43; 
		LCD_DAT=0x0A; 
	  LCD_DAT=0x0F; 
	  LCD_DAT=0x00; 
		LCD_DAT=0x00; 
		LCD_DAT=0x00; 
		LCD_DAT=0x00; 
		
	LCD_CMD=0xE1; 
		LCD_DAT=0x00; 
		LCD_DAT=0x15; 
		LCD_DAT=0x17; 
		LCD_DAT=0x07; 
		LCD_DAT=0x11; 
    LCD_DAT=0x06; 
	  LCD_DAT=0x2B; 
		LCD_DAT=0x56; 
  	LCD_DAT=0x3C; 
	  LCD_DAT=0x05; 
	  LCD_DAT=0x10; 
	  LCD_DAT=0x0F; 
		LCD_DAT=0x3F; 
		LCD_DAT=0x3F; 
		LCD_DAT=0x0F; 
 
#if DISPLAY_ORIENTATION_X

  LCD_CMD=0x36;
    LCD_DAT=0x68;

	LCD_CMD=0x2A; 
		LCD_DAT=0x00;
		LCD_DAT=0x00;
		LCD_DAT=0x01;
		LCD_DAT=0x3F;
		
	LCD_CMD=0x2B; 
		LCD_DAT=0x00;
		LCD_DAT=0x00;
		LCD_DAT=0x00;
		LCD_DAT=0xEF;	

#else

  LCD_CMD=0x36;
	  LCD_DAT=0x08;

	LCD_CMD=0x2B; 
		LCD_DAT=0x00;
		LCD_DAT=0x00;
		LCD_DAT=0x01;
		LCD_DAT=0x3F;
		
	LCD_CMD=0x2A; 
		LCD_DAT=0x00;
		LCD_DAT=0x00;
		LCD_DAT=0x00;
		LCD_DAT=0xEF;
		
#endif  // DISPLAY_ORIENTATION_X 横屏显示

	LCD_CMD=0x11;
		TIMDelay_Nms(120);
	LCD_CMD=0x29;
    TIMDelay_Nms(120);
  // 开启背光显示
//	GPIOD->ODR |= 1<<12;
        //新板子
//	GPIOB->ODR |= 1<<15;
}

// 刷点
void LCD_Point(u16 x,u16 y,u16 color)
{	 
	LCD_CMD=0x2A;
	  LCD_DAT=0;
		LCD_DAT=x&0xFF;  
	LCD_CMD=0x2B;
	  LCD_DAT=0;
		LCD_DAT=y&0xFF;
	LCD_CMD=0x2C;	 
	  LCD_DAT=color; 
}

// 刷屏  
void LCD_Clear(u16 color)
{
	u32 point;
	LCD_CMD=0x2A;
		LCD_DAT=0;
		LCD_DAT=0;  
	LCD_CMD=0x2B;
		LCD_DAT=0;
		LCD_DAT=0;
	LCD_CMD=0x2C;  
	for(point=0;point<240*320;point++)
	{
		LCD_DAT=color;	   
	}
}
//PWM初始化
void PWM_Init(void)
{
   uint16_t PrescalerValue = 0;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  GPIO_InitTypeDef GPIO_InitStructure;
  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);   
 //GPIO clock enable
  GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_WriteBit(GPIOC,GPIO_Pin_6,Bit_SET);
    
   /* -----------------------------------------------------------------------
    TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles:
    The TIM3CLK frequency is set to SystemCoreClock (Hz), to get TIM3 counter
    clock at 24 MHz the Prescaler is computed as following:
     - Prescaler = (TIM3CLK / TIM3 counter clock) - 1
    SystemCoreClock is set to 72 MHz for Low-density, Medium-density, High-density
    and Connectivity line devices and to 24 MHz for Low-Density Value line and
    Medium-Density Value line devices

    The TIM3 is running at 36 KHz: TIM3 Frequency = TIM3 counter clock/(ARR + 1)
                                                  = 24 MHz / 666 = 36 KHz
    TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
    TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
    TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR)* 100 = 25%
    TIM3 Channel4 duty cycle = (TIM3_CCR4/ TIM3_ARR)* 100 = 12.5%
  ----------------------------------------------------------------------- */
  /* Compute the prescaler value */
  PrescalerValue = (uint16_t) (SystemCoreClock / 24000000) - 1;
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 665;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
//  

}

void PWM_Set(uint16_t pwm)
{
   TIM_OCInitTypeDef  TIM_OCInitStructure;
     /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = pwm;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM3, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
  TIM_ARRPreloadConfig(TIM3, ENABLE);
    /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);
}





