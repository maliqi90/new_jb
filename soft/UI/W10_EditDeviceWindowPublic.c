#define W10_EDITDEVICE_WINDOW_WIN_PRIVATE
#include "include.h"
#include "GUI.h"
#include "LISTVIEW.h"
#include "FRAMEWIN.h"


extern void WinLoad(uint8_t index);
 LISTVIEW_Handle W10_hListView;
static DEVICE_CONFIG device_inf;
 uint8_t last_select = 0,select = 0;
//static uint8_t select_num = 0;
static uint8_t device_num = 0;//设备编号
uint8_t device_total = 0;//器件类型页数
uint8_t device_allindex[16];//存储存在器件编号
//static uint8_t page_num = 0;
  char * _aTable_1[18][5] = {
    { "01", "NO", "卧室","1","烟感"},
    { "02", "NO", "卧室","1","烟感"},
    { "03", "NO", "卧室","1","烟感"},
    { "04", "NO", "卧室","1","烟感"},
    { "05", "NO", "卧室","1","烟感"},
    { "06", "NO", "卧室","1","烟感"},
    { "07", "NO", "卧室","1","烟感"},
    { "08", "NO", "卧室","1","烟感"},
    { "09", "NO", "卧室","1","烟感"},
    { "10", "NO", "卧室","1","烟感"},
    { "11", "NO", "卧室","1","烟感"},
    { "12", "NO", "卧室","1","烟感"},
    { "13", "NO", "卧室","1","烟感"},
    { "14", "NO", "卧室","1","烟感"},
    { "15", "NO", "卧室","1","烟感"},
    { "16", "NO", "卧室","1","烟感"},
    { "17", "NO", "卧室","1","烟感"},
    { "18", "NO", "卧室","1","烟感"},
  };
 const char event__type[23][16]= {
   {"正常"},//0
   {"报警"},//1
   {"故障"},//2
   {"故障恢复"},//3
   {"系统"},//4
   {"启动"},//5
   {"停止"},//6
   {"反馈"},//7
   {"反馈撤销"},//8
   {"屏蔽"},//9
   {"屏蔽解除"},//10
   {"监管"},//11
   {"监管解除"},//12
   {"复位"},    //13
   {"关机"},    //14 
   {"开机"},    //15
   {"消音"},    //16
   {"回路短路"},//17
   {"回路短路恢复"},//18
   {"主电故障"},//19
   {"主电故障恢复"},//20
   {"备电故障"},//21
   {"备电故障恢复"},//22

};
const char device_caption1[DEVICE_CAPTION_NUM][10] = 
{
    {"客厅"},
    {"卫生间"},
    {"卧室"},
    {"书房"},
    {"阳台"},
};
const DEVICE_TYPE device_type1[] =
{
    {DEVICE_POINT_PY_SMOKE_FIRE_DETECTOR,"烟感",0},
    {DEVICE_FEEL_FIRE_DETECTOR,"感温",1},
    {DEVICE_HAND_FIRE_DETECTOR,"手报",2},
    {0xff,"空白",3},
};
const char* zonenum[] = 
{
    "1","2","3","4","5"

};

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

        if((device_inf.DeviceCondition&0x01) == 0x01) //是否存在
        {
            _aTable_1[i][1] = "YES";
            _aTable_1[i][2] = (char*)device_caption1[device_inf.DeviceCaption];//位置
            _aTable_1[i][3] = (char*)zonenum[device_inf.DeviceZone-1];//区号
            _aTable_1[i][4] = (char*)device_type1[device_inf.DeviceType].caption;//位置

        }
        else 
        {
            _aTable_1[i][1] = "NO";
            device_inf.DeviceCaption = 0;
            device_inf.DeviceZone = 1;
             device_inf.DeviceType = 0;
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



   W10_hListView = LISTVIEW_Create(0, 30, 320, 180, 0, 1234, WM_CF_SHOW, 0);
   // LISTVIEW_SetFont(_hListView,&GUI_Font24_ASCII);
    
    
     GUI_Exec();
     hHeader = LISTVIEW_GetHeader(W10_hListView);

     WM_SetFocus(W10_hListView);
    LISTVIEW_SetGridVis(W10_hListView,1);//网格可见
    //LISTVIEW_SetFont(_hListView,GUI_FONT_HZ24);

    GUI_SetBkColor(GUI_WHITE);
    GUI_SetColor(GUI_BLACK);  
    HEADER_SetFont(hHeader,GUI_FONT_HZ24);
    LISTVIEW_AddColumn(W10_hListView, 49, "地址",         GUI_TA_VCENTER);
     LISTVIEW_AddColumn(W10_hListView, 49, "在线",         GUI_TA_VCENTER);
   
    LISTVIEW_AddColumn(W10_hListView, 72, "位置", GUI_TA_VCENTER);

    LISTVIEW_AddColumn(W10_hListView, 48, "区号", GUI_TA_VCENTER);
    LISTVIEW_AddColumn(W10_hListView, 115, "类型", GUI_TA_VCENTER);
     
    // SCROLLBAR_CreateAttached(_hListView, SCROLLBAR_CF_VERTICAL);//创建附加至窗口的SCROLLBAR
     HEADER_SetTextColor(hHeader, GUI_BLUE);
    LISTVIEW_SetDefaultBkColor(hHeader,GUI_BLUE) ;
    LISTVIEW_SetFont(W10_hListView,GUI_FONT_HZ24);
    Dis_Device_(&device_inf);
    for(i = 0;i < 16;i++)
    {
        LISTVIEW_AddRow(W10_hListView, (const GUI_ConstString *)&_aTable_1[i][0]);
        
    }
      GUI_Exec();
     
        GUI_Exec();
    for(i = 0;i <= select;i++)
    {
      LISTVIEW_IncSel(W10_hListView);
    }
      GUI_Exec();
 
}


#if 0
static void Dis_AllDevice(uint8_t page_num)
{
     int i,j,devicenum;
     devicenum = 0;
//    device_total = 16;
    GUI_SetBkColor(GUI_WHITE);
    GUI_SetColor(GUI_BLACK);    
    GUI_SetFont(GUI_FONT_HZ24);
    GUI_DispStringAt("在线：",10,95);
    GUI_SetFont(&GUI_Font24_1);
    GUI_DispDecAt(device_total,80,95,2);
    GUI_DispStringAt("<1/6>",250,95);
    GUI_DispDecAt(page_num+1,260,95,1);
        for(i = 1;i < 4;i++)
        {
            
//            if((page_num*3+i) >device_total)
//            {
//                return;
//            }
            norflash_read_device(device_allindex[page_num*3+i-1],&device_inf);
            if(device_inf.DeviceCondition == 0)
            {
                device_inf.DeviceType = 255;
            }
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetColor(GUI_BLACK);
            //显示编号
            GUI_SetFont(&GUI_Font24_1);
            GUI_DispDecAt(device_allindex[page_num*3+i-1], 10,95+27*i, 3);
                //显示区号
            if(device_inf.DeviceZone > 9)
            {
                device_inf.DeviceZone = 0;
            }
            GUI_DispDecAt(device_inf.DeviceZone, 180,95+27*i, 1);
            //显示类型编号
        //    GUI_DispDecAt(device_inf.DeviceType/100, 225,65, 1);
        //    GUI_DispDecAt((device_inf.DeviceType/10)%10, 235,65, 1);
        //    GUI_DispDecAt(device_inf.DeviceType%10, 235,65, 1);
            //显示设备类型
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetColor(GUI_BLACK);    
            GUI_SetFont(GUI_FONT_HZ24);
            if(device_inf.DeviceType >= DEIVCE_TYPE_NUM)
            {
                device_inf.DeviceType = DEIVCE_TYPE_NUM-1;
            }
            GUI_DispStringAt(device_type1[device_inf.DeviceType].caption, 260,95+27*i);
            //显示位置
            if(device_inf.DeviceCaption >=(DEVICE_CAPTION_NUM-1))
            {
                device_inf.DeviceCaption = 0;
            }
            GUI_DispStringAt(device_caption1[device_inf.DeviceCaption], 95,95+27*i);

            //显示设备类型 编号
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetColor(GUI_BLACK);  
            GUI_SetFont(&GUI_Font24_1); 
            GUI_DispDecAt(device_type1[device_inf.DeviceType].type, 225,95+27*i, 3);
  
  
   }    
}

//显示编辑的器件
static void Dis_Device(uint8_t num)
{
    
    if(device_inf.DeviceCondition == 0)
    {
        device_inf.DeviceType = 255;
    }
    GUI_SetBkColor(GUI_WHITE);
	GUI_SetColor(GUI_BLACK);
    //显示编号
	GUI_SetFont(&GUI_Font24_1);
        //显示区号
    if(device_inf.DeviceZone > 9)
    {
        device_inf.DeviceZone = 0;
    }
    GUI_DispDecAt(device_inf.DeviceZone, 180,65, 1);
    //显示类型编号
//    GUI_DispDecAt(device_inf.DeviceType/100, 225,65, 1);
//    GUI_DispDecAt((device_inf.DeviceType/10)%10, 235,65, 1);
//    GUI_DispDecAt(device_inf.DeviceType%10, 235,65, 1);
    //显示设备类型
    GUI_SetBkColor(GUI_WHITE);
	GUI_SetColor(GUI_BLACK);    
    GUI_SetFont(GUI_FONT_HZ24);
    if(device_inf.DeviceType >= DEIVCE_TYPE_NUM)
    {
        device_inf.DeviceType = DEIVCE_TYPE_NUM-1;
    }
    GUI_DispStringAt(device_type1[device_inf.DeviceType].caption, 260,65);
    //显示位置
    if(device_inf.DeviceCaption >=(DEVICE_CAPTION_NUM-1))
    {
        device_inf.DeviceCaption = 0;
    }
    GUI_DispStringAt(device_caption1[device_inf.DeviceCaption], 95,65);

    //显示设备类型 编号
    GUI_SetBkColor(GUI_WHITE);
	GUI_SetColor(GUI_BLACK);  
    GUI_SetFont(&GUI_Font24_1); 
    GUI_DispDecAt(device_type1[device_inf.DeviceType].type/100, 225,65, 1);
    GUI_DispDecAt((device_type1[device_inf.DeviceType].type/10)%10, 235,65, 1);
    GUI_DispDecAt(device_type1[device_inf.DeviceType].type%10, 245,65, 1);    
}

#endif
void W10_EditDeivceWindowDisplay(void)
{
//    device_num = 1;
//    select = 0;
//    select_num = 0;
//    device_inf.DeviceType = 0;
//    norflash_read_device(device_num,&device_inf);
//    Edit_Device(select_num,select);
//    Dis_AllDevice(0);
    select = last_select;
    Dis_Tabel();
}

void W10_EditDeivceWindowProcess(void)
{
//    int i;
    switch (WinKeyValue )
    {
        case KEY_OK:
             last_select = select;
             LISTVIEW_Delete(W10_hListView);
             WinLoad(25);//进入器件编辑界面

            break;

        case KEY_UP:
            
         LISTVIEW_DecSel(W10_hListView);
            GUI_Exec();
           if(select > 0)
           {
               select--;
           }


            break;

        case KEY_DOWN:
            LISTVIEW_IncSel(W10_hListView);
            GUI_Exec();
             if(select < 15)
             {
                 select++;
             }

            break;

        case KEY_SLIENCE:
            device_num = 0;
            select = 0;
            LISTVIEW_Delete(W10_hListView);
            GUI_Exec();
            break;

        case KEY_FIRE:

            break;

        default:

            break;





    }
}

