#define W26_PASSWORD_WINDOW_WIN_PRIVATE
#include "include.h"
extern void WinLoad(uint8_t index);
extern uint8_t         WinIndex;
uint8_t lastwindows = 0;//上页窗口地址
 uint8_t password_pointer = 0;
char const set_pass[] = {2,4,2,4};//进入设置密码
char const reset_pass[] = {2,2,4,4};
static uint8_t pass_flag = 0;//密码标志位
void dis_xinghao(uint8_t pass)
{
     	GUI_SetFont(&GUI_Font32_1);
	GUI_SetColor(GUI_BLACK);
    GUI_SetBkColor(GUI_LIGHTGRAY);
    //GUI_DispStringAt("-----------------------------------------------",20,75);
   // GUI_DispStringAt("_  _  _  _",100,115);   
  
    switch(pass)
    {
        case 0:
            GUI_DispStringAt("*",100,110);
            break;
        case 1:
            GUI_DispStringAt("*",132,110);
            break;
        case 2:
             GUI_DispStringAt("*",164,110);
            break;
        case 3:
             GUI_DispStringAt("*",196,110);
            break;
        default:
            break;
    }
}
void W26_PasswordWindowDisplay(void)
{
   //dis_xinghao(password_pointer);
  
}
void W26_PasswordWindowProcess(void)
{
    

    switch (WinKeyValue )
    {
        case KEY_OK:
            WinKeyValue = NULL;
             if((pass_flag == 4)&&(password_pointer == 4))
             {
                 pass_flag = 0;
                 password_pointer = 0;
                
                 WinLoad(lastwindows); 
                 return ;
             }
             else
             {
                 GUI_SetColor(GUI_LIGHTGRAY);
	             GUI_FillRect(90, 115, 200, 140);
                 GUI_SetFont(&GUI_Font32_1);
	             GUI_SetColor(GUI_BLACK);
                 GUI_SetBkColor(GUI_LIGHTGRAY);
                 GUI_DispStringAt("_  _  _  _",100,115);    
                 password_pointer =0;
                 pass_flag = 0;
               
                 return ;
             }
           // break;

        case KEY_UP:
             if(password_pointer == 0)
             {
                 if((lastwindows == 8)||(lastwindows == 22))//设置密码/回路登录
                 {
                      pass_flag++;
                 }
                 if(lastwindows == 13)//恢复出厂设置密码
                     
                 {
                      pass_flag++;
                 }
                
             }
             if(password_pointer == 2)
             {
                 if((lastwindows == 8)||(lastwindows == 22))//设置密码
                 {
                      pass_flag++;
                 }
                
             }
             if(password_pointer == 1)
             {

                if(lastwindows == 13)//恢复出厂设置密码
                     
                 {
                      pass_flag++;
                 }
                
             }

            break;

        case KEY_DOWN:
             if(password_pointer == 1)
             {
                 if((lastwindows == 8)||(lastwindows == 22))//设置密码
                 {
                      pass_flag++;
                 }

                
             }
             if(password_pointer == 3)
             {
                 if((lastwindows == 8)||(lastwindows == 22))//设置密码
                 {
                      pass_flag++;
                 }
                  if(lastwindows == 13)//恢复出厂设置密码
                     
                 {
                      pass_flag++;
                 }
                
             }
             if(password_pointer == 2)
             {
                 if(lastwindows == 13)//恢复出厂设置密码
                     
                 {
                      pass_flag++;
                 }  
             }
            break;

        case KEY_SLIENCE:
            password_pointer=0;
           pass_flag = 0;
            break;

        case KEY_FIRE:

            break;

        default:

            break;





    }
    dis_xinghao(password_pointer);
    password_pointer++;
//    if(password_pointer == 5)
//    {
//        password_pointer=0;
//    }
}

