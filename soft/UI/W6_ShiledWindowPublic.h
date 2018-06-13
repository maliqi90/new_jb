#ifndef _W6_SHIELD_WINDOW_WIN_H_
#define _W6_SHIELD_WINDOW_WIN_H_
#ifdef W6_SHIELD_WINDOW_WIN_PRIVATE
	#define W6_SHIELD_WINDOW_WIN_PUBLIC
#else
	#define W6_SHIELD_WINDOW_WIN_PUBLIC extern
#endif

#define W6_SHIELD_WINDOW_WIN	6
void W6_ShieldWindowDisplay(void);
void W6_ShieldWindowProcess(void);

//用户定义



#endif

