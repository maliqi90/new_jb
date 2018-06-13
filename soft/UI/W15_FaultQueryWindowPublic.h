#ifndef _W15_FAULTQUERY_WINDOW_WIN_H_
#define _W15_FAULTQUERY_WINDOW_WIN_H_
#ifdef W15_FAULTQUERY_WINDOW_WIN_PRIVATE
	#define W15_FAULTQUERY_WINDOW_WIN_PUBLIC
#else
	#define W15_FAULTQUERY_WINDOW_WIN_PUBLIC extern
#endif

#define W15_FAULTQUERY_WINDOW_WIN	15
void W15_FaultQueryWindowDisplay(void);
void W15_FaultQueryWindowProcess(void);

//用户定义



#endif

