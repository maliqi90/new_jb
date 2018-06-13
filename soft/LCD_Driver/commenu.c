#include "GUI.h"
#include "PROGBAR.h"
#include <stddef.h>
#include <jbfont.h>
#include <newfont.h>
#include "guzhang.h"
#include "huojing.h"
#include "new1font.h"
#include "began.h"
#include "power.h"
#include "delay.h"
#include "DIALOG.h"
//#include "DIALOG.h"
extern uint8_t wav_buf[1024];
WM_HWIN  frme; 
//static uint32_t nWritten = 0;
extern uint8_t         WinIndex;
extern uint8_t device_total;
extern uint8_t last_select;
//   static uint32_t address__ = 0x5A2000;
//   static uint16_t  temp__ = 0;
//#define ID_WINDOW_0 (GUI_ID_USER + 0x00)
//#define ID_SLIDER_0 (GUI_ID_USER + 0x02)

///*********************************************************************
//*
//*       _aDialogCreate
//*/
//static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
//  /*{ WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 320, 240, 0, 0x0, 0 },*/
//  { SLIDER_CreateIndirect, "Slider", ID_SLIDER_0, 102, 106, 80, 20, 0, 0x0, 0 },
//  // USER START (Optionally insert additional widgets)
//  // USER END
//};
///*********************************************************************
//*
//*       _cbDialog
//*/
//static void _cbDialog(WM_MESSAGE * pMsg) {
//  int NCode;
//  int Id;
//  // USER START (Optionally insert additional variables)
//  // USER END

//  switch (pMsg->MsgId) {
//  case WM_NOTIFY_PARENT:
//    Id    = WM_GetId(pMsg->hWinSrc);
//    NCode = pMsg->Data.v;
//    switch(Id) {
//    case ID_SLIDER_0: // Notifications sent by 'Slider'
//      switch(NCode) {
//      case WM_NOTIFICATION_CLICKED:
//        // USER START (Optionally insert code for reacting on notification message)
//        // USER END
//        break;
//      case WM_NOTIFICATION_RELEASED:
//        // USER START (Optionally insert code for reacting on notification message)
//        // USER END
//        break;
//      case WM_NOTIFICATION_VALUE_CHANGED:
//        // USER START (Optionally insert code for reacting on notification message)
//        // USER END
//        break;
//      // USER START (Optionally insert additional code for further notification handling)
//      // USER END
//      }
//      break;
//    // USER START (Optionally insert additional code for further Ids)
//    // USER END
//    }
//    break;
//  // USER START (Optionally insert additional message handling)
//  // USER END
//  default:
//    WM_DefaultProc(pMsg);
//    break;
//  }
//}
///*********************************************************************
//*
//*       CreateWindow
//*/
//WM_HWIN CreateWindow(void);
//WM_HWIN CreateWindow(void) {
//  WM_HWIN hWin;

//  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
//  return hWin;
//}


uint8_t slienceflag = 0;
uint8_t voice_flag = 0;
//volatile int lcd_time = 0;
uint8_t Singal_num = 5;
uint8_t Power_num  = 3;
/* 实际的测试需要是图像宽度的4倍即可，切记(也就是保证每个像素如果是32位数据的情况) */
static char _acBuffer[480*4];
 void _ShowBMPEx(uint8_t bmp,int x,int y); 
//显示国泰怡安图片 相关文字
//void began(void)
//{
// GUI_Clear();
// GUI_SetBkColor(GUI_WHITE);  
// GUI_Clear();
// GUI_DrawBitmap(&bmbegan,30,10);

// //设置字体格式 颜色 对应字库文件
// GUI_SetFont(&GUI_Fontpower);
// GUI_SetColor(GUI_BLACK);
// GUI_UC_SetEncodeUTF8();
// //显示国泰怡安家庭报警控制器
//	GUI_DispStringAt((const char *)gtyajtbjkzq1,170,85);
// //显示设备登录中
// GUI_DispStringAt((const char *)sbdlz,120,175);
//}
extern void MainTask(void);
 void began(void)
{
 GUI_Clear();
 GUI_SetBkColor(GUI_WHITE);  
 GUI_Clear();
// GUI_DrawBitmap(&bmbegan,30,10);
 GUI_SetColor(GUI_BLACK);


 GUI_SetFont(GUI_FONT_HZ24);
 GUI_DispStringAt("家用火灾报警控制",100,100);
 GUI_DispStringAt("设备登录中",120,175);


 
}
//火警事件查询
void dis_fireevent(void)
{
    char arr[20]={0};
    //静态框架
    GUI_Clear();
	GUI_SetColor(GUI_GRAY);
	GUI_FillRect(0, 0, 320, 40);
    GUI_SetColor(GUI_LIGHTGRAY);
	GUI_FillRect(0, 35, 320, 240);

	//显示表头设置 白字灰底
    GUI_SetColor(GUI_WHITE);
	GUI_SetBkColor(GUI_GRAY);
    GUI_SetFont(GUI_FONT_HZ24);
    GUI_DispStringAt("火警记录查询",10,6);


	//划线
	GUI_SetFont(&GUI_Font8_1);
	GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_LIGHTGRAY);
    //GUI_DispStringAt("-----------------------------------------------",20,75);
    GUI_DispStringAt("-----------------------------------------------",20,115);
//	GUI_DispStringAt("-----------------------------------------------",20,155);
	GUI_DispStringAt("-----------------------------------------------",20,195);    
            //显示时间
    //GUI_SetBkColor(GUI_WHITE);
    GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font20_1);
	sprintf(arr,"%d-%02d-%02d %02d:%02d:%02d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
    GUI_DispStringAt(arr,150,217);
}
//故障事件查寻
void dis_faultevent(void)
{
    	char arr[20]={0};
    //静态框架
    GUI_Clear();
	GUI_SetColor(GUI_GRAY);
	GUI_FillRect(0, 0, 320, 40);
    GUI_SetColor(GUI_LIGHTGRAY);
	GUI_FillRect(0, 35, 320, 240);

	//显示表头设置 白字灰底
    GUI_SetColor(GUI_WHITE);
	GUI_SetBkColor(GUI_GRAY);
    GUI_SetFont(GUI_FONT_HZ24);
    GUI_DispStringAt("故障记录查询",10,6);


	//划线
	GUI_SetFont(&GUI_Font8_1);
	GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_LIGHTGRAY);
    //GUI_DispStringAt("-----------------------------------------------",20,75);
    GUI_DispStringAt("-----------------------------------------------",20,115);
//	GUI_DispStringAt("-----------------------------------------------",20,155);
	GUI_DispStringAt("-----------------------------------------------",20,195);    
                //显示时间
   // GUI_SetBkColor(GUI_WHITE);
    GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font20_1);
	sprintf(arr,"%d-%02d-%02d %02d:%02d:%02d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
    GUI_DispStringAt(arr,150,217);
}
//屏蔽事件查询
void dis_sheildevent(void)
{
    	char arr[20]={0};
    //静态框架
    GUI_Clear();
	GUI_SetColor(GUI_GRAY);
	GUI_FillRect(0, 0, 320, 40);
    GUI_SetColor(GUI_LIGHTGRAY);
	GUI_FillRect(0, 35, 320, 240);

	//显示表头设置 白字灰底
    GUI_SetColor(GUI_WHITE);
	GUI_SetBkColor(GUI_GRAY);
    GUI_SetFont(GUI_FONT_HZ24);
    GUI_DispStringAt("屏蔽记录查询",10,6);


	//划线
	GUI_SetFont(&GUI_Font8_1);
	GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_LIGHTGRAY);
    //GUI_DispStringAt("-----------------------------------------------",20,75);
    GUI_DispStringAt("-----------------------------------------------",20,115);
//	GUI_DispStringAt("-----------------------------------------------",20,155);
	GUI_DispStringAt("-----------------------------------------------",20,195);    
                //显示时间
    //GUI_SetBkColor(GUI_WHITE);
    GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font20_1);
	sprintf(arr,"%d-%02d-%02d %02d:%02d:%02d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
    GUI_DispStringAt(arr,150,217);
}
//事件记录查询
void dis_devent(void)
{
    	char arr[20]={0};
    //静态框架
    GUI_Clear();
	GUI_SetColor(GUI_GRAY);
	GUI_FillRect(0, 0, 320, 40);
    GUI_SetColor(GUI_LIGHTGRAY);
	GUI_FillRect(0, 35, 320, 240);

	//显示表头设置 白字灰底
    GUI_SetColor(GUI_WHITE);
	GUI_SetBkColor(GUI_GRAY);
    GUI_SetFont(GUI_FONT_HZ24);
    GUI_DispStringAt("事件记录查询",10,6);


	//划线
	GUI_SetFont(&GUI_Font8_1);
	GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_LIGHTGRAY);
    //GUI_DispStringAt("-----------------------------------------------",20,75);
    GUI_DispStringAt("-----------------------------------------------",20,115);
//	GUI_DispStringAt("-----------------------------------------------",20,155);
	GUI_DispStringAt("-----------------------------------------------",20,195);    
     //   GUI_SetBkColor(GUI_WHITE);
    GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font20_1);
	sprintf(arr,"%d-%02d-%02d %02d:%02d:%02d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
    GUI_DispStringAt(arr,150,217);
}
//背光亮度调节界面
void Dis_Light_Set(void)
{
    	char arr[20]={0};
   //静态框架
    GUI_Clear();

	GUI_SetColor(GUI_GRAY);
	GUI_FillRect(0, 0, 320, 40);
    GUI_SetColor(GUI_LIGHTGRAY);
	GUI_FillRect(0, 35, 320, 240);

//	//显示表头主菜单 白字灰底
    GUI_SetColor(GUI_WHITE);
	GUI_SetBkColor(GUI_GRAY);
    GUI_SetFont(GUI_FONT_HZ24);
//	GUI_UC_SetEncodeUTF8();
    GUI_DispStringAt("背光亮度调节",10,6);
	GUI_SetBkColor(GUI_LIGHTGRAY);
	//GUI_SetColor(GUI_BLACK); 
    GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font20_1);
	sprintf(arr,"%d-%02d-%02d %02d:%02d:%02d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
    GUI_DispStringAt(arr,150,217);   

}
//显示恢复出厂设置界面
void Dis_ClearFlash(void)
{
    char arr[20]={0};
   //静态框架
    GUI_Clear();
	GUI_SetColor(GUI_GRAY);
	GUI_FillRect(0, 0, 320, 40);
    GUI_SetColor(GUI_LIGHTGRAY);
	GUI_FillRect(0, 35, 320, 240); 
	//显示表头主菜单 白字灰底
    GUI_SetColor(GUI_WHITE);
	GUI_SetBkColor(GUI_GRAY); 
    GUI_SetFont(GUI_FONT_HZ24); 
    GUI_DispStringAt("恢复出厂设置",10,6);  
    
    GUI_SetColor(GUI_BLACK);
	GUI_SetBkColor(GUI_LIGHTGRAY);
    GUI_SetFont(GUI_FONT_HZ24); 
    GUI_DispStringAt("恢复出厂设置",70,60);
	GUI_SetFont(&GUI_Font24_ASCII);
    GUI_DispStringAt("?",220,60);
    
     GUI_SetColor(GUI_BLACK);
	GUI_SetBkColor(GUI_LIGHTGRAY);
    GUI_SetFont(GUI_FONT_HZ24); 
    GUI_DispStringAt("确定",50,120);
	GUI_DispStringAt("取消",220,120);
	GUI_SetFont(&GUI_Font24_ASCII);
    GUI_DispStringAt("[",45,120);
	GUI_DispStringAt("]",103,120);

	GUI_DispStringAt("[",215,120);
	GUI_DispStringAt("]",268,120);
     GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font20_1);
	sprintf(arr,"%d-%02d-%02d %02d:%02d:%02d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
    GUI_DispStringAt(arr,150,217);     


     
}
//显示进度条
 void DemoProgBar(void) {
  int i;
  //创建两个进度条句柄
  PROGBAR_Handle ahProgBar[1];
  //设置坐标位置
  ahProgBar[0] = PROGBAR_Create(50,200,220,20, WM_CF_SHOW);
  //使能进度条
  PROGBAR_EnableMemdev(ahProgBar[0]);
  //设置进度条一的字体
  PROGBAR_SetFont(ahProgBar[0], &GUI_Font8x16);
  //gui延时
  GUI_LCD_delay(500);
  //进度条配置完毕
  //设置进度条一的字体
  PROGBAR_SetFont(ahProgBar[0], &GUI_Font8x16);
  PROGBAR_SetBarColor(ahProgBar[0], 0, GUI_GRAY);
  PROGBAR_SetBarColor(ahProgBar[0], 1, GUI_LIGHTGRAY);
  //如果第二个参数选择NULL 默认显示百分比
  PROGBAR_SetText(ahProgBar[0], NULL);
  //向进度条内写入数值
    for (i=0; i<=100; i++) {
      PROGBAR_SetValue(ahProgBar[0], i);
      GUI_LCD_delay(50);
    }
    GUI_LCD_delay(500);
    WM_DeleteWindow(ahProgBar[0]);
		GUI_Clear();
		GUI_LCD_delay(100);
}
//信号显示
void Dis_Signal(uint8_t num,GUI_COLOR color)
{
  GUI_SetColor(color);
 GUI_FillRect(290, 0, 315, 25); //??????
 GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
 GUI_SetColor(GUI_WHITE);
//GUI_SetPenSize(10);
    switch(num)
    {
        case 0:
             GUI_SetBkColor(color);
             GUI_SetFont(&GUI_Font8_ASCII);
             GUI_DispStringAt("NoSig",290,10);
            break;
        case 1:
             GUI_DrawVLine(295,23,25);  
        break;
        case 2:
             GUI_DrawVLine(295,23,25); 
             GUI_DrawVLine(300,20,25);
        break;
        case 3:
             GUI_DrawVLine(295,23,25); 
             GUI_DrawVLine(300,20,25); 
             GUI_DrawVLine(305,15,25); 
        break;  
        case 4:
             GUI_DrawVLine(295,23,25); 
             GUI_DrawVLine(300,20,25); 
             GUI_DrawVLine(305,15,25);                     
             GUI_DrawVLine(310,10,25);
         break;  
        case 5:
             GUI_DrawVLine(295,23,25); 
             GUI_DrawVLine(300,20,25); 
             GUI_DrawVLine(305,15,25);                     
             GUI_DrawVLine(310,10,25);
             GUI_DrawVLine(315,5,25);        
        break;    
        case 6:
             GUI_SetBkColor(color);
             GUI_SetFont(&GUI_Font8_ASCII);
             GUI_DispStringAt("NoSim",290,10);
            break;
        default:
            break;        
        
        
    }




 
}
//电量显示
void Dis_Power(uint8_t num,GUI_COLOR color)
{
     GUI_SetColor(color);
     GUI_FillRect(245, 10, 285, 25); //清除电池电量
     GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
     GUI_SetColor(GUI_WHITE);
     GUI_FillRect(280, 13, 285, 22);
    //GUI_SetPenSize(10);
//     GUI_DrawVLine(315,5,25);
//     GUI_DrawVLine(310,10,25);
//     GUI_DrawVLine(305,15,25);
//     GUI_DrawVLine(300,20,25);
//     GUI_DrawVLine(295,23,25);
     
    //GUI_DrawVLine(310,20,30);
    //GUI_DrawVLine(320,30,25);

    //画电池电量线外框
    GUI_DrawVLine(245,10,25);
    GUI_DrawVLine(285,13,22);

    GUI_DrawHLine(10,245,280);
    GUI_DrawHLine(25,245,280);

    GUI_DrawVLine(280,10,13);
    GUI_DrawVLine(280,22,25);

    GUI_DrawHLine(13,280,285);
    GUI_DrawHLine(22,280,285);
    switch(num)
    {
        case 0:
            break;
        case 1:
            GUI_FillRect(245, 10, 250, 25);
            break;
        case 2:
            GUI_FillRect(245, 10, 260, 25);
            break;
       
        case 3:
            GUI_FillRect(245, 10, 270, 25);
            break;
       
        case 4:
            GUI_FillRect(245, 10, 280, 25);
        break;
        default:
            break;
            
            
        
                
    }        
}
void Stay_Flag(void)
{
 char arr[20]={0};
//????
  GUI_SetBkColor(GUI_LIGHTBLUE);
  GUI_Clear();
  //????????
  GUI_SetFont(GUI_FONT_HZ24);
  //GUI_SetFont(&GUI_Fontjbfont);
  GUI_SetColor(GUI_WHITE);
  //GUI_UC_SetEncodeUTF8();
 GUI_DispStringAt("家用火灾报警控制器",45,40);
//  GUI_DispStringAt(gtyajtbjkzq,20,40);
 GUI_DispStringAt("监控中",120,155);
//  GUI_DispStringAt(jkz,110,155);
   GUI_SetFont(&GUI_Font16_1);
  GUI_DispStringAt("=================================",20,65);

   _ShowBMPEx(PIC_STAY1,110,75);

   Dis_Power(4,GUI_LIGHTBLUE);
  // Dis_Signal(0,GUI_LIGHTBLUE);
  // sprintf(arr,"%d-%d-%d %d:%d:%d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
	GUI_SetFont(&GUI_Font20_1);
	sprintf(arr,"%d-%02d-%02d %02d:%02d:%02d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
    GUI_DispStringAt(arr,150,217);
//	HEX2ASCII(Timer_value.year/100,arr);
//  GUI_DispStringAt(arr,120,217);
//	HEX2ASCII(Timer_value.year%100,arr);
//  GUI_DispStringAt(arr,144,217);
//	HEX2ASCII(Timer_value.month,arr);
//	GUI_DispStringAt(arr,174,217);
//	HEX2ASCII(Timer_value.date,arr);
//	GUI_DispStringAt(arr,204,217);
//	HEX2ASCII(Timer_value.hour,arr);
//	GUI_DispStringAt(arr,234,217);
//	HEX2ASCII(Timer_value.min,arr);
//  GUI_DispStringAt(arr,264,217);
//	HEX2ASCII(Timer_value.sec,arr);
//	GUI_DispStringAt(arr,294,217);
//	
//	GUI_DispStringAt("-",168,217);
////	GUI_LCD_delay(300);
//	GUI_DispStringAt("-",198,217);
////	GUI_LCD_delay(300);
//	GUI_DispStringAt(" ",228,217);
////	GUI_LCD_delay(300);
//  GUI_DispStringAt(":",258,217);
////	GUI_LCD_delay(300);
//  GUI_DispStringAt(":",288,217);
//  GUI_LCD_delay(500);
//	//GUI_LCD_delay(500);
  _ShowBMPEx(PIC_LOGO,5,210);
  Time_interr=1;
    RTC_ITConfig(RTC_IT_SEC, ENABLE);	//使能秒中断
   if(EventAlarmCount !=0)
   {
     _ShowBMPEx(PIC_FIRE,2,2);  
   }
    if(EventFaultCount !=0)
   {
     _ShowBMPEx(PIC_FAULT,30,2);  
   }
   if(slienceflag == 1)
   {
      _ShowBMPEx(PIC_SLIENCE,55,2); //消音图标
   }
}

void menudis(void)
{
     	char arr[20]={0};
   //静态框架
    GUI_Clear();
	GUI_SetColor(GUI_GRAY);
	GUI_FillRect(0, 0, 320, 40);
    GUI_SetColor(GUI_LIGHTGRAY);
	GUI_FillRect(0, 35, 320, 240);
//    Dis_Signal(5,GUI_GRAY);//显示信号
//    Dis_Power(4,GUI_GRAY);//显示电量
	//显示表头主菜单 白字灰底
    GUI_SetColor(GUI_WHITE);
	GUI_SetBkColor(GUI_GRAY);
     GUI_SetFont(GUI_FONT_HZ24);
//  GUI_SetFont(&GUI_Fontjbfont);
	//GUI_UC_SetEncodeUTF8();
  //GUI_DispStringAt((const char *)zcd,10,6);
    GUI_DispStringAt("主菜单",10,6);
	
	//划线
	GUI_SetFont(&GUI_Font8_1);
	GUI_SetColor(GUI_BLACK);
  GUI_SetBkColor(GUI_LIGHTGRAY);
  GUI_DispStringAt("-----------------------------------------------",20,75); 
  GUI_DispStringAt("-----------------------------------------------",20,115);
	GUI_DispStringAt("-----------------------------------------------",20,155);
	GUI_DispStringAt("-----------------------------------------------",20,195);
     
	//显示四项需要设置的内容
  GUI_SetColor(GUI_BLACK);
	GUI_SetBkColor(GUI_LIGHTGRAY);
   GUI_SetFont(GUI_FONT_HZ24);
	//GUI_UC_SetEncodeUTF8();
    GUI_DispStringAt("设置",20,50);
    GUI_DispStringAt("查询",20,90);
    GUI_DispStringAt("自检",20,130);
    GUI_DispStringAt("自动登录",20,170);
     GUI_SetFont(&GUI_Font24_1);
	GUI_DispStringAt("<1/2>",30,215);   
      // sprintf(arr,"%d-%d-%d %d:%d:%d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
	GUI_SetFont(&GUI_Font20_1);
	sprintf(arr,"%d-%02d-%02d %02d:%02d:%02d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
    GUI_DispStringAt(arr,150,217);
}
void menu(uint8_t pointer)
{


   switch(pointer)
  {
    case 0:
           //释放查询选中
           GUI_SetColor(GUI_LIGHTGRAY);
	       GUI_FillRect(20, 80, 300, 115);
           GUI_SetColor(GUI_BLACK);
	       GUI_SetBkColor(GUI_LIGHTGRAY);
            GUI_SetFont(GUI_FONT_HZ24);
	       //GUI_UC_SetEncodeUTF8();
          GUI_DispStringAt("查询",20,90);
           //GUI_DispStringAt((const char *)cx,20,90);
           //设置被选中
           GUI_SetColor(GUI_GRAY);
	       GUI_FillRect(20, 40, 300, 75); 
	       GUI_SetColor(GUI_WHITE);
	       GUI_SetBkColor(GUI_GRAY);
            GUI_SetFont(GUI_FONT_HZ24);
	       //GUI_UC_SetEncodeUTF8();
            GUI_DispStringAt("设置",20,50);
           //GUI_DispStringAt((const char *)sz,20,50);
    

        break;
    case 1:
        //释放自检选中
           GUI_SetColor(GUI_LIGHTGRAY);
	       GUI_FillRect(20, 120, 300, 155);
           GUI_SetColor(GUI_BLACK);
	       GUI_SetBkColor(GUI_LIGHTGRAY);
            GUI_SetFont(GUI_FONT_HZ24);
	       //GUI_UC_SetEncodeUTF8();
           GUI_DispStringAt("自检",20,130);
        //释放设置选中
    	  GUI_SetColor(GUI_LIGHTGRAY);
	      GUI_FillRect(20, 40, 300, 75);
          GUI_SetColor(GUI_BLACK);
	      GUI_SetBkColor(GUI_LIGHTGRAY);
           GUI_SetFont(GUI_FONT_HZ24);
//	      GUI_UC_SetEncodeUTF8();
          GUI_DispStringAt("设置",20,50);
	      //查询被选中
	       GUI_SetColor(GUI_GRAY);
	       GUI_FillRect(20, 80, 300, 115);
	       GUI_SetColor(GUI_WHITE);
	       GUI_SetBkColor(GUI_GRAY);
            GUI_SetFont(GUI_FONT_HZ24);
//	       GUI_UC_SetEncodeUTF8();
           GUI_DispStringAt("查询",20,90);


        break;
    case 2:
          //自动登录被释放
           GUI_SetColor(GUI_LIGHTGRAY);
	       GUI_FillRect(20, 160, 300, 195);
           GUI_SetColor(GUI_BLACK);
	       GUI_SetBkColor(GUI_LIGHTGRAY);
            GUI_SetFont(GUI_FONT_HZ24);
	      // GUI_UC_SetEncodeUTF8();
           GUI_DispStringAt("自动登录",20,170);

        //释放查询选中
           GUI_SetColor(GUI_LIGHTGRAY);
	       GUI_FillRect(20, 80, 300, 115);
           GUI_SetColor(GUI_BLACK);
	       GUI_SetBkColor(GUI_LIGHTGRAY);
            GUI_SetFont(GUI_FONT_HZ24);
//	       GUI_UC_SetEncodeUTF8();
           GUI_DispStringAt("查询",20,90);
           //自检选中
           GUI_SetColor(GUI_GRAY);
           GUI_FillRect(20, 120, 300, 155);
	       GUI_SetColor(GUI_WHITE);
 	       GUI_SetBkColor(GUI_GRAY);
           GUI_SetFont(GUI_FONT_HZ24);
//	       GUI_UC_SetEncodeUTF8();
           GUI_DispStringAt("自检",20,130);
    
        break;
    case 3:
        //释放自检选中
           GUI_Clear();
	       GUI_SetColor(GUI_GRAY);
	       GUI_FillRect(0, 0, 320, 40);
           GUI_SetColor(GUI_LIGHTGRAY);
	       GUI_FillRect(0, 35, 320, 240);

	     //显示表头主菜单 白字灰底
           GUI_SetColor(GUI_WHITE);
	       GUI_SetBkColor(GUI_GRAY);
           GUI_SetFont(GUI_FONT_HZ24);

           GUI_DispStringAt("主菜单",10,6);
	
	//划线
	      GUI_SetFont(&GUI_Font8_1);
	      GUI_SetColor(GUI_BLACK);
          GUI_SetBkColor(GUI_LIGHTGRAY);
          GUI_DispStringAt("-----------------------------------------------",20,75); 
          GUI_DispStringAt("-----------------------------------------------",20,115);
	      GUI_DispStringAt("-----------------------------------------------",20,155);
	      GUI_DispStringAt("-----------------------------------------------",20,195);
     
	    //显示四项需要设置的内容
          GUI_SetColor(GUI_BLACK);
	      GUI_SetBkColor(GUI_LIGHTGRAY);
          GUI_SetFont(GUI_FONT_HZ24);
	     //GUI_UC_SetEncodeUTF8();
          GUI_DispStringAt("设置",20,50);
          GUI_DispStringAt("查询",20,90);
          GUI_DispStringAt("自检",20,130);
          GUI_DispStringAt("自动登录",20,170);
          GUI_SetFont(&GUI_Font24_1);
	      GUI_DispStringAt("<1/2>",30,215);   
           GUI_SetColor(GUI_LIGHTGRAY);
	       GUI_FillRect(20, 120, 300, 155);
           GUI_SetColor(GUI_BLACK);
	       GUI_SetBkColor(GUI_LIGHTGRAY);
           GUI_SetFont(GUI_FONT_HZ24);
//	       GUI_UC_SetEncodeUTF8();
           GUI_DispStringAt("自检",20,130);
           //自动登录被选择
           GUI_SetColor(GUI_GRAY);
	       GUI_FillRect(20, 160, 300, 195);
	       GUI_SetColor(GUI_WHITE);
	       GUI_SetBkColor(GUI_GRAY);
           GUI_SetFont(GUI_FONT_HZ24);
//	       GUI_UC_SetEncodeUTF8();
           GUI_DispStringAt("自动登录",20,170);
        break;
    case 4:
           GUI_Clear();
	       GUI_SetColor(GUI_GRAY);
	       GUI_FillRect(0, 0, 320, 40);
           GUI_SetColor(GUI_LIGHTGRAY);
	       GUI_FillRect(0, 35, 320, 240);

           GUI_SetColor(GUI_WHITE);
	       GUI_SetBkColor(GUI_GRAY);
           GUI_SetFont(GUI_FONT_HZ24);
           GUI_DispStringAt("主菜单",10,6);
	
	//划线
	      GUI_SetFont(&GUI_Font8_1);
	      GUI_SetColor(GUI_BLACK);
          GUI_SetBkColor(GUI_LIGHTGRAY);
          GUI_DispStringAt("-----------------------------------------------",20,75); 
          GUI_DispStringAt("-----------------------------------------------",20,115);
	      GUI_DispStringAt("-----------------------------------------------",20,155);
	      GUI_DispStringAt("-----------------------------------------------",20,195);
          GUI_SetFont(&GUI_Font24_1);
	       GUI_DispStringAt("<2/2>",30,215); 
           
               //释放器件点灯
           GUI_SetColor(GUI_LIGHTGRAY);
	       GUI_FillRect(20, 80, 300, 115);
           GUI_SetColor(GUI_BLACK);
	       GUI_SetBkColor(GUI_LIGHTGRAY);
            GUI_SetFont(GUI_FONT_HZ24);
	       //GUI_UC_SetEncodeUTF8();
          GUI_DispStringAt("器件点灯",20,90);
           //GUI_DispStringAt((const char *)cx,20,90);
           //背光调节
           GUI_SetColor(GUI_GRAY);
	       GUI_FillRect(20, 40, 300, 75); 
	       GUI_SetColor(GUI_WHITE);
	       GUI_SetBkColor(GUI_GRAY);
            GUI_SetFont(GUI_FONT_HZ24);
	       //GUI_UC_SetEncodeUTF8();
            GUI_DispStringAt("背光调节",20,50);
           //GUI_DispStringAt((const char *)sz,20,50);
        break;
    case 5:
           GUI_Clear();
	       GUI_SetColor(GUI_GRAY);
	       GUI_FillRect(0, 0, 320, 40);
           GUI_SetColor(GUI_LIGHTGRAY);
	       GUI_FillRect(0, 35, 320, 240);

           GUI_SetColor(GUI_WHITE);
	       GUI_SetBkColor(GUI_GRAY);
           GUI_SetFont(GUI_FONT_HZ24);
           GUI_DispStringAt("主菜单",10,6);
	
	//划线
	      GUI_SetFont(&GUI_Font8_1);
	      GUI_SetColor(GUI_BLACK);
          GUI_SetBkColor(GUI_LIGHTGRAY);
          GUI_DispStringAt("-----------------------------------------------",20,75); 
          GUI_DispStringAt("-----------------------------------------------",20,115);
	      GUI_DispStringAt("-----------------------------------------------",20,155);
	      GUI_DispStringAt("-----------------------------------------------",20,195);
          GUI_SetFont(&GUI_Font24_1);
	       GUI_DispStringAt("<2/2>",30,215); 
        //释放背光调节
    	  GUI_SetColor(GUI_LIGHTGRAY);
	      GUI_FillRect(20, 40, 300, 75);
          GUI_SetColor(GUI_BLACK);
	      GUI_SetBkColor(GUI_LIGHTGRAY);
           GUI_SetFont(GUI_FONT_HZ24);
//	      GUI_UC_SetEncodeUTF8();
          GUI_DispStringAt("背光调节",20,50);
	      //选择器件点灯
	       GUI_SetColor(GUI_GRAY);
	       GUI_FillRect(20, 80, 300, 115);
	       GUI_SetColor(GUI_WHITE);
	       GUI_SetBkColor(GUI_GRAY);
            GUI_SetFont(GUI_FONT_HZ24);
//	       GUI_UC_SetEncodeUTF8();
           GUI_DispStringAt("器件点灯",20,90);
        break;
    default:
        break;
  }

}
void setdis(void)
{
    char arr[20] = {0,};
   //静态框架
    GUI_Clear();
	GUI_SetColor(GUI_GRAY);
	GUI_FillRect(0, 0, 320, 40);
    GUI_SetColor(GUI_LIGHTGRAY);
	GUI_FillRect(0, 35, 320, 240);

	//显示表头设置 白字灰底
    GUI_SetColor(GUI_WHITE);
	GUI_SetBkColor(GUI_GRAY);
//  GUI_SetFont(&GUI_Fontjbfont);
//	GUI_UC_SetEncodeUTF8();
    GUI_SetFont(GUI_FONT_HZ24);
    GUI_DispStringAt("设置",10,6);
	
	
	//划线
	GUI_SetFont(&GUI_Font8_1);
	GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_LIGHTGRAY);
    GUI_DispStringAt("-----------------------------------------------",20,75); 
    GUI_DispStringAt("-----------------------------------------------",20,115);
	GUI_DispStringAt("-----------------------------------------------",20,155);
	GUI_DispStringAt("-----------------------------------------------",20,195);
     
	//显示四项需要设置的内容
    GUI_SetColor(GUI_BLACK);
	GUI_SetBkColor(GUI_LIGHTGRAY);
    GUI_SetFont(GUI_FONT_HZ24);
	//GUI_UC_SetEncodeUTF8();
    GUI_DispStringAt("时间设置",20,50);
	GUI_DispStringAt("设备管理",20,90);
	GUI_DispStringAt("设置电话号码",20,130);
	GUI_DispStringAt("屏蔽解除屏蔽",20,170);
//	GUI_DispStringAt((const char *)xz,5,215);
//	GUI_DispStringAt("恢复出厂设置",270,215);


	//显示翻页
    GUI_SetFont(&GUI_Font24_1);
	GUI_DispStringAt("<1/2>",30,215);   
 //      sprintf(arr,"%d-%d-%d %d:%d:%d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
	GUI_SetFont(&GUI_Font20_1);
	sprintf(arr,"%d-%02d-%02d %02d:%02d:%02d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
    GUI_DispStringAt(arr,150,217); 
}
void set(uint8_t pointer)
{

	
    switch(pointer)
    {
    case 0:
           //器件编辑被释放
   	       GUI_SetColor(GUI_LIGHTGRAY);
	       GUI_FillRect(20, 80, 300, 115);
           GUI_SetColor(GUI_BLACK);
	       GUI_SetBkColor(GUI_LIGHTGRAY);
           GUI_SetFont(GUI_FONT_HZ24);
//	       GUI_UC_SetEncodeUTF8();
           GUI_DispStringAt("设备管理",20,90);
        	//时间设置被选中
           GUI_SetColor(GUI_GRAY);
	       GUI_FillRect(20, 40, 300, 75);
	       GUI_SetColor(GUI_WHITE);
	       GUI_SetBkColor(GUI_GRAY);
           GUI_SetFont(GUI_FONT_HZ24);
//	       GUI_UC_SetEncodeUTF8();
           GUI_DispStringAt("时间设置",20,50);
        break;
    case 1:
            //设置电话号码被释放
    	   GUI_SetColor(GUI_LIGHTGRAY);
	       GUI_FillRect(20, 120, 300, 155);
           GUI_SetColor(GUI_BLACK);
	       GUI_SetBkColor(GUI_LIGHTGRAY);
           GUI_SetFont(GUI_FONT_HZ24);
//	       GUI_UC_SetEncodeUTF8();
           GUI_DispStringAt("设置电话号码",20,130);      
        //时间设置被释放
           GUI_SetColor(GUI_LIGHTGRAY);
	       GUI_FillRect(20, 40, 300, 75);
           GUI_SetColor(GUI_BLACK);
	       GUI_SetBkColor(GUI_LIGHTGRAY);
           GUI_SetFont(GUI_FONT_HZ24);
//	       GUI_UC_SetEncodeUTF8();
           GUI_DispStringAt("时间设置",20,50);
        	//器件编辑被选中
	       GUI_SetColor(GUI_GRAY);
	       GUI_FillRect(20, 80, 300, 115);
	       GUI_SetColor(GUI_WHITE);
	       GUI_SetBkColor(GUI_GRAY);
           GUI_SetFont(GUI_FONT_HZ24);
//	       GUI_UC_SetEncodeUTF8();
           GUI_DispStringAt("设备管理",20,90);
        break;
    case 2:
           //器件编辑被释放
   	       GUI_SetColor(GUI_LIGHTGRAY);
	       GUI_FillRect(20, 80, 300, 115);
           GUI_SetColor(GUI_BLACK);
	       GUI_SetBkColor(GUI_LIGHTGRAY);
           GUI_SetFont(GUI_FONT_HZ24);
//	       GUI_UC_SetEncodeUTF8();
           GUI_DispStringAt("设备管理",20,90);
    	    //屏蔽解除被释放
           GUI_SetColor(GUI_LIGHTGRAY);
	       GUI_FillRect(20, 160, 300, 195);
	       GUI_SetColor(GUI_BLACK);
	       GUI_SetBkColor(GUI_LIGHTGRAY);
           GUI_SetFont(GUI_FONT_HZ24);
//	       GUI_UC_SetEncodeUTF8();
           GUI_DispStringAt("屏蔽解除屏蔽",20,170);
        	//设置电话号码被选中
           GUI_SetColor(GUI_GRAY);
	       GUI_FillRect(20, 120, 300, 155);
	       GUI_SetColor(GUI_WHITE);
	       GUI_SetBkColor(GUI_GRAY);
           GUI_SetFont(GUI_FONT_HZ24);
//	       GUI_UC_SetEncodeUTF8();
           GUI_DispStringAt("设置电话号码",20,130);
        break;
    case 3:
             GUI_SetColor(GUI_LIGHTGRAY);
	       GUI_FillRect(20, 40, 300, 75);
           GUI_SetColor(GUI_BLACK);
	       GUI_SetBkColor(GUI_LIGHTGRAY);
           GUI_SetFont(GUI_FONT_HZ24);
//	       GUI_UC_SetEncodeUTF8();
           GUI_DispStringAt("时间设置",20,50);     
           GUI_SetColor(GUI_LIGHTGRAY);
	       GUI_FillRect(20, 80, 300, 115);
           GUI_SetColor(GUI_BLACK);
	       GUI_SetBkColor(GUI_LIGHTGRAY);
           GUI_SetFont(GUI_FONT_HZ24);
//	       GUI_UC_SetEncodeUTF8();
           GUI_DispStringAt("设备管理",20,90);
    	    //屏蔽解除被释放
           GUI_SetColor(GUI_LIGHTGRAY);
	       GUI_FillRect(20, 160, 300, 195);
	       GUI_SetColor(GUI_BLACK);
	       GUI_SetBkColor(GUI_LIGHTGRAY);
           GUI_SetFont(GUI_FONT_HZ24);
//	       GUI_UC_SetEncodeUTF8();
           GUI_DispStringAt("屏蔽解除屏蔽",20,170);
          //设置电话号码被释放
    	   GUI_SetColor(GUI_LIGHTGRAY);
	       GUI_FillRect(20, 120, 300, 155);
           GUI_SetColor(GUI_BLACK);
	       GUI_SetBkColor(GUI_LIGHTGRAY);
           GUI_SetFont(GUI_FONT_HZ24);
//	       GUI_UC_SetEncodeUTF8();
           GUI_DispStringAt("设置电话号码",20,130);
	      //屏蔽解除屏蔽被选中
          GUI_SetColor(GUI_GRAY);
	      GUI_FillRect(20, 160, 300, 195);
	      GUI_SetColor(GUI_WHITE);
	      GUI_SetBkColor(GUI_GRAY);
          GUI_SetFont(GUI_FONT_HZ24);
//	      GUI_UC_SetEncodeUTF8();

          GUI_DispStringAt("屏蔽解除屏蔽",20,170);
          GUI_SetFont(&GUI_Font24_1);
           GUI_SetBkColor(GUI_LIGHTGRAY);
           GUI_SetColor(GUI_BLACK);
	      GUI_DispStringAt("<1/2>",30,215);
        break;
    case 4:
        	//换页显示
	       //静态框架
          GUI_Clear();
	      GUI_SetColor(GUI_GRAY);
	      GUI_FillRect(0, 0, 320, 40);
          GUI_SetColor(GUI_LIGHTGRAY);
	      GUI_FillRect(0, 35, 320, 240);

	     //显示表头设置 白字灰底
          GUI_SetColor(GUI_WHITE);
	      GUI_SetBkColor(GUI_GRAY);
          GUI_SetFont(GUI_FONT_HZ24);
//	      GUI_UC_SetEncodeUTF8();
          GUI_DispStringAt("设置",10,6);
	
	
	      //划线
	      GUI_SetFont(&GUI_Font8_1);
	      GUI_SetColor(GUI_BLACK);
          GUI_SetBkColor(GUI_LIGHTGRAY);
          GUI_DispStringAt("-----------------------------------------------",20,75); 
          GUI_DispStringAt("-----------------------------------------------",20,115);
	      GUI_DispStringAt("-----------------------------------------------",20,155);
	      GUI_DispStringAt("-----------------------------------------------",20,195);
	
	      //显示翻页
          GUI_SetFont(&GUI_Font24_1);
	      GUI_DispStringAt("<2/2>",30,215);
		

	    //恢复出厂设置被选中
         GUI_SetColor(GUI_GRAY);
	     GUI_FillRect(20, 40, 300, 75);
	     GUI_SetColor(GUI_WHITE);
	     GUI_SetBkColor(GUI_GRAY);
         GUI_SetFont(GUI_FONT_HZ24);
//	     GUI_UC_SetEncodeUTF8();
         GUI_DispStringAt("恢复出厂设置",20,50);
         GUI_SetBkColor(GUI_LIGHTGRAY);
         GUI_SetColor(GUI_BLACK);
         GUI_DispStringAt("查看本机信息",20,90);
         GUI_DispStringAt("通信设置",20,130);
        break;
    case 5:
        	//换页显示
	       //静态框架
          GUI_Clear();
	      GUI_SetColor(GUI_GRAY);
	      GUI_FillRect(0, 0, 320, 40);
          GUI_SetColor(GUI_LIGHTGRAY);
	      GUI_FillRect(0, 35, 320, 240);

	     //显示表头设置 白字灰底
          GUI_SetColor(GUI_WHITE);
	      GUI_SetBkColor(GUI_GRAY);
          GUI_SetFont(GUI_FONT_HZ24);
//	      GUI_UC_SetEncodeUTF8();
          GUI_DispStringAt("设置",10,6);
	
	
	      //划线
	      GUI_SetFont(&GUI_Font8_1);
	      GUI_SetColor(GUI_BLACK);
          GUI_SetBkColor(GUI_LIGHTGRAY);
          GUI_DispStringAt("-----------------------------------------------",20,75); 
          GUI_DispStringAt("-----------------------------------------------",20,115);
	      GUI_DispStringAt("-----------------------------------------------",20,155);
	      GUI_DispStringAt("-----------------------------------------------",20,195);
	
	      //显示翻页
          GUI_SetFont(&GUI_Font24_1);
	      GUI_DispStringAt("<2/2>",30,215);        //恢复出厂设置被释放
          //调节背光亮度被选中
           GUI_SetColor(GUI_LIGHTGRAY);
	       GUI_FillRect(20, 40, 300, 75);
           GUI_SetColor(GUI_BLACK);
	       GUI_SetBkColor(GUI_LIGHTGRAY);
           GUI_SetFont(GUI_FONT_HZ24);
//	       GUI_UC_SetEncodeUTF8();
           GUI_DispStringAt("恢复出厂设置",20,50);
            GUI_DispStringAt("通信设置",20,130);
           //
           GUI_SetColor(GUI_GRAY);
	       GUI_FillRect(20, 80, 300, 115);
	       GUI_SetColor(GUI_WHITE);
	       GUI_SetBkColor(GUI_GRAY);
           GUI_SetFont(GUI_FONT_HZ24);
//	       GUI_UC_SetEncodeUTF8();
           GUI_DispStringAt("查看本机信息",20,90);
        break;
    case 6:
                  GUI_Clear();
	      GUI_SetColor(GUI_GRAY);
	      GUI_FillRect(0, 0, 320, 40);
          GUI_SetColor(GUI_LIGHTGRAY);
	      GUI_FillRect(0, 35, 320, 240);

	     //显示表头设置 白字灰底
          GUI_SetColor(GUI_WHITE);
	      GUI_SetBkColor(GUI_GRAY);
          GUI_SetFont(GUI_FONT_HZ24);
//	      GUI_UC_SetEncodeUTF8();
          GUI_DispStringAt("设置",10,6);
	
	
	      //划线
	      GUI_SetFont(&GUI_Font8_1);
	      GUI_SetColor(GUI_BLACK);
          GUI_SetBkColor(GUI_LIGHTGRAY);
          GUI_DispStringAt("-----------------------------------------------",20,75); 
          GUI_DispStringAt("-----------------------------------------------",20,115);
	      GUI_DispStringAt("-----------------------------------------------",20,155);
	      GUI_DispStringAt("-----------------------------------------------",20,195);
	
	      //显示翻页
          GUI_SetFont(&GUI_Font24_1);
	      GUI_DispStringAt("<2/2>",30,215);        //恢复出厂设置被释放
          //调节背光亮度被选中
           GUI_SetColor(GUI_LIGHTGRAY);
	       GUI_FillRect(20, 40, 300, 75);
           GUI_SetColor(GUI_BLACK);
	       GUI_SetBkColor(GUI_LIGHTGRAY);
           GUI_SetFont(GUI_FONT_HZ24);
//	       GUI_UC_SetEncodeUTF8();
           GUI_DispStringAt("恢复出厂设置",20,50);
            GUI_DispStringAt("查看本机信息",20,90);
           //
           GUI_SetColor(GUI_GRAY);
	       GUI_FillRect(20, 120, 300, 155);
	       GUI_SetColor(GUI_WHITE);
	       GUI_SetBkColor(GUI_GRAY);
           GUI_SetFont(GUI_FONT_HZ24);
//	       GUI_UC_SetEncodeUTF8();
           GUI_DispStringAt("通信设置",20,130);
        break;
    default:
        break;
            
   }


}
void inquirydis(void)
{
      char arr[20] = {0,};
   //静态框架
    GUI_Clear();
	GUI_SetColor(GUI_GRAY);
	GUI_FillRect(0, 0, 320, 40);
    GUI_SetColor(GUI_LIGHTGRAY);
	GUI_FillRect(0, 35, 320, 240);

	//显示表头查询 白字灰底
    GUI_SetColor(GUI_WHITE);
	GUI_SetBkColor(GUI_GRAY);
    GUI_SetFont(GUI_FONT_HZ24);
//	GUI_UC_SetEncodeUTF8();
    GUI_DispStringAt("查询",10,6);
	
	
	//划线
	GUI_SetFont(&GUI_Font8_1);
	GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_LIGHTGRAY);
    GUI_DispStringAt("-----------------------------------------------",20,75); 
    GUI_DispStringAt("-----------------------------------------------",20,115);
	GUI_DispStringAt("-----------------------------------------------",20,155);
	GUI_DispStringAt("-----------------------------------------------",20,195);
     
	//显示四项需要设置的内容
    GUI_SetColor(GUI_BLACK);
	GUI_SetBkColor(GUI_LIGHTGRAY);
    GUI_SetFont(GUI_FONT_HZ24);
//	GUI_UC_SetEncodeUTF8();
    GUI_DispStringAt("火警记录查询",20,50);
	GUI_DispStringAt("事件记录查询",20,90);
//	GUI_DispStringAt("屏蔽记录查询",20,130);
//	GUI_DispStringAt("事件记录查询",20,170);
//	GUI_DispStringAt((const char *)xz,5,215);
//	GUI_DispStringAt((const char *)fh,270,215); 
   //    sprintf(arr,"%d-%d-%d %d:%d:%d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
	GUI_SetFont(&GUI_Font20_1);
	sprintf(arr,"%d-%02d-%02d %02d:%02d:%02d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
    GUI_DispStringAt(arr,150,217); 
}
void inquiry(uint8_t pointer)
{
  switch(pointer)
  {
      case 0:
          	//故障事件查询被释放
	        GUI_SetColor(GUI_LIGHTGRAY);
	        GUI_FillRect(20, 80, 300, 115);
            GUI_SetColor(GUI_BLACK);
	        GUI_SetBkColor(GUI_LIGHTGRAY);
            GUI_SetFont(GUI_FONT_HZ24);
//	        GUI_UC_SetEncodeUTF8();
            GUI_DispStringAt("事件记录查询",20,90);
          	 //火警查询被选中
            GUI_SetColor(GUI_GRAY);
	        GUI_FillRect(20, 40, 300, 75);
	        GUI_SetColor(GUI_WHITE);
	        GUI_SetBkColor(GUI_GRAY);
            GUI_SetFont(GUI_FONT_HZ24);
//	        GUI_UC_SetEncodeUTF8();
            GUI_DispStringAt("火警记录查询",20,50);
          break;
      case 1:
          	//火警事件查询被释放
	        GUI_SetColor(GUI_LIGHTGRAY);
	        GUI_FillRect(20, 40, 300, 75);
            GUI_SetColor(GUI_BLACK);
	        GUI_SetBkColor(GUI_LIGHTGRAY);
            GUI_SetFont(GUI_FONT_HZ24);
//	        GUI_UC_SetEncodeUTF8();
            GUI_DispStringAt("火警记录查询",20,50);
          	//屏蔽事件被释放
//	        GUI_SetColor(GUI_LIGHTGRAY);
//	        GUI_FillRect(20, 120, 300, 155);
//            GUI_SetColor(GUI_BLACK);
//	        GUI_SetBkColor(GUI_LIGHTGRAY);
//            GUI_SetFont(GUI_FONT_HZ24);
////	        GUI_UC_SetEncodeUTF8();
//            GUI_DispStringAt("屏蔽记录查询",20,130);    
	       //故障事件查询被选中
	        GUI_SetColor(GUI_GRAY);
	        GUI_FillRect(20, 80, 300, 115);
	        GUI_SetColor(GUI_WHITE);
	        GUI_SetBkColor(GUI_GRAY);
            GUI_SetFont(GUI_FONT_HZ24);
//	        GUI_UC_SetEncodeUTF8();
            GUI_DispStringAt("事件记录查询",20,90);
          break;
//      case 2:
//           //故障事件查询被释放
//	        GUI_SetColor(GUI_LIGHTGRAY);
//	        GUI_FillRect(20, 80, 300, 115);
//            GUI_SetColor(GUI_BLACK);
//	        GUI_SetBkColor(GUI_LIGHTGRAY);
//            GUI_SetFont(GUI_FONT_HZ24);
////	        GUI_UC_SetEncodeUTF8();
//            GUI_DispStringAt("故障记录查询",20,90);
//		    //事件记录被释放
//	        GUI_SetColor(GUI_LIGHTGRAY);
//	        GUI_FillRect(20, 160, 300, 195);
//            GUI_SetColor(GUI_BLACK);
//	        GUI_SetBkColor(GUI_LIGHTGRAY);
//            GUI_SetFont(GUI_FONT_HZ24);
////	        GUI_UC_SetEncodeUTF8();
//            GUI_DispStringAt("事件记录查询",20,170);
//	       //屏蔽事件查询被选中
//            GUI_SetColor(GUI_GRAY);
//	        GUI_FillRect(20, 120, 300, 155);
//	        GUI_SetColor(GUI_WHITE);
//	        GUI_SetBkColor(GUI_GRAY);
//            GUI_SetFont(GUI_FONT_HZ24);
////	        GUI_UC_SetEncodeUTF8();
//            GUI_DispStringAt("屏蔽记录查询",20,130);
//          break;
//      case 3:
//          	//屏蔽事件被释放
//	        GUI_SetColor(GUI_LIGHTGRAY);
//	        GUI_FillRect(20, 120, 300, 155);
//            GUI_SetColor(GUI_BLACK);
//	        GUI_SetBkColor(GUI_LIGHTGRAY);
//            GUI_SetFont(GUI_FONT_HZ24);
////	        GUI_UC_SetEncodeUTF8();
//            GUI_DispStringAt("屏蔽记录查询",20,130);

//	       //事件记录被选中
//            GUI_SetColor(GUI_GRAY);
//	        GUI_FillRect(20, 160, 300, 195);
//	        GUI_SetColor(GUI_WHITE);
//	        GUI_SetBkColor(GUI_GRAY);
//            GUI_SetFont(GUI_FONT_HZ24);
////	        GUI_UC_SetEncodeUTF8();
//            GUI_DispStringAt("事件记录查询",20,170);
//          break;
      default:
          break;

  }


}

void chekself(void)
{
   //静态框架
  GUI_Clear();
	GUI_SetColor(GUI_GRAY);
	GUI_FillRect(0, 0, 320, 40);
  GUI_SetColor(GUI_LIGHTGRAY);
	GUI_FillRect(0, 35, 320, 240);

	//显示表头查询 白字灰底
  GUI_SetColor(GUI_WHITE);
  GUI_SetBkColor(GUI_GRAY);
  GUI_SetFont(GUI_FONT_HZ24);
  GUI_DispStringAt("自检",10,6);
  GUI_SetColor(GUI_BLACK);
  GUI_SetBkColor(GUI_LIGHTGRAY);
  GUI_DispStringAt("屏幕自检",100,120);

	
}
void qjdd_dis(void)
{
   //静态框架
    GUI_Clear();
	GUI_SetColor(GUI_GRAY);
	GUI_FillRect(0, 0, 320, 40);
    GUI_SetColor(GUI_LIGHTGRAY);
	GUI_FillRect(0, 35, 320, 240);

	//显示表头主菜单 白字灰底
    GUI_SetColor(GUI_WHITE);
	GUI_SetBkColor(GUI_GRAY);
    GUI_SetFont(GUI_FONT_HZ24);
//	GUI_UC_SetEncodeUTF8();
    GUI_DispStringAt("器件点灯",10,6);
    GUI_SetColor(GUI_BLUE);
	GUI_SetBkColor(GUI_LIGHTGRAY);
    GUI_DispStringAt("按功能键编辑",2,215);
//	GUI_SetBkColor(GUI_LIGHTGRAY);
//	GUI_SetColor(GUI_BLACK);    
//    GUI_DispStringAt("编号",100,80);
//    
//    GUI_DispStringAt("点灯",80,150);
//    GUI_DispStringAt("灭灯",180,150);
//    
//    GUI_SetFont(&GUI_Font24_ASCII);
//    GUI_DispStringAt("[",75,150);
//	GUI_DispStringAt("]",128,150);
//    
//    GUI_DispStringAt("[",175,150);
//	GUI_DispStringAt("]",228,150);
//    
//    GUI_SetColor(GUI_WHITE);  
//    GUI_FillRect(150, 80, 190, 103);	

	

		
}

void tim_set(void)
{
//	int i;
	char arr[20]={0};
   //静态框架
    GUI_Clear();
	GUI_SetColor(GUI_GRAY);
	GUI_FillRect(0, 0, 320, 40);
    GUI_SetColor(GUI_LIGHTGRAY);
	GUI_FillRect(0, 35, 320, 240);

	//显示表头时间设置 白字灰底
    GUI_SetColor(GUI_WHITE);
	GUI_SetBkColor(GUI_GRAY);
    GUI_SetFont(GUI_FONT_HZ24);
//	GUI_UC_SetEncodeUTF8();
    GUI_DispStringAt("时间设置",10,6);
	
	//显示选择 返回
	//显示四项需要设置的内容
//    GUI_SetColor(GUI_BLACK);
//	GUI_SetBkColor(GUI_LIGHTGRAY);
//        GUI_SetFont(GUI_FONT_HZ24);
////	GUI_UC_SetEncodeUTF8();
//	GUI_DispStringAt((const char *)xz,5,215);
//	GUI_DispStringAt((const char *)qd,270,215);

	//显示年月日时分秒
	 GUI_SetColor(GUI_WHITE);
	 GUI_SetBkColor(GUI_GRAY);
	 GUI_SetFont(&GUI_Font32_1);
	 
 // GUI_SetFont(&GUI_Fontjbfont);
	GUI_SetFont(&GUI_Font32_1);
	HEX2ASCII(Timer_value.year/100,arr);  //
  GUI_DispStringAt(arr,48,80);
	HEX2ASCII(Timer_value.year%100,arr);
  GUI_DispStringAt(arr,80,80);
	
	
	GUI_SetBkColor(GUI_WHITE);
  GUI_SetColor(GUI_BLACK);
  GUI_SetFont(&GUI_Font32_1);
	HEX2ASCII(Timer_value.month,arr);  //
	GUI_DispStringAt(arr,160,80);
	HEX2ASCII(Timer_value.date,arr);
	GUI_DispStringAt(arr,240,80);
	HEX2ASCII(Timer_value.hour,arr);
	GUI_DispStringAt(arr,80,130);
	HEX2ASCII(Timer_value.min,arr);
  GUI_DispStringAt(arr,160,130);
	HEX2ASCII(Timer_value.sec,arr);
	GUI_DispStringAt(arr,240,130);
	
	
//    GUI_SetBkColor(GUI_WHITE);
//    GUI_SetColor(GUI_BLACK);
//    GUI_SetFont(&GUI_Font32_1);
//    GUI_DispStringAt("2017",48,80);
//	GUI_DispStringAt("01",160,80);
//	GUI_DispStringAt("06",240,80);
//	GUI_DispStringAt("15",80,130);
//    GUI_DispStringAt("37",160,130);
//	GUI_DispStringAt("00",240,130);
	

    //显示分隔符
	GUI_SetBkColor(GUI_LIGHTGRAY);
    GUI_SetColor(GUI_BLACK);
    GUI_SetFont(&GUI_Font32_1);
    GUI_DispCharAt('-',128,80);
	GUI_DispCharAt('-',208,80);
	GUI_DispCharAt(':',128,130);
	GUI_DispCharAt(':',208,130);
//    GUI_LCD_delay(500);
		
	
//    //调整年
//	GUI_SetColor(GUI_GRAY);
//	GUI_FillRect(48, 80, 107, 112);
//	GUI_SetBkColor(GUI_GRAY);
//    GUI_SetColor(GUI_BLACK);
//    for(i=2017;i<2030;i++)
//	{
//	  //GUI_DispStringAt("2017",48,80);
//	  GUI_DispDecAt(i, 48, 80, 4);
//	  GUI_LCD_delay(300);
//	}
//    
//	GUI_SetBkColor(GUI_WHITE);
//    GUI_SetColor(GUI_BLACK);
//    GUI_SetFont(&GUI_Font32_1);
//    GUI_DispStringAt("2029",48,80);

//	//调整月
//	GUI_SetColor(GUI_GRAY);
//	GUI_FillRect(160, 80, 190, 112);
//	GUI_SetBkColor(GUI_GRAY);
//    GUI_SetColor(GUI_BLACK);
//    for(i=1;i<13;i++)
//	{
//	  GUI_DispDecAt(i, 160, 80, 2);
//	 GUI_LCD_delay(300);
//	}
//    
//	GUI_SetBkColor(GUI_WHITE);
//    GUI_SetColor(GUI_BLACK);
//    GUI_SetFont(&GUI_Font32_1);
//    GUI_DispStringAt("12",160,80);
      // sprintf(arr,"%d-%d-%d %d:%d:%d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
	GUI_SetFont(&GUI_Font20_1);
	sprintf(arr,"%d-%02d-%02d %02d:%02d:%02d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
    GUI_DispStringAt(arr,150,217);  
}

/*
********************************************************************************
*	函 数 名: void TimeSet_Show(char num)  
*	功能说明: 时间显示光标
*	形    参: 无
*	返 回 值: 无
********************************************************************************
*/
void TimeSet_Show(char num)
{
  char arr[2]={0};
  switch (num)
	{
		case 0:
		  GUI_SetColor(GUI_WHITE);
	    GUI_SetBkColor(GUI_GRAY);
	    GUI_SetFont(&GUI_Font32_1);
			HEX2ASCII(Timer_value.year/100,arr);  //显示年
      GUI_DispStringAt(arr,48,80);
	    HEX2ASCII(Timer_value.year%100,arr);
      GUI_DispStringAt(arr,80,80);
			GUI_SetBkColor(GUI_WHITE);
      GUI_SetColor(GUI_BLACK);
      GUI_SetFont(&GUI_Font32_1);
		  HEX2ASCII(Timer_value.sec,arr);
			GUI_DispStringAt(arr,240,130);
      break; 
		case 1:
		  GUI_SetColor(GUI_WHITE);
	    GUI_SetBkColor(GUI_GRAY);
	    GUI_SetFont(&GUI_Font32_1);
			HEX2ASCII(Timer_value.month,arr);
      GUI_DispStringAt(arr,160,80);
			GUI_SetBkColor(GUI_WHITE);
      GUI_SetColor(GUI_BLACK);
      GUI_SetFont(&GUI_Font32_1);
		  HEX2ASCII(Timer_value.year/100,arr);  //显示月
      GUI_DispStringAt(arr,48,80);
	    HEX2ASCII(Timer_value.year%100,arr);
      GUI_DispStringAt(arr,80,80);	
		  break;
		case 2:
		  GUI_SetColor(GUI_WHITE);
	    GUI_SetBkColor(GUI_GRAY);
	    GUI_SetFont(&GUI_Font32_1);
		  HEX2ASCII(Timer_value.date,arr);//显示日
      GUI_DispStringAt(arr,240,80);
			GUI_SetBkColor(GUI_WHITE);
      GUI_SetColor(GUI_BLACK);
      GUI_SetFont(&GUI_Font32_1);
		  HEX2ASCII(Timer_value.month,arr);//显示月
      GUI_DispStringAt(arr,160,80);
		  break;
		case 3:
		  GUI_SetColor(GUI_WHITE);
	    GUI_SetBkColor(GUI_GRAY);
	    GUI_SetFont(&GUI_Font32_1);
		  HEX2ASCII(Timer_value.hour,arr);//显示时
      GUI_DispStringAt(arr,80,130);
			GUI_SetBkColor(GUI_WHITE);
      GUI_SetColor(GUI_BLACK);
      GUI_SetFont(&GUI_Font32_1);
		  HEX2ASCII(Timer_value.date,arr);//显示日
      GUI_DispStringAt(arr,240,80);
		  break;
		case 4:
		  GUI_SetColor(GUI_WHITE);
	    GUI_SetBkColor(GUI_GRAY);
	    GUI_SetFont(&GUI_Font32_1);
			HEX2ASCII(Timer_value.min,arr);//显示分
			GUI_DispStringAt(arr,160,130);
			GUI_SetBkColor(GUI_WHITE);
      GUI_SetColor(GUI_BLACK);
      GUI_SetFont(&GUI_Font32_1);
		  HEX2ASCII(Timer_value.hour,arr);//显示时
      GUI_DispStringAt(arr,80,130);
		  break;
		case 5:
		  GUI_SetColor(GUI_WHITE);
	    GUI_SetBkColor(GUI_GRAY);
	    GUI_SetFont(&GUI_Font32_1);
			HEX2ASCII(Timer_value.sec,arr);
			GUI_DispStringAt(arr,240,130);//显示秒
			GUI_SetBkColor(GUI_WHITE);
      GUI_SetColor(GUI_BLACK);
      GUI_SetFont(&GUI_Font32_1);
		  HEX2ASCII(Timer_value.min,arr);//显示分
			GUI_DispStringAt(arr,160,130);
		  break;
		default:
      break;
	}

}
	

/*
********************************************************************************
*	函 数 名: void time_show(void)   
*	功能说明: 时间显示
*	形    参: 无
*	返 回 值: 无
********************************************************************************
*/
void time_show(void)
{
//	int i;
	char arr[20]={0};
	
	time_num(); //读取时间
	
   //底边 时间底框
//  GUI_SetColor(GUI_BLUE);
//  GUI_FillRect(0, 0, 320, 20);
//  GUI_FillRect(0, 215, 320, 240);
    GUI_SetFont(&GUI_Font20_1);
    switch(WinIndex)
    {
        case W2_MAIN_WINDOW_WIN:
        //case W10_EDITDEVICE_WINDOW_WIN:
              GUI_SetBkColor(GUI_LIGHTBLUE);
              
        	  GUI_SetColor(GUI_LIGHTBLUE);
	         // GUI_FillRect(170, 217, 320, 240);
              GUI_SetColor(GUI_WHITE);
              break;
//        case W10_EDITDEVICE_WINDOW_WIN:
//              GUI_SetBkColor(GUI_GRAY);
//              GUI_SetColor(GUI_WHITE);
//            break;
        case W4_FAULT_WINDOW_WIN:
        case W5_FIRE_WINDOW_WIN:
        case W6_SHIELD_WINDOW_WIN:

             GUI_SetBkColor(GUI_WHITE);
             GUI_SetColor(GUI_WHITE);
	         // GUI_FillRect(170, 217, 320, 240);
             GUI_SetColor(GUI_BLACK);
             break;
         case W10_EDITDEVICE_WINDOW_WIN:
        case W15_FAULTQUERY_WINDOW_WIN:
        case W16_SHIELDQUERY_WINDOW_WIN:
        case W18_QUERY_WINDOW_WIN:
        case W3_MENU_WINDOW_WIN:
        case W8_SET_WINDOW_WIN:
        case W9_SETTIME_WINDOW_WIN:
        case W17_EVENTQUERY_WINDOW_WIN:
        case W11_SETPHONE_WINDOW_WIN:
        case W12_SHIELDSET_WINDOW_WIN:
        case W13_RESET_WINDOW_WIN:
        case W19_DEVICELIGHT_WINDOW_WIN:
        case W20_LIGHT_SET_WINDOW_WIN:
        case W22_LOOPLOGIN_WINDOW_WIN:
        case W23_INFO_WINDOW_WIN:
        case W24_COMMUNICATION_WINDOW_WIN:
        case W25_DEVICE_SET_WINDOW_WIN:
        case W26_PASSWORD_WINDOW_WIN:
             GUI_SetBkColor(GUI_LIGHTGRAY);
             GUI_SetColor(GUI_LIGHTGRAY);
	         //GUI_FillRect(170, 217, 320, 240);        
             GUI_SetColor(GUI_BLACK);
            break;
        default :
            //GUI_SetBkColor(GUI_LIGHTGRAY);
            break;
    }
  
    
 // GUI_DispStringAt("2017-11-06 11:29 ",150,217);
	//	//显示年月日时分秒
	sprintf(arr,"%d-%02d-%02d %02d:%02d:%02d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
    GUI_DispStringAt(arr,150,217);
//	HEX2ASCII(Timer_value.year/100,arr);
//  GUI_DispStringAt(arr,120,217);
//	HEX2ASCII(Timer_value.year%100,arr);
//  GUI_DispStringAt(arr,144,217);
//	HEX2ASCII(Timer_value.month,arr);
//	GUI_DispStringAt(arr,174,217);
//	HEX2ASCII(Timer_value.date,arr);
//	GUI_DispStringAt(arr,204,217);
//	HEX2ASCII(Timer_value.hour,arr);
//	GUI_DispStringAt(arr,234,217);
//	HEX2ASCII(Timer_value.min,arr);
//  GUI_DispStringAt(arr,264,217);
//	HEX2ASCII(Timer_value.sec,arr);
//	GUI_DispStringAt(arr,294,217);
//	
////  GUI_SetFont(&GUI_Font24_1);
////  GUI_SetBkColor(GUI_BLUE);
////  GUI_SetColor(GUI_WHITE);
//	GUI_DispStringAt("-",168,217);
////	GUI_LCD_delay(300);
//	GUI_DispStringAt("-",198,217);
////	GUI_LCD_delay(300);
//	GUI_DispStringAt(" ",228,217);
////	GUI_LCD_delay(300);
//  GUI_DispStringAt(":",258,217);
////	GUI_LCD_delay(300);
//  GUI_DispStringAt(":",288,217);
  //GUI_LCD_delay(100);
	//GUI_LCD_delay(500);


		
}
/*
********************************************************************************
*	函 数 名: Save_Time_Bkp(void)
*	功能说明: 保存时间到bkp寄存器
*	形    参: 无
*	返 回 值: 0：成功 
********************************************************************************
*/
void  Save_Time_Bkp(void)
{
    BKP_WriteBackupRegister(BKP_DR2, Timer_value.year);	//向指定的后备寄存器中写入用户程序数据
	BKP_WriteBackupRegister(BKP_DR3, Timer_value.month);	//向指定的后备寄存器中写入用户程序数据
	BKP_WriteBackupRegister(BKP_DR4, Timer_value.date);	//向指定的后备寄存器中写入用户程序数据
	BKP_WriteBackupRegister(BKP_DR5, Timer_value.hour);	//向指定的后备寄存器中写入用户程序数据
	BKP_WriteBackupRegister(BKP_DR6, Timer_value.min);	//向指定的后备寄存器中写入用户程序数据
	BKP_WriteBackupRegister(BKP_DR7, Timer_value.sec);	//向指定的后备寄存器中写入用户程序数据

}
/*
********************************************************************************
*	函 数 名: void Read_Time_Bkp(void)
*	功能说明: 读取BKP寄存器内保存的时间值
*	形    参: 无
*	返 回 值: 无
********************************************************************************
*/

void Read_Time_Bkp(void)
{
    Timer_value.year = BKP_ReadBackupRegister(BKP_DR2);
    Timer_value.month = BKP_ReadBackupRegister(BKP_DR3);
	Timer_value.date = BKP_ReadBackupRegister(BKP_DR4);
	Timer_value.hour = BKP_ReadBackupRegister(BKP_DR5);
	Timer_value.min = BKP_ReadBackupRegister(BKP_DR6);
	Timer_value.sec = BKP_ReadBackupRegister(BKP_DR7);
  if(Timer_value.year == 0)
	{
		Timer_value.year = 2018;
		Timer_value.month = 6;
		Timer_value.date = 18;
		Timer_value.hour = 8;
		Timer_value.min = 30;
		Timer_value.sec = 0;
		
	}
}

/*
********************************************************************************
*	函 数 名: void Write_Time_RTC(void)
*	功能说明: 保存时间到RTC
*	形    参: 无
*	返 回 值: 0：成功 
********************************************************************************
*/

void Write_Time_RTC(void)
{
	 u32 temp=0;
     RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	  PWR->CR|=1<<8;                  					 //取消备份区写保护
	  RTC_EnterConfigMode();						     //允许配置 	  				
	  RTC_WaitForLastTask();                     //等待上一次对RTC寄存器的写操作是否已经完成 
	  temp = Timer_value.hour*3600+Timer_value.min*60+Timer_value.sec;
	  RTC_SetCounter(temp);
	  RTC_WaitForLastTask();	
    
}
//器件编辑界面

void qjbjdis(void)
{
    char arr[20] = {0,};
    GUI_Clear();
	GUI_SetColor(GUI_GRAY);
	GUI_FillRect(0, 0, 320, 35);
    GUI_SetColor(GUI_LIGHTGRAY);
	GUI_FillRect(0, 35, 320, 240);

	//显示表头器件编辑 白字灰底
    GUI_SetColor(GUI_WHITE);
	GUI_SetBkColor(GUI_GRAY);
    GUI_SetFont(GUI_FONT_HZ24);
	//GUI_UC_SetEncodeUTF8();
    GUI_DispStringAt("设备编辑",10,6); 
    GUI_SetBkColor(GUI_GRAY);
    GUI_SetColor(GUI_WHITE);    
    GUI_SetFont(GUI_FONT_HZ24);
    GUI_DispStringAt("地址：",110,6);
    GUI_SetFont(&GUI_Font24_1);
    GUI_DispDecAt(last_select+1,170,6,2);  
    //显示编辑位置
    GUI_SetColor(GUI_BLACK);
	GUI_SetBkColor(GUI_LIGHTGRAY);
    GUI_SetFont(GUI_FONT_HZ24);
    GUI_DispStringAt("在线",10,38);//
    GUI_DispStringAt("位置",80,38);//
    GUI_DispStringAt("区号",165,38);//
    GUI_DispStringAt("类型",235,38);//
   GUI_SetColor(GUI_BLUE); 
    GUI_DispStringAt("按功能键移动光标",10,100);
    GUI_DispStringAt("按返回键返回",10,130);
    GUI_DispStringAt("按上下键改变数值",10,160);
    //绘制编辑区域
 	GUI_SetColor(GUI_WHITE);
	GUI_FillRect(10, 65, 50, 90);
	GUI_FillRect(70, 65, 150, 90);
	GUI_FillRect(170, 65, 200, 90);
	GUI_FillRect(220, 65, 315, 90);
    
    //绘制器件查询区域
//    GUI_FillRect(10, 95, 315, 200);
    
     //编号
//    GUI_SetBkColor(GUI_WHITE);
//	GUI_SetColor(GUI_BLACK);
//	GUI_SetFont(&GUI_Font24_1);
//    GUI_DispDecAt(0, 15,65, 1);
//    GUI_DispDecAt(0, 30,65, 1);
//    GUI_DispDecAt(0, 45,65, 1);
    //位置
    GUI_SetFont(GUI_FONT_HZ24);
    //保存按钮
    
    GUI_SetColor(GUI_BLACK);
	GUI_SetBkColor(GUI_WHITE);
    GUI_SetFont(GUI_FONT_HZ24);
//    GUI_DispStringAt("下页", 203,215);
    GUI_DispStringAt("NO", 10,65);
    GUI_DispStringAt("卧室", 70,65);

    //类型
    GUI_DispStringAt("烟感", 230,65);
    
    
    //区号
    GUI_SetFont(&GUI_Font24_1);
    GUI_DispDecAt(1, 180,65, 1);
    GUI_SetBkColor(GUI_LIGHTGRAY);
      GUI_SetColor(GUI_BLACK); 
  //  sprintf(arr,"%d-%d-%d %d:%d:%d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
	GUI_SetFont(&GUI_Font20_1);
	sprintf(arr,"%d-%02d-%02d %02d:%02d:%02d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
    GUI_DispStringAt(arr,150,217); 
	    
}
//器件管理界面
void qjbj_dis(void)
{
    char arr[20] = {0,};
   //静态框架  菜单基本样式
    GUI_Clear();
	GUI_SetColor(GUI_GRAY);
	GUI_FillRect(0, 0, 320, 35);
    GUI_SetColor(GUI_LIGHTGRAY);
	GUI_FillRect(0, 35, 320, 240);
    GUI_SetBkColor(GUI_GRAY);
    GUI_SetColor(GUI_WHITE);   
    GUI_SetFont(&GUI_Font24_1);
    GUI_DispDecAt(device_total,220,6,2);
	//显示表头器件编辑 白字灰底
    GUI_SetColor(GUI_WHITE);
	GUI_SetBkColor(GUI_GRAY);
    GUI_SetFont(GUI_FONT_HZ24);
	//GUI_UC_SetEncodeUTF8();
    GUI_DispStringAt("设备管理",10,6);
        GUI_SetBkColor(GUI_GRAY);
    GUI_SetColor(GUI_WHITE);    
    GUI_SetFont(GUI_FONT_HZ24);
    GUI_DispStringAt("在线总数:",110,6);
    GUI_SetColor(GUI_BLUE); 
      GUI_SetBkColor(GUI_LIGHTGRAY);
       GUI_SetColor(GUI_BLUE); 
    GUI_DispStringAt("按功能键编辑",10,215);
      GUI_SetColor(GUI_BLACK); 
   // sprintf(arr,"%d-%d-%d %d:%d:%d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
	GUI_SetFont(&GUI_Font20_1);
	sprintf(arr,"%d-%02d-%02d %02d:%02d:%02d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
    GUI_DispStringAt(arr,150,217); 

    //显示编辑位置
//    GUI_SetColor(GUI_BLACK);
//	GUI_SetBkColor(GUI_LIGHTGRAY);
//    GUI_SetFont(GUI_FONT_HZ24);
//    GUI_DispStringAt("编号",10,38);//
//    GUI_DispStringAt("位置",90,38);//
//    GUI_DispStringAt("区号",165,38);//
//    GUI_DispStringAt("类型",240,38);//
//    //绘制编辑区域
// 	GUI_SetColor(GUI_WHITE);
//	GUI_FillRect(10, 65, 65, 90);
//	GUI_FillRect(90, 65, 150, 90);
//	GUI_FillRect(170, 65, 200, 90);
//	GUI_FillRect(220, 65, 315, 90);
//    
//    //绘制器件查询区域
//    GUI_FillRect(10, 95, 315, 200);
    
//     //编号
//    GUI_SetBkColor(GUI_WHITE);
//	GUI_SetColor(GUI_BLACK);
//	GUI_SetFont(&GUI_Font24_1);
//    GUI_DispDecAt(0, 15,65, 1);
//    GUI_DispDecAt(0, 30,65, 1);
//    GUI_DispDecAt(0, 45,65, 1);
//    //位置
//    GUI_SetFont(GUI_FONT_HZ24);
    //保存按钮
    
//    GUI_DispStringAt("保存", 13,215);
//    GUI_DispStringAt("翻页", 260,215);
////    GUI_DispStringAt("下页", 203,215);
//    
//    GUI_DispStringAt("客厅", 95,65);

//    //类型
//    GUI_DispStringAt("烟感", 260,65);
    
    
    //区号
//    GUI_SetFont(&GUI_Font24_1);
//    GUI_DispDecAt(0, 180,65, 1);
//    //保存按钮外框
//    GUI_SetFont(&GUI_Font24_ASCII);
//    GUI_DispStringAt("[",10,215);
//	GUI_DispStringAt("]",60,215);
//    
////    //下页外框
////    GUI_DispStringAt("[",120,215);
////	GUI_DispStringAt("]",170,215);
//    //上页外框
//    GUI_DispStringAt("[",253,215);
//	GUI_DispStringAt("]",308,215);
	
}


void set_phone_num(void)
{
    char arr[20] = {0,};
   //静态框架
    GUI_Clear();
	GUI_SetColor(GUI_GRAY);
	GUI_FillRect(0, 0, 320, 40);
    GUI_SetColor(GUI_LIGHTGRAY);
	GUI_FillRect(0, 35, 320, 240);

	//显示表头设置电话号码 白字灰底
    GUI_SetColor(GUI_WHITE);
	GUI_SetBkColor(GUI_GRAY);
    GUI_SetFont(GUI_FONT_HZ24);
//	GUI_UC_SetEncodeUTF8();
    GUI_DispStringAt("设置电话号码",10,6);

	//显示选择 返回
//    GUI_SetColor(GUI_BLACK);
//	GUI_SetBkColor(GUI_LIGHTGRAY);
//	GUI_SetFont(&GUI_Fontjbfont);
//	GUI_UC_SetEncodeUTF8();
//    GUI_DispStringAt((const char *)xz,5,215);
//	GUI_DispStringAt((const char *)fh,270,215);

	//显示电话号码1
	GUI_SetColor(GUI_BLACK);
	GUI_SetBkColor(GUI_LIGHTGRAY);
    GUI_SetFont(GUI_FONT_HZ24);
//	GUI_UC_SetEncodeUTF8();
    GUI_DispStringAt("电话号码",10,60);
	GUI_SetFont(&GUI_Font24_1);
	GUI_DispStringAt("1:",110,57);
    
	//显示电话号码2
    GUI_SetColor(GUI_BLACK);
	GUI_SetBkColor(GUI_LIGHTGRAY);
    GUI_SetFont(GUI_FONT_HZ24);
//	GUI_UC_SetEncodeUTF8();
    GUI_DispStringAt("电话号码",10,110);
	GUI_SetFont(&GUI_Font24_1);
	GUI_DispStringAt("2:",110,107);
    
	//显示号码1表格 横线
	GUI_DrawHLine(60, 140, 316);
	GUI_DrawHLine(86, 140, 316);

	//显示号码1表格 竖线
    GUI_DrawLine(140, 60, 140, 86);
	GUI_DrawLine(156, 60, 156, 86);
	GUI_DrawLine(172, 60, 172, 86);
	GUI_DrawLine(188, 60, 188, 86);
	GUI_DrawLine(204, 60, 204, 86);
	GUI_DrawLine(220, 60, 220, 86);
	GUI_DrawLine(236, 60, 236, 86);
	GUI_DrawLine(252, 60, 252, 86);
	GUI_DrawLine(268, 60, 268, 86);
	GUI_DrawLine(284, 60, 284, 86);
	GUI_DrawLine(300, 60, 300, 86);
	GUI_DrawLine(316, 60, 316, 86);


	//显示号码2表格 横线
    GUI_DrawHLine(110, 140, 316);
	GUI_DrawHLine(136, 140, 316);
	//显示号码2表格 竖线
	GUI_DrawLine(140, 110, 140, 136);
	GUI_DrawLine(156, 110, 156, 136);
	GUI_DrawLine(172, 110, 172, 136);
	GUI_DrawLine(188, 110, 188, 136);
	GUI_DrawLine(204, 110, 204, 136);
	GUI_DrawLine(220, 110, 220, 136);
	GUI_DrawLine(236, 110, 236, 136);
	GUI_DrawLine(252, 110, 252, 136);
	GUI_DrawLine(268, 110, 268, 136);
	GUI_DrawLine(284, 110, 284, 136);
	GUI_DrawLine(300, 110, 300, 136);
	GUI_DrawLine(316, 110, 316, 136);

	//数字填充号码1
	GUI_SetColor(GUI_BLACK);
	GUI_SetBkColor(GUI_LIGHTGRAY);
	GUI_SetFont(&GUI_Font24_1);
    GUI_DispDecAt(1, 142, 61, 1);
	GUI_DispDecAt(3, 158, 61, 1);
	GUI_DispDecAt(8, 174, 61, 1);
	GUI_DispDecAt(7, 190, 61, 1);
	GUI_DispDecAt(2, 206, 61, 1);
	GUI_DispDecAt(1, 222, 61, 1);
	GUI_DispDecAt(5, 238, 61, 1);
	GUI_DispDecAt(3, 254, 61, 1);
	GUI_DispDecAt(2, 270, 61, 1);
	GUI_DispDecAt(4, 286, 61, 1);
	GUI_DispDecAt(4, 302, 61, 1);
   
	//数字填充号码2
    GUI_SetColor(GUI_BLACK);
	GUI_SetBkColor(GUI_LIGHTGRAY);
	GUI_SetFont(&GUI_Font24_1);
    GUI_DispDecAt(1, 142, 111, 1);
	GUI_DispDecAt(3, 158, 111, 1);
	GUI_DispDecAt(8, 174, 111, 1);
	GUI_DispDecAt(7, 190, 111, 1);
	GUI_DispDecAt(2, 206, 111, 1);
	GUI_DispDecAt(1, 222, 111, 1);
	GUI_DispDecAt(5, 238, 111, 1);
	GUI_DispDecAt(3, 254, 111, 1);
	GUI_DispDecAt(2, 270, 111, 1);
	GUI_DispDecAt(4, 286, 111, 1);
	GUI_DispDecAt(4, 302, 111, 1);
      GUI_SetBkColor(GUI_LIGHTGRAY);
       GUI_SetColor(GUI_BLUE); 
    GUI_DispStringAt("按功能键编辑",10,215);
    GUI_SetColor(GUI_BLACK); 
   // sprintf(arr,"%d-%d-%d %d:%d:%d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
	GUI_SetFont(&GUI_Font20_1);
	sprintf(arr,"%d-%02d-%02d %02d:%02d:%02d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
    GUI_DispStringAt(arr,150,217);

}

void pbjcpb_dis(void)
{
    char arr[20] = {0,};
   //静态框架
    GUI_Clear();
	GUI_SetColor(GUI_GRAY);
	GUI_FillRect(0, 0, 320, 40);
    GUI_SetColor(GUI_LIGHTGRAY);
	GUI_FillRect(0, 35, 320, 240);

	//显示表头主菜单 白字灰底
    GUI_SetColor(GUI_WHITE);
	GUI_SetBkColor(GUI_GRAY);
    GUI_SetFont(GUI_FONT_HZ24);
//	GUI_UC_SetEncodeUTF8();
    GUI_DispStringAt("屏蔽解除屏蔽",10,6);
	
      GUI_SetBkColor(GUI_LIGHTGRAY);
       GUI_SetColor(GUI_BLUE); 
    GUI_DispStringAt("按功能键编辑",10,215);
  //  sprintf(arr,"%d-%d-%d %d:%d:%d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
	GUI_SetFont(&GUI_Font20_1);
    GUI_SetColor(GUI_BLACK);
	sprintf(arr,"%d-%02d-%02d %02d:%02d:%02d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
    GUI_DispStringAt(arr,150,217);  
    
    

}
//屏蔽界面
void Shield(void)
{
    char arr[20] = {0,};
    GUI_Clear();
	GUI_SetColor(GUI_YELLOW);
	GUI_FillRect(0, 0, 320, 40);
    GUI_SetColor(GUI_WHITE);
	GUI_FillRect(0, 35, 320, 240);

	//显示表头故障事件 红底白字
    GUI_SetColor(GUI_BLACK);
	GUI_SetBkColor(GUI_YELLOW);
    GUI_SetFont(GUI_FONT_HZ24);
//	GUI_UC_SetEncodeUTF8();
    GUI_DispStringAt("屏蔽事件",20,6);
     GUI_SetColor(GUI_BLACK);
//	GUI_SetBkColor(GUI_WHITE);
     GUI_DispStringAt("屏蔽总数：",170,6);
    //显示屏蔽总数
    GUI_SetFont(&GUI_Font24_1);
    GUI_DispDecAt(EventShieldRamCurrentPointer,280,6,2);
    	//划线
	GUI_SetFont(&GUI_Font8_1);
	GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_WHITE);
    //GUI_DispStringAt("-----------------------------------------------",20,75);
    GUI_DispStringAt("-----------------------------------------------",20,115);
//	GUI_DispStringAt("-----------------------------------------------",20,155);
	GUI_DispStringAt("-----------------------------------------------",20,195);    
   // _ShowBMPEx(PIC_SLIENCE,10,35);
    //   sprintf(arr,"%d-%d-%d %d:%d:%d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
	GUI_SetFont(&GUI_Font20_1);
	sprintf(arr,"%d-%02d-%02d %02d:%02d:%02d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
    GUI_DispStringAt(arr,150,217);
}
//故障界面
void guzhang(void)
{
    char arr[20] = {0,};
   //静态框架
    GUI_Clear();
	GUI_SetColor(GUI_YELLOW);
	GUI_FillRect(0, 0, 320, 40);
    GUI_SetColor(GUI_WHITE);
	GUI_FillRect(0, 35, 320, 240);

	//显示表头故障事件 红底白字
    GUI_SetColor(GUI_BLACK);
	GUI_SetBkColor(GUI_YELLOW);
    GUI_SetFont(GUI_FONT_HZ24);
//	GUI_UC_SetEncodeUTF8();
    GUI_DispStringAt("故障事件",20,6);
//     GUI_SetColor(GUI_BLACK);
//	GUI_SetBkColor(GUI_WHITE);
     GUI_DispStringAt("故障总数：",170,6);
   // _ShowBMPEx(PIC_FAULT,10,200);
        //显示屏蔽总数
    GUI_SetFont(&GUI_Font24_1);
    GUI_DispDecAt(EventFaultCount,280,6,2);
    	//划线
	GUI_SetFont(&GUI_Font8_1);
	GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_WHITE);
    //GUI_DispStringAt("-----------------------------------------------",20,75);
    GUI_DispStringAt("-----------------------------------------------",20,115);
//	GUI_DispStringAt("-----------------------------------------------",20,155);
	GUI_DispStringAt("-----------------------------------------------",20,195);  
	//显示故障图标
//	GUI_DrawBitmap(&bmguzhang,10,50);

	//显示文字内容
//    GUI_SetColor(GUI_BLACK);
//	GUI_SetBkColor(GUI_WHITE);
//    GUI_SetFont(&GUI_Fontnew1font);
//	GUI_UC_SetEncodeUTF8();
//	GUI_DispStringAt((const char *)zdgz,135,60);
//    GUI_DispStringAt((const char *)qjkcl,100,170);
//	GUI_SetFont(&GUI_Fontjbfont);
//	GUI_UC_SetEncodeUTF8();
//    GUI_DispStringAt((const char *)sj,135,90);

//	GUI_SetFont(&GUI_Font24_1);
//    GUI_DispStringAt(":2017-11-9",184,90);
//	GUI_DispStringAt("12:31",235,115);
     //  sprintf(arr,"%d-%d-%d %d:%d:%d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
	GUI_SetFont(&GUI_Font20_1);
	sprintf(arr,"%d-%02d-%02d %02d:%02d:%02d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
    GUI_DispStringAt(arr,150,217);
}


void huojing(void)  
{
   //静态框架
    GUI_Clear();
	GUI_SetColor(GUI_RED);
	GUI_FillRect(0, 0, 320, 40);
    GUI_SetColor(GUI_WHITE);
	GUI_FillRect(0, 35, 320, 240);

	//显示表头火警事件 红底白字
    GUI_SetColor(GUI_WHITE);
	GUI_SetBkColor(GUI_RED);
     GUI_SetFont(GUI_FONT_HZ24);
//    GUI_SetFont(&GUI_Fontjbfont);
//	GUI_UC_SetEncodeUTF8();
    GUI_DispStringAt("火警事件",20,6);
         GUI_SetColor(GUI_WHITE);
	GUI_SetBkColor(GUI_RED);
     GUI_DispStringAt("火警总数：",170,6);
    
	//显示火警图标
   //  _ShowBMPEx(PIC_FIRE,10,50);
	//GUI_DrawBitmap(&bmhuojing,10,50);
    //显示屏蔽总数
    GUI_SetFont(&GUI_Font24_1);
    GUI_DispDecAt(EventAlarmCount,280,6,2);
    	//划线
	GUI_SetFont(&GUI_Font8_1);
	GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_WHITE);
    //GUI_DispStringAt("-----------------------------------------------",20,75);
    GUI_DispStringAt("-----------------------------------------------",20,115);
//	GUI_DispStringAt("-----------------------------------------------",20,155);
	GUI_DispStringAt("-----------------------------------------------",20,195);    
//	//显示文字内容
//    GUI_SetColor(GUI_RED);
//	GUI_SetBkColor(GUI_WHITE);
//    GUI_SetFont(&GUI_Fontnewfont);
//	GUI_UC_SetEncodeUTF8();
//	GUI_DispStringAt((const char *)ws,135,60);
//    
//	GUI_SetFont(&GUI_Fontjbfont);
//	GUI_UC_SetEncodeUTF8();
//    GUI_DispStringAt((const char *)hj,200,60);
//    GUI_DispStringAt((const char *)sj,135,90);

//	GUI_SetFont(&GUI_Font24_1);
//	GUI_DispStringAt("2",184,60);
//    GUI_DispStringAt(":2017-11-9",184,90);
//	GUI_DispStringAt("12:31",235,115);

//	//显示报警信息已发送
//	GUI_SetColor(GUI_RED);
//	GUI_SetBkColor(GUI_WHITE);
//    GUI_SetFont(&GUI_Fontnew1font);
//	GUI_UC_SetEncodeUTF8();
//	GUI_DispStringAt((const char *)bjxx,10,150);
//    GUI_DispStringAt((const char *)yfs,120,150);
//    GUI_SetFont(&GUI_Font24_1);
//	GUI_DispStringAt(":",107,150);

//	//显示号码1 2拨通 未拨通
//    GUI_SetColor(GUI_RED);
//	GUI_SetBkColor(GUI_WHITE);
//    GUI_SetFont(&GUI_Fontnew1font);
//	GUI_UC_SetEncodeUTF8();
//	GUI_DispStringAt((const char *)hm,10,175);
//    GUI_DispStringAt((const char *)hm,10,200);
//    GUI_DispStringAt((const char *)ybt,215,175);
//    GUI_DispStringAt((const char *)wbt,215,200);
//    GUI_SetFont(&GUI_Font24_1);
//	GUI_DispStringAt("1",59,175);
//	GUI_DispStringAt("2",59,200);
//	GUI_DispStringAt(":13725468521:",71,175);
//    GUI_DispStringAt(":15832475614:",71,200);
}

void GUI_LCD_delay(int ms)
{
  GUI_Exec();
	TIMDelay_Nms(ms);
}
/*
*********************************************************************************************************
*	函 数 名: _GetData
*	功能说明: 被函数GUI_BMP_DrawEx()调用
*	形    参：p             FIL类型数据
*             NumBytesReq   请求读取的字节数
*             ppData        数据指针
*             Off           如果Off = 1，那么将重新从其实位置读取                 
*	返 回 值: 返回读取的字节数
*********************************************************************************************************
*/
static int _GetData(void *p, const U8 ** ppData, unsigned NumBytesReq, U32 Off) 
{
	//U32 i;
	static int FileAddress = 0;
    int FileAddressBase = 0;//图片地址
	uint32_t NumBytesRead = 0;
//	FIL *PicFile;
    uint8_t bmpnum ;
    uint32_t *ponit;
    ponit = p;
    bmpnum = *ponit;
//	PicFile = (FIL *)p;
    switch(bmpnum)
    {
        case  1:
            FileAddressBase = PIC_20K_ADDR_BASE; //待机图标
            break;
        case 2:
             FileAddressBase = PIC_10K_ADDR_BASE; //logo
            break;
        case 3:
            FileAddressBase = PIC_10K_ADDR_BASE+10*1024;//警告
            break;
        case 4: FileAddressBase = PIC_10K_ADDR_BASE+20*1024;//火警图标地址
            break;
        case 5: FileAddressBase = PIC_10K_ADDR_BASE+30*1024;//静音图标
              break;
        case 6: FileAddressBase = 0x5A2000;
        break;
        default:break;
    }
	/*
	* 检测缓存大小
	*/
	if (NumBytesReq > sizeof(_acBuffer)) {
	NumBytesReq = sizeof(_acBuffer);
	}

	/*
	* 设置读取位置
	*/
	if(Off == 1) FileAddress = 0;
	else FileAddress = Off;

        sFLASH_NOR_Read((uint8_t *)&_acBuffer[0],FileAddress+FileAddressBase,NumBytesReq);
        NumBytesRead = NumBytesReq;

	

	/*
	* 让指针ppData指向读取的函数
	*/
	*ppData = (const U8 *)&_acBuffer[0];

	/*
	* 返回读取的字节数
	*/
	return NumBytesRead;
}
/**********************************************************************************************************
*	函 数 名: _ShowBMPEx
*	功能说明: 显示BMP图片
*	形    参: bmp 要显示图片的编号
*	返 回 值: 无
*********************************************************************************************************
*/
 void _ShowBMPEx(uint8_t bmp,int x,int y) 
{	

    	
//    /* 打开文件 */		
//	result = f_open(&file, sFilename, FA_OPEN_EXISTING | FA_READ | FA_OPEN_ALWAYS);
//	if (result != FR_OK)
//	{
//		return;
//	}
	

	GUI_BMP_DrawEx(_GetData, &bmp, x, y);

}
///*
//*********************************************************************************************************
//*	函 数 名: _WriteByte2File()
//*	功能说明: 写文件到SD卡或者其他存储介质
//*	形    参：Data 要写的数据， p 指向FIL类型变量      	
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//void _WriteByte2File(U8 Data, void * p) 
//{
//    sFLASH_NOR_Read
//	result = f_write (p, &Data, 1, &bw);
//}

void phone_windows(void)
{
     GUI_Clear();
	GUI_SetColor(GUI_RED);
	GUI_FillRect(0, 0, 320, 40);
    GUI_SetColor(GUI_WHITE);
	GUI_FillRect(0, 35, 320, 240);
    _ShowBMPEx(PIC_FIRE,10,50);
}
//自动登录界面
void LoopLogin(void)
{
	char arr[20]={0};
    GUI_Clear();
	GUI_SetColor(GUI_GRAY);
	GUI_FillRect(0, 0, 320, 40);
    GUI_SetColor(GUI_LIGHTGRAY);
	GUI_FillRect(0, 35, 320, 240);

	//显示表头主菜单 白字灰底
    GUI_SetColor(GUI_WHITE);
	GUI_SetBkColor(GUI_GRAY);
    GUI_SetFont(GUI_FONT_HZ24);
//	GUI_UC_SetEncodeUTF8();
    GUI_DispStringAt("自动登录",10,6);   

    GUI_SetColor(GUI_BLUE);
	GUI_SetBkColor(GUI_LIGHTGRAY);
    GUI_SetFont(GUI_FONT_HZ24);
//	GUI_UC_SetEncodeUTF8();
    GUI_DispStringAt("按确定键登录",10,216);  
    //   sprintf(arr,"%d-%d-%d %d:%d:%d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
	GUI_SetFont(&GUI_Font20_1);
    GUI_SetColor(GUI_BLACK);
	sprintf(arr,"%d-%02d-%02d %02d:%02d:%02d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
    GUI_DispStringAt(arr,150,217);    
}
//本机信息查询
void Info(void)
{
    char arr[20] = {0,};
    GUI_Clear();
	GUI_SetColor(GUI_GRAY);
	GUI_FillRect(0, 0, 320, 40);
    GUI_SetColor(GUI_LIGHTGRAY);
	GUI_FillRect(0, 35, 320, 240);

	//显示表头主菜单 白字灰底
    GUI_SetColor(GUI_WHITE);
	GUI_SetBkColor(GUI_GRAY);
    GUI_SetFont(GUI_FONT_HZ24);
//	GUI_UC_SetEncodeUTF8();
    GUI_DispStringAt("本机信息查询",10,6); 
     GUI_SetColor(GUI_BLACK);
    	GUI_SetBkColor(GUI_LIGHTGRAY);
  //  sprintf(arr,"%d-%d-%d %d:%d:%d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
	GUI_SetFont(&GUI_Font20_1);
	sprintf(arr,"%d-%02d-%02d %02d:%02d:%02d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
    GUI_DispStringAt(arr,150,217);    
}
//通信设置界面
void communcition(void)
{
    char arr[20] = {0,};
    GUI_Clear();
	GUI_SetColor(GUI_GRAY);
	GUI_FillRect(0, 0, 320, 40);
    GUI_SetColor(GUI_LIGHTGRAY);
	GUI_FillRect(0, 35, 320, 240);

	//显示表头主菜单 白字灰底
    GUI_SetColor(GUI_BLACK);
	GUI_SetBkColor(GUI_GRAY);
    GUI_SetFont(GUI_FONT_HZ24);
//	GUI_UC_SetEncodeUTF8();
    GUI_DispStringAt("通信设置",10,6); 
    GUI_SetBkColor(GUI_LIGHTGRAY);
    GUI_DispStringAt("CAN端口：",10,50);  
    GUI_DispStringAt("地址：",190,100); 
    GUI_DispStringAt("地址：",190,50); 
    GUI_DispStringAt("总线端口:",10,100);   
    GUI_SetColor(GUI_WHITE);
    GUI_FillRect(120, 50, 170, 75);    
    GUI_FillRect(250, 50, 310, 75); 
    GUI_FillRect(120, 100, 170, 125);    
    GUI_FillRect(250, 100, 310, 125); 
          GUI_SetBkColor(GUI_LIGHTGRAY);
      GUI_SetColor(GUI_BLUE);
    GUI_DispStringAt("按功能键编辑",10,215);
       GUI_SetColor(GUI_BLACK);
    //sprintf(arr,"%d-%d-%d %d:%d:%d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
	GUI_SetFont(&GUI_Font20_1);
	sprintf(arr,"%d-%02d-%02d %02d:%02d:%02d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
    GUI_DispStringAt(arr,150,217); 
}
//设备编辑
void DeviceSet(void)
{
    GUI_Clear();
	GUI_SetColor(GUI_GRAY);
	GUI_FillRect(0, 0, 320, 40);
    GUI_SetColor(GUI_LIGHTGRAY);
	GUI_FillRect(0, 35, 320, 240);

	//显示表头主菜单 白字灰底
    GUI_SetColor(GUI_WHITE);
	GUI_SetBkColor(GUI_GRAY);
    GUI_SetFont(GUI_FONT_HZ24);
//	GUI_UC_SetEncodeUTF8();
    GUI_DispStringAt("设备编辑",10,6);     
}
//密码输入
void PasswordLogin(void)
{
    char arr[20]={0};
    GUI_Clear();
	GUI_SetColor(GUI_GRAY);
	GUI_FillRect(0, 0, 320, 40);
    GUI_SetColor(GUI_LIGHTGRAY);
	GUI_FillRect(0, 35, 320, 240);

	//显示表头主菜单 白字灰底
    GUI_SetColor(GUI_WHITE);
	GUI_SetBkColor(GUI_GRAY);
    GUI_SetFont(GUI_FONT_HZ24);
//	GUI_UC_SetEncodeUTF8();
    GUI_DispStringAt("密码输入",10,6);  
// frme =   FRAMEWIN_CreateEx(50,50,160,80,0,WM_CF_SHOW,0,1,"PassWord",NULL);  
//    FRAMEWIN_GetActive(frme);
//    FRAMEWIN_SetClientColor(frme,GUI_WHITE);
//    FRAMEWIN_SetBarColor(frme,1,GUI_BLUE);
//    FRAMEWIN_SetFont(frme, GUI_FONT_32B_ASCII); //--------------(2)
//    FRAMEWIN_SetTextAlign(frme, GUI_TA_HCENTER | GUI_TA_VCENTER); //--------------(3)
//    FRAMEWIN_SetText(frme, "armfly");           //--------------(4) 
  	GUI_SetFont(&GUI_Font32_1);
	GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_LIGHTGRAY);
    //GUI_DispStringAt("-----------------------------------------------",20,75);
    GUI_DispStringAt("_  _  _  _",100,115);
    //   sprintf(arr,"%d-%d-%d %d:%d:%d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
	GUI_SetFont(&GUI_Font20_1);
	sprintf(arr,"%d-%02d-%02d %02d:%02d:%02d",Timer_value.year,Timer_value.month,Timer_value.date,Timer_value.hour,Timer_value.min,Timer_value.sec);
    GUI_DispStringAt(arr,150,217);     
}
void Message_(char *str,uint16_t x,uint16_t y)
{
     GUI_SetColor(GUI_BLUE);
	 GUI_SetBkColor(GUI_LIGHTGRAY);
     GUI_SetFont(GUI_FONT_HZ24);
	 GUI_DispStringAt(str,x,y);    
}
//截图程序
/*static void _WriteByte2File(uint8_t Data,void *p)
{
 
   
    // USB_Send(&Data,1);
      //TIMDelay_Nms(5);
    USB_Rx_Buffer[temp__++] = Data;
    if(temp__ == 2048)
    {
        temp__ = 0;
        norflash_write_flash(address__,USB_Rx_Buffer,2048);
        address__+=2048;
    }
  
    //
}
void emWin_CreateBMPPicture(uint8_t windex)
{
    uint8_t hFile;
    uint8_t sendbuff[20];
    nWritten = 0;
    USB_Rx_Buffer[0] = 0xAA;
    USB_Rx_Buffer[1] = 0xBB;
    USB_Rx_Buffer[2] = 0x02;
    sendbuff[0] = 0xAA;
    sendbuff[1] = 0xBB;
    sendbuff[2] = 0x01;
    sendbuff[3] = windex;
     USB_Send(sendbuff,4);
     TIMDelay_Nms(5);
    GUI_BMP_SerializeEx(_WriteByte2File,0,0,320,240, &USB_Rx_Buffer);
     sendbuff[2] = 0x02;
    USB_Send(sendbuff,3);
    //
}*/
