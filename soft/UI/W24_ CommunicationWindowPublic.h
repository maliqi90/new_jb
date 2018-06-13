#ifndef _W24_COMMUNICATION_WINDOW_WIN_H_
#define _W24_COMMUNICATION_WINDOW_WIN_H_
#ifdef W24_COMMUNICATION_WINDOW_WIN_PRIVATE
	#define W24_COMMUNICATION_WINDOW_WIN_PUBLIC
#else
	#define W24_COMMUNICATION_WINDOW_WIN_PUBLIC extern
#endif

#define W24_COMMUNICATION_WINDOW_WIN	24
void W24_CommunicationWindowDisplay(void);
void W24_CommunicationWindowProcess(void);

typedef struct 
{
    uint8_t Can_Set;//CAN是否打开标志位
    uint8_t Bus_Set;//总线开关是否打开标志位
    uint8_t Can_Addr;//CAN总线地址
    uint8_t Bus_Addr;//总线地址
    
}Communication_Set;
//用户定义
extern Communication_Set comm_set;
extern uint8_t busaddr_flag;

#endif

