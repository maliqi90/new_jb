#ifndef _W8_SET_WINDOW_WIN_H_
#define _W8_SET_WINDOW_WIN_H_
#ifdef W8_SET_WINDOW_WIN_PRIVATE
	#define W8_SET_WINDOW_WIN_PUBLIC
#else
	#define W8_SET_WINDOW_WIN_PUBLIC extern
#endif

#define W8_SET_WINDOW_WIN	8
void W8_SetWindowDisplay(void);
void W8_SetWindowProcess(void);

//用户定义



#endif

