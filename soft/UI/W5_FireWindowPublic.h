#ifndef _W5_FIRE_WINDOW_WIN_H_
#define _W5_FIRE_WINDOW_WIN_H_
#ifdef W5_FIRE_WINDOW_WIN_PRIVATE
	#define W5_FIRE_WINDOW_WIN_PUBLIC
#else
	#define W5_FIRE_WINDOW_WIN_PUBLIC extern
#endif

#define W5_FIRE_WINDOW_WIN	5
void W5_FireWindowDisplay(void);
void W5_FireWindowProcess(void);

//用户定义



#endif

