#define W23_INFO_WINDOW_WIN_PRIVATE
#include "include.h"
extern uint8_t         WinIndex;

//显示本机信息
void Dis_Info(void)
{
       GUI_SetColor(GUI_BLACK);
       GUI_SetBkColor(GUI_LIGHTGRAY);
       GUI_SetFont(GUI_FONT_HZ24);
       GUI_DispStringAt("软件版本：",20,55);
//       GUI_DispStringAt("",55,100);
//       GUI_DispStringAt("本机电话号码：",20,100);
    
       GUI_SetFont(&GUI_Font24_1);
       GUI_DispStringAt("V1.00",140,55);

}

void W23_InfoWindowDisplay(void)
{
       Dis_Info();
}
void W23_InfoWindowProcess(void)
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

