#define W18_QUERY_WINDOW_WIN_PRIVATE
#include "include.h"
extern uint8_t         WinPointer; 
void W18_QueryWindowDisplay(void)
{
    inquiry(WinPointer);
}

void W18_QueryWindowProcess(void)
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

