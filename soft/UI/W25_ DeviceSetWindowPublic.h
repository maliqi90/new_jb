#ifndef _W25_DEVICE_SET_WINDOW_WIN_H_
#define _W25_DEVICE_SET_WINDOW_WIN_H_
#ifdef W25_DEVICE_SET_WINDOW_WIN_PRIVATE
	#define W25_DEVICE_SET_WINDOW_WIN_PUBLIC
#else
	#define W25_DEVICE_SET_WINDOW_WIN_PUBLIC extern
#endif
#include "DIALOG.h"
#define W25_DEVICE_SET_WINDOW_WIN	25
void W25_DeviceSetWindowDisplay(void);
void W25_DeviceSetWindowProcess(void);
WM_HWIN CreateWindow(void);
//用户定义



#endif

