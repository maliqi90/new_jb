#define W22_LOOPLOGIN_WINDOW_WIN_PRIVATE
#include "include.h"
#include "UI_APP.h"
#include "GUI.h"
#include "LISTVIEW.h"
#include "FRAMEWIN.h"
extern uint8_t         WinIndex;
PROGBAR_ loop_bar;
uint32_t loopbartimer = 0;
static uint8_t select = 0;
 LISTVIEW_Handle W22_hListView;
 PROGBAR_Handle LoopBar;
static DEVICE_CONFIG device_inf;
static uint8_t dev_total = 0;
extern   char * _aTable_1[18][5];
void loopbar(void)
{
    if(loop_bar.flag == 1)
    {
        if((GetTickCount()-loopbartimer)>1000)
        {    loop_bar.value++;
            loopbartimer = GetTickCount();
            PROGBAR_SetValue(LoopBar,loop_bar.value);
            WinKeyTimer = 10;
              lcd_time = LCD_TIME;
            if(loop_bar.value>=31)
            {
                loop_bar.flag = 0;
                WM_DeleteWindow(LoopBar);
                GUI_Exec();
                if(loop_login_flag == 1)
                {
                    WinLoad(22);
                }

              //	GUI_UC_SetEncodeUTF8();
                GUI_SetColor(GUI_LIGHTGRAY);
	            GUI_FillRect(10, 216, 160, 240);
                GUI_SetColor(GUI_BLUE);
	            GUI_SetBkColor(GUI_LIGHTGRAY);
                GUI_SetFont(GUI_FONT_HZ24);
               GUI_DispStringAt("按确认键保存",10,216);   
                
            }
            GUI_Exec();
        }
    }
}
 static void Dis_Device_(DEVICE_CONFIG *device)
 {
     char i;
     for(i = 0;i <16;i++)
     {
         
         device_inf.DeviceCondition = ALLDevices[1][i+1].DeviceCondition;
         if(ALLDevices[1][i+1].DeviceType == 0x17)
         {
              device_inf.DeviceType      = 0;
         }
         device_inf.DeviceCaption = 0;
         device_inf.DeviceZone = 1;
         if(device_inf.DeviceCondition == 0xff)
         {
             device_inf.DeviceCondition = 0;
         }
        if((device_inf.DeviceCondition&0x01) == 0x01) //是否存在
        {
            dev_total++;
            _aTable_1[i][1] = "YES";
            _aTable_1[i][2] = (char*)device_caption1[device_inf.DeviceCaption];//位置
            _aTable_1[i][3] = (char*)zonenum[device_inf.DeviceZone-1];//区号
            _aTable_1[i][4] = (char*)device_type1[device_inf.DeviceType-1].caption;//位置

        }
        else 
        {
            _aTable_1[i][1] = "NO";
            _aTable_1[i][2] = (char*)device_caption1[device_inf.DeviceCaption];//位置
            _aTable_1[i][3] = (char*)zonenum[device_inf.DeviceZone-1];//区号
            _aTable_1[i][4] = (char*)device_type1[device_inf.DeviceType].caption;//位置

        }


     }
     
 }
//创建表格
static  void Dis_Tabel(void)
{
    char i;
        HEADER_Handle hHeader;



   W22_hListView = LISTVIEW_Create(0, 30, 320, 180, 0, 1234, WM_CF_SHOW, 0);
   // LISTVIEW_SetFont(_hListView,&GUI_Font24_ASCII);
    
    
     GUI_Exec();
     hHeader = LISTVIEW_GetHeader(W22_hListView);

     WM_SetFocus(W22_hListView);
    LISTVIEW_SetGridVis(W22_hListView,1);//网格可见
    //LISTVIEW_SetFont(_hListView,GUI_FONT_HZ24);


    HEADER_SetFont(hHeader,GUI_FONT_HZ24);
    LISTVIEW_AddColumn(W22_hListView, 49, "地址",         GUI_TA_VCENTER);
     LISTVIEW_AddColumn(W22_hListView, 49, "在线",         GUI_TA_VCENTER);
   
    LISTVIEW_AddColumn(W22_hListView, 72, "位置", GUI_TA_VCENTER);

    LISTVIEW_AddColumn(W22_hListView, 48, "区号", GUI_TA_VCENTER);
    LISTVIEW_AddColumn(W22_hListView, 115, "类型", GUI_TA_VCENTER);
     
    // SCROLLBAR_CreateAttached(_hListView, SCROLLBAR_CF_VERTICAL);//创建附加至窗口的SCROLLBAR
     HEADER_SetTextColor(hHeader, GUI_BLUE);
    LISTVIEW_SetDefaultBkColor(hHeader,GUI_BLUE) ;
    LISTVIEW_SetFont(W22_hListView,GUI_FONT_HZ24);
    Dis_Device_(&device_inf);
    for(i = 0;i < 16;i++)
    {
     LISTVIEW_AddRow(W22_hListView, (const GUI_ConstString *)_aTable_1[i]);
        GUI_Exec();
    }
    for(i = 0;i < 1;i++)
    {
      LISTVIEW_IncSel(W22_hListView);
    }
      GUI_Exec();
 
}
void dis_device(void)
{
      GUI_SetColor(GUI_WHITE);
	  GUI_SetBkColor(GUI_GRAY);
      GUI_SetFont(GUI_FONT_HZ24);
      GUI_DispStringAt("器件总数：",105,6);   
      GUI_DispDecAt(dev_total,210,6,2);
      
}
void W22_LoopLoginWindowDisplay(void)
{
   // dis_device();
    if(loop_login_flag == 0)
    {
        LoopBar = PROGBAR_CreateEx(80,120,150,20,0,WM_CF_SHOW,PROGBAR_CF_HORIZONTAL,0);
        PROGBAR_SetBarColor(LoopBar,0,GUI_BLUE);
        PROGBAR_SetFont(LoopBar,&GUI_Font24_ASCII);
        PROGBAR_SetMinMax(LoopBar,0,30);
        PROGBAR_SetValue(LoopBar,0);
        GUI_Exec();
    }
    else
    {
        Dis_Tabel();
        dis_device();
       
    }
}
void W22_LoopLoginWindowProcess(void)
{
    switch (WinKeyValue )
    {
        case KEY_OK:
            if( loop_login_flag == 0)
            {
                
                AutoRegister_flag = 1;
                dev_total = 0;
                AutoRegInit(2);//回路自动登录初始化
                loop_bar.value = 0;
                loop_bar.flag = 1;
                GUI_SetColor(GUI_BLUE);
                GUI_SetBkColor(GUI_LIGHTGRAY);
                GUI_SetFont(GUI_FONT_HZ24);
                //	GUI_UC_SetEncodeUTF8();
                GUI_DispStringAt("正在登录中...",10,216);  
            }
            else
            {
                loop_login_flag = 2;
                LoopCfgSaveToFlash();
                GUI_SetColor(GUI_LIGHTGRAY);
	            GUI_FillRect(10, 216, 300, 240);
                GUI_SetColor(GUI_BLUE);
	            GUI_SetBkColor(GUI_LIGHTGRAY);
                GUI_SetFont(GUI_FONT_HZ24);
               GUI_DispStringAt("保存完成！",10,216);   
            }

            break;

        case KEY_UP:
          LISTVIEW_DecSel(W22_hListView);
            GUI_Exec();
           if(select > 0)
           {
               select--;
           }
            break;

        case KEY_DOWN:
            LISTVIEW_IncSel(W22_hListView);
            GUI_Exec();
             if(select < 15)
             {
                 select++;
             }
            break;

        case KEY_SLIENCE:
             LISTVIEW_Delete(W22_hListView);
             WM_DeleteWindow(LoopBar);
             loop_login_flag = 0;
             GUI_Exec();
            break;

        case KEY_FIRE:

            break;

        default:

            break;





    }
}

