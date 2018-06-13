#define W4_MAIN_WINDOW_WIN_PRIVATE
#include "include.h"
extern uint8_t         WinIndex;
extern const DEVICE_TYPE device_type1[];
static uint8_t page = 0; 
static  uint8_t page_total = 0;
void Dis_Fault(uint8_t num,SYSTEM_EVENT pevent,uint16_t x,uint16_t y,uint8_t sel)
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
        if(pevent.EventCode == EVENT_TYPE_POWER_FAULT)
        {
            GUI_SetColor(GUI_BLACK);
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetFont(GUI_FONT_HZ24);
            GUI_DispStringAt("主电故障",x+30,y);           
        }
        else if(pevent.EventCode == EVENT_TYPE_BATPOWER_FAULT)
        {
            GUI_SetColor(GUI_BLACK);
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetFont(GUI_FONT_HZ24);
            GUI_DispStringAt("备电故障",x+30,y);              
        }
        else if(pevent.EventCode == EVENT_TYPE_LOOP_FAULT)
        {
            GUI_SetColor(GUI_BLACK);
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetFont(GUI_FONT_HZ24);
            GUI_DispStringAt("回路短路",x+30,y);
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
              GUI_DispDecAt(pevent.Zone+1,x+152,y,1);
            }
            else//位置信息2个字
            {
               GUI_DispDecAt(pevent.Zone+1,x+128,y,1);
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
void FaultWindows(void)
{
    
    U16 i;
    char arr[20]={0};
    WinIndex = W4_FAULT_WINDOW_WIN;
    Time_interr = 0;
    guzhang();
      for(i = 0;i <2;i++)
        {
            if(i+page*2 == EventFaultCount)
                break;
                
          Dis_Fault(i+1+page*2,EventFaultRamBuffer[i+page*2],20,50+i*80,0);
        }
        //显示页码
   GUI_SetColor(GUI_BLACK);
   GUI_SetBkColor(GUI_WHITE);     
   GUI_SetFont(&GUI_Font24_1);
   GUI_DispStringAt("<",30,215);
   GUI_DispDecAt(page+1,42,215,1);
   GUI_DispStringAt("/",52,215);
   GUI_DispDecAt(page_total,60,215,1);
   GUI_DispStringAt(">",72,215);
        //显示时间
    GUI_SetBkColor(GUI_WHITE);
    GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font20_1);
	sprintf(arr,"%d-%d-%d %d:%d:%d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
    GUI_DispStringAt(arr,170,217);

}


void W4_FaultWindowDisplay(void)
{
  if(GetEventFaultCount() == 0)
  {
      W6_ShieldWindowDisplay();
      return;
  }
   if(EventFaultCount%2 == 0)
    {
        page_total = EventFaultCount/2;
    }
    else
    {
        page_total = EventFaultCount/2+1;
    }
   // WinLoad(W6_SHIELD_WINDOW_WIN);
//   if(EventShieldRamCurrentPointer == 0)
//   {
//       WinLoad(W2_MAIN_WINDOW_WIN);
//       return;
//   }
    
     guzhang();
    FaultWindows();
}

void W4_FaultWindowProcess(void)
{
     if(EventFaultCount%2 == 0)
    {
        page_total = EventFaultCount/2;
    }
    else
    {
        page_total = EventFaultCount/2+1;
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
            FaultWindows();
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
              FaultWindows();
            break;

        case KEY_SLIENCE:
             if(voice_flag == 1)
             {
                 slienceflag = 1;
             }
             W6_ShieldWindowDisplay();
            break;

        case KEY_FIRE:

            break;

        default:

            break;





    }
}

