#define W9_SETTIME_WINDOW_WIN_PRIVATE
#include "include.h"

u8 SetTime_flag = 0;

void W9_SetTimeWindowDisplay(void)
{
	WinKeyValue = 0;
	SetTime_flag=0;
	
}

void W9_SetTimeWindowProcess(void)
{
	char arr[2]={0};
	
	   
    switch (WinKeyValue )
    {
        case KEY_FIRE:

				    break;
        case KEY_UP:
					 GUI_SetColor(GUI_WHITE);
	         GUI_SetBkColor(GUI_GRAY);
	         GUI_SetFont(&GUI_Font32_1);
					if(SetTime_flag==0)  //显示年
					{
					 Timer_value.year++;
						
					 HEX2ASCII(Timer_value.year/100,arr);
           GUI_DispStringAt(arr,48,80);
	         HEX2ASCII(Timer_value.year%100,arr);
           GUI_DispStringAt(arr,80,80);
					}
					else if(SetTime_flag==1)  //显示月
					{
						
					  if(Timer_value.month>=12) Timer_value.month=0;
	          else Timer_value.month++;
						HEX2ASCII(Timer_value.month,arr);
            GUI_DispStringAt(arr,160,80);
					}
					else if(SetTime_flag==2)   //显示日
					{
					  if((Timer_value.month==2)&&(((Is_Leap_Year(Timer_value.year))&&(Timer_value.date>=29))||((!Is_Leap_Year(Timer_value.year))&&(Timer_value.date>=28))))
				    {Timer_value.date = 1;}
				    else if(((Timer_value.month==1)||(Timer_value.month==3)||(Timer_value.month==5)||(Timer_value.month==7)||(Timer_value.month==8)||(Timer_value.month==10)||(Timer_value.month==12))&&(Timer_value.date>=31))
		        {Timer_value.date=1;}
		        else if(((Timer_value.month==4)||(Timer_value.month==6)||(Timer_value.month==9)||(Timer_value.month==11))&&(Timer_value.date>=30))
		        {Timer_value.date=1;}
		        else
		        Timer_value.date++;
					  HEX2ASCII(Timer_value.date,arr);
            GUI_DispStringAt(arr,240,80);
					}
					else if(SetTime_flag==3)  //显示时
					{ 
					  if(Timer_value.hour>=23) Timer_value.hour=0;
	          else Timer_value.hour++;
					  HEX2ASCII(Timer_value.hour,arr);
            GUI_DispStringAt(arr,80,130);
					}
					else if(SetTime_flag==4) //显示分
					{
					  if(Timer_value.min>=59) Timer_value.min=0;
	          else Timer_value.min++;
					  HEX2ASCII(Timer_value.min,arr);
				    GUI_DispStringAt(arr,160,130);
					}
					else if(SetTime_flag==5) //显示秒
					{
					  if(Timer_value.sec>=59) Timer_value.sec=0;
	          else Timer_value.sec++;
						HEX2ASCII(Timer_value.sec,arr);
				    GUI_DispStringAt(arr,240,130);
						
					}
					
				 Write_Time_RTC();
				 Save_Time_Bkp();
            break;

        case KEY_DOWN:
					 GUI_SetColor(GUI_WHITE);
	         GUI_SetBkColor(GUI_GRAY);
	         GUI_SetFont(&GUI_Font32_1);
					if(SetTime_flag==0)  //显示年
					{
					 Timer_value.year--;
					 HEX2ASCII(Timer_value.year/100,arr);
           GUI_DispStringAt(arr,48,80);
	         HEX2ASCII(Timer_value.year%100,arr);
           GUI_DispStringAt(arr,80,80);
					}
					else if(SetTime_flag==1)  //显示月
					{
					  if(Timer_value.month<=1) Timer_value.month=12;
	          else Timer_value.month--;
						HEX2ASCII(Timer_value.month,arr);
            GUI_DispStringAt(arr,160,80);
					}
					else if(SetTime_flag==2)    //显示日
					{
					  if((Timer_value.month==2)&&((Is_Leap_Year(Timer_value.year))&&(Timer_value.date<=1)))
				    {Timer_value.date = 29;}
				    else if((Timer_value.month==2)&&((!Is_Leap_Year(Timer_value.year))&&(Timer_value.date<=1)))
				    {
				      Timer_value.date = 28;
				    }
				    else if(((Timer_value.month==1)||(Timer_value.month==3)||(Timer_value.month==5)||(Timer_value.month==7)||(Timer_value.month==8)||(Timer_value.month==10)||(Timer_value.month==12))&&(Timer_value.date<=1))
		        {Timer_value.date=31;}
		        else if(((Timer_value.month==4)||(Timer_value.month==6)||(Timer_value.month==9)||(Timer_value.month==11))&&(Timer_value.date<=1))
		        {Timer_value.date=30;}
		        else 
				 	  Timer_value.date--;
						HEX2ASCII(Timer_value.date,arr);
            GUI_DispStringAt(arr,240,80);
					}
					else if(SetTime_flag==3) //显示时
					{
					  if(Timer_value.hour<=0) Timer_value.hour=23;
		        else  Timer_value.hour--;
						HEX2ASCII(Timer_value.hour,arr);
            GUI_DispStringAt(arr,80,130);
					}
					else if(SetTime_flag==4) //显示分
					{
					  if(Timer_value.min<=0) Timer_value.min=59;
		        else  Timer_value.min--;
					  HEX2ASCII(Timer_value.min,arr);
				    GUI_DispStringAt(arr,160,130);
					}
					else if(SetTime_flag==5) //显示秒
					{
					  if(Timer_value.sec<=0) Timer_value.sec=59;
		        else  Timer_value.sec--;
						HEX2ASCII(Timer_value.sec,arr);
				    GUI_DispStringAt(arr,240,130);
					}
          Write_Time_RTC();
				  Save_Time_Bkp();
            break;

        case KEY_SLIENCE:
					
          
            break;

        case  KEY_OK:
               SetTime_flag++;
			   if(SetTime_flag>5) SetTime_flag=0;
				TimeSet_Show(SetTime_flag);
            break;

        default:

            break;





    }
}

