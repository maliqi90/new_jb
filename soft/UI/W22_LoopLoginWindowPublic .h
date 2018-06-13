#ifndef _W22_LOOPLOGIN_WINDOW_WIN_H_
#define _W22_LOOPLOGIN_WINDOW_WIN_H_
#ifdef W22_LOOPLOGIN_WINDOW_WIN_PRIVATE
	#define W22_LOOPLOGIN_WINDOW_WIN_PUBLIC
#else
	#define W22_LOOPLOGIN_WINDOW_WIN_PUBLIC extern
#endif
#include "GUI.h"
#include "LISTVIEW.h"
#include "FRAMEWIN.h"
#include "PROGBAR.h"
#define W22_LOOPLOGIN_WINDOW_WIN	22
void W22_LoopLoginWindowDisplay(void);
void W22_LoopLoginWindowProcess(void);

    typedef struct
    {
        uint8_t flag;
        uint8_t value;
    } PROGBAR_;
//用户定义
void loopbar(void);
extern  LISTVIEW_Handle W22_hListView;;
 extern PROGBAR_Handle LoopBar;
#endif

