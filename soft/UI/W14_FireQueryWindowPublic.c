#define W14_FIREQUERY_WINDOW_WIN_PRIVATE
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
        GUI_SetBkColor(GUI_LIGHTGRAY);
        GUI_SetFont(&GUI_Font24_1);
        GUI_DispDecAt(num,x,y,2);
        if(pevent.EventAddress == 0xFD)//输入火警
        {
            GUI_SetColor(GUI_BLACK);
            GUI_SetBkColor(GUI_LIGHTGRAY);
            GUI_SetFont(GUI_FONT_HZ24);
            GUI_DispStringAt("输入火警",x+30,y);
        }
        else if(pevent.EventAddress == 0xFE)//手报火警
        {
            GUI_SetColor(GUI_BLACK);
            GUI_SetBkColor(GUI_LIGHTGRAY);
            GUI_SetFont(GUI_FONT_HZ24);
            GUI_DispStringAt("手报火警",x+30,y);            
        }
        else 
        {
        //显示地址
        GUI_SetColor(GUI_BLACK);
        GUI_SetBkColor(GUI_LIGHTGRAY);
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
        GUI_SetBkColor(GUI_LIGHTGRAY);
        GUI_SetFont(&GUI_Font24_1);
        GUI_DispDecAt(num,x,y,2);
        //显示地址
        GUI_SetColor(GUI_WHITE);
        GUI_SetBkColor(GUI_LIGHTGRAY);
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
    SYSTEM_EVENT event;
    WinIndex = W14_FIREQUERY_WINDOW_WIN;
    Time_interr = 0;
    dis_fireevent();
      for(i = 0;i <2;i++)
        {
            if(i+page*2 == EventFlashAlarmCurrentPointer)
                break;
          norflash_read_AlarmEvent(i+page*2,&event);
          Dis_Fire(i+1+page*2,event,20,50+i*80,0);
        }
        //显示页码
   GUI_SetColor(GUI_BLACK);
   GUI_SetBkColor(GUI_LIGHTGRAY);     
   GUI_SetFont(&GUI_Font24_1);
   GUI_DispStringAt("<",30,215);
   GUI_DispDecAt(page+1,42,215,3);
   GUI_DispStringAt("/",82,215);
   GUI_DispDecAt(page_total,90,215,3);
   GUI_DispStringAt(">",132,215);

}
void W14_FireQueryWindowDisplay(void)
{
    if(EventFlashAlarmCurrentPointer%2 == 0)
    {
        page_total = EventFlashAlarmCurrentPointer/2;
    }
    else
    {
        page_total = EventFlashAlarmCurrentPointer/2+1;
    }
    
     dis_fireevent();
    FireWindows();
}

void W14_FireQueryWindowProcess(void)
{
    if(EventFlashAlarmCurrentPointer%2 == 0)
    {
        page_total = EventFlashAlarmCurrentPointer/2;
    }
    else
    {
        page_total = EventFlashAlarmCurrentPointer/2+1;
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

            break;

        case KEY_FIRE:

            break;

        default:

            break;





    }
}

