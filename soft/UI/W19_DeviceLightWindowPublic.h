#ifndef _W19_DEVICELIGHT_WINDOW_WIN_H_
#define _W19_DEVICELIGHT_WINDOW_WIN_H_
#ifdef W19_DEVICELIGHT_WINDOW_WIN_PRIVATE
	#define W19_DEVICELIGHT_WINDOW_WIN_PUBLIC
#else
	#define W19_DEVICELIGHT_WINDOW_WIN_PUBLIC extern
#endif
#include "GUI.h"
#include "LISTVIEW.h"
#include "FRAMEWIN.h"
#define W19_DEVICELIGHT_WINDOW_WIN	19
void W19_DeviceLightWindowDisplay(void);
void W19_DeviceLightWindowProcess(void);
extern  LISTVIEW_Handle W19_hListView;
//用户定义



#endif

