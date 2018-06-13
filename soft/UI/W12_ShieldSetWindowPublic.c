#define W12_SHIELDSET_WINDOW_WIN_PRIVATE
#include "include.h"
#include "GUI.h"
#include "LISTVIEW.h"
#include "FRAMEWIN.h"
static uint8_t select = 0;
static uint8_t select_num = 0;
static uint8_t device_num = 0;
 LISTVIEW_Handle W12_hListView;
static DEVICE_CONFIG device_inf;
extern char * _aTable_1[18][5];
char dis_row[17];
/*
static void Dis_Device(uint8_t num,uint8_t sel)
{
    GUI_SetBkColor(GUI_WHITE);
	GUI_SetColor(GUI_BLACK);
    //显示编号
	GUI_SetFont(&GUI_Font24_1);
    GUI_DispDecAt(device_num, 153,80, 3);
    GUI_SetBkColor(GUI_LIGHTGRAY);
	GUI_SetColor(GUI_BLACK);
    GUI_SetFont(GUI_FONT_HZ24);
    GUI_DispStringAt("屏蔽",50,150);
    GUI_DispStringAt("解除屏蔽",150,150);
    GUI_SetColor(GUI_LIGHTGRAY);
    GUI_FillRect(100, 200, 300, 240);
    switch(sel)
    {
        case 0: //选择百位
                device_num = num*100 + ((device_num/10)%10)*10 + (device_num%10);
                //选择编号百位
                GUI_SetBkColor(GUI_GRAY);
	            GUI_SetColor(GUI_WHITE);
                GUI_SetFont(&GUI_Font24_1);
                GUI_DispDecAt(device_num/100, 153,80, 1);
            break;
        case 1://选择十位
               device_num = (device_num/100)*100 + num*10 + (device_num%10);
               //选择编号十位
                GUI_SetBkColor(GUI_GRAY);
	            GUI_SetColor(GUI_WHITE);
                GUI_SetFont(&GUI_Font24_1);
                GUI_DispDecAt((device_num/10)%10, 166,80, 1);
            break;
        case 2://选择个位
                device_num = (device_num/100)*100 + ((device_num/10)%10)*10 + num;
                //选择编号个位
                GUI_SetBkColor(GUI_GRAY);
	            GUI_SetColor(GUI_WHITE);
                GUI_SetFont(&GUI_Font24_1);    
                GUI_DispDecAt(device_num%10, 179,80, 1); 
            break;
        case 3://选择屏蔽
                GUI_SetBkColor(GUI_GRAY);
	            GUI_SetColor(GUI_WHITE);
                GUI_SetFont(GUI_FONT_HZ24);
                GUI_DispStringAt("屏蔽",50,150);
                  GUI_SetColor(GUI_RED);
                GUI_SetBkColor(GUI_LIGHTGRAY);
                GUI_DispStringAt("按上键屏蔽！", 100,200);
            break;
        case 4://选择解除屏蔽
                GUI_SetBkColor(GUI_GRAY);
	            GUI_SetColor(GUI_WHITE);
                GUI_SetFont(GUI_FONT_HZ24);
                GUI_DispStringAt("解除屏蔽",150,150);
                GUI_SetColor(GUI_RED);
                 GUI_SetBkColor(GUI_LIGHTGRAY);
                GUI_DispStringAt("按上键解除屏蔽！", 100,200);
            break;
        default:
            break;
    }
}
#endf*/
//创建表格806895
 static void Dis_Device_(DEVICE_CONFIG *device)
 {
     char i;
     memset(dis_row,0,sizeof(dis_row));
     for(i = 0;i <16;i++)
     {
        norflash_read_device(i+2,device);
         if(device_inf.DeviceCondition == 0xff)
         {
             device_inf.DeviceCondition = 0;
         }
        if((device_inf.DeviceCondition&0x02) == 0x02) //是否屏蔽
        {
            _aTable_1[i][1] = "YES";
            _aTable_1[i][2] = (char*)device_caption1[device_inf.DeviceCaption];//位置
            _aTable_1[i][3] = (char*)zonenum[device_inf.DeviceZone-1];//区号
            _aTable_1[i][4] = (char*)device_type1[device_inf.DeviceType].caption;//位置

        }
        else 
        {
            dis_row[i] = 0x01;
            _aTable_1[i][1] = "NO";

        }
        if((device_inf.DeviceCondition&0x02) == 0x02)
        {
             dis_row[i] |= 0x02;
        }


     }
     
 }
static void Dis_Tabel(void)
{
    char i;
        HEADER_Handle hHeader;



   W12_hListView = LISTVIEW_Create(0, 30, 320, 180, 0, 1234, WM_CF_SHOW, 0);
   // LISTVIEW_SetFont(_hListView,&GUI_Font24_ASCII);
    
    
     GUI_Exec();
     hHeader = LISTVIEW_GetHeader(W12_hListView);

     WM_SetFocus(W12_hListView);
    LISTVIEW_SetGridVis(W12_hListView,1);//网格可见
    //LISTVIEW_SetFont(_hListView,GUI_FONT_HZ24);


    HEADER_SetFont(hHeader,GUI_FONT_HZ24);
    LISTVIEW_AddColumn(W12_hListView, 49, "地址",         GUI_TA_VCENTER);
     LISTVIEW_AddColumn(W12_hListView, 49, "屏蔽",         GUI_TA_VCENTER);
   
    LISTVIEW_AddColumn(W12_hListView, 72, "位置", GUI_TA_VCENTER);

    LISTVIEW_AddColumn(W12_hListView, 48, "区号", GUI_TA_VCENTER);
    LISTVIEW_AddColumn(W12_hListView, 115, "类型", GUI_TA_VCENTER);
     
    // SCROLLBAR_CreateAttached(_hListView, SCROLLBAR_CF_VERTICAL);//创建附加至窗口的SCROLLBAR
     HEADER_SetTextColor(hHeader, GUI_BLUE);
    LISTVIEW_SetDefaultBkColor(hHeader,GUI_BLUE) ;
    LISTVIEW_SetFont(W12_hListView,GUI_FONT_HZ24);
    Dis_Device_(&device_inf);
    for(i = 0;i < 16;i++)
    {
        LISTVIEW_AddRow(W12_hListView, (const GUI_ConstString *)_aTable_1[i]);
        GUI_Exec();
    }
//    for(i = 0;i < 16;i ++)
//    {
//        if(dis_row[i] == 1)
//        {
//            LISTVIEW_DisableRow(_hListView,i);
//        }
//    }
   // dis_row[0] |=0x20;
//    for(i = 0;i < 16;i ++)
//    {
//        if((dis_row[i]&0x02) == 0x02)
//        {
//            for(j = 0;j < 5;j++)
//            {
//               LISTVIEW_SetItemTextColor(_hListView,j,i,LISTVIEW_CI_UNSEL,GUI_ORANGE);
//                   GUI_Exec();
//            }
//        }
//    }
    for(i = 0;i <= select;i++)
    {
      LISTVIEW_IncSel(W12_hListView);
    }
      GUI_Exec();
 
}
void W12_ShieldSetWindowDisplay(void)
{
    //device_num = 1;
    //Dis_Device(0,0);
      Dis_Tabel();
}

void W12_ShieldSetWindowProcess(void)
{
    switch (WinKeyValue )
    {
        case KEY_OK:
             if((dis_row[select]&0x02) ==0x02)
             {
                 dis_row[select] &=~(0x02);
                if(OpenSet(select+2))
                {
                     LISTVIEW_Delete(W12_hListView);
                    GUI_SetColor(GUI_BLUE);
                    GUI_SetBkColor(GUI_LIGHTGRAY);
                    GUI_SetFont(&GUI_Font24_1);
                    GUI_DispDecAt(device_num, 100,200, 3);
                    GUI_SetFont(GUI_FONT_HZ24);
                    GUI_DispStringAt("操作成功！ ", 132,200);  
                }
                else
                {
                   GUI_SetColor(GUI_BLUE);
                   GUI_SetBkColor(GUI_LIGHTGRAY);
                   GUI_SetFont(&GUI_Font24_1);
                   GUI_DispDecAt(device_num, 100,200, 3);
                   GUI_SetFont(GUI_FONT_HZ24);
                   GUI_DispStringAt("操作失败！  ", 132,200);  
                }
             }else
             {
                 dis_row[select] |=0x02;
                 if(ShieldSet(select+2))
                  {
                       LISTVIEW_Delete(W12_hListView);
                     GUI_SetColor(GUI_BLUE);
                     GUI_SetBkColor(GUI_LIGHTGRAY);
                     GUI_DispStringAt("屏蔽成功！", 100,200);
                   }
                   else
                   {
                     GUI_SetColor(GUI_BLUE);
                     GUI_SetBkColor(GUI_LIGHTGRAY);
                     GUI_DispStringAt("屏蔽失败！", 100,200);  
                   }
             }

          /*  switch(select)
            {
                case 0:
                    select_num = (device_num/10)%10;
                    break;
                case 1:
                    select_num = device_num%10;
                    break;
//                case 2:
//                    select_num = device_inf.DeviceCaption;
//                    break;
//                case 3:
//                    select_num = device_inf.DeviceZone;
//                    break;
//                case 4:
//                    select_num = device_inf.DeviceType;
                    break;
                case 4:
                    select_num = device_num/100;       
                    break;
            }
            if(select < 4)
            {
             select++;
            }
            else
            {
                select = 0;
            }
            Dis_Device(select_num,select);*/
            break;

        case KEY_UP:
             LISTVIEW_DecSel(W12_hListView);
            GUI_Exec();
           if(select > 0)
           {
               select--;
           }
           /* if((select>=0)&&(select<=2))
            {
                if(select == 0)
                {
                    if(select_num >= 2)
                    {
                        select_num = 0;
                    }
                    else
                    {
                        select_num++;
                    }
                }
                else if(select == 1)
                {
                    if((device_num/100) < 2)
                    {
                        if(select_num > 9)
                        {
                          select_num = 0;
                        }
                        else
                        {
                          select_num++;
                        }
                    }
                    else
                    {
                        if(select_num >= 5)
                        {
                            select_num = 0;
                        }
                        else
                        {
                            select_num++;
                        }
                    }
                }
                else if(select == 2)
                {
                     if(device_num/100 < 2)
                    {
                        if(select_num > 9)
                        {
                          select_num = 0;
                        }
                        else
                        {
                          select_num++;
                        }
                    }
                    else
                    { 
                        if((device_num/10)%10 < 5)
                        {
                             if(select_num > 9)
                            {
                              select_num = 0;
                            }
                            else
                            {
                              select_num++;
                            }                           
                        }
                        else
                        {
                            if(select_num >= 5)
                            {
                                select_num = 0;
                            }
                            else
                            {
                                select_num++;
                            } 
                        }
                        
                    }                   
                }
                

            }else if(select == 3)
            {
                if(ShieldSet(device_num))
                {
                    GUI_SetColor(GUI_RED);
                    GUI_SetBkColor(GUI_LIGHTGRAY);
                    GUI_DispStringAt("屏蔽成功！", 100,200);
                }
                else
                {
                    GUI_SetColor(GUI_RED);
                    GUI_SetBkColor(GUI_LIGHTGRAY);
                    GUI_DispStringAt("屏蔽失败！", 100,200);  
                }
                //屏蔽
                break;
            }
            else if(select == 4)
            {
                if(OpenSet(device_num))
                {
                    GUI_SetColor(GUI_RED);
                    GUI_SetBkColor(GUI_LIGHTGRAY);
                    GUI_SetFont(&GUI_Font24_1);
                    GUI_DispDecAt(device_num, 100,200, 3);
                    GUI_SetFont(GUI_FONT_HZ24);
                    GUI_DispStringAt("操作成功！ ", 132,200);  
                }
                else
                {
                   GUI_SetColor(GUI_RED);
                   GUI_SetBkColor(GUI_LIGHTGRAY);
                   GUI_SetFont(&GUI_Font24_1);
                   GUI_DispDecAt(device_num, 100,200, 3);
                   GUI_SetFont(GUI_FONT_HZ24);
                   GUI_DispStringAt("操作失败！  ", 132,200);  
                }
                //解除屏蔽
                break;
            }
            Dis_Device(select_num,select);*/
            break;

        case KEY_DOWN:
             LISTVIEW_IncSel(W12_hListView);
             GUI_Exec();
             if(select < 15)
             {
                 select++;
             }
            /* if((select >= 0)&&(select <=2))
             {
                 if(select == 0)
                 {
                    if(select_num == 0)
                    {
                        select_num = 2;
                    }
                    else
                    {
                        select_num--;
                    }    
                 }
                 else if(select == 1)
                 {
                     if((device_num/100) < 2)
                     {
                         if(select_num == 0)
                         {
                             select_num = 9;
                         }
                         else
                         {
                             select_num--;
                         }
                     }
                     else
                     {
                         if(select_num == 0)
                         {
                             select_num = 5;
                         }
                         else
                         {
                             select_num--;
                         }
                     }
                 }
                 else if(select == 2)
                 {
                      if(device_num/100 < 2)
                      {
                          if(select_num == 0)
                          {
                              select_num = 9;
                          }
                          else
                          {
                              select_num--;
                          }
                      }
                      else if((device_num/10)%10 < 5)
                      {
                          if(select_num == 0)
                          {
                              select_num = 9;
                          }
                          else 
                          {
                              select_num--;
                          }
                      }
                      else
                      {
                          if(select_num == 0)
                          {
                              select_num =5;
                          }
                          
                          else
                          {
                              select_num --;
                          }
                      }
                     
                 }

             }
             Dis_Device(select_num,select);*/
            break;

        case KEY_SLIENCE:
             select_num = 0;
             select = 0;
             LISTVIEW_Delete(W12_hListView);
             GUI_Exec();
            break;

        case KEY_FIRE:

            break;

        default:

            break;





    }
}

