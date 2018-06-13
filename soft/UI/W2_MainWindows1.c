#define W2_MAIN_WINDOW_WIN_PRIVATE
#include "include.h"
extern uint8_t         WinIndex;
extern void WinLoad(uint8_t index);
void W2_MainWindowDisplay(void)
{
    //WinLoad(W2_MAIN_WINDOW_WIN);
   WinIndex = W2_MAIN_WINDOW_WIN;
    
   // Stay_Flag();
}

void W2_MainWindowProcess(void)
{
    switch (WinKeyValue )
    {
        case KEY_OK:
             //Time_interr = 1;
            break;

        case KEY_UP:
           
            break;

        case KEY_DOWN:
          

            break;

        case KEY_SLIENCE:
           
             W5_FireWindowDisplay();
            break;

        case KEY_FIRE:
          
            break;

        default:

            break;





    }
}

