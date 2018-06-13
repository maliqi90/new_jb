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
    uint8_t Can_Set;//CAN�Ƿ�򿪱�־λ
    uint8_t Bus_Set;//���߿����Ƿ�򿪱�־λ
    uint8_t Can_Addr;//CAN���ߵ�ַ
    uint8_t Bus_Addr;//���ߵ�ַ
    
}Communication_Set;
//�û�����
extern Communication_Set comm_set;
extern uint8_t busaddr_flag;

#endif

