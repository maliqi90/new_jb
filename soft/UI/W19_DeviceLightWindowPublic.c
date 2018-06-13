#define W19_DEVICELIGHT_WINDOW_WIN_PRIVATE
#include "include.h"
#include "loop.h"
//#include "can.h"
#include "loop_hw.h"
#include "LoopDataStruct.h"
#include "LoopOpt.h"
//#include "SysTick.h"
#include "LoopDevTables.h"
//#include "communication.h"
#include "DeviceTypeDefine.h"
#include "def.h"
extern volatile LOOPS LOOP[2]; 
extern char * _aTable_1[18][5];
static uint8_t select = 0;
static uint8_t select_num = 0;
//static uint8_t device_num = 0;
 LISTVIEW_Handle W19_hListView;
static DEVICE_CONFIG device_inf;
static char flag[18];
 static void Dis_Device_(DEVICE_CONFIG *device)
 {
     char i;
     for(i = 0;i <16;i++)
     {
        norflash_read_device(i+2,device);
         if(device_inf.DeviceCondition == 0xff)
         {
             device_inf.DeviceCondition = 0;
         }
        if((device_inf.DeviceCondition&0x01) == 0x01) //�Ƿ����
        {
            _aTable_1[i][1] = "YES";
            _aTable_1[i][2] = (char*)device_caption1[device_inf.DeviceCaption];//λ��
            _aTable_1[i][3] = (char*)zonenum[device_inf.DeviceZone-1];//����
            _aTable_1[i][4] = (char*)device_type1[device_inf.DeviceType].caption;//λ��

        }
        else 
        {
            _aTable_1[i][1] = "NO";
                        device_inf.DeviceCaption = 0;
            device_inf.DeviceZone = 1;
             device_inf.DeviceType = 0;
            _aTable_1[i][2] = (char*)device_caption1[device_inf.DeviceCaption];//λ��
            _aTable_1[i][3] = (char*)zonenum[device_inf.DeviceZone-1];//����
            _aTable_1[i][4] = (char*)device_type1[device_inf.DeviceType].caption;//λ��

        }


     }
     
 }
static void Dis_Tabel(void)
{
    char i;
        HEADER_Handle hHeader;



   W19_hListView = LISTVIEW_Create(0, 30, 320, 180, 0, 1234, WM_CF_SHOW, 0);
   // LISTVIEW_SetFont(_hListView,&GUI_Font24_ASCII);
    
    
     GUI_Exec();
     hHeader = LISTVIEW_GetHeader(W19_hListView);

     WM_SetFocus(W19_hListView);
    LISTVIEW_SetGridVis(W19_hListView,1);//����ɼ�
    //LISTVIEW_SetFont(_hListView,GUI_FONT_HZ24);


    HEADER_SetFont(hHeader,GUI_FONT_HZ24);
    LISTVIEW_AddColumn(W19_hListView, 49, "��ַ",         GUI_TA_VCENTER);
     LISTVIEW_AddColumn(W19_hListView, 49, "����",         GUI_TA_VCENTER);
   
    LISTVIEW_AddColumn(W19_hListView, 72, "λ��", GUI_TA_VCENTER);

    LISTVIEW_AddColumn(W19_hListView, 48, "����", GUI_TA_VCENTER);
    LISTVIEW_AddColumn(W19_hListView, 115, "����", GUI_TA_VCENTER);
     
    // SCROLLBAR_CreateAttached(_hListView, SCROLLBAR_CF_VERTICAL);//�������������ڵ�SCROLLBAR
     HEADER_SetTextColor(hHeader, GUI_BLUE);
    LISTVIEW_SetDefaultBkColor(hHeader,GUI_BLUE) ;
    LISTVIEW_SetFont(W19_hListView,GUI_FONT_HZ24);
    Dis_Device_(&device_inf);
    for(i = 0;i < 16;i++)
    {
        LISTVIEW_AddRow(W19_hListView, (const GUI_ConstString *)_aTable_1[i]);
        GUI_Exec();
    }
    for(i = 0;i <= select;i++)
    {
      LISTVIEW_IncSel(W19_hListView);
    }
      GUI_Exec();
 
}

void W19_DeviceLightWindowDisplay(void)
{
     //device_num = 1;
   // Dis_Device(0,0); 
    Dis_Tabel();  
    memset(flag,0,18);  
}

void W19_DeviceLightWindowProcess(void)
{
    uint8_t tempAddr;
    switch (WinKeyValue )
    {
        case KEY_OK:
            tempAddr = select;
            tempAddr++;
            if(flag[tempAddr] == 0)
            {
                flag[tempAddr] = 1;
                if (0x0a != (ALLDevices[1][tempAddr].Comd & 0x0f))  //���
                {
                    if (0 != LOOP[1].DevLedTestAddr)  // ��ǰһ��
                    {
                        ALLDevices[1][LOOP[1].DevLedTestAddr].Comd = XJCMD(1, LOOP[1].DevLedTestAddr);
                    }
                    LOOP[1].DevLedTestAddr = 2;
                    LOOP[1].DevLedTestAddr = tempAddr;
                    light_dev(tempAddr);
                    break;
                }
            }
            else
            {
               flag[tempAddr] = 0;
                if (0 != LOOP[1].DevLedTestAddr) //���
                {
                    ALLDevices[1][LOOP[1].DevLedTestAddr].Comd = XJCMD(1, LOOP[1].DevLedTestAddr);
                    LOOP[1].DevLedTestAddr = 0;
                }
                break;
            }
           // break;

        case KEY_UP:
            LISTVIEW_DecSel(W19_hListView);
            GUI_Exec();
           if(select > 0)
           {
               select--;
           }
          
            break;

        case KEY_DOWN:
             LISTVIEW_IncSel(W19_hListView);
             GUI_Exec();
             if(select < 15)
             {
                 select++;
             }
            break;
        case KEY_SLIENCE:
             select_num = 0;
             select = 0;
             LISTVIEW_Delete(W19_hListView);
             GUI_Exec();
            break;

        case KEY_FIRE:

            break;

        default:

            break;





    }
}

