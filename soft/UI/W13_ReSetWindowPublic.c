#define W13_RESET_WINDOW_WIN_PRIVATE
#include "include.h"
#include "UI_APP.h"
#include "GUI.h"
#include "LISTVIEW.h"
#include "FRAMEWIN.h"
extern uint8_t WinIndex;
PROGBAR_ Reset_bar;
static PROGBAR_Handle Reset_Bar;
extern void WinLoad(uint8_t index);
extern uint8_t  LastWinPointer;
extern uint8_t         WinPointer; 
static uint8_t sel = 0;
void Reset(void)
{
    //
}
void Dis_Reset(uint8_t num)
{
    if(num == 0)
    {
      GUI_SetColor(GUI_WHITE);
	  GUI_SetBkColor(GUI_GRAY);
      GUI_SetFont(GUI_FONT_HZ24);
      GUI_DispStringAt("确定",50,120); 
        
      GUI_SetColor(GUI_BLACK);
	  GUI_SetBkColor(GUI_LIGHTGRAY);
      GUI_SetFont(GUI_FONT_HZ24);
	  GUI_DispStringAt("取消",220,120);
    }
    else
    {
        
      GUI_SetColor(GUI_BLACK);
	  GUI_SetBkColor(GUI_LIGHTGRAY);
      GUI_SetFont(GUI_FONT_HZ24);
      GUI_DispStringAt("确定",50,120); 
        
      GUI_SetColor(GUI_WHITE);
	  GUI_SetBkColor(GUI_GRAY);
      GUI_SetFont(GUI_FONT_HZ24);
	  GUI_DispStringAt("取消",220,120);        
    }    
}
//初始化存储

void ResetFlash(void)
{
     int i;
    uint8_t phone[12];
    Communication_Set comm_set_;
     DEVICE_CONFIG device_inf;
     GUI_SetColor(GUI_LIGHTGRAY);
	 GUI_FillRect(0, 35, 320, 240); 
    
     GUI_SetColor(GUI_WHITE);
	 GUI_SetBkColor(GUI_GRAY); 
     GUI_SetFont(GUI_FONT_HZ24); 
     GUI_DispStringAt("恢复出厂设置",10,6);  
     GUI_SetColor(GUI_BLACK);
	 GUI_SetBkColor(GUI_LIGHTGRAY); 
     GUI_DispStringAt("恢复出厂设置中...",70,80);  
     memset(&device_inf,0,sizeof(device_inf));
     Reset_Bar = PROGBAR_CreateEx(80,120,150,20,0,WM_CF_SHOW,PROGBAR_CF_HORIZONTAL,0);
     PROGBAR_SetBarColor(Reset_Bar,0,GUI_BLUE);
     PROGBAR_SetFont(Reset_Bar,&GUI_Font24_ASCII);
     PROGBAR_SetMinMax(Reset_Bar,0,56);
     PROGBAR_SetValue(Reset_Bar,0);
     GUI_Exec();
     memset(phone,0,sizeof(phone));
     memset(&comm_set_,0,sizeof(comm_set_));
     comm_set_.Bus_Addr = 1;
       PROGBAR_SetValue(Reset_Bar,1);
       GUI_Exec();
     norflash_write_phone(1,phone);
      PROGBAR_SetValue(Reset_Bar,2);
      GUI_Exec();
     norflash_write_phone(2,phone);
     norflash_write_comm_set((uint8_t*)&comm_set_);
      PROGBAR_SetValue(Reset_Bar,3);
      GUI_Exec();
    //初始化器件
    for(i = 0;i < 16;i++)
    {
       norflash_write_device(i+1,&device_inf.DeviceCondition);
       PROGBAR_SetValue(Reset_Bar,4+i);
        GUI_Exec();
    }
    sFLASH_NOR_SectorErase(0XD4000); //擦除屏蔽记录
    for(i = 0;i < 36;i++)
    {
      sFLASH_NOR_SectorErase((202+i)*4096); 
         PROGBAR_SetValue(Reset_Bar,20+i);
        GUI_Exec();
    }
    norflash_write_eventflag(0);
     GUI_SetColor(GUI_BLUE);
     GUI_SetBkColor(GUI_LIGHTGRAY);
     GUI_SetFont(GUI_FONT_HZ24);
     GUI_DispStringAt("完成！", 150,280);
   SCB->AIRCR	= ((0x5FA << 16) | (SCB->AIRCR & (0x700)) | (1 << 2)); //复位
//     for(i = 0;i < 50;i++)
//    {
//      sFLASH_NOR_SectorErase((1280+i)*4096); 
//    }

}
void W13_ReSetWindowDisplay(void)
{

  Dis_Reset(sel);
}

void W13_ReSetWindowProcess(void)
{
    switch (WinKeyValue )
    {
        case KEY_OK:
             if(sel == 0)
             {
                ResetFlash();
//                GUI_SetColor(GUI_RED);
//                GUI_SetBkColor(GUI_GRAY);
//                 GUI_SetFont(GUI_FONT_HZ24);
//                GUI_DispStringAt("保存成功！", 150,6);
             }
             else
             {
               WinLoad(8);
             }
            break;

        case KEY_UP:
            sel++;
            if(sel == 2)
            {
                sel = 0;
            }
             Dis_Reset(sel);
            break;
 
        case KEY_DOWN:
              if(sel > 0)
              {
                  sel--;
              }
              else
              {
                  sel++;
              }
               Dis_Reset(sel);
            break;

        case KEY_SLIENCE:

            break;

        case KEY_FIRE:

            break;

        default:

            break;





    }
}

