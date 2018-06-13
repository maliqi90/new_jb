#ifndef _W26_PASSWORD_WINDOW_WIN_H_
#define _W26_PASSWORD_WINDOW_WIN_H_
#ifdef W26_PASSWORD_WINDOW_WIN_PRIVATE
	#define W26_PASSWORD_WINDOW_WIN_PUBLIC
#else
	#define W26_PASSWORD_WINDOW_WIN_PUBLIC extern
#endif

#define W26_PASSWORD_WINDOW_WIN	26
void W26_PasswordWindowDisplay(void);
void W26_PasswordWindowProcess(void);

//用户定义



#endif

