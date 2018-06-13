#ifndef _W13_RESET_WINDOW_WIN_H_
#define _W13_RESET_WINDOW_WIN_H_
#ifdef W13_RESET_WINDOW_WIN_PRIVATE
	#define W13_RESET_WINDOW_WIN_PUBLIC
#else
	#define W13_RESET_WINDOW_WIN_PUBLIC extern
#endif

#define W13_RESET_WINDOW_WIN	13
void W13_ReSetWindowDisplay(void);
void W13_ReSetWindowProcess(void);

//用户定义



#endif

