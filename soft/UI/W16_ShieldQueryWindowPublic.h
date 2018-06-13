#ifndef _W16_SHIELDQUERY_WINDOW_WIN_H_
#define _W16_SHIELDQUERY_WINDOW_WIN_H_
#ifdef W16_SHIELDQUERY_WINDOW_WIN_PRIVATE
	#define W16_SHIELDQUERY_WINDOW_WIN_PUBLIC
#else
	#define W16_SHIELDQUERY_WINDOW_WIN_PUBLIC extern
#endif

#define W16_SHIELDQUERY_WINDOW_WIN	16
void W16_ShieldQueryWindowDisplay(void);
void W16_ShieldQueryWindowProcess(void);

//用户定义



#endif

