#ifndef _W21_PHONE_WINDOW_WIN_H_
#define _W21_PHONE_WINDOW_WIN_H_
#ifdef W21_PHONE_WINDOW_WIN_PRIVATE
	#define W21_PHONE_WINDOW_WIN_PUBLIC
#else
	#define W21_PHONE_WINDOW_WIN_PUBLIC extern
#endif

#define W21_PHONE_WINDOW_WIN	21
void W21_PhoneWindowDisplay(void);
void W21_PhoneWindowProcess(void);
extern uint8_t call_flag;
//用户定义



#endif

