#include "rtc.h"
#include "commenu.h"

u32 timer_number1=0; //ʱ������� 
u32 timer_number2=0; //ʱ�䳬ʱ������ 
volatile u8 timer_interr_flag=0; //���жϱ�־λ
extern uint8_t WinIndex;
tm Timer_value;  //ʱ�ӽṹ��  ��ʼֵ��2017��2��8��
static uint16_t bat_timecount = 0;
 uint8_t bat_flag = 0;
//�·����ݱ�											 
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //���������ݱ�	  
//ƽ����·����ڱ�
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
//������·����ڱ�
const u8 mon_table1[12]={31,29,31,30,31,30,31,31,30,31,30,31};

void power_check(void);
/****************************************************************************
* ��    �ƣ�void NVIC_Configuration(void)
* ��    �ܣ��ж�Դ����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void NVIC_RTC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	
  /* Enable the RTC Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;					//�����ⲿ�ж�Դ�����жϣ� 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
}

void GPIO_Init1(void )
{
  GPIO_InitTypeDef GPIO_InitStructure;	  

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
  /*  ����PD6*/
	GPIO_InitStructure.GPIO_Pin=LED_ALARM_PIN;	  
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;//��� 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	   
	GPIO_Init(LED_ALARM_CTR,&GPIO_InitStructure);		  /* ��ʼ��GPIO */
	
	/*  ����PD3*/
	GPIO_InitStructure.GPIO_Pin=LED_FAULT_PIN;	   
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;//��� 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	   
	GPIO_Init(LED_FAULT_CTR,&GPIO_InitStructure);		  /* ��ʼ��GPIO */
}

///****************************************************************************
//* ��    �ƣ�char RTC_Init(void)
//* ��    �ܣ�RTC��ʼ������
//* ��ڲ�������
//* ���ڲ�����0���ɹ� ������ʧ��
//* ˵    ����
//* ���÷�����
//****************************************************************************/
char RTC_Init(void)
{
	//����ǲ��ǵ�һ������ʱ��
//	u8 temp=0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��   
	PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ������� 
	if (BKP_ReadBackupRegister(BKP_DR1) != 0x5050)		//��ָ���ĺ󱸼Ĵ����ж�������:��������д���ָ�����ݲ����
	{	
		BKP_DeInit();	//��λ�������� 
		RCC_LSEConfig(RCC_LSE_ON);	//�����ⲿ���پ���(LSE),ʹ��������پ���
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
		{	    
		}
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��    
		RCC_RTCCLKCmd(ENABLE);	//ʹ��RTCʱ��  
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_WaitForSynchro();		//�ȴ�RTC�Ĵ���ͬ��  
		RTC_ITConfig(RTC_IT_SEC, ENABLE);		//ʹ��RTC���ж�
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_EnterConfigMode();/// ��������	
		RTC_SetPrescaler(32767); //����RTCԤ��Ƶ��ֵ
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��  
	  PWR_BackupAccessCmd(ENABLE);	//ʹ��RTC�ͺ󱸼Ĵ������� 
	  RTC_SetCounter(0x00);	//����RTC��������ֵ
  	RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д������� 
		RTC_ExitConfigMode(); //�˳�����ģʽ  
		BKP_WriteBackupRegister(BKP_DR1, 0X5050);	//��ָ���ĺ󱸼Ĵ�����д���û���������
	}		
	else//ϵͳ������ʱ
	{

		RTC_WaitForSynchro();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_ITConfig(RTC_IT_SEC, ENABLE);	//ʹ��RTC���ж�
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
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
  if(RTC_GetITStatus(RTC_IT_SEC) != RESET)				 //��ȡ���ж�״̬
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
      if(WinIndex == W2_MAIN_WINDOW_WIN)//���½���
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
      //������жϱ�־
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
      
    /* ʱ�Ӹ��±�־��λ */
    RTC_WaitForLastTask();							     //�ȴ���һ�ζ�RTC�Ĵ�����д�����Ƿ��Ѿ����   
//		
//		if(GPIO_ReadOutputDataBit(LED_FAULT_CTR,LED_FAULT_PIN)) LED_FAULT_ON;
//		else LED_FAULT_OFF;		
    	
    if(RTC_GetCounter() == 0x00015180)				     //��ǰʱ����23:59:59ʱ ��λΪ0:0:0 	 0x00015180
    {
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	  PWR->CR|=1<<8;                  					 //ȡ��������д����
	  RTC_EnterConfigMode();						     //�������� 	  				
	  RTC_WaitForLastTask();                     //�ȴ���һ�ζ�RTC�Ĵ�����д�����Ƿ��Ѿ���� 
			timer_number1++;                       //  ʱ�������        
      RTC_SetCounter(0x0);								 //д�븴λֵ
      RTC_WaitForLastTask();							 //�ȴ���һ�ζ�RTC�Ĵ�����д�����Ƿ��Ѿ����    
    }
	  else if(RTC_GetCounter() > 0x00015180)				 //���ٴ��ϵ�����ֵ����0x00015180�� ��λΪ��ǰֵȡģ0x00015180��	
    {
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	  PWR->CR|=1<<8;                                     //ȡ��������д����
	  RTC_EnterConfigMode();			                 //�������� 
	  RTC_WaitForLastTask();                             //�ȴ���һ�ζ�RTC�Ĵ�����д�����Ƿ��Ѿ���� 
      timer_number2 = RTC_GetCounter();                //			
      RTC_SetCounter(RTC_GetCounter()%0x00015180);		 //д�븴λֵ
      RTC_WaitForLastTask();							 //�ȴ���һ�ζ�RTC�Ĵ�����д�����Ƿ��Ѿ����    
    }
  }
	
}

/*
********************************************************************************
*	�� �� ��: u8 Is_Leap_Year(u16 year)
*	����˵��: �ж��Ƿ������꺯��
*	��    ��: year:���
*	�� �� ֵ: ������ǲ�������.1,��.0,����
********************************************************************************
*/
u8 Is_Leap_Year(u16 year)
{			  
	if(year%4==0) //�����ܱ�4����
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;//�����00��β,��Ҫ�ܱ�400���� 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}	 		


/*
********************************************************************************
*	�� �� ��: void HEX2ASCII(u8 hex_data,char* arr)     
*	����˵��: hexתASCII
*	��    ��: ��
*	�� �� ֵ: 0���ɹ� 
********************************************************************************
*/
void HEX2ASCII(u8 hex_data,char* arr)    //HEXתΪASCII�ӳ���     
{   
	unsigned char ASCII_data;
	ASCII_data = hex_data;
	arr[0]=ASCII_data/10+'0';
	arr[1]=ASCII_data%10+'0';
}   


/*
********************************************************************************
*	�� �� ��: u8 time_num(void)
*	����˵��: ����ʱ���ꡢ�¡���
*	��    ��: ��
*	�� �� ֵ: 0���ɹ� 
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
	
	temp = timer_number1;  //��ȡ��ǰʱ������  
	temp2 = timer_number2/86400;//��ȡ��������
	temp = temp+temp2;
	temp3 = RTC_GetCounter();
	if(temp>0) //����һ��
	{
		
    for(i=0;i<(Timer_value.month-1);i++)	
	  if(Is_Leap_Year(Timer_value.year))//�����ǲ�������
		  daycnt += mon_table1[i]; //���㵱������
	  else
		  daycnt += mon_table[i]; //���㵱������
	  temp = temp+daycnt+Timer_value.date;
	  while(temp>365)
	  {  	
		 if(Is_Leap_Year(Timer_value.year))//������
			{
			 if(temp>366) temp-=366; //temp-=366;//�����������
			 else break;//{temp1++;break;}  
			}
			else temp-=365;//temp-=365;	  //ƽ�� 
			temp1++; 
	  } 
	  Timer_value.year+=temp1;//�õ����
	  temp1 = 0;
	 
	  while(temp>=28)//������һ����
		{
		 if(Is_Leap_Year(Timer_value.year)&&temp1==1)//�����ǲ�������/2�·�
		 {
			if(temp>29)
			{
				temp-=29;//���������
				temp1++;
			}
			else break; 
		 }
		 else 
		 {
			 if(temp>mon_table[temp1])
			 {
				 temp-=mon_table[temp1];//ƽ��
				 temp1++;
			 }						 
			 else break;
		 }	 
	 }
		Timer_value.month = temp1+1; //�õ��·�
		Timer_value.date = temp;   //�õ�����
	
		timer_number1 = 0;
		timer_number2 = 0;
	  time_show();
	 //return 0;
 }
 else ;

  temp=temp3%86400;     		//�õ�������   	   
	Timer_value.hour=temp/3600;     	//Сʱ
  Timer_value.min=(temp%3600)/60; 	//����	
	Timer_value.sec=(temp%3600)%60; 	//����

	Save_Time_Bkp();  //��ʱ��д��BKP �洢�� ��D2:�� D3:�� D4:�� D5:ʱ D6:�� D7:�� ��
return 1;
}

//RTCʱ��ˢ�º���
void RTC_Dis(void)
{
//    char i;
//    char flag = 0;
//    uint8_t send_buff[10];
    if(timer_interr_flag)
    {
//        lcd_time--;
//        //
//        if(lcd_time == 0)
//        {
//            //Ϣ��
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
                send_buff[1] = EVENT_TYPE_BATPOWER_FAULT_RE;//������ϻָ�
                send_buff[2] = 230;
                send_buff[3] = 0; 
                send_buff[4] = 0;
                send_buff[5] = 0;
                send_buff[6] = 0;
                Loop_Revice(send_buff);
             }
            
        }*/

        //��ʾ�ź�
        
        if((gprsstate.Gprs_simflag == 0)&&(gprsstate.Gprs_CSQ == 0))//û�в���sim��
        {
             Dis_Signal(6,GUI_LIGHTBLUE);
        }
        else if((gprsstate.Gprs_simflag == 1)&&(gprsstate.Gprs_CSQ == 0))//û���ź�
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





