#ifndef _UI_APP_H
#define _UI_APP_H
#define CLOSE_LCD_TIME          100 //背光倒计时
#include "include.h"
//extern unsigned int  lcd_time;  //背光倒计时
//uint8_t         WinKeyValue;//键值
extern uint8_t         WinIndex;	//当前窗口索引
extern uint8_t         WinPointer;
extern void WinKeyProcess(void);
// char const NullCursor[] = {0};
//窗口结构体声明
typedef struct _WinClass
{
    uint8_t QuitWin;
    uint8_t property; //0默认界面//1设置界面//2显示界面//
    uint8_t Pointer;  //界面上下级菜单的个数
    uint8_t winclass_pr;//第几级菜单
    uint8_t WinBranch[8];//界面下级菜单
//    char const *pCursor;	//界面光标
    void (*WinDisplay)(void);//界面显示函数
    void (*KeyTask)(void); //界面按键处理函数
   
}WIN_CLASS;
// char const TimeSetCursor[] = {0x06,0x04,0x02,0x02,0x02,0x02,0x02};
extern WIN_CLASS const WinClass[];
extern void WinLoad(uint8_t index);
extern void Key_Task(void);
#endif

