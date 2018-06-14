#ifndef _W7_SHELFCHECK_WINDOW_WIN_H_
#define _W7_SHELFCHECK_WINDOW_WIN_H_
#ifdef W7_SHELFCHECK_WINDOW_WIN_PRIVATE
	#define W7_SHELFCHECK_WINDOW_WIN_PUBLIC
#else
	#define W7_SHELFCHECK_WINDOW_WIN_PUBLIC extern
#endif

#define W7_SHELFCHECK_WINDOW_WIN	7
void W7_SelfcheckWindowDisplay(void);
void W7_SelfcheckWindowProcess(void);
extern void selfcheck(void);
//用户定义



#endif

