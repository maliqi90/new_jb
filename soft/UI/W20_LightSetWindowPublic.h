#ifndef _W20_LIGHT_SET_WINDOW_WIN_H_
#define _W20_LIGHT_SET_WINDOW_WIN_H_
#ifdef W20_LIGHT_SET_WINDOW_WIN_PRIVATE
	#define W20_LIGHT_SET_WINDOW_WIN_PUBLIC
#else
	#define W20_LIGHT_SET_WINDOW_WIN_PUBLIC extern
#endif

#define W20_LIGHT_SET_WINDOW_WIN	20
    #include "DIALOG.h"
void W20_LightSetWindowDisplay(void);
void W20_LightSetWindowProcess(void);
extern WM_HWIN  slider;
//用户定义



#endif

