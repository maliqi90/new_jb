#ifndef _W17_EVENTQUERY_WINDOW_WIN_H_
#define _W17_EVENTQUERY_WINDOW_WIN_H_
#ifdef W17_EVENTQUERY_WINDOW_WIN_PRIVATE
	#define W17_EVENTQUERY_WINDOW_WIN_PUBLIC
#else
	#define W17_EVENTQUERY_WINDOW_WIN_PUBLIC extern
#endif

#define W17_EVENTQUERY_WINDOW_WIN	17
void W17_EventQueryWindowDisplay(void);
void W17_EventQueryWindowProcess(void);

//用户定义



#endif

