#include "rtc.h"
#include "commenu.h"

u32 timer_number1=0; //时间计数器 
u32 timer_number2=0; //时间超时计数器 
volatile u8 timer_interr_flag=0; //秒中断标志位
extern uint8_t WinIndex;
extern uint8_t self_check_flag;
tm Timer_value;  //时钟结构体  初始值：2017年2月8日
static uint16_t bat_timecount = 0;
 uint8_t bat_flag = 0;
//月份数据表											 
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表	  
//平年的月份日期表
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
//闰年的月份日期表
const u8 mon_table1[12]={31,29,31,30,31,30,31,31,30,31,30,31};

void power_check(void);
/****************************************************************************
* 名    称：void NVIC_Configuration(void)
* 功    能：中断源配置
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void NVIC_RTC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	
  /* Enable the RTC Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;					//配置外部中断源（秒中断） 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
}

void GPIO_Init1(void )
{
  GPIO_InitTypeDef GPIO_InitStructure;	  

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
  /*  配置PD6*/
	GPIO_InitStructure.GPIO_Pin=LED_ALARM_PIN;	  
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;//输出 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	   
	GPIO_Init(LED_ALARM_CTR,&GPIO_InitStructure);		  /* 初始化GPIO */
	
	/*  配置PD3*/
	GPIO_InitStructure.GPIO_Pin=LED_FAULT_PIN;	   
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;//输出 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	   
	GPIO_Init(LED_FAULT_CTR,&GPIO_InitStructure);		  /* 初始化GPIO */
}

///****************************************************************************
//* 名    称：char RTC_Init(void)
//* 功    能：RTC初始化函数
//* 入口参数：无
//* 出口参数：0：成功 其他：失败
//* 说    明：
//* 调用方法：
//****************************************************************************/
char RTC_Init(void)
{
	//检查是不是第一次配置时钟
//	u8 temp=0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
	PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问 
	if (BKP_ReadBackupRegister(BKP_DR1) != 0x5050)		//从指定的后备寄存器中读出数据:读出了与写入的指定数据不相乎
	{	
		BKP_DeInit();	//复位备份区域 
		RCC_LSEConfig(RCC_LSE_ON);	//设置外部低速晶振(LSE),使用外设低速晶振
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
		{	    
		}
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//设置RTC时钟(RTCCLK),选择LSE作为RTC时钟    
		RCC_RTCCLKCmd(ENABLE);	//使能RTC时钟  
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		RTC_WaitForSynchro();		//等待RTC寄存器同步  
		RTC_ITConfig(RTC_IT_SEC, ENABLE);		//使能RTC秒中断
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		RTC_EnterConfigMode();/// 允许配置	
		RTC_SetPrescaler(32767); //设置RTC预分频的值
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟  
	  PWR_BackupAccessCmd(ENABLE);	//使能RTC和后备寄存器访问 
	  RTC_SetCounter(0x00);	//设置RTC计数器的值
  	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成 
		RTC_ExitConfigMode(); //退出配置模式  
		BKP_WriteBackupRegister(BKP_DR1, 0X5050);	//向指定的后备寄存器中写入用户程序数据
	}		
	else//系统继续计时
	{

		RTC_WaitForSynchro();	//等待最近一次对RTC寄存器的写操作完成
		RTC_ITConfig(RTC_IT_SEC, ENABLE);	//使能RTC秒中断
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
	}	
		NVIC_RTC_Configuration();

	return 0; //ok

}		 				    
/*******************************************************************************
* Function Name  : RTC_IRQHandler
* Description    : This function handles RTC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_IRQHandler(void)
{
  if(RTC_GetITStatus(RTC_IT_SEC) != RESET)				 //读取秒中断状态
  {
//		  if((GetTickCount()-bat_timecount) > 3000)
//			{
//         //read_power();
//				 bat_flag = 1;
//			}
//			if(bat_flag == 1)
//			{
//				read_power();
//			}
      if(WinIndex == W2_MAIN_WINDOW_WIN)//更新界面
      {
         power_check();
      }
		timer_interr_flag = 1;
    RTC_ClearITPendingBit(RTC_IT_SEC);	
      
      
      if(power_flag == 1)
      {
          if((uint32_t)ADC_GetConversionValue(ADC1) > 100)
          {
              power_flag = 0;
               ADC_ITConfig(ADC1,ADC_IT_AWD,ENABLE);
          }
      }
      //清除秒中断标志
      if(EventAlarmCount == 0)
      {
           if(lcd_time>=1)
          {
           // lcd_time--;
          }else if(lcd_time == 0)
          {
              PWM_Set(0);
          }         
      }
      if(WinKeyTimer !=0)
      {
          WinKeyTimer--;
          if(WinKeyTimer == 0)
          {
            if(WinIndex == W12_SHIELDSET_WINDOW_WIN)
           {
               LISTVIEW_Delete(W12_hListView);
               GUI_Exec();
           }
           else if(WinIndex == W10_EDITDEVICE_WINDOW_WIN)
           {
               LISTVIEW_Delete(W10_hListView);
               GUI_Exec();               
           }
           else if(WinIndex == W19_DEVICELIGHT_WINDOW_WIN)
           {
              LISTVIEW_Delete(W19_hListView);
               GUI_Exec();     
           }
           else if(WinIndex == W22_LOOPLOGIN_WINDOW_WIN)
           {
                  LISTVIEW_Delete(W22_hListView);
                  PROGBAR_Delete(LoopBar);
						     loop_login_flag = 0;
                  GUI_Exec();  
           }
           else if(WinIndex == W20_LIGHT_SET_WINDOW_WIN)
           {
                 PROGBAR_Delete(slider);
           
                GUI_Exec(); 
           }
              W5_FireWindowDisplay();
          }
      }
      else
      {
          if(EventSwitchTimer !=0)
          {
              EventSwitchTimer--;
              if(EventSwitchTimer == 0)
              {
                     if(WinIndex == W12_SHIELDSET_WINDOW_WIN)
                   {
                       LISTVIEW_Delete(W12_hListView);
                       GUI_Exec();
                   }
                   else if(WinIndex == W10_EDITDEVICE_WINDOW_WIN)
                   {
                       LISTVIEW_Delete(W10_hListView);
                       GUI_Exec();               
                   }
                   else if(WinIndex == W19_DEVICELIGHT_WINDOW_WIN)
                   {
                      LISTVIEW_Delete(W19_hListView);
                       GUI_Exec();     
                   }
                   else if(WinIndex == W22_LOOPLOGIN_WINDOW_WIN)
                   {
                          LISTVIEW_Delete(W22_hListView);
                          PROGBAR_Delete(LoopBar);
										      loop_login_flag = 0;
                          GUI_Exec();  
                   }
                   else if(WinIndex == W20_LIGHT_SET_WINDOW_WIN)
                   {
                         PROGBAR_Delete(slider);
                   
                        GUI_Exec(); 
                   }
                  W5_FireWindowDisplay();
              }
          }
          
      }
      
    /* 时钟更新标志置位 */
    RTC_WaitForLastTask();							     //等待上一次对RTC寄存器的写操作是否已经完成   
//		
//		if(GPIO_ReadOutputDataBit(LED_FAULT_CTR,LED_FAULT_PIN)) LED_FAULT_ON;
//		else LED_FAULT_OFF;		
    	
    if(RTC_GetCounter() == 0x00015180)				     //当前时间是23:59:59时 复位为0:0:0 	 0x00015180
    {
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	  PWR->CR|=1<<8;                  					 //取消备份区写保护
	  RTC_EnterConfigMode();						     //允许配置 	  				
	  RTC_WaitForLastTask();                     //等待上一次对RTC寄存器的写操作是否已经完成 
			timer_number1++;                       //  时间计数器        
      RTC_SetCounter(0x0);								 //写入复位值
      RTC_WaitForLastTask();							 //等待上一次对RTC寄存器的写操作是否已经完成    
    }
	  else if(RTC_GetCounter() > 0x00015180)				 //当再次上电后计数值超过0x00015180， 复位为当前值取模0x00015180。	
    {
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	  PWR->CR|=1<<8;                                     //取消备份区写保护
	  RTC_EnterConfigMode();			                 //允许配置 
	  RTC_WaitForLastTask();                             //等待上一次对RTC寄存器的写操作是否已经完成 
      timer_number2 = RTC_GetCounter();                //			
      RTC_SetCounter(RTC_GetCounter()%0x00015180);		 //写入复位值
      RTC_WaitForLastTask();							 //等待上一次对RTC寄存器的写操作是否已经完成    
    }
  }
	
}

/*
********************************************************************************
*	函 数 名: u8 Is_Leap_Year(u16 year)
*	功能说明: 判断是否是闰年函数
*	形    参: year:年份
*	返 回 值: 该年份是不是闰年.1,是.0,不是
********************************************************************************
*/
u8 Is_Leap_Year(u16 year)
{			  
	if(year%4==0) //必须能被4整除
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;//如果以00结尾,还要能被400整除 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}	 		


/*
********************************************************************************
*	函 数 名: void HEX2ASCII(u8 hex_data,char* arr)     
*	功能说明: hex转ASCII
*	形    参: 无
*	返 回 值: 0：成功 
********************************************************************************
*/
void HEX2ASCII(u8 hex_data,char* arr)    //HEX转为ASCII子程序     
{   
	unsigned char ASCII_data;
	ASCII_data = hex_data;
	arr[0]=ASCII_data/10+'0';
	arr[1]=ASCII_data%10+'0';
}   


/*
********************************************************************************
*	函 数 名: u8 time_num(void)
*	功能说明: 计算时间年、月、日
*	形    参: 无
*	返 回 值: 0：成功 
********************************************************************************
*/
u8 time_num(void)
{
	u8 i=0;
	u16 daycnt = 0;
	u32 temp = 0;
	u16 temp1 = 0;
	u32 temp2 = 0;
	u32 temp3 = 0;
//	char arr[2]={0};
	Read_Time_Bkp();
	
	temp = timer_number1;  //读取当前时间天数  
	temp2 = timer_number2/86400;//读取掉电天数
	temp = temp+temp2;
	temp3 = RTC_GetCounter();
	if(temp>0) //超过一天
	{
		
    for(i=0;i<(Timer_value.month-1);i++)	
	  if(Is_Leap_Year(Timer_value.year))//当年是不是闰年
		  daycnt += mon_table1[i]; //计算当年天数
	  else
		  daycnt += mon_table[i]; //计算当年天数
	  temp = temp+daycnt+Timer_value.date;
	  while(temp>365)
	  {  	
		 if(Is_Leap_Year(Timer_value.year))//是闰年
			{
			 if(temp>366) temp-=366; //temp-=366;//闰年的秒钟数
			 else break;//{temp1++;break;}  
			}
			else temp-=365;//temp-=365;	  //平年 
			temp1++; 
	  } 
	  Timer_value.year+=temp1;//得到年份
	  temp1 = 0;
	 
	  while(temp>=28)//超过了一个月
		{
		 if(Is_Leap_Year(Timer_value.year)&&temp1==1)//当年是不是闰年/2月份
		 {
			if(temp>29)
			{
				temp-=29;//闰年的月数
				temp1++;
			}
			else break; 
		 }
		 else 
		 {
			 if(temp>mon_table[temp1])
			 {
				 temp-=mon_table[temp1];//平年
				 temp1++;
			 }						 
			 else break;
		 }	 
	 }
		Timer_value.month = temp1+1; //得到月份
		Timer_value.date = temp;   //得到日期
	
		timer_number1 = 0;
		timer_number2 = 0;
	  time_show();
	 //return 0;
 }
 else ;

  temp=temp3%86400;     		//得到秒钟数   	   
	Timer_value.hour=temp/3600;     	//小时
  Timer_value.min=(temp%3600)/60; 	//分钟	
	Timer_value.sec=(temp%3600)%60; 	//秒钟

	Save_Time_Bkp();  //把时间写入BKP 存储器 （D2:年 D3:月 D4:日 D5:时 D6:分 D7:秒 ）
return 1;
}

//RTC时钟刷新函数
void RTC_Dis(void)
{
//    char i;
//    char flag = 0;
//    uint8_t send_buff[10];
    if((timer_interr_flag)&&(self_check_flag == 0))
    {
//        lcd_time--;
//        //
//        if(lcd_time == 0)
//        {
//            //息屏
//        }
        time_num();
        time_show();
        timer_interr_flag = 0;
        

        
       
       // Dis_Power(0,GUI_RED);


    }   
}
void power_check(void)
{
        char i;
        uint8_t send_buff[10];

     /*   for(i = 0;i < 5;i++)
        {
             
            if(powernum&(1<<i))
            {
                continue;
            }
              Dis_Power(i,GUI_LIGHTBLUE);
               // flag= 1;
                 Led_Ctrl(LEDBAT_ON);
              if((bat_flag == 1)&&(i > 0))
              {
                bat_flag = 0;
                send_buff[1] = EVENT_TYPE_BATPOWER_FAULT_RE;//备电故障恢复
                send_buff[2] = 230;
                send_buff[3] = 0; 
                send_buff[4] = 0;
                send_buff[5] = 0;
                send_buff[6] = 0;
                Loop_Revice(send_buff);
             }
            
        }*/

        //显示信号
        
        if((gprsstate.Gprs_simflag == 0)&&(gprsstate.Gprs_CSQ == 0))//没有插入sim卡
        {
             Dis_Signal(6,GUI_LIGHTBLUE);
        }
        else if((gprsstate.Gprs_simflag == 1)&&(gprsstate.Gprs_CSQ == 0))//没有信号
        {
             Dis_Signal(0,GUI_LIGHTBLUE);
        }else if((gprsstate.Gprs_simflag == 1)&&(gprsstate.Gprs_CSQ != 0))
        {
            if(gprsstate.Gprs_CSQ < 12)
            {
                 Dis_Signal(0,GUI_LIGHTBLUE);
            }
            else if((gprsstate.Gprs_CSQ < 16)&&(gprsstate.Gprs_CSQ > 12))
            {
                 Dis_Signal(1,GUI_LIGHTBLUE);
            }
            else if((gprsstate.Gprs_CSQ < 20)&&(gprsstate.Gprs_CSQ > 16))
            {
                 Dis_Signal(2,GUI_LIGHTBLUE);
            }
            else if((gprsstate.Gprs_CSQ < 24)&&(gprsstate.Gprs_CSQ > 20))
            {
                 Dis_Signal(3,GUI_LIGHTBLUE);
            }
            else if((gprsstate.Gprs_CSQ < 28)&&(gprsstate.Gprs_CSQ > 24))
            {
                 Dis_Signal(4,GUI_LIGHTBLUE);
            }
            else if((gprsstate.Gprs_CSQ > 28))
            {
                 Dis_Signal(5,GUI_LIGHTBLUE);
            }
        }
}





