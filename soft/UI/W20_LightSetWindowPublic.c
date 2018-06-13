#define W20_DEVICESET_WINDOW_WIN_PRIVATE
#include "include.h"

#include "DIALOG.h"

#define ID_WINDOW_0 (GUI_ID_USER + 0x00)
#define ID_SLIDER_0 (GUI_ID_USER + 0x02)
WM_HWIN  slider;


 uint16_t light_set= 0;//背光调节设置
//static void Dis_num(uint8_t num)
//{
//    GUI_SetBkColor(GUI_WHITE);
//	GUI_SetColor(GUI_RED);
//    //显示编号
//	GUI_SetFont(&GUI_Font24_1);
//    GUI_DispDecAt(num, 153,100, 3);   
//}
void W20_LightSetWindowDisplay(void)
{

    slider = PROGBAR_Create(102, 106,100,20,WM_CF_SHOW);
    GUI_Exec();
    PROGBAR_SetBarColor(slider,0,GUI_GREEN);
    PROGBAR_SetMinMax(slider,0,100);
    PROGBAR_SetText(slider,"");
    norflash_read_LightSet(&light_set);
    PROGBAR_SetValue(slider,light_set/5);
            //PROGBAR_SetText(slider,(const char*)(light_set+0x30));
    GUI_SetBkColor(GUI_LIGHTGRAY);
	GUI_SetColor(GUI_RED);
    //显示编号
	GUI_SetFont(&GUI_Font24_1);
    GUI_DispDecAt(light_set/5, 210,106, 3);  
   // SLIDER_SetValue(slider,20);
     GUI_Exec();
    //light_set = 0;
    

}

void W20_LightSetWindowProcess(void)
{
//    char i = 0;
    
    switch (WinKeyValue )
    {
        case KEY_OK:

            break;

        case KEY_UP:
            //light_set =  SLIDER_GetValue(slider);
            if(light_set < 500)
            {
             light_set+=50;
 
            }
            PWM_Set(light_set);
            norflash_write_LightSet(light_set);
             PROGBAR_SetValue(slider,light_set/5);
             GUI_SetColor(GUI_RED);

	         GUI_SetFont(&GUI_Font24_1);
             GUI_DispDecAt(light_set/5, 210,106, 3);  
             GUI_Exec();

            break;

        case KEY_DOWN:
            if(light_set>50)
            {
             light_set-=50;
            }
             PROGBAR_SetValue(slider,light_set/5);
             GUI_SetColor(GUI_RED);
	         GUI_SetFont(&GUI_Font24_1);
             GUI_DispDecAt(light_set/5, 210,106, 3);  
            PWM_Set(light_set);
            norflash_write_LightSet(light_set);
             GUI_Exec();
            break;

        case KEY_SLIENCE:
             PROGBAR_Delete(slider);
             GUI_Exec();
            break;

        case KEY_FIRE:

            break;

        default:

            break;



    //WinKeyValue = KEY_NULL;

    }
}

