#ifndef _W9_SETTIME_WINDOW_WIN_H_
#define _W9_SETTIME_WINDOW_WIN_H_
#ifdef W9_SETTIME_WINDOW_WIN_PRIVATE
	#define WW9_SETTIME_WINDOW_WIN_PUBLIC
#else
	#define W9_SETTIME_WINDOW_WIN_PUBLIC extern
#endif

#define W9_SETTIME_WINDOW_WIN	9
void W9_SetTimeWindowDisplay(void);
void W9_SetTimeWindowProcess(void);

//用户定义



#endif

