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
static uint8_t device_num = 0;//�豸���
uint8_t device_total = 0;//��������ҳ��
uint8_t device_allindex[16];//�洢�����������
//static uint8_t page_num = 0;
  char * _aTable_1[18][5] = {
    { "01", "NO", "����","1","�̸�"},
    { "02", "NO", "����","1","�̸�"},
    { "03", "NO", "����","1","�̸�"},
    { "04", "NO", "����","1","�̸�"},
    { "05", "NO", "����","1","�̸�"},
    { "06", "NO", "����","1","�̸�"},
    { "07", "NO", "����","1","�̸�"},
    { "08", "NO", "����","1","�̸�"},
    { "09", "NO", "����","1","�̸�"},
    { "10", "NO", "����","1","�̸�"},
    { "11", "NO", "����","1","�̸�"},
    { "12", "NO", "����","1","�̸�"},
    { "13", "NO", "����","1","�̸�"},
    { "14", "NO", "����","1","�̸�"},
    { "15", "NO", "����","1","�̸�"},
    { "16", "NO", "����","1","�̸�"},
    { "17", "NO", "����","1","�̸�"},
    { "18", "NO", "����","1","�̸�"},
  };
 const char event__type[23][16]= {
   {"����"},//0
   {"����"},//1
   {"����"},//2
   {"���ϻָ�"},//3
   {"ϵͳ"},//4
   {"����"},//5
   {"ֹͣ"},//6
   {"����"},//7
   {"��������"},//8
   {"����"},//9
   {"���ν��"},//10
   {"���"},//11
   {"��ܽ��"},//12
   {"��λ"},    //13
   {"�ػ�"},    //14 
   {"����"},    //15
   {"����"},    //16
   {"��·��·"},//17
   {"��·��·�ָ�"},//18
   {"�������"},//19
   {"������ϻָ�"},//20
   {"�������"},//21
   {"������ϻָ�"},//22

};
const char device_caption1[DEVICE_CAPTION_NUM][10] = 
{
    {"����"},
    {"������"},
    {"����"},
    {"�鷿"},
    {"��̨"},
};
const DEVICE_TYPE device_type1[] =
{
    {DEVICE_POINT_PY_SMOKE_FIRE_DETECTOR,"�̸�",0},
    {DEVICE_FEEL_FIRE_DETECTOR,"����",1},
    {DEVICE_HAND_FIRE_DETECTOR,"�ֱ�",2},
    {0xff,"�հ�",3},
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
//�������
static  void Dis_Tabel(void)
{
    char i;
        HEADER_Handle hHeader;



   W10_hListView = LISTVIEW_Create(0, 30, 320, 180, 0, 1234, WM_CF_SHOW, 0);
   // LISTVIEW_SetFont(_hListView,&GUI_Font24_ASCII);
    
    
     GUI_Exec();
     hHeader = LISTVIEW_GetHeader(W10_hListView);

     WM_SetFocus(W10_hListView);
    LISTVIEW_SetGridVis(W10_hListView,1);//����ɼ�
    //LISTVIEW_SetFont(_hListView,GUI_FONT_HZ24);

    GUI_SetBkColor(GUI_WHITE);
    GUI_SetColor(GUI_BLACK);  
    HEADER_SetFont(hHeader,GUI_FONT_HZ24);
    LISTVIEW_AddColumn(W10_hListView, 49, "��ַ",         GUI_TA_VCENTER);
     LISTVIEW_AddColumn(W10_hListView, 49, "����",         GUI_TA_VCENTER);
   
    LISTVIEW_AddColumn(W10_hListView, 72, "λ��", GUI_TA_VCENTER);

    LISTVIEW_AddColumn(W10_hListView, 48, "����", GUI_TA_VCENTER);
    LISTVIEW_AddColumn(W10_hListView, 115, "����", GUI_TA_VCENTER);
     
    // SCROLLBAR_CreateAttached(_hListView, SCROLLBAR_CF_VERTICAL);//�������������ڵ�SCROLLBAR
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
    GUI_DispStringAt("���ߣ�",10,95);
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
            //��ʾ���
            GUI_SetFont(&GUI_Font24_1);
            GUI_DispDecAt(device_allindex[page_num*3+i-1], 10,95+27*i, 3);
                //��ʾ����
            if(device_inf.DeviceZone > 9)
            {
                device_inf.DeviceZone = 0;
            }
            GUI_DispDecAt(device_inf.DeviceZone, 180,95+27*i, 1);
            //��ʾ���ͱ��
        //    GUI_DispDecAt(device_inf.DeviceType/100, 225,65, 1);
        //    GUI_DispDecAt((device_inf.DeviceType/10)%10, 235,65, 1);
        //    GUI_DispDecAt(device_inf.DeviceType%10, 235,65, 1);
            //��ʾ�豸����
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetColor(GUI_BLACK);    
            GUI_SetFont(GUI_FONT_HZ24);
            if(device_inf.DeviceType >= DEIVCE_TYPE_NUM)
            {
                device_inf.DeviceType = DEIVCE_TYPE_NUM-1;
            }
            GUI_DispStringAt(device_type1[device_inf.DeviceType].caption, 260,95+27*i);
            //��ʾλ��
            if(device_inf.DeviceCaption >=(DEVICE_CAPTION_NUM-1))
            {
                device_inf.DeviceCaption = 0;
            }
            GUI_DispStringAt(device_caption1[device_inf.DeviceCaption], 95,95+27*i);

            //��ʾ�豸���� ���
            GUI_SetBkColor(GUI_WHITE);
            GUI_SetColor(GUI_BLACK);  
            GUI_SetFont(&GUI_Font24_1); 
            GUI_DispDecAt(device_type1[device_inf.DeviceType].type, 225,95+27*i, 3);
  
  
   }    
}

//��ʾ�༭������
static void Dis_Device(uint8_t num)
{
    
    if(device_inf.DeviceCondition == 0)
    {
        device_inf.DeviceType = 255;
    }
    GUI_SetBkColor(GUI_WHITE);
	GUI_SetColor(GUI_BLACK);
    //��ʾ���
	GUI_SetFont(&GUI_Font24_1);
        //��ʾ����
    if(device_inf.DeviceZone > 9)
    {
        device_inf.DeviceZone = 0;
    }
    GUI_DispDecAt(device_inf.DeviceZone, 180,65, 1);
    //��ʾ���ͱ��
//    GUI_DispDecAt(device_inf.DeviceType/100, 225,65, 1);
//    GUI_DispDecAt((device_inf.DeviceType/10)%10, 235,65, 1);
//    GUI_DispDecAt(device_inf.DeviceType%10, 235,65, 1);
    //��ʾ�豸����
    GUI_SetBkColor(GUI_WHITE);
	GUI_SetColor(GUI_BLACK);    
    GUI_SetFont(GUI_FONT_HZ24);
    if(device_inf.DeviceType >= DEIVCE_TYPE_NUM)
    {
        device_inf.DeviceType = DEIVCE_TYPE_NUM-1;
    }
    GUI_DispStringAt(device_type1[device_inf.DeviceType].caption, 260,65);
    //��ʾλ��
    if(device_inf.DeviceCaption >=(DEVICE_CAPTION_NUM-1))
    {
        device_inf.DeviceCaption = 0;
    }
    GUI_DispStringAt(device_caption1[device_inf.DeviceCaption], 95,65);

    //��ʾ�豸���� ���
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
             WinLoad(25);//���������༭����

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

