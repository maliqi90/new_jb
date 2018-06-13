#define W15_FAULTQUERY_WINDOW_WIN_PRIVATE
#include "include.h"
extern uint8_t         WinIndex;
extern const DEVICE_TYPE device_type1[];
static uint8_t page = 0; 
static  uint8_t page_total = 0;
static void Dis_Fault(uint8_t num,SYSTEM_EVENT pevent,uint16_t x,uint16_t y,uint8_t sel)
{
    uint8_t date[12];
    uint8_t time[12];
    if(sel == 0)
    {
        //��ʾ�¼����
        GUI_SetColor(GUI_BLACK);
        GUI_SetBkColor(GUI_WHITE);
        GUI_SetFont(&GUI_Font24_1);
        GUI_DispDecAt(num,x,y,2);
        //��ʾ��ַ
        GUI_SetColor(GUI_BLACK);
        GUI_SetBkColor(GUI_WHITE);
        GUI_SetFont(GUI_FONT_HZ24);
//        GUI_DispStringAt("����",x+30,y);
        GUI_DispStringAt("��",x+55,y);
        //��ʾλ��
        GUI_DispStringAt(device_caption1[pevent.EventCaption],x+80,y);
        //��ʾ����
         GUI_DispStringAt((const char*)&device_type1[pevent.EventDeviceType].caption,x+210,y);
        
        GUI_SetFont(&GUI_Font24_1);
        GUI_DispDecAt(pevent.EventAddress,x+30,y,2);
        if(pevent.EventCaption == 1)//λ����Ϣ3����
        {
          GUI_DispDecAt(pevent.Zone,x+152,y,1);
        }
        else//λ����Ϣ2����
        {
           GUI_DispDecAt(pevent.Zone,x+128,y,1);
        }
        
        //��ʾʱ��
        DateToString((char*)&date[0],&pevent.EventYear);
        TimeToString((char*)&time[0],&pevent.EventHour);
        GUI_DispStringAt((const char*)date,x+15,y+40);
        GUI_DispStringAt((const char*)time,x+145,y+40);
  }
  else
  {
        GUI_SetColor(GUI_GRAY);
        GUI_FillRect(0,y,340,y+65);
              //��ʾ�¼����
        GUI_SetColor(GUI_WHITE);
        GUI_SetBkColor(GUI_LIGHTGRAY);
        GUI_SetFont(&GUI_Font24_1);
        GUI_DispDecAt(num,x,y,2);
        //��ʾ��ַ
        GUI_SetColor(GUI_WHITE);
        GUI_SetBkColor(GUI_LIGHTGRAY);
        GUI_SetFont(GUI_FONT_HZ24);
        GUI_DispStringAt("����",x+30,y);
        GUI_DispStringAt("��",x+95,y);
        //��ʾλ��
        GUI_DispStringAt(device_caption1[pevent.EventCaption],x+140,y);
        //��ʾ����
         GUI_DispStringAt((const char*)&device_type1[pevent.EventDeviceType].caption,x+230,y);
        
        GUI_SetFont(&GUI_Font24_1);
        GUI_DispDecAt(pevent.EventAddress,x+75,y,2);
         GUI_DispDecAt(pevent.Zone,x+185,y,1);
        //��ʾʱ��
        DateToString((char*)&date[0],&pevent.EventYear);
        TimeToString((char*)&time[0],&pevent.EventHour);
        GUI_DispStringAt((const char*)date,x+15,y+40);
        GUI_DispStringAt((const char*)time,x+145,y+40);
  }
}
static void FaultWindows(void)
{
    
    U16 i;
    SYSTEM_EVENT event;
    WinIndex = W14_FIREQUERY_WINDOW_WIN;
    Time_interr = 0;
    dis_fireevent();
      for(i = 0;i <2;i++)
        {
            if(i+page*2 == EventFlashFaultCurrentPointer)
                break;
          norflash_read_AlarmEvent(i+page*2,&event);
          Dis_Fault(i+1+page*2,event,20,50+i*80,0);
        }
        //��ʾҳ��
   GUI_SetColor(GUI_BLACK);
   GUI_SetBkColor(GUI_LIGHTGRAY);     
   GUI_SetFont(&GUI_Font24_1);
   GUI_DispStringAt("<",30,215);
   GUI_DispDecAt(page+1,42,215,1);
   GUI_DispStringAt("/",52,215);
   GUI_DispDecAt(page_total,60,215,1);
   GUI_DispStringAt(">",72,215);

}
void W15_FaultQueryWindowDisplay(void)
{
    
}

void W15_FaultQueryWindowProcess(void)
{
    switch (WinKeyValue )
    {
        case KEY_OK:

            break;

        case KEY_UP:
            break;

        case KEY_DOWN:

            break;

        case KEY_SLIENCE:

            break;

        case KEY_FIRE:

            break;

        default:

            break;





    }
}
