#define W25_DEVICE_SET_WINDOW_WIN_PRIVATE
#include "include.h"
#include "DIALOG.h"
#include "MESSAGEBOX.h"
//static WM_HWIN  hWin;
extern uint8_t         WinIndex;
//static uint8_t device_num = 0;//设备编号
static uint8_t select_num = 0;
static DEVICE_CONFIG device_inf;
extern const DEVICE_TYPE device_type1[]; 
static uint8_t device_select = 0;
extern uint8_t last_select;
extern uint8_t device_total;
extern const char* zonenum[];
static void Edit_Device(uint8_t num,uint8_t sel)
{
    //读取设备信息
   // char arr[20] = {0,};
 	GUI_SetColor(GUI_WHITE);
	GUI_FillRect(10, 65, 50, 90);
	GUI_FillRect(70, 65, 150, 90);
	GUI_FillRect(170, 65, 200, 90);
	GUI_FillRect(220, 65, 315, 90);
    

    //更新设备信息
    GUI_SetBkColor(GUI_WHITE);
	GUI_SetColor(GUI_BLACK);

 
    //显示区号
    if(device_inf.DeviceZone > 6)
    {
        device_inf.DeviceZone = 1;
    }
    GUI_DispDecAt(device_inf.DeviceZone, 180,65, 1);
    //显示类型编号
//    GUI_DispDecAt(device_inf.DeviceType/100, 225,65, 1);
//    GUI_DispDecAt((device_inf.DeviceType/10)%10, 235,65, 1);
//    GUI_DispDecAt(device_inf.DeviceType%10, 235,65, 1);
    //显示是否存在
        GUI_SetFont(&GUI_Font24_1);
      if(device_inf.DeviceCondition == 0xff)
      {
          device_inf.DeviceCondition = 0;
      }
    if(device_inf.DeviceCondition == 0)
      {
          GUI_DispStringAt("NO", 10,65);
      }
      else
      {
         GUI_DispStringAt("YES", 10,65); 
      }    
    //显示设备类型
    GUI_SetBkColor(GUI_WHITE);
	GUI_SetColor(GUI_BLACK);    
    GUI_SetFont(GUI_FONT_HZ24);
    if(device_inf.DeviceType >= DEIVCE_TYPE_NUM)
    {
        device_inf.DeviceType = 0;
    }
    GUI_DispStringAt(device_type1[device_inf.DeviceType].caption, 230,65);
    //显示位置
    if(device_inf.DeviceCaption >=(DEVICE_CAPTION_NUM-1))
    {
        device_inf.DeviceCaption = 0;
    }
    GUI_DispStringAt(device_caption1[device_inf.DeviceCaption], 70,65);

//    GUI_DispStringAt("下页", 203,215);
    //显示设备类型 编号
//    GUI_SetBkColor(GUI_WHITE);
//	GUI_SetColor(GUI_BLACK);  
//    GUI_SetFont(&GUI_Font24_1); 
//    GUI_DispDecAt(device_type1[device_inf.DeviceType].type/100, 225,65, 1);
//    GUI_DispDecAt((device_type1[device_inf.DeviceType].type/10)%10, 235,65, 1);
//    GUI_DispDecAt(device_type1[device_inf.DeviceType].type%10, 245,65, 1);
    switch(sel)
    {

        case 0: 
                //是否在线
                GUI_SetColor(GUI_WHITE);
                GUI_FillRect(10, 65, 50, 90);
                GUI_SetBkColor(GUI_BLUE);
	            GUI_SetColor(GUI_WHITE);
                GUI_SetFont(&GUI_Font24_1);    
//                GUI_DispDecAt(device_num%10, 45,65, 1); 
                if((num == 0)||(num == 0xff))
                {
                    GUI_DispStringAt("NO", 10,65);
                }
                else
                {
                    GUI_DispStringAt("YES", 10,65); 
                }
                device_inf.DeviceCondition = num;
                //norflash_read_device(device_num,&device_inf);
                //Dis_Device(device_num);//刷新设备
                      
            break;
        case 1:  //选择位置
                GUI_SetColor(GUI_WHITE);
                GUI_FillRect(70, 65, 150, 90);
                device_inf.DeviceCaption = num;
                GUI_SetBkColor(GUI_BLUE);
	            GUI_SetColor(GUI_WHITE);
                GUI_SetFont(GUI_FONT_HZ24);
                GUI_DispStringAt(device_caption1[device_inf.DeviceCaption], 70,65);
               

            break;
        case 2://选择区号
                if(num == 0)
                   num = 1;
                GUI_SetColor(GUI_WHITE);
                GUI_FillRect(170, 65, 200, 90);
                device_inf.DeviceZone = num;
                GUI_SetBkColor(GUI_BLUE);
	            GUI_SetColor(GUI_WHITE);
                GUI_SetFont(&GUI_Font24_1);   
                GUI_DispStringAt(zonenum[device_inf.DeviceZone-1], 180,65); 
                

            break;
        case 3://选择设备类型
                GUI_SetColor(GUI_WHITE);
                GUI_FillRect(220, 65, 315, 90);
                device_inf.DeviceType = num;
                GUI_SetBkColor(GUI_BLUE);
	            GUI_SetColor(GUI_WHITE);
//                GUI_SetFont(&GUI_Font24_1); 
//                GUI_DispDecAt(device_type1[device_inf.DeviceType].type/100, 225,65, 1);
//                GUI_DispDecAt((device_type1[device_inf.DeviceType].type/10)%10, 235,65, 1);
//                GUI_DispDecAt(device_type1[device_inf.DeviceType].type%10, 245,65, 1);
                GUI_SetFont(GUI_FONT_HZ24); 
                GUI_DispStringAt(device_type1[device_inf.DeviceType].caption, 230,65);
                

            break;


          
        default:
            break;
    }
//          GUI_SetBkColor(GUI_LIGHTGRAY);
//       GUI_SetColor(GUI_WHITE); 
//    GUI_DispStringAt("按功能键编辑",10,215);
//    sprintf(arr,"%d-%d-%d %d:%d:%d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
//	GUI_SetFont(&GUI_Font20_1);
//    GUI_DispStringAt(arr,170,217);  

}
void W25_DeviceSetWindowDisplay(void)
{

    device_select = 0;
  
//    device_inf.DeviceType = 0;
    norflash_read_device(last_select+2,&device_inf);
    select_num = device_inf.DeviceCondition;
    Edit_Device(select_num,device_select);
}

void W25_DeviceSetWindowProcess(void)
{
    switch (WinKeyValue )
    {
        case KEY_OK:
             if(device_select < 5)
            {
             device_select++;
            }
            if(device_select == 4)//保存
            {
                device_select = 0;
                GUI_SetColor(GUI_BLUE);
                GUI_SetBkColor(GUI_GRAY);
                GUI_SetFont(GUI_FONT_HZ24);
                GUI_DispStringAt("保存成功！", 10,210);
               norflash_write_device(last_select+2,&device_inf.DeviceCondition);
            }

            switch(device_select)
            {

                case 0:
                    select_num = device_inf.DeviceCondition;
                    break;
                case 1:
                    select_num = device_inf.DeviceCaption;
                    break;
                case 2:
                    select_num = device_inf.DeviceZone;
                    break;
                case 3:
                    select_num = device_inf.DeviceType;       
                    break;
            }


            Edit_Device(select_num,device_select);
            WinKeyValue = NULL;
            break;

        case KEY_UP:
             if(device_select == 0) //是否在线
             {
                 if(select_num == 0)
                 {
                     select_num = 1;
                 }
                 else
                 {
                     select_num = 0;
                 }
             }
             else if(device_select == 1)//位置
             {
                 if(select_num == 0)
                 {
                     select_num = DEVICE_CAPTION_NUM-1;
                 }
                 else 
                 {
                     select_num--;
                 }
             }
             else if(device_select == 2)//区号
             {
                 if(select_num >=5)
                {
                    select_num = 0;
                }
                else
                {
                    select_num++;
                }

             }
             else if(device_select == 3)//设备类型
             {
                 if(select_num == 0)
                 {
                     select_num = DEIVCE_TYPE_NUM-1;
                 }
                 else
                 {
                     select_num--;
                 }
             }
             Edit_Device(select_num,device_select);
             WinKeyValue = NULL;
            break;

        case KEY_DOWN:
            if(device_select == 0)//是否在线
            {
                if(select_num == 1)
                 {
                    select_num = 0;
                 }
                 else
                 {
                    select_num = 1;
                 }
            }
            else if(device_select == 1)//位置
            {
                if(select_num >= (DEVICE_CAPTION_NUM-1))
                {
                    select_num = 0;
                }
                else
                {
                    select_num++;
                }
            }
            else if(device_select == 2)//区号
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
            else if(device_select == 3)//类型
            {
                 if(select_num >=(DEIVCE_TYPE_NUM-1))
                {
                    select_num = 0;
                }
                else
                {
                    select_num++;
                }
            }
            Edit_Device(select_num,device_select);
            WinKeyValue = NULL;
            break;

        case KEY_SLIENCE:
             //WM_DeleteWindow(hWin);
            break;

        case KEY_FIRE:

            break;

        default:

            break;





    }
}

