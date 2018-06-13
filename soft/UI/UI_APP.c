#include "include.h"
#include "UI_APP.h"
#include "GUI.h"
#include "DIALOG.h"
extern  void _ShowBMPEx(uint8_t bmp,int x,int y); 
uint8_t         WinIndex = 0;	//当前窗口索引
uint8_t         WinPointer = 0; //光标指针
uint8_t         LastWinPointer[4];//上一个窗体光标位置
volatile unsigned int  lcd_time;  //背光倒计时
uint8_t voicenum = 0;
extern  uint16_t light_set;
//static uint8_t buff[4] = "AT\n";
extern uint8_t lastwindows;
extern uint8_t password_pointer;
uint8_t voicenumflag = 0;
uint8_t hand_fire_flag = 0;
/* 实际的测试需要是图像宽度的4倍即可，切记(也就是保证每个像素如果是32位数据的情况) */
//static char _acBuffer[480*4];
//窗口界面函数数组
WIN_CLASS const WinClass[] =
{
    //登录界面
    {
     0,
     0,
     0,
     0,
     0,0,0,0,0,0,0,0,
//     NullCursor,
     W1_MainWindowDisplay,
     W1_MainWindowProcess
    },
   //监控界面
    {
     0,
     0,
     0,
     0,
     0,0,0,0,0,0,0,0,
//     NullCursor,
     W1_MainWindowDisplay,
     W1_MainWindowProcess
    },
    {
   //监控界面
     0,
     0,
     0,
        0,
     0,0,0,0,0,0,0,0,
//     NullCursor,
     W2_MainWindowDisplay,
     W2_MainWindowProcess
    }
    ,
    //主菜单界面
    {
     2,
     1,
     6,
     1,
     8,18,7,22,20,19,0,0, //设置/查询/自检/器件点灯
//     NullCursor,
     W3_MenuWindowDisplay,
     W3_MenuWindowProcess  
        
    },
    //故障显示界面
    {
     0,
     2,
     0,
     0,
     0,0,0,0,0,0,0,0,
//     NullCursor,
     W4_FaultWindowDisplay,
     W4_FaultWindowProcess  
        
    },
    //火警界面
     {
      0,
      2,
      0,
         0,
      0,0,0,0,0,0,0,0,
//      NullCursor,
      W5_FireWindowDisplay,
      W5_FireWindowProcess  
        
     }, 
     //屏蔽界面
     {
      0,
      2,
      0,
         0,
      0,0,0,0,0,0,0,0,
//      NullCursor,
      W6_ShieldWindowDisplay,
      W6_ShieldWindowProcess  
        
     },  
     //自检界面
     {
      3,
      0,
      0,
         0,
      0,0,0,0,0,0,0,0,
//      NullCursor,
      W7_SelfcheckWindowDisplay,
      W7_SelfcheckWindowProcess  
        
     },
     //设置界面
     {
      3,
      1,
      7,
      2,
      9,10,11,12,13,23,24,0,
//      NullCursor,
      W8_SetWindowDisplay,
      W8_SetWindowProcess  
        
     },
     //时间设置界面
     {
      8,
      0,
      0,
      3,
      0,0,0,0,0,0,0,0,
//      NullCursor,
      W9_SetTimeWindowDisplay,
      W9_SetTimeWindowProcess  
        
     },
     //器件编辑界面
     {
      8,
      0,
      0,
      3,
      0,0,0,0,0,0,0,0,
//      NullCursor,
      W10_EditDeivceWindowDisplay,
      W10_EditDeivceWindowProcess  
        
     },  
     // 电话号码设置界面
     {
      8,
      0,
      0,
      3,
     0,0,0,0,0,0,0,0,
//      NullCursor,
      W11_SetPhoneWindowDisplay,
      W11_SetPhoneWindowProcess  
        
     },  
      // 屏蔽设置界面 
     {
      8,
      0,
      0,
      3,
      0,0,0,0,0,0,0,0,
//      NullCursor,
      W12_ShieldSetWindowDisplay,
      W12_ShieldSetWindowProcess  
        
     }, 
     //恢复出厂设置界面
     {
      8,
      1,
      0,
      3,
      0,0,0,0,0,0,0,0,
//      NullCursor,
      W13_ReSetWindowDisplay,
      W13_ReSetWindowProcess  
        
     }, 
     //火警查询界面
     {
      18,
      0,
      0,
      3,
      0,0,0,0,0,0,0,0,
//      NullCursor,
      W14_FireQueryWindowDisplay,
      W14_FireQueryWindowProcess  
        
     }, 
     //故障查询界面 
     {
      18,
      0,
      0,
      3,
      0,0,0,0,0,0,0,0,
//      NullCursor,
      W15_FaultQueryWindowDisplay,
      W15_FaultQueryWindowProcess  
        
     }, 
     //屏蔽查询界面
     {
      18,
      0,
      0,
      3,
      0,0,0,0,0,0,0,0,
//      NullCursor,
      W16_ShieldQueryWindowDisplay,
      W16_ShieldQueryWindowProcess  
        
     }, 
     //事件查询界面
     {
      18,
      0,
      0,
      3,
     0,0,0,0,0,0,0,0,
//      NullCursor,
      W17_EventQueryWindowDisplay,
      W17_EventQueryWindowProcess  
        
     },  
     //查询界面
     {
      3,
      1,
      2,
      2,
      14,17,0,0,0,0,0,0, //火警查询/故障查询/屏蔽查询/事件查询
//      NullCursor,
      W18_QueryWindowDisplay,
      W18_QueryWindowProcess  
        
     },
     //器件点灯界面
     {
      3,
      0,
      0,
      2,
      0,0,0,0,0,0,0,0,
//      NullCursor,
      W19_DeviceLightWindowDisplay,
      W19_DeviceLightWindowProcess  
        
     }, 
     //背光调节界面
     {
      3,
      0,
      0,
      2,
      0,0,0,0,0,0,0,0,
      W20_LightSetWindowDisplay,
      W20_LightSetWindowProcess,
     }  
     , 
     //电话拨打界面
     {
      0,
      0,
      0,
      0,
      0,0,0,0,0,0,0,0,
      W21_PhoneWindowDisplay,
      W21_PhoneWindowProcess,
     }  
     ,
     //自动登录界面
     {
      3,
      0,
      0,
      2,
      0,0,0,0,0,0,0,0,
//      NullCursor,
      W22_LoopLoginWindowDisplay,
      W22_LoopLoginWindowProcess          
     }
     //23本机信息查询
     , 	
     {
      8,
      1,
      0,
      3,
      0,0,0,0,0,0,0,0,
//      NullCursor,
      W23_InfoWindowDisplay,
      W23_InfoWindowProcess          
     }
     //24通信设置
     , 
     {
      8,
      0,
      0,
      3,
      0,0,0,0,0,0,0,0,
//      NullCursor,
      W24_CommunicationWindowDisplay,
      W24_CommunicationWindowProcess          
     }
     //25 器件设置
     ,  
     {
      10,
      1,
      0,
      3,
      0,0,0,0,0,0,0,0,
//      NullCursor,
      W25_DeviceSetWindowDisplay,
      W25_DeviceSetWindowProcess          
     }
     //26 密码输入框
     , 
     {
      3,
      1,
      0,
      3,
      0,0,0,0,0,0,0,0,
//      NullCursor,
      W26_PasswordWindowDisplay,
      W26_PasswordWindowProcess          
     }
           

};
void WinLoad(uint8_t index)
{
    WinIndex = index;
    //WinPointer = 0;
   switch(index)
   {
       case 0:
           
           break;
       case W1_MAIN_WINDOW_WIN:
           began();
           break;
       case W2_MAIN_WINDOW_WIN:
             Stay_Flag();
             Time_interr = 1;
           break;
       case W3_MENU_WINDOW_WIN:
           menudis();
           break;
       case W4_FAULT_WINDOW_WIN:
           guzhang();
           break;
       case W5_FIRE_WINDOW_WIN:
           huojing();
           break;
       case W6_SHIELD_WINDOW_WIN:
           Shield();
           break;
       case W7_SHELFCHECK_WINDOW_WIN:
            chekself();
           break;
       case W8_SET_WINDOW_WIN:
             setdis();
           break;
       case W9_SETTIME_WINDOW_WIN:
           tim_set();
           break;
       case W10_EDITDEVICE_WINDOW_WIN:
           qjbj_dis();
           break;
       case W11_SETPHONE_WINDOW_WIN:
           set_phone_num();
           break;
       case W12_SHIELDSET_WINDOW_WIN:
           pbjcpb_dis();
           break;
       case W13_RESET_WINDOW_WIN:
            Dis_ClearFlash();
           break;
       case W14_FIREQUERY_WINDOW_WIN:
           dis_fireevent();
           break;
       case W15_FAULTQUERY_WINDOW_WIN:
           dis_faultevent();
           break;
       case W16_SHIELDQUERY_WINDOW_WIN:
           dis_sheildevent();
           break;
       case W17_EVENTQUERY_WINDOW_WIN:
           dis_devent();
           break;
       case W18_QUERY_WINDOW_WIN:
            inquirydis();
           break;
       case W19_DEVICELIGHT_WINDOW_WIN:
           qjdd_dis();
           break;
       case W20_LIGHT_SET_WINDOW_WIN :
           Dis_Light_Set();

             break;
       case W21_PHONE_WINDOW_WIN:
           phone_windows();
           break;
       case W22_LOOPLOGIN_WINDOW_WIN:
            LoopLogin();
           break;
       case W23_INFO_WINDOW_WIN:
           Info();
           break;
       case W24_COMMUNICATION_WINDOW_WIN:
           communcition();
           break;
       case W25_DEVICE_SET_WINDOW_WIN:
            qjbjdis();
           break;
       case W26_PASSWORD_WINDOW_WIN:
           PasswordLogin();
           password_pointer=0;
           break;
       default:
           break;
       
   }
   WinClass[WinIndex].WinDisplay();
  // WinClass[WinIndex].KeyTask(); 
}

void WinKeyProcess(void)
{
    
    uint8_t winindex = 0;
    uint8_t send_buff[7];
   // lcd_time = CLOSE_LCD_TIME;//点亮背光
    //添加点亮背光函数
    if (EventSwitchTimer != 0)
    {
        EventSwitchTimer = 10;
    }
    if (WinKeyTimer != 0)
    {
        WinKeyTimer = 10;
    }
    lcd_time = LCD_TIME;
   PWM_Set(light_set);
    WinClass[WinIndex].KeyTask();//进入各窗口函数
    switch (WinKeyValue )
    {
        case KEY_OK:
                      //  voicenumflag++;
//        if(voicenumflag == 1)
//        {
//            Play_Voice(NORFLASH_BATFAULT_VOICE_BASE,0);
//        }
//        else if(voicenumflag == 2)
//        {
//             Play_Voice(NORFLASH_FAULT_VOICE_BASE,1);
//        }
//        else if(voicenumflag == 3)
//        {
//             Play_Voice(NORFLASH_HONMEFIRE_VOICE_BASE,0);
//        }
//        else if(voicenumflag == 4)
//        {
//             Play_Voice(NORFLASH_KAIJI_VOICE_BASE,0);
//        }
//         else if(voicenumflag == 5)
//         {
//             Play_Voice(NORFLASH_COMMFAULT_VOICE_BASE,0); 
//         }
//         else if(voicenumflag == 6)
//         {
//              Play_Voice(NORFLASH_POWERFAULT_VOICE_BASE,0);  
//             
//         }else if(voicenumflag == 7)
//         {
//              Play_Voice(NORFLASH_FIRE_VOICE_BASE,1);  
//             voicenumflag = 0;
//         }
//             EventAlarmCount = 1;
//        Led_Ctrl(LEDARAM_ON);
              //uart2_send(4,buff);
             if((WinIndex == W2_MAIN_WINDOW_WIN)||(WinIndex == W4_FAULT_WINDOW_WIN)||(WinIndex == W5_FIRE_WINDOW_WIN)||(WinIndex == W6_SHIELD_WINDOW_WIN))
             {
                 Time_interr = 0;
                 WinPointer = 0;
                WinLoad(W3_MENU_WINDOW_WIN);
                 WinKeyTimer = 10;
                 break;
                                 
             }  
             else
             {

                     if(WinClass[WinIndex].property == 1)
                     {
                         Time_interr = 0;
                        winindex = WinClass[WinIndex].WinBranch[WinPointer];
                         if(winindex == 8)
                         { 
                             lastwindows = 8;
                             WinLoad(26); 
                         }
                         
                         else if(winindex == 13)
                         {
                            lastwindows = 13;
                             WinLoad(26);
                         }
                         else
                         {
                          if(winindex == 0)
                           {
                              break;
                           }
                           LastWinPointer[WinClass[WinIndex].winclass_pr] = WinPointer;//保存上一个窗体光标指针
                           WinPointer = 0;
                           WinLoad(winindex); 
                       }
                          key_flag = 0;
                          WinKeyValue = KEY_NULL;                
                          break;
                       
                                 
             }
           }


            break;

        case KEY_UP:
//             EventFaultCount = 1;
//            Led_Ctrl(LEDFAULT_ON);
         // emWin_CreateBMPPicture(WinIndex);

             if(WinClass[WinIndex].property == 1)
             {
                if(WinPointer > 0)
                {
                    WinPointer--;
                }
                else
                {
                    WinPointer = WinClass[WinIndex].property-1;
                }
               
                 WinClass[WinIndex].WinDisplay();

             }
            break;

        case KEY_DOWN:
//            EventAlarmCount = 0;
//            Led_Ctrl(LEDARAM_OFF);
            if(WinClass[WinIndex].property == 1)
            {
                if(WinPointer < WinClass[WinIndex].Pointer)
                {
                    WinPointer++;
                    if(WinPointer == WinClass[WinIndex].Pointer)
                    {
                        WinPointer = 0;
                        WinLoad(WinIndex); 
                    }
                }
 
                WinClass[WinIndex].WinDisplay();
            }
            break;

        case KEY_SLIENCE:
            //Voice(SLIENCE_VOICE,1);
//         Led_Ctrl(LEDFAULT_OFF);
//        EventFaultCount = 0;
            slience();
             
             if(voice_flag == 1)
             {
                 slienceflag = 1;
             }
            winindex = WinClass[WinIndex].QuitWin;
            if(winindex == 0)
            {                  
                break;
            }
            if(WinIndex == W26_PASSWORD_WINDOW_WIN)
            {

                if(lastwindows == 8)
                {
                    WinPointer = LastWinPointer[WinClass[winindex].winclass_pr];//初始化光标指针
                    WinLoad(3);
                }
                else if(lastwindows == 13)
                {
                    WinPointer = 4;//初始化光标指针
                    WinLoad(8);   
                     lastwindows =0;                    
                }

            }
            else
            {
               WinPointer = LastWinPointer[WinClass[winindex].winclass_pr];//初始化光标指针
                if(lastwindows == 13)
                {
                    lastwindows =0;
                     WinPointer = 4;//初始化光标指针
                }
               WinLoad(winindex);
            }

            break;

        case KEY_FIRE:
            if(hand_fire_flag == 0)
            {
               // GUI_Init();
              // WinLoad(W2_MAIN_WINDOW_WIN);
                hand_fire_flag = 1;
               send_buff[1] = EVENT_TYPE_ALARM;
               send_buff[2] = 230;
               send_buff[3] = 0xFE;
               send_buff[4] = 0;
               send_buff[5] = 0;
               send_buff[6] = 0;
               Loop_Revice(send_buff);
            }
             // Voice(voicenum,2);
              //voicenum++;
//            if(voicenum == 10)
//            {
//              voicenum = 0;
//            }
            break;
        case KEY_RESET:
            SCB->AIRCR	= ((0x5FA << 16) | (SCB->AIRCR & (0x700)) | (1 << 2)); //复位
            break;
        case KEY_SHUTDOWN: //关机键
            
            break;
        default:

            break;
     
                
    }

    WinKeyValue = KEY_NULL;
    key_flag = 0;
}
    
//按键处理程序
void Key_Task(void)
{
   if(key_flag)
   {
      WinKeyProcess();
   }       
}


