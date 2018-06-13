#include "voice.h"
#include "sflash_spi.h"
void MyDAC_Init(void);//DAC channel1 Configuration.
void Timerx_Init(u16 arr,u16 psc);
WAV_file wav1;//wav1
WAV_Paly play; //语音播放结构体
u8 wav_buf[1024];
u16 DApc;
u8 CHanalnum = 1;
u8 Bitnum = 8;;
//u32 voice_address = 0;
u8 volume=1;
/******************************************
函数名称：Voice_GpioInit
函数功能：语音模块GPIO口初始化
*******************************************/
void Voice_GpioInit(void)
{
//   GPIO_InitTypeDef GPIO_InitStructure;
//   wav1.SampleBits = 8;
//    wav1.SampleRate = 16000;
//    wav1.CHnum = 1;
//    wav1.DATAlen = 0x01e026;
//    //wav1.chRIFF = "RIFF";
//    wav1.speed = 0x1f40;
//    wav1.CHnum = 1;

//	 sFLASH_NOR_Read(wav_buf,NORFLASH_FONT_BASE_SECTOR,2048);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_0|GPIO_Pin_1;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  GPIO_Init(P_DATA, &GPIO_InitStructure);
//    
//    
	AUDIO_CTR_OFF;//功放芯片使能
    AUDIO_SECL_OFF;
//    Voice(0,1);
     MyDAC_Init();
     play.palyflag = 0;
     play.DACdone = 0;
     play.DApc = 0;
     play.DAPointer = 0;
     play.Bitnum = 8;
     memset(wav_buf,0,sizeof(wav_buf));

}

/*******************************
;模块名称:Line_1A_WTN6(UC8 SB_DATA)
;功 能:实现一线串口通信函数
;入 参: SB_DATA 为发送数据
;出 参:
;P_DATA 为数据口
********************************/
void Line_1A_WTN6( uint8_t SB_DATA)
{
    uint8_t S_DATA;
	  uint8_t j = 0;
	  // bit: B_DATA;
    S_DATA = SB_DATA;
    //P_DATA = 0;
	  GPIO_WriteBit(P_DATA,P_DATA_PIN,Bit_RESET);
    TIMDelay_Nms(5); //延时 5ms
    //B_DATA = S_DATA&0X01
    for(j=0;j<8;j++)
    {
      if(S_DATA&0X01)
       {
          GPIO_WriteBit(P_DATA,P_DATA_PIN,Bit_SET);
          TIMDelay_N10us(60); //延时 600us
				  GPIO_WriteBit(P_DATA,P_DATA_PIN,Bit_RESET);
          TIMDelay_N10us(20); //延时 200us
        }
        else
        {
           GPIO_WriteBit(P_DATA,P_DATA_PIN,Bit_SET);
          TIMDelay_N10us(20); //延时 200us
           GPIO_WriteBit(P_DATA,P_DATA_PIN,Bit_RESET);
          TIMDelay_N10us(60); //延时 600us
        }
        S_DATA = S_DATA>>1;
        
    }
     GPIO_WriteBit(P_DATA,P_DATA_PIN,Bit_SET);
}
/*--------------------------------------
;模块名称:Line_2A_WTN6(UC8 SB_DATA)
;功 能:实现二线串口通信函数
;入 参:SB_DATA 为发送数据
;出 参:
; CLK_2A //时钟线
; P_DATA_2A //数据线
;-------------------------------------*/
void Line_2A_WTN6(uint8_t SB_DATA)
{
	  uint8_t S_DATA;
	  uint8_t j = 0;
	  S_DATA = SB_DATA;
    //CLK_2A = 1; //时钟线
	  GPIO_WriteBit(P_CLK,P_CLK_PIN,Bit_SET);
    //P_DATA_2A = 1; //数据线
	  GPIO_WriteBit(P_DATA,P_DATA_PIN,Bit_SET);
   // S_DATA = SB_DATA;
    //CLK_2A = 0;
	  GPIO_WriteBit(P_CLK,P_CLK_PIN,Bit_RESET);
    TIMDelay_Nms(5); //延时 5ms
   // B_DATA = S_DATA&0X01;
    for(j=0;j<8;j++)
    {
			GPIO_WriteBit(P_CLK,P_CLK_PIN,Bit_RESET);
      // CLK_2A = 0; // 拉低
			 if(SB_DATA&0x01)
			 {
				 GPIO_WriteBit(P_DATA,P_DATA_PIN,Bit_SET);
				 TIMDelay_N10us(30); //延时 300us
				 GPIO_WriteBit(P_CLK,P_CLK_PIN,Bit_SET);
				 TIMDelay_N10us(30); //延时 300us
			 }
			 else
			 {
					GPIO_WriteBit(P_DATA,P_DATA_PIN,Bit_RESET);
				  TIMDelay_N10us(30); //延时 300us
				  GPIO_WriteBit(P_CLK,P_CLK_PIN,Bit_SET);
				  TIMDelay_N10us(30); //延时 300us			 
			 }

       S_DATA = S_DATA>>1;
    }
		GPIO_WriteBit(P_DATA,P_DATA_PIN,Bit_SET);
		GPIO_WriteBit(P_CLK,P_CLK_PIN,Bit_SET);

}
/************************************************************************************
函数名称：void Voice(uint8_t voicenum uint8_t count)
函数功能：播放语音
参数    ：uint8_t voicenum 语音段    uint8_t count 播放次数  1 播放一次 2循环播放
***********************************************************************************/
void Voice(uint8_t voicenum, uint8_t count)
{
      AUDIO_SECL_ON;//打开模拟开关，功放接通
      AUDIO_CTR_ON; //功放芯片使能
      Line_1A_WTN6(0XEF);
    switch(voicenum)
    {
        case FIRE_VOICE: //火警音
        case FAULT_VOICE://故障音
        case BAT_FAULT_VOICE://电池电量低音
        case NORMAL_VOICE://正常音
        case POLL_VOICE://巡检音
        case NETFIRE_VOICE://联网火警音
            if(count == 1)
            {
              Line_1A_WTN6(voicenum);
            }
            else
            {
               Line_1A_WTN6(voicenum);
               TIMDelay_Nms(500);
               Line_1A_WTN6(0xF2);                
            }
            break;
        case SLIENCE_VOICE:
              Line_1A_WTN6(0xFE); //停止播放当前语音
              //AUDIO_CTR_OFF;//关闭功放芯片
            break;
    }
}
void DAC1_SetData(u16 data)
{
	DAC->DHR12R1=data;//Í¨µÀ1µÄ12Î»ÓÒ¶ÔÆëÊý¾Ý
	DAC->SWTRIGR|=0x01;//Èí¼þÆô¶¯×ª»»
}

void DAC2_SetData(u16 data)
{
	DAC->DHR12R2=data;//通道1的12位右对齐数据
	DAC->SWTRIGR|=0x02;//软件启动转换
}
void MyDAC_Init(void)//DAC channel1 Configuration
{
    GPIO_InitTypeDef GPIO_InitStructure;
  	unsigned int tmpreg1=0,tmpreg2=0;
 	RCC->APB2ENR|=1<<2;//使能PORTA时钟
	RCC->APB1ENR|=RCC_APB1Periph_DAC;//使能DAC时钟
// 	GPIOA->CRL&=0XFF00FFFF; 
//	GPIOA->CRL|=0X00440000;//PA4,5 ¸¡¿ÕÊäÈë   	 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
//  	tmpreg1=DAC->CR;//Get the DAC CR value  
//  	tmpreg1&=~(CR_CLEAR_Mask<<DAC_Channel_1);//Clear BOFFx, TENx, TSELx, WAVEx and MAMPx bits  
//  	tmpreg2=(DAC_Trigger_Software|DAC_WaveGeneration_None|DAC_LFSRUnmask_Bits8_0|DAC_OutputBuffer_Enable); 
//  	tmpreg1|=tmpreg2<<DAC_Channel_1;//Calculate CR register value depending on DAC_Channel 
//  	DAC->CR=tmpreg1;//Write to DAC CR 
//	DAC->CR|=CR_EN_Set<<DAC_Channel_1;//DAC Channel1使能，PA4自动连接到DAC
//	DAC1_SetData(0x000);

  	tmpreg1=DAC->CR;//Get the DAC CR value  
  	tmpreg1&=~(CR_CLEAR_Mask<<DAC_Channel_1);//Clear BOFFx, TENx, TSELx, WAVEx and MAMPx bits  
  	tmpreg2=(DAC_Trigger_Software|DAC_WaveGeneration_None|DAC_LFSRUnmask_Bits8_0|DAC_OutputBuffer_Enable); 
  	tmpreg1|=tmpreg2<<DAC_Channel_1;//Calculate CR register value depending on DAC_Channel 
  	DAC->CR=tmpreg1;//Write to DAC CR 
	DAC1_SetData(0x000);
}

//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void Timerx_Init(u16 arr,u16 psc)
{
    NVIC_InitTypeDef NVIC_InitStructure;
	RCC->APB1ENR|=1<<5;//TIM7时钟使能    
 	TIM7->ARR=arr;  //设定计数器自动重装值//刚好1ms    
	TIM7->PSC=psc;  //预分频器7200,得到10Khz的计数时钟
	//这两个东东要同时设置才可以使用中断
	TIM7->DIER|=1<<0;   //允许更新中断				
	TIM7->DIER|=1<<6;   //允许触发中断
		  							    
	TIM7->CR1|=0x01;    //使能定时器3
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
  	//MY_NVIC_Init(1,3,TIM3_IRQChannel,2);//抢占1，子优先级3，组2									 
}
void TIM7_IRQHandler(void)
{ 		    		  			    
	u16 temp;
    if(play.palyflag ==1)
    {
        if(TIM7->SR&0X0001)//溢出中断
        {
            if(play.CHanalnum==1)//单声道
            {
                if(play.Bitnum==8)//8位精度
                {
    //				DAC->DHR12R1=wav_buf[DApc]*10/volume;//通道1的12位右对齐数据
                    DAC->DHR12R1=wav_buf[play.DApc]*16;//通道1的12位右对齐数据
                    DAC->SWTRIGR|=0x01;//软件启动两个通道的转换
                    play.DApc++;
                }
                else if(play.Bitnum==16)//16位精度(先低位后高位)
                {
                    temp=(((u8)(wav_buf[play.DApc+1])<<8)+(wav_buf[play.DApc]))*10/volume;
    //				DAC->DHR12L1=temp;
                    DAC->DHR12L2=temp;
                    DAC->SWTRIGR|=0x03;//软件启动两个通道的转换
                    play.DApc+=2;				
                }
            }
    //		else if(CHanalnum==2)//立体声	   10110010	 10110010
    //		{
    //			if(Bitnum==8)//8位精度
    //			{
    //				DAC->DHR12R1=wav_buf[DApc]*10/volume;//通道2的12位右对齐数据
    //				DApc++;
    //				DAC->DHR12R2=wav_buf[DApc]*10/volume;//通道2的12位右对齐数据
    //				DApc++;
    //				DAC->SWTRIGR|=0x03;//软件启动两个通道的转换
    //			}
    //			else if(Bitnum==16)//16位精度(先低位后高位)
    //			{
    //				DAC->DHR12L1=(((u8)(wav_buf[DApc+1]-0x80)<<4)|(wav_buf[DApc]>>4))*10/volume;//通道1的12位右对齐数据
    //				DApc+=2;
    //				DAC->DHR12L2=(((u8)(wav_buf[DApc+1]-0x80)<<4)|(wav_buf[DApc]>>4))*10/volume;//通道1的12位右对齐数据
    //				DApc+=2;
    //				DAC->SWTRIGR|=0x03;//软件启动两个通道的转换				
    //			}
    //		}		
            if(play.DApc==512)play.DACdone=1;
            if(play.DApc==1024){play.DApc=0;play.DACdone=2; }				    				   				     	    	
        }				   
	TIM7->SR&=~(1<<0);//清除中断标志位 	 
   }    
}
/***************************************
函数名称：Play_Voice
函数功能：
****************************************/
u8 Play_Voice(u32 addr,u8 type)
{
  u32 address;
  u8  buff[44]; 
//  if(type == 0) //消音
//  {
//     TIM7->CR1&=~(0x01); //关闭定时器
//     DAC->CR&= ~(CR_EN_Set<<DAC_Channel_2);//关闭DAC
//     play.palyflag = 0;
//     return 0;
//  }
    play.playcount = 0;
    switch(addr)
    {
        case NORFLASH_FIRE_VOICE_BASE:
             play.voice_address1 = NORFLASH_HONMEFIRE_VOICE_BASE;
            break;
        case NORFLASH_BATFAULT_VOICE_BASE:
        case NORFLASH_COMMFAULT_VOICE_BASE:
        case NORFLASH_POWERFAULT_VOICE_BASE:
            play.voice_address1 = NORFLASH_FAULT_VOICE_BASE;
            break;

        default :break;
    }

    
   AUDIO_CTR_ON;
    AUDIO_SECL_ON;
  address = addr;//算出音频文件首地址
  play.voice_address = address;
  sFLASH_NOR_Read(buff,address,44);  //读取wav文件头文件 
  memcpy(wav1.chRIFF,buff,4);//Chunk ID RIFF
  wav1.DATAlen = (u32)(buff[7]<<24)+(u32)(buff[6]<<16)+(u32)(buff[5]<<8)+(u32)(buff[4]);//Chunk Size
  memcpy(wav1.chWAV,buff+8,4);//Format :WAVE
  memcpy(wav1.chFMT,buff+14,4);//Format :FMT
  wav1.CHnum = buff[22] + (u16)(buff[23]<<8);//声道
  wav1.SampleRate = buff[24] +(u32)(buff[25]<<8) + (u32)(buff[26]<<16) + (u32)(buff[27]<<24);//采样率
  wav1.SampleBits = buff[28] + (u32)(buff[29]<<8) + (u32)(buff[30]<<16) + (u32)(buff[31]<<24);
  wav1.Bitnum = buff[34] + (u16)(buff[35]<<8); //采样位数  
  wav1.wavlen = buff[40] + (U32)(buff[41]<<8) + (u32)(buff[42]<<16) + (u32)(buff[43]<<24);//音频文件长度
// if(addr == NORFLASH_FIRE_VOICE_BASE)
// {
//     wav1.wavlen = wav1.wavlen/2;
// }
  
  play.Bitnum = wav1.Bitnum;
  play.DAPointer = 44-1;
  play.DACdone = 0;
  play.DApc = 0;
  play.palyflag = 1;
  play.CHanalnum = wav1.CHnum;
  play.playtype = type;
  play.DAPointer = address+play.DAPointer;
  sFLASH_NOR_Read(wav_buf,play.DAPointer,1024);
  play.DAPointer+=1024;
  DAC->CR|=CR_EN_Set<<DAC_Channel_1;//使能DAC
  Timerx_Init(1000000/(wav1.SampleRate),72); //初始化定时器

  
    return 1;
  
    
}
//u8 Playwav(void)
//{
//    u32 point = 0x58;
//    u16 i,times;
//    DACdone=0;
//    DApc=44;
//    times=(wav1.DATAlen>>10)-1;
//    while(1)
//	{	
//		while(!DACdone);
//		DACdone=0;
//		sFLASH_NOR_Read(wav_buf,NORFLASH_FONT_BASE_SECTOR+point,1024);
//        point+=1024;
//		while(!DACdone);
//		DACdone=0;
//		sFLASH_NOR_Read(wav_buf+1024,NORFLASH_FONT_BASE_SECTOR+point,1024);
//        point+=1024;
////		if((KEY1&KEY2&KEY3&KEY4)==0)
////		{			
////			if((KEY1&KEY2)==0){TIM3->CR1&=~0x01;break;}//¹Ø¶¨Ê±Æ÷
////			else if(KEY3==0&&volume<255)volume++;
////			else if(KEY4==0&&volume>10)volume--;				  
////		}
//	}
//}
//语音播放任务处理函数
void PaltWav_Task(void)
{
    uint32_t temp;
    u8  buff[44]; 
    if(play.palyflag == 1)
    {
        if(play.playcount == 0)
        {
            temp = play.voice_address;
        }
        else
        {
            temp = play.voice_address1;
        }
        
        if((play.DAPointer-temp) <wav1.wavlen)
        {
            if(play.DACdone == 1)
            {
              play.DACdone=0;
              sFLASH_NOR_Read(wav_buf,play.DAPointer,512); 
              play.DAPointer+=512;                
            }
            else if(play.DACdone == 2)
            {
              play.DACdone=0;
              sFLASH_NOR_Read(wav_buf+512,play.DAPointer,512);   
              play.DAPointer+=512;                 
            }
        }
        else
        {
            if(play.playtype == 1)
            {
                play.DAPointer = 44-1;
                play.DACdone = 1;
                play.DApc = 0;
                play.DAPointer = play.voice_address +play.DAPointer;
            }
            else if(play.playtype == 2)
            {
                if(play.playcount == 0)
                {
                    play.playcount = 1;
                    play.DAPointer = 44-1;
                    play.DACdone = 1;
                    play.DApc = 0;
                    play.DAPointer = play.voice_address1 +play.DAPointer; 
                    
                  sFLASH_NOR_Read(buff,play.voice_address1,44);  //读取wav文件头文件 
                  memcpy(wav1.chRIFF,buff,4);//Chunk ID RIFF
                  wav1.DATAlen = (u32)(buff[7]<<24)+(u32)(buff[6]<<16)+(u32)(buff[5]<<8)+(u32)(buff[4]);//Chunk Size
                  memcpy(wav1.chWAV,buff+8,4);//Format :WAVE
                  memcpy(wav1.chFMT,buff+14,4);//Format :FMT
                  wav1.CHnum = buff[22] + (u16)(buff[23]<<8);//声道
                  wav1.SampleRate = buff[24] +(u32)(buff[25]<<8) + (u32)(buff[26]<<16) + (u32)(buff[27]<<24);//采样率
                  wav1.SampleBits = buff[28] + (u32)(buff[29]<<8) + (u32)(buff[30]<<16) + (u32)(buff[31]<<24);
                  wav1.Bitnum = buff[34] + (u16)(buff[35]<<8); //采样位数  
                  wav1.wavlen = buff[40] + (U32)(buff[41]<<8) + (u32)(buff[42]<<16) + (u32)(buff[43]<<24);//音频文件长度
                // if(addr == NORFLASH_FIRE_VOICE_BASE)
                // {
                //     wav1.wavlen = wav1.wavlen/2;
                // }
                  
                  play.Bitnum = wav1.Bitnum;
//                  play.DAPointer = 44-1;
                  play.DACdone = 0;
                  play.DApc = 0;
                //  play.palyflag = 1;
                  play.CHanalnum = wav1.CHnum;
                  sFLASH_NOR_Read(wav_buf,play.DAPointer,1024);
                  play.DAPointer+=1024;
                  Timerx_Init(1000000/(wav1.SampleRate),72); //初始化定时器
                }
                else if(play.playcount == 1)
                {
                    play.playcount = 0;
                    play.DAPointer = 44-1;
                    play.DACdone = 1;
                    play.DApc = 0;
                    play.DAPointer = play.voice_address +play.DAPointer; 
                    sFLASH_NOR_Read(buff,play.voice_address,44);  //读取wav文件头文件 
                   memcpy(wav1.chRIFF,buff,4);//Chunk ID RIFF
                   wav1.DATAlen = (u32)(buff[7]<<24)+(u32)(buff[6]<<16)+(u32)(buff[5]<<8)+(u32)(buff[4]);//Chunk Size
                   memcpy(wav1.chWAV,buff+8,4);//Format :WAVE
                   memcpy(wav1.chFMT,buff+14,4);//Format :FMT
                   wav1.CHnum = buff[22] + (u16)(buff[23]<<8);//声道
                   wav1.SampleRate = buff[24] +(u32)(buff[25]<<8) + (u32)(buff[26]<<16) + (u32)(buff[27]<<24);//采样率
                    wav1.SampleBits = buff[28] + (u32)(buff[29]<<8) + (u32)(buff[30]<<16) + (u32)(buff[31]<<24);
                   wav1.Bitnum = buff[34] + (u16)(buff[35]<<8); //采样位数  
                   wav1.wavlen = buff[40] + (U32)(buff[41]<<8) + (u32)(buff[42]<<16) + (u32)(buff[43]<<24);//音频文件长度
                // if(addr == NORFLASH_FIRE_VOICE_BASE)
                // {
                //     wav1.wavlen = wav1.wavlen/2;
                // }
                  
                  play.Bitnum = wav1.Bitnum;
//                  play.DAPointer = 44-1;
                  play.DACdone = 0;
                  play.DApc = 0;
                  //play.palyflag = 1;
                  play.CHanalnum = wav1.CHnum;
                  sFLASH_NOR_Read(wav_buf,play.DAPointer,1024);
                  play.DAPointer+=1024;
                    Timerx_Init(1000000/(wav1.SampleRate),72); //初始化定时器
                }
            }
            else
            {
              play.palyflag = 0; 
              play.DACdone = 0;
              TIM7->CR1&=~(0x01); //关闭定时器
              DAC->CR&= ~(CR_EN_Set<<DAC_Channel_1);//关闭DAC
              play.palyflag = 0; 
              GPIO_ResetBits(GPIOA,GPIO_Pin_4);
             //AUDIO_CTR_OFF;
             AUDIO_SECL_OFF;            
            }

        }
    }
}
//消音函数
void slience(void)
{
     play.palyflag = 0; 
     play.DACdone = 0;
     TIM7->CR1&=~(0x01); //关闭定时器
     DAC->CR&= ~(CR_EN_Set<<DAC_Channel_1);//关闭DAC
     play.palyflag = 0;  
    // AUDIO_CTR_OFF;
     AUDIO_SECL_OFF;       
}

