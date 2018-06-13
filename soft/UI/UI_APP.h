#ifndef _UI_APP_H
#define _UI_APP_H
#define CLOSE_LCD_TIME          100 //���⵹��ʱ
#include "include.h"
//extern unsigned int  lcd_time;  //���⵹��ʱ
//uint8_t         WinKeyValue;//��ֵ
extern uint8_t         WinIndex;	//��ǰ��������
extern uint8_t         WinPointer;
extern void WinKeyProcess(void);
// char const NullCursor[] = {0};
//���ڽṹ������
typedef struct _WinClass
{
    uint8_t QuitWin;
    uint8_t property; //0Ĭ�Ͻ���//1���ý���//2��ʾ����//
    uint8_t Pointer;  //�������¼��˵��ĸ���
    uint8_t winclass_pr;//�ڼ����˵�
    uint8_t WinBranch[8];//�����¼��˵�
//    char const *pCursor;	//������
    void (*WinDisplay)(void);//������ʾ����
    void (*KeyTask)(void); //���水��������
   
}WIN_CLASS;
// char const TimeSetCursor[] = {0x06,0x04,0x02,0x02,0x02,0x02,0x02};
extern WIN_CLASS const WinClass[];
extern void WinLoad(uint8_t index);
extern void Key_Task(void);
#endif

