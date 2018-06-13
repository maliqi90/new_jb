#define W7_SHELFCHECK_WINDOW_WIN_PRIVATE
#include "include.h"
#include "UI_APP.h"

uint8_t voiceself_flag = 0;
extern uint8_t         LastWinPointer[4];
extern uint8_t         WinPointer;
//extern void WinLoad(uint8_t index);
//声音自检
void self_voice(void)
{
    voiceself_flag = 1;
}
void lcd_self(void)
{
   // Play_Voice(NORFLASH_FIRE_VOICE_BASE,1);
    LED_ALARM_ON;
    LED_FAULT_ON ;
    LED_SELTFTTEST_ON;
    LED_BAT_ON;
    LED_POWER_ON;
    GUI_SetColor(GUI_WHITE);//白色
	GUI_FillRect(0, 0, 320, 240);
    TIMDelay_Nms(500);
    TIMDelay_Nms(500);
    GUI_SetColor(GUI_RED);//红色
	GUI_FillRect(0, 0, 320, 240);
    TIMDelay_Nms(500);
    TIMDelay_Nms(500);
    GUI_SetColor(GUI_BLACK);//黑色
	GUI_FillRect(0, 0, 320, 240);
    TIMDelay_Nms(500);
    TIMDelay_Nms(500);
//     Play_Voice(NORFLASH_FAULT_VOICE_BASE,1);
    GUI_SetColor(GUI_GREEN);//蓝色
	GUI_FillRect(0, 0, 320, 240);
    TIMDelay_Nms(500);
    TIMDelay_Nms(500);
    GUI_SetColor(GUI_YELLOW);//黄色
	GUI_FillRect(0, 0, 320, 240);
    TIMDelay_Nms(500);
    TIMDelay_Nms(500);
//     Play_Voice(NORFLASH_COMMFAULT_VOICE_BASE,1);
        LED_ALARM_OFF;
    LED_FAULT_OFF ;
    LED_SELTFTTEST_OFF;
    LED_BAT_OFF;
    LED_POWER_OFF;
}
void W7_SelfcheckWindowDisplay(void)
{
    //lED灯自检
    //屏幕自检
    lcd_self();

            WinPointer = LastWinPointer[WinClass[W3_MENU_WINDOW_WIN].winclass_pr];//初始化光标指针
            WinLoad(W3_MENU_WINDOW_WIN);
}

void W7_SelfcheckWindowProcess(void)
{
    switch (WinKeyValue )
    {
        case KEY_OK:
//            voicenumflag++;
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

