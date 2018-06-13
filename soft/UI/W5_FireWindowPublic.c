#define W5_FIRE_WINDOW_WIN_PRIVATE
#include "include.h"
extern uint8_t         WinIndex;
extern const DEVICE_TYPE device_type1[];
static uint8_t page = 0; 
static  uint8_t page_total = 0;
static void Dis_Fire(uint8_t num,SYSTEM_EVENT pevent,uint16_t x,uint16_t y,uint8_t sel)
{
    uint8_t date[12];
    uint8_t time[12];
    if(sel == 0)
    {
        //显示事件编号
        GUI_SetColor(GUI_BLACK);
        GUI_SetBkColor(GUI_WHITE);
        GUI_SetFont(&GUI_Font24_1);
        GUI_DispDecAt(num,x,y,2);
        if(pevent.EventAddress == 0xFD)//输入火警
        {
            GUI_SetColor(GUI_BLACK);
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetFont(GUI_FONT_HZ24);
            GUI_DispStringAt("输入火警",x+30,y);
        }
        else if(pevent.EventAddress == 0xFE)//手报火警
        {
            GUI_SetColor(GUI_BLACK);
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetFont(GUI_FONT_HZ24);
            GUI_DispStringAt("手报火警",x+30,y);            
        }
        else 
        {
             //显示地址
            GUI_SetColor(GUI_BLACK);
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetFont(GUI_FONT_HZ24);
    //        GUI_DispStringAt("本机",x+30,y);
            GUI_DispStringAt("号",x+55,y);
            //显示位置
            GUI_DispStringAt(device_caption1[pevent.EventCaption],x+80,y);
            //显示类型
             GUI_DispStringAt((const char*)&device_type1[pevent.EventDeviceType].caption,x+210,y);
            
            GUI_SetFont(&GUI_Font24_1);
            GUI_DispDecAt(pevent.EventAddress,x+30,y,2);
            if(pevent.EventCaption == 1)//位置信息3个字
            {
              GUI_DispDecAt(pevent.Zone,x+152,y,1);
            }
            else//位置信息2个字
            {
               GUI_DispDecAt(pevent.Zone,x+128,y,1);
            }           
        }

        
        //显示时间
        DateToString((char*)&date[0],&pevent.EventYear);
        TimeToString((char*)&time[0],&pevent.EventHour);
        GUI_DispStringAt((const char*)date,x+15,y+40);
        GUI_DispStringAt((const char*)time,x+145,y+40);
  }
  else
  {
        GUI_SetColor(GUI_GRAY);
        GUI_FillRect(0,y,340,y+65);
              //显示事件编号
        GUI_SetColor(GUI_WHITE);
        GUI_SetBkColor(GUI_GRAY);
        GUI_SetFont(&GUI_Font24_1);
        GUI_DispDecAt(num,x,y,2);
        //显示地址
        GUI_SetColor(GUI_WHITE);
        GUI_SetBkColor(GUI_GRAY);
        GUI_SetFont(GUI_FONT_HZ24);
        GUI_DispStringAt("本机",x+30,y);
        GUI_DispStringAt("号",x+95,y);
        //显示位置
        GUI_DispStringAt(device_caption1[pevent.EventCaption],x+140,y);
        //显示类型
         GUI_DispStringAt((const char*)&device_type1[pevent.EventDeviceType].caption,x+230,y);
        
        GUI_SetFont(&GUI_Font24_1);
        GUI_DispDecAt(pevent.EventAddress,x+75,y,2);
         GUI_DispDecAt(pevent.Zone,x+185,y,1);
        //显示时间
        DateToString((char*)&date[0],&pevent.EventYear);
        TimeToString((char*)&time[0],&pevent.EventHour);
        GUI_DispStringAt((const char*)date,x+15,y+40);
        GUI_DispStringAt((const char*)time,x+145,y+40);
  }
}
static void FireWindows(void)
{
    
    U16 i;
    WinIndex = W5_FIRE_WINDOW_WIN;
    Time_interr = 0;
    huojing();
      for(i = 0;i <2;i++)
        {
            if(i+page*2 == EventAlarmCount)
                break;
                
          Dis_Fire(i+1+page*2,EventFireRamBuffer[i+page*2],20,50+i*80,0);
        }
        //显示页码
   GUI_SetColor(GUI_BLACK);
   GUI_SetBkColor(GUI_WHITE);     
   GUI_SetFont(&GUI_Font20_1);
   GUI_DispStringAt("<",260,200);
   GUI_DispDecAt(page+1,272,200,1);
   GUI_DispStringAt("/",282,200);
   GUI_DispDecAt(page_total,290,200,1);
   GUI_DispStringAt(">",302,200);

}

void W5_FireWindowDisplay(void)
{
     uint8_t buf[11];
    // uint8_t phone[2][11];
      uint8_t i;
//    uint8_t temp = 0;
    if(GetEventAlarmCount() ==0)
    {
        W4_FaultWindowDisplay();
        return;
    }
    if(EventAlarmCount%2 == 0)
    {
        page_total = EventAlarmCount/2;
    }
    else
    {
        page_total = EventAlarmCount/2+1;
    }
   // WinLoad(W6_SHIELD_WINDOW_WIN);
//   if(EventShieldRamCurrentPointer == 0)
//   {
//       WinLoad(W2_MAIN_WINDOW_WIN);
//       return;
//   }
    
     huojing();
    FireWindows();
    if(gprsstate.Gprs_simflag == 0)//无SIM卡
    {
        GUI_SetColor(GUI_BLACK);
        GUI_SetBkColor(GUI_WHITE);
        GUI_SetFont(GUI_FONT_HZ24);
        GUI_DispStringAt("无SIM卡!",10,215);
    }
    else
    {
        if(gprsstate.Gprs_CSQ == 0)
        {
           GUI_SetColor(GUI_BLACK);
           GUI_SetBkColor(GUI_WHITE);
           GUI_SetFont(GUI_FONT_HZ24);
           GUI_DispStringAt("无信号！",10,215); 
        }
        else
        {
           if(phone_flag == 0)
           {
              GUI_SetColor(GUI_BLACK);
              GUI_SetBkColor(GUI_WHITE);
              GUI_SetFont(GUI_FONT_HZ24);
              GUI_DispStringAt("无联系人！",10,215); 
           }
           else
           {
             switch(call_flag)
              {
                 case 1://拨号中
                       GUI_SetColor(GUI_BLUE);
                       GUI_SetBkColor(GUI_WHITE);
                       GUI_SetFont(GUI_FONT_HZ24);
                       //sprintf((char *)buf,"拨号：");
                       GUI_DispStringAt("拨号",5,215); 
                       GUI_SetFont(&GUI_Font20_1);
                       
                       if((phone_flag == 0x01)||(phone_flag == 0x03))
                       {
                           Call_PhoneNum(&phone[0][0]);
                           for(i = 0;i < 11;i ++)
                            {
                              buf[i] = phone[0][i]+0x30;  
                            }
                       }
                       else if(phone_flag == 0x02)
                       {
                           Call_PhoneNum(&phone[1][0]);
                           for(i = 0;i < 11;i ++)
                            {
                                buf[i] = phone[1][i]+0x30;  
                            }
                       }
                        GUI_DispStringAt((char *)buf,55,215); 
                 break;
                 case 2://已接通
                 break;
                 case 3: //取消通话
                 break;
                 case 4: //已挂断
                 break;
       }
    }
    }
}

//    if(call_falg == 0)
//    {

//        norflash_read_phone(1,(uint8_t *)phone);
//        for(i = 0;i < 11;i ++)
//        {
//          phone[i] +=0x30;  
//        }
//        sprintf((char*)buf,"ATD%s;\r\n",phone);
//    }
}

void W5_FireWindowProcess(void)
{
     if(EventAlarmCount%2 == 0)
    {
        page_total = EventAlarmCount/2;
    }
    else
    {
        page_total = EventAlarmCount/2+1;
    }
    switch (WinKeyValue )
    {
        case KEY_OK:

            break;

        case KEY_UP:
            if(page < (page_total-1))
            {
                page++;
            }
            else
            {
                page = 0;
            }
            FireWindows();
            break;

        case KEY_DOWN:
             if(page == 0)
             {
                 page = page_total-1;
             }
             else
             {
                 page--;
             }
              FireWindows();
            break;

        case KEY_SLIENCE:
             if(voice_flag == 1)
             {
                 slienceflag = 1;
             }
            EventSwitchTimer = 10;
            W4_FaultWindowDisplay();
            break;

        case KEY_FIRE:

            break;

        default:

            break;





    }
}

