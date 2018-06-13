#define W21_PHONE_WINDOW_WIN_PRIVATE
#include "include.h"
extern uint8_t         WinIndex;
uint8_t call_flag = 0;
void Call_Phone(void)
{
    uint8_t phone[2][12];
    char i = 0;
   char phone_string[12];

    norflash_read_phone(1,&phone[0][0]);//读取电话号码1
    norflash_read_phone(2,&phone[1][0]);//读取电话号码2
    for(i = 0;i < 12;i ++)
    {   
        phone_string[i] = phone[0][i]+0x30;
    }
    GUI_SetColor(GUI_RED);
	GUI_SetBkColor(GUI_WHITE);
	GUI_SetFont(&GUI_Font32_1);
    GUI_DispStringAt(phone_string,90,100);
    GUI_SetFont(GUI_FONT_HZ24);
    GUI_DispStringAt("正在呼叫",120,150);

    
}
void W21_PhoneWindowDisplay(void)
{
     phone_windows();
     WinIndex = W21_PHONE_WINDOW_WIN;
     Time_interr = 0;
      Call_Phone();
}
void W21_PhoneWindowProcess(void)
{
    switch (WinKeyValue )
    {
        case KEY_OK:

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

