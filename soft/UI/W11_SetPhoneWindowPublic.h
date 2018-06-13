#ifndef _W11_SETPHONE_WINDOW_WIN_H_
#define _W11_SETPHONE_WINDOW_WIN_H_
#ifdef W11_SETPHONE_WINDOW_WIN_PRIVATE
	#define W11_SETPHONE_WINDOW_WIN_PUBLIC
#else
	#define W11_SETPHONEE_WINDOW_WIN_PUBLIC extern
#endif

#define W11_SETPHONE_WINDOW_WIN	11
void W11_SetPhoneWindowDisplay(void);
void W11_SetPhoneWindowProcess(void);
void phone_init(void);
extern uint8_t phone[2][11];
extern uint8_t phone_flag;
//用户定义



#endif

