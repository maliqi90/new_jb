#ifndef _GPRS_H_
#define _GPRS_H_

#include "stm32f10x.h"
#include "stdint.h"
#include "def.h"

#ifdef UART3_PRIVATE
	#define UART3_PUBLIC
#else
	#define UART3_PUBLIC extern
#endif
#

#define UART3_BUFFER_COUNT  	10
#define UART3_BUFFER_LENGTH  	64
extern const char gprs_checksignal[];//��ѯGPRS�ź�
extern const char gprs_checksim[];//��ѯsim��

//����ṹ��
#define TASKS    10 //��Ϣ�������
#define DATAS    10 //��̬�ڴ����
#define DLENGTH  64 //��̬�ڴ���ֽ���
#define UARTLENGTH    64      //��
struct Task
{
	uint8_t Type;//
	uint8_t Date;//��Ч���ݳ���
	uint8_t *pDate;// ��Ӧ��������õ��������׵�ַ
	struct Task *Next;//ָ����һ������
};	
//�������ݽṹ��


struct UsartDate
{
	uint8_t State;//���ڽ���״̬��ʶ
	uint8_t Time;//���ڽ��ռ�ʱ
	uint8_t RXlenth;//���ڽ������ݰ�����
	uint8_t *RXbuf;//���ڽ����������û������ݿռ���׵�ַ
};
extern struct Task *FirstTask;//ͷ����ָ��
extern  struct Task *LastTask;//β����ָ��
extern struct Task  TaskBuf[TASKS];//����Ĵ�����
extern struct UsartDate Usat3_data;//��������
extern uint8_t usart3_buff[10][3];
UART3_PUBLIC uint8_t Uart3SendBuffer[UART3_BUFFER_COUNT][UART3_BUFFER_LENGTH];
UART3_PUBLIC uint8_t Uart3SendIn;
UART3_PUBLIC uint8_t Uart3SendOut;
UART3_PUBLIC uint8_t Uart3SendStat;
UART3_PUBLIC uint8_t Uart3SendPos;
UART3_PUBLIC uint32_t Uart3SendTimer;

UART3_PUBLIC uint8_t Uart3ReceiveBuffer[UART3_BUFFER_COUNT][UART3_BUFFER_LENGTH];
//UART4_PUBLIC U8 Uart4ReceiveCurrentBuffer[UART4_BUFFER_LENGTH];
UART3_PUBLIC uint8_t Uart3ReceiveIn;
UART3_PUBLIC uint8_t Uart3ReceiveOut;
UART3_PUBLIC uint8_t Uart3ReceiveCount;
UART3_PUBLIC uint8_t Uart3ReceiveSumCheck;
UART3_PUBLIC uint32_t Uart3ReceiveOverTimer;

UART3_PUBLIC uint32_t Uart3_Gprs_PollTimer;
UART3_PUBLIC uint16_t EventRamUart2Pointer;

UART3_PUBLIC uint8_t  switch_timer;

void gprs_init(void);
typedef struct
{
    char Gprs_simflag;//sim���Ƿ����
    char Gprs_CSQ;   //�ź�����
    char Gprs_flag;//GPRS���ñ�־λ
    
}Gprs_State;


	
    extern Gprs_State gprsstate;
//����ӿں���
void UART4_Init(void);
uint8_t * Uart3SendCommand(void);
void UART3_Task(void);
void  InitTask(void);
void uart3_send(uint8_t len, uint8_t *buff);
//void uart4_send(uint8_t des_addr, uint8_t cmd, uint8_t len, uint8_t *buff);
void Rev_Istr(void);
void UART3_Task(void);
uint8_t   AddTask1(uint8_t Type ,uint8_t  Data,uint8_t  *pData);
extern Gprs_State gprsstate;
extern void Call_PhoneNum(uint8_t *phone);
extern void GPRS_STATAS(void);
#endif
