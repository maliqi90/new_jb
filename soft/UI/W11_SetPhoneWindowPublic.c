#define W11_SETPHONE_WINDOW_WIN_PRIVATE
#include "include.h"
uint8_t phone[2][11];
uint8_t phone_flag = 0;
static uint8_t select_num = 0;
/****************************************************
函数名称：void phonenum_dis(uint8_t *num,uint8_t sel,int x,int y)
函数功能：显示电话号码
输入参数：
输出参数
*******************************************************/
void phonenum_dis(uint8_t sel)
{
    int i;
    int x1 = 142,y1 = 61,x2 = 142,y2 = 111;
    GUI_SetColor(GUI_BLACK);
	GUI_SetBkColor(GUI_LIGHTGRAY);
	GUI_SetFont(&GUI_Font24_1);
    for(i = 0;i < 22;i++)
    {
        if(phone[i/11][i%11] >9)
        {
            phone[i/11][i%11] = 0;
        }
        if(i > 10)
        {
           GUI_DispDecAt(phone[i/11][i%11], x2+(i-11)*16, y2, 1); 
        }
        else
        {
            GUI_DispDecAt(phone[i/11][i%11], x1+i*16, y1, 1);
        }
        
    } 
    //反显选中的
     GUI_SetColor(GUI_WHITE);
	GUI_SetBkColor(GUI_GRAY);
	GUI_SetFont(&GUI_Font24_1);
    if(sel>10)
    {
        GUI_DispDecAt(phone[sel/11][sel%11],x2+(sel-11)*16, y2, 1);  
    }
    else
    {
        GUI_DispDecAt(phone[sel/11][sel%11],x1+sel*16, y1, 1);
    }

    
    
}
void W11_SetPhoneWindowDisplay(void)
{
    select_num = 0;
    norflash_read_phone(1,&phone[0][0]);
    norflash_read_phone(2,&phone[1][0]);
    phonenum_dis(0);
}

void W11_SetPhoneWindowProcess(void)
{
    switch (WinKeyValue )
    {
        case KEY_OK:
            if(select_num < 21)
            {
               select_num++;
            }
            else
            {
                select_num = 0;
            }
            phonenum_dis(select_num);
            break;

        case KEY_UP:
            if(phone[select_num/11][select_num%11] > 9)
            {
                phone[select_num/11][select_num%11] = 0;
            }
            else
            {
                phone[select_num/11][select_num%11]++;
            }
            phonenum_dis(select_num);
            break;

        case KEY_DOWN:
             if(phone[select_num/11][select_num%11]==0)
             {
                 phone[select_num/11][select_num%11] = 9;
             }
             else
             {
                  phone[select_num/11][select_num%11]--;
             }
             phonenum_dis(select_num);
            break;

        case KEY_SLIENCE:
              norflash_write_phone(1,&phone[0][0]);
              norflash_write_phone(2,&phone[1][0]);
            break;

        case KEY_FIRE:

            break;

        default:

            break;





    }
}
//电话号码初始化
void phone_init(void)
{
    uint8_t temp = 0;
    uint8_t i,j;
    norflash_read_phone(1,(uint8_t *)&phone[0][0]);
    norflash_read_phone(2,(uint8_t *)&phone[1][0]);
    for(i = 0;i < 2;i ++)
    {
        for(j = 0;j < 11;j++)
        {
            if(phone[i][j] == 0)
            {
                temp++;
            }
           if(phone[i][j]>9)
           {
               if(i == 0)
               {
                   phone_flag = 0;
               }
               else
               {
                    phone_flag &= ~(0x02);
               }
               break;
           }
        }
        if((temp == 11)&&(i == 0)&&(j == 11))
        {
            phone_flag = 0;
        }
        else if((temp < 11)&&(i == 0)&&(j == 11))
        {
             phone_flag |= 0x01;
        }
        
        if((temp == 11)&&(i == 1)&&(j == 11))
        {
            phone_flag = 0;
        }
        else if((temp < 11)&&(i == 1)&&(j == 11))
        {
             phone_flag |= 0x02;
        }
        
       
    }
    if(phone_flag == 0)
    {
      GUI_SetColor(GUI_RED);
	  GUI_SetBkColor(GUI_LIGHTBLUE);
	  GUI_SetFont(GUI_FONT_HZ24);
      GUI_DispStringAt("未设置联系人！",160,180);       
    }
}
