#define W3_MENU_WINDOW_WIN_PRIVATE
#include "include.h"
extern uint8_t         WinPointer; 
void W3_MenuWindowDisplay(void)
{
   menu(WinPointer);   
}

void W3_MenuWindowProcess(void)
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

