#ifndef _W12_SHIELDSET_WINDOW_WIN_H_
#define _W12_SHIELDSET_WINDOW_WIN_H_
#ifdef W12_SHIELDSET_WINDOW_WIN_PRIVATE
	#define W12_SHIELDSET_WINDOW_WIN_PUBLIC
#else
	#define W12_SHIELDSET_WINDOW_WIN_PUBLIC extern
#endif
#include "GUI.h"
#include "LISTVIEW.h"
#include "FRAMEWIN.h"
#define W12_SHIELDSET_WINDOW_WIN	12
void W12_ShieldSetWindowDisplay(void);
void W12_ShieldSetWindowProcess(void);
extern char dis_row[17];
extern  LISTVIEW_Handle W12_hListView;
//用户定义



#endif

