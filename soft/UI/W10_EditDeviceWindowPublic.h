#ifndef _W10_EDITDEVICE_WINDOW_WIN_H_
#define _W10_EDITDEVICE_WINDOW_WIN_H_
#ifdef W10_EDITDEVICE_WINDOW_WIN_PRIVATE
	#define W10_EDITDEVICE_WINDOW_WIN_PUBLIC
#else
	#define W10_EDITDEVICE_WINDOW_WIN_PUBLIC extern
#endif
#include "GUI.h"
#include "LISTVIEW.h"
#include "FRAMEWIN.h"
#define W10_EDITDEVICE_WINDOW_WIN	10
void W10_EditDeivceWindowDisplay(void);
void W10_EditDeivceWindowProcess(void);
extern const char device_caption1[][10];
extern uint8_t device_allindex[16];
extern const char* zonenum[];
extern const DEVICE_TYPE device_type1[];
extern  LISTVIEW_Handle W10_hListView;
//extern  void Dis_Device_(DEVICE_CONFIG *device);

//�û�����
//λ����Ϣ
//const char device_caption1[][10] = 
//{
//    {"����  "},
//    {"������"},
//    {"����  "},
//    {"�鷿  "},
//    {"��̨  "},
//};

////��������
//const char device_type[][10] = 
//{
//    {"�̸�"},
//    {"�¸�"},
//    {"�ֱ�"},
//};


#endif

