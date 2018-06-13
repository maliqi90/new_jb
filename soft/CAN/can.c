#include <string.h>
#include <stdlib.h>
#include "can.h"
#include "def.h"
#include "usb_istr.h"
//#include "communication.h"
//#include "SysTick.h"

//������	4BIT				1BIT			4BIT				10BIT			    10BIT
//ID		  ID28��ID25			ID24		ID23��ID20		ID19��ID10		ID9��ID0
//��;	 �������ȼ�	 ������ʶ	  ֡���			  Դ��ַ			Ŀ�ĵ�ַ

#define COM_TIMEOUT			200		// 2s��ʱ
#define CAN_FRAME_NUM		200		//can֡����  (��Heap_Size  ��CAN_BUFF_SIZE �й�)


#define COM_IDLE			0		//δ����
//#define COM_SENDING			1		//֡����
#define COM_WAIT_SEND		2		//�ȴ�����ʣ��֡
#define COM_WAIT_ANSWER		3		//�ȴ�����
#define COM_RECEIVING		4		//������
#define COM_RECEIVED		5		//�������
#define COM_COMFIRM			6		//ȷ������
//#define COM_COMFIRM_SEND	7		//ȷ��������
#define COM_END				8		//ȷ�����ݽ���

u16 dev_addr; 			
can_rx_msg rx_buff;
BOOL can_send_flag = FALSE;			//can�������ͱ�־

struct com_node *SEND_HEAD = NULL;	//��������
struct com_node *REC_HEAD  = NULL;	//��������
//struct com_node *ANS_HEAD  = NULL;	//ȷ������

u8 	 frame_num = 0;
BOOL send_buff_full = FALSE;
volatile uint8_t Usb_Can_Flag = 0;//USB��CAN�л�����
void enable_can_interrupt(void)
{
//	CAN1->IER |= (1 << 15) | (1 << 10) | (1 << 9) | (1 << 1) | (1 << 0);
	CAN1->IER |= 1 << 1;
}
BOOL is_send_buff_full(void)
{
	return send_buff_full;
}
void disable_can_interrutp(void)
{
	CAN1->IER = 0;
}
/*******************************************************************************
 �� �� ��    ��CAN1_CONFIG
 ����������can����50KBPS
 ��ڲ�������
 ���ڲ������� 
 ����ֵ	    ����
********************************************************************************/
void CAN1_CONFIG(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
   u32 wait_ack = 0x00000000;
 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
    
      /* Configure CAN pin: RX */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* Configure CAN pin: TX */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
     GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE);
	/* Exit from sleep mode */
	CAN1->MCR &= (~(u32)CAN_MCR_SLEEP);

	/* Request initialisation */
	CAN1->MCR |= CAN_MCR_INRQ ;

	/* Wait the acknowledge */
	while (((CAN1->MSR & CAN_MSR_INAK) != CAN_MSR_INAK) && (wait_ack != 0xFFFF))
	{
		wait_ack++;
	}

	/* Check acknowledge */
	if ((CAN1->MSR & CAN_MSR_INAK) == CAN_MSR_INAK)
	{
		/* Set the time triggered communication mode */
		CAN1->MCR &= ~CAN_MCR_TTCM;

		/* Set the automatic bus-off management */
		//CAN1->MCR &= ~(u32)CAN_MCR_ABOM;
		CAN1->MCR |= (u32)CAN_MCR_ABOM;

		/* Set the automatic wake-up mode */
		CAN1->MCR &= ~(u32)CAN_MCR_AWUM;

		/* Set the no automatic retransmission */
	  	CAN1->MCR &= ~(u32)CAN_MCR_NART;
	  	//CAN1->MCR |= (u32)CAN_MCR_NART;

		/* Set the receive FIFO locked mode */
		CAN1->MCR &= ~(u32)CAN_MCR_RFLM;

		/* Set the transmit FIFO priority */
		CAN1->MCR &= ~(u32)CAN_MCR_TXFP;

		/* Set the bit timing register */
//		CAN1->BTR = 0x3000b;// 500KBPS
		CAN1->BTR = 0x3e0023;//  50KBPS

		/* Request leave initialisation */
		CAN1->MCR &= ~(u32)CAN_MCR_INRQ;

		/* Wait the acknowledge */
		wait_ack = 0;

		while (((CAN1->MSR & CAN_MSR_INAK) == CAN_MSR_INAK) && (wait_ack != 0xFFFF))
		{
	 		wait_ack++;
		}		

		//�����ж�ʹ��
		CAN1->IER = 1 << 1;
		
		NVIC->IP[USB_LP_CAN1_RX0_IRQn] = 0x30;
		NVIC->ISER[USB_LP_CAN1_RX0_IRQn >> 0x05] = (u32)0x01 << (USB_LP_CAN1_RX0_IRQn & (u8)0x1F);

//		NVIC->IP[CAN1_SCE_IRQn] = 0x30;
//		NVIC->ISER[CAN1_SCE_IRQn >> 0x05] = (u32)0x01 << (CAN1_SCE_IRQn & (u8)0x1F);

//		NVIC->IP[USB_HP_CAN1_TX_IRQn] = 0x20;
//		NVIC->ISER[USB_HP_CAN1_TX_IRQn >> 0x05] = (u32)0x01 << (USB_HP_CAN1_TX_IRQn & (u8)0x1F);
	}
}
void CAN1_CONFIG1(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{
    
  	GPIO_InitTypeDef       GPIO_InitStructure; 
	CAN_InitTypeDef        CAN_InitStructure;
 	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	NVIC_InitTypeDef       NVIC_InitStructure;
    GPIO_PinRemapConfig(GPIO_Remap2_CAN1,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
      /* Configure CAN pin: RX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
  
      /* Configure CAN pin: TX */
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(GPIOB, &GPIO_InitStructure);
	//CAN��Ԫ����
	CAN_InitStructure.CAN_TTCM=DISABLE;						                //��ʱ�䴥��ͨ��ģʽ;  
	CAN_InitStructure.CAN_ABOM=DISABLE;						                //����Զ����߹���; 
	CAN_InitStructure.CAN_AWUM=DISABLE;						                //˯��ģʽͨ���������(���CAN->MCR��SLEEPλ);
	CAN_InitStructure.CAN_NART=DISABLE;						              	//��ֹ�����Զ�����;
	CAN_InitStructure.CAN_RFLM=DISABLE;						                //���Ĳ�����,�µĸ��Ǿɵ�; 
	CAN_InitStructure.CAN_TXFP=DISABLE;						                //���ȼ��ɱ��ı�ʶ������;
	CAN_InitStructure.CAN_Mode= mode;	                            		//ģʽ���ã� mode:0,��ͨģʽ;1,�ػ�ģʽ; 

	//���ò�����  BaudRate = APB1 / ((BS1 + BS2 + 1) * Prescaler)
	CAN_InitStructure.CAN_SJW=tsjw;				                        	//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
	CAN_InitStructure.CAN_BS1=tbs1;                               			//Tbs1=tbs1+1��ʱ�䵥λCAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStructure.CAN_BS2=tbs2;                               			//Tbs2=tbs2+1��ʱ�䵥λCAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStructure.CAN_Prescaler=brp;                          			//��Ƶϵ��(Fdiv)Ϊbrp+1	//1~1024
	CAN_Init(CAN1, &CAN_InitStructure);

	CAN_FilterInitStructure.CAN_FilterNumber=14;	                  		//������14
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;			//32λ 
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;              			//32λID
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;          			//32λMASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;		//������0������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; 					//���������0
	CAN_FilterInit(&CAN_FilterInitStructure);								//�˲�����ʼ��	

	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     			// �����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            			// �����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
//	NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     			// �����ȼ�Ϊ1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            			// �����ȼ�Ϊ0
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	CAN_ITConfig(CAN1,CAN_IT_FF0,ENABLE);									 //FIFO0��Ϣ�Һ��ж�����.		
	CAN_ITConfig(CAN1,CAN_IT_TME, ENABLE);      
}
//���뵽�����е�һ����Ϣ
void insert_list(struct com_node * head, struct com_node *pnode)
{
	struct com_node *p;
	struct com_node *q;
	
	if (head == NULL)
		return;

	p = head->pnext;
	q = head;
	while (p != NULL)
	{
		q = p;
		p = p->pnext;
	}
	q->pnext = pnode;
	pnode->pnext = NULL;
}
#if 0
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	u8 cnt;
	can_rx_msg rx_msg;
	struct com_node *p;
	struct com_node *q;
//	static struct com_node *rec;
//	static u8 frame_no  = 0;
	
	cnt = CAN1->RF0R & 3;

	while (cnt != 0)
	{										//fifo�����ж�
		can_rec(&rx_msg);
		
		CAN1->RF0R |= CAN_RF0R_RFOM0;
		
		cnt = CAN1->RF0R & 7;
		
		if (rx_msg.IDE == CAN_ID_STD)
			continue;
		
		p = (struct com_node*)malloc(sizeof(struct com_node));
		if (p == NULL)
		{
			//ɾ�����ͻ�������һ֡����
			p = SEND_HEAD->pnext;
			q = SEND_HEAD;

			if (p != NULL)
			{
				while (p->pnext != NULL)
				{
					q = p;
					p = p->pnext;					
				}

				q->pnext = NULL;
				free(p);												//????? ����frame_num��δ����

				p = (struct com_node*)malloc(sizeof(struct com_node));//????? ����frame_num��δ����
				if (p == NULL)
				{
					continue;
				}
			}
			else
			{
				continue;
			}
		}

		p->pnext = NULL;
		
		p->id = rx_msg.ExtId;
		memcpy(p->buff, rx_msg.Data, 8);

		insert_list(REC_HEAD, p);
	}
}
#endif
/*******************************************************************************
 �� �� ��    ��get_loop_addr
 ������������ȡ��·����ַ
 ��ڲ�������
 ���ڲ������� 
 ����ֵ	    ����·����ַ
********************************************************************************/
u16 get_loop_addr(void)
{
	u16  addr = 0;
	u16  temp1, temp2;

	temp1 = GPIOA->IDR;
	temp2 = GPIOB->IDR;
	//AD6
	if (temp1 & (1 << 0))
		addr |= 1 << 6;
	//AD3
	if (!(temp2 & (1 << 1)))
		addr |= 1 << 5;
	//AD4
	if (!(temp2 & (1 << 0)))
		addr |= 1 << 4;
	//AD5
	if (!(temp1 & (1 << 6)))
		addr |= 1 << 3;
	//AD2
	if (temp2 & (1 << 10))
		addr |= 1 << 2;
	//AD1
	if (temp2 & (1 << 11))
		addr |= 1 << 1;
	//AD0	
	if (temp1 & (1 << 8))
		addr |= 1 << 0;
	
	temp1 = addr & (1 << 6);
	temp1 >>= 6;
	
	temp2 = addr & (7 << 3);
	temp2 >>= 3;
	
	addr &= 0x7;
	
	if (temp1 != 0)
	{
		addr += 4;
		addr += temp2 * 6;	
	}	
	
	addr <<= 1;
	
	addr += 8;
	return addr;
}



//��ȡ�����е�һ����Ϣ
BOOL get_list(struct com_node * head, struct com_node *pnode)
{
	struct com_node *p;
	struct com_node *q;
	
	if (head == NULL)
		return FALSE;

	p = head->pnext;
	q = head;
	while (p != NULL)
	{
		q = p;
		p = p->pnext;
	}
	q->pnext = pnode;
	pnode->pnext = NULL;

	return TRUE;
}

BOOL is_mailbox_empty(void)
{
	if ((CAN1->TSR & CAN_TSR_TME0) != CAN_TSR_TME0)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

void can_transmit(can_tx_msg * tx)
{
	/* Set up the Id */
	CAN1->sTxMailBox[0].TIR &= 1 << 0;
	
	if (tx->IDE == CAN_ID_STD)
	{
		CAN1->sTxMailBox[0].TIR |= ((tx->StdId << 21) | tx->RTR);
	}
	else
	{
		CAN1->sTxMailBox[0].TIR |= ((tx->ExtId << 3) | tx->IDE | tx->RTR);
	}

	/* Set up the DLC */
	tx->DLC &= (u8)0x0000000F;
	
	CAN1->sTxMailBox[0].TDTR &= (u32)0xFFFFFFF0;
	CAN1->sTxMailBox[0].TDTR |= tx->DLC;

	/* Set up the data field */
	CAN1->sTxMailBox[0].TDLR = (((u32)tx->Data[3] << 24) |
								((u32)tx->Data[2] << 16) |
								((u32)tx->Data[1] << 8) | 
								((u32)tx->Data[0]));
	CAN1->sTxMailBox[0].TDHR = (((u32)tx->Data[7] << 24) |
								((u32)tx->Data[6] << 16) |
								((u32)tx->Data[5] << 8) |
								((u32)tx->Data[4]));
	/* Request transmission */
	CAN1->sTxMailBox[0].TIR |= 1 << 0;
}

void can_send(u16 des_addr, u16 src_addr, u8 pro, u8 len, u8 *data)
{
	u8 l = len;
	struct com_node *p;

	p = (struct com_node *)malloc(sizeof(struct com_node));
	if (p == NULL)
	{
		return;
	}

	frame_num++;

	if (frame_num > CAN_FRAME_NUM)
		send_buff_full = TRUE;
	
	p->pnext = NULL;
	
	p->id  = pro << 25;
	p->id |= src_addr << 10;
	p->id |= des_addr;

	p->status = COM_IDLE;

	if (l > CAN_BUFF_SIZE)
		l = CAN_BUFF_SIZE;
		
	memcpy(p->buff, data, l);
	p->len = l;	

	insert_list(SEND_HEAD, p);
}

void can_send_confirm(u16 des_addr, u16 src_addr, u8 pro, u8 len, u8 *data)
{
	u8 l = len;
	struct com_node *p;

	p = (struct com_node *)malloc(sizeof(struct com_node));
	if (p == NULL)
	{
		return;
	}
	frame_num++;

	if (frame_num > CAN_FRAME_NUM)
		send_buff_full = TRUE;

	p->pnext = NULL;

	p->id  = pro << 25;
	p->id |= src_addr << 10;
	p->id |= des_addr;

	p->status = COM_COMFIRM;

	if (l > CAN_BUFF_SIZE)
		l = CAN_BUFF_SIZE;
		
	memcpy(p->buff, data, l);
	p->len = l;	

	insert_list(SEND_HEAD, p);
}


void can_rec(can_rx_msg * rx)
{
	/* Get the Id */
	rx->IDE = (u8)0x04 & CAN1->sFIFOMailBox[0].RIR;
	
	if (rx->IDE == CAN_ID_STD)
	{
		rx->StdId = (u32)0x000007FF & (CAN1->sFIFOMailBox[0].RIR >> 21);
	}
	else
	{
		rx->ExtId = (u32)0x1FFFFFFF & (CAN1->sFIFOMailBox[0].RIR >> 3);
	}

	rx->RTR = (u8)0x02 & CAN1->sFIFOMailBox[0].RIR;
	/* Get the DLC */
	rx->DLC = (u8)0x0F & CAN1->sFIFOMailBox[0].RDTR;
	/* Get the FMI */
	rx->FMI = (u8)0xFF & (CAN1->sFIFOMailBox[0].RDTR >> 8);
	/* Get the data field */
	rx->Data[0] = (u8)0xFF & CAN1->sFIFOMailBox[0].RDLR;
	rx->Data[1] = (u8)0xFF & (CAN1->sFIFOMailBox[0].RDLR >> 8);
	rx->Data[2] = (u8)0xFF & (CAN1->sFIFOMailBox[0].RDLR >> 16);
	rx->Data[3] = (u8)0xFF & (CAN1->sFIFOMailBox[0].RDLR >> 24);
	rx->Data[4] = (u8)0xFF & CAN1->sFIFOMailBox[0].RDHR;
	rx->Data[5] = (u8)0xFF & (CAN1->sFIFOMailBox[0].RDHR >> 8);
	rx->Data[6] = (u8)0xFF & (CAN1->sFIFOMailBox[0].RDHR >> 16);
	rx->Data[7] = (u8)0xFF & (CAN1->sFIFOMailBox[0].RDHR >> 24);
}


/*******************************************************************************
 �� �� ��    ��get_can_buff
 ������������ȡcan���յ�������
 ��ڲ�������
 ���ڲ������� 
 ����ֵ	    ����·����ַ
********************************************************************************/
BOOL get_can_buff(struct com_node *buff)
{
	u32 timer;
	u8 len;
	struct com_node *p;
	struct com_node *q;
	static u8 frame_no	= 0;
	can_tx_msg tx;
	BOOL flag = FALSE;

	//ɾ��ȷ��֡
	p = SEND_HEAD->pnext;
	q = SEND_HEAD;

	while (p != NULL)
	{
		if (p->status == COM_END)
		{
			q->pnext = p->pnext;
			free(p);
			p = q->pnext;

			if (frame_num > 0)	
			{
				frame_num--;

				if (frame_num < (CAN_FRAME_NUM - 50))
					send_buff_full = FALSE;
			}
		}
		else
		{
			q = p;
			p = p->pnext;
		}
	}

	//����Ӧ��ʱ������֡	
	p = SEND_HEAD->pnext;

	timer = GetTickCount();
	
	while (p != NULL)
	{
		if (p->status == COM_WAIT_ANSWER)
		{
			if (timer >= p->timer)
				timer -= p->timer;
			else
				timer = 0xffffffff - p->timer + timer;

			if (timer > COM_TIMEOUT)
			{
				if (p->pos > 8)
				{
					if (p->pos % 8)
						p->pos -= p->pos % 8;
					else
						p->pos -= 8;						
				}
				else
					p->pos  = 0;

				frame_no--;
				
				p->status = COM_WAIT_SEND;
			}	
		}

		p = p->pnext;
	}

	//�����Ҫ���͵�����֡
	//�������ȼ�
	// 1������Ӧ��֡
	// 2��δ����������ݰ�
	// 3��δ���͹������ݰ�
	//����Ӧ��֡

	p = SEND_HEAD->pnext;
	while (p != NULL)
	{
		if (p->status == COM_COMFIRM)
		{
			if (is_mailbox_empty())
			{
//				p->status = COM_COMFIRM_SEND;
				p->status = COM_END;
				
				tx.ExtId  = p->id | (1 << 24);
				tx.RTR    = CAN_RTR_DATA;
				tx.IDE    = CAN_ID_EXT;
				tx.DLC    = p->len;
				memcpy(tx.Data, p->buff, p->len);
				
				can_transmit(&tx);

				flag = TRUE;

				break;
			}
			else
			{
				flag = TRUE;
				break;
			}
		}

		p = p->pnext;
	}
	
	//����δ����������ݰ�
	if (can_send_flag)
	{
		if (!flag)
		{
			p = SEND_HEAD->pnext;
			
			while (p != NULL)
			{
				if (p->status == COM_WAIT_ANSWER/* || p->status == COM_SENDING*/)
				{
					flag = TRUE;
					break;
				}

				if (p->status == COM_WAIT_SEND)
				{
					if (is_mailbox_empty())
					{			  						
						frame_no++;
						
						p->id &= ~(0xf << 20);
						p->id |= (frame_no & 0xf) << 20;

//						p->status = COM_SENDING;
						p->status = COM_WAIT_ANSWER;
						p->timer  = GetTickCount();

						if ((p->len - p->pos) > 8)
						{
							len = 8;
							p->id &= ~(1 << 24);
						}
						else
						{						
							len = p->len - p->pos;
							p->id |= 1 << 24;
						}

						tx.ExtId = p->id;
						tx.RTR	 = CAN_RTR_DATA;
						tx.IDE	 = CAN_ID_EXT;
						tx.DLC	 = len;

						memcpy(tx.Data, p->buff + p->pos, len);
						
						p->pos += len;
						
						can_transmit(&tx);
						
						flag = TRUE;
					
						break;
					}
					else
					{
						flag = TRUE;
						break;
					}
				}
				p = p->pnext;
			}
		}

		//����δ���͹������ݰ�
		if (!flag)
		{
			p = SEND_HEAD->pnext;
			
			while (p != NULL)
			{
				if (p->status == COM_IDLE)
				{
					if (is_mailbox_empty())
					{
						frame_no = 0;
						
						p->id &= ~(0xf << 20);
						p->id |= (frame_no & 0xf) << 20;
						
						p->pos = 0;
						
//						p->status = COM_SENDING;
						p->status = COM_WAIT_ANSWER;
						p->timer  = GetTickCount();
						
						if ((p->len - p->pos) > 8)
						{
							p->id &= ~(1 << 24);
							len    = 8;
						}
						else
						{
							p->id |= 1 << 24;
							len = p->len - p->pos;
						}
						
						tx.ExtId = p->id;
						tx.RTR	 = CAN_RTR_DATA;
						tx.IDE	 = CAN_ID_EXT;
						tx.DLC	 = len;
						
						memcpy(tx.Data, p->buff + p->pos, len);

						p->pos += len;
						
						can_transmit(&tx);

						break;
					}
					else
					{
						break;
					}
				}
				p = p->pnext;
			}
		}
	}
	//��ȡ���յ�����
	p = REC_HEAD->pnext;
	
	if (p == NULL)
	{
		return FALSE;
	}
	memcpy(buff, p, sizeof(struct com_node));

	REC_HEAD->pnext = p->pnext;

	free(p);

	return TRUE;
}

void confirm_send(void)
{
	struct com_node *p;	
	struct com_node *q;
	
	p = SEND_HEAD->pnext;
	q = SEND_HEAD;
	
	//���ҷ��ͽ����İ��������Ƴ�
	while (p != NULL)
	{
		if (p->status == COM_WAIT_ANSWER)
		{
			if (p->pos == p->len)
			{
				q->pnext = p->pnext;
				free(p);
				p = q->pnext;

				if (frame_num > 0)	
				{
					frame_num--;

					if (frame_num < (CAN_FRAME_NUM - 50))
						send_buff_full = FALSE;
				}
			}
			else
			{
				q = p;
				p = p->pnext;
			}
		}
		else
		{
			q = p;
			p = p->pnext;
		}
	}	
} 

/*******************************************************************************
 �� �� ��    ��can_init
 ����������can���ù�����
 ��ڲ�������
 ���ڲ������� 
 ����ֵ	    ����
********************************************************************************/
void can_init(void)
{
	SEND_HEAD = (struct com_node*) malloc(sizeof(struct com_node));
	REC_HEAD  = (struct com_node*) malloc(sizeof(struct com_node));
//	ANS_HEAD  = (struct com_node*) malloc(sizeof(struct com_node));

	SEND_HEAD->pnext = NULL;
	REC_HEAD->pnext  = NULL;
//	ANS_HEAD->pnext  = NULL;
	
	dev_addr = 0;//get_loop_addr();
	
	//���ù�����
	/* Initialisation mode for the filter */
	CAN1->FMR |= 1 << 0;
	
	/* Filter Deactivation */
	CAN1->FA1R &= ~(1 << 0);
	
	/* Filter Scale */
	/* 32-bit scale for the filter */
	CAN1->FS1R |= (1 << 0);
	/* 32-bit identifier or First 32-bit identifier */
	CAN1->sFilterRegister[0].FR1 = (u32)0 << 3;
	/* 32-bit mask or Second 32-bit identifier */
	CAN1->sFilterRegister[0].FR2 = ((u32)0xFFFFFFFF << 3);
	
	/* Filter Mode */
	CAN1->FM1R &= ~(1 << 0);
	
	/* Filter FIFO assignment */
	CAN1->FFA1R &= ~(1 << 0);
	
	/* Filter activation */
	CAN1->FA1R |= 1 << 0;

	/* Filter Deactivation */
	CAN1->FA1R &= ~(1 << 1);
	
	/* Filter Scale */
	/* 32-bit scale for the filter */
	CAN1->FS1R |= (1 << 1);
	/* 32-bit identifier or First 32-bit identifier */
	CAN1->sFilterRegister[1].FR1 = (u32)dev_addr << 3;
	/* 32-bit mask or Second 32-bit identifier */
	CAN1->sFilterRegister[1].FR2 = ((u32)0x3FF << 3);
	
	/* Filter Mode */
	CAN1->FM1R &= ~(1 << 1);
	
	/* Filter FIFO assignment */
	CAN1->FFA1R &= ~(1 << 1);
	
	/* Filter activation */
	CAN1->FA1R |= 1 << 1;

	/* Filter Deactivation */
	CAN1->FA1R &= ~(1 << 2);
	
	/* Filter Scale */
	/* 32-bit scale for the filter */
	CAN1->FS1R |= (1 << 2);
	/* 32-bit identifier or First 32-bit identifier */
	CAN1->sFilterRegister[2].FR1 = (u32)(dev_addr + 1) << 3;
	/* 32-bit mask or Second 32-bit identifier */
	CAN1->sFilterRegister[2].FR2 = ((u32)0x3FF << 3);
	
	/* Filter Mode */
	CAN1->FM1R &= ~(1 << 2);
	
	/* Filter FIFO assignment */
	CAN1->FFA1R &= ~(1 << 2);
	
	/* Filter activation */
	CAN1->FA1R |= 1 << 2;

	/* Leave the initialisation mode for the filter */
	CAN1->FMR &= ~(1 << 0);
}

void allow_active_send(void)
{
	can_send_flag = TRUE;
}

void prohibit_active_send(void)
{
	can_send_flag = FALSE;
}

//void USB_LP_CAN1_RX0_IRQHandler(void)
//{
//	u8 cnt;
//	can_rx_msg rx_msg;
//	struct com_node *p;
//	struct com_node *q;
//	static struct com_node *rec;
//	static u8 frame_no  = 0;
//	if(Usb_Can_Flag == 0)
//    {
//       USB_Istr(); 
//    }
//    else
//    {
//        cnt = CAN1->RF0R & 3;

//        while (cnt != 0)
//        {										//fifo�����ж�
//            can_rec(&rx_msg);
//            
//            CAN1->RF0R |= CAN_RF0R_RFOM0;
//            
//            cnt = CAN1->RF0R & 7;
//            
//            if (rx_msg.IDE == CAN_ID_STD)
//                continue;
//            
//            p = (struct com_node*)malloc(sizeof(struct com_node));
//            if (p == NULL)
//            {
//                //ɾ�����ͻ�������һ֡����
//                p = SEND_HEAD->pnext;
//                q = SEND_HEAD;

//                if (p != NULL)
//                {
//                    while (p->pnext != NULL)
//                    {
//                        q = p;
//                        p = p->pnext;					
//                    }

//                    q->pnext = NULL;
//                    free(p);												//????? ����frame_num��δ����

//                    p = (struct com_node*)malloc(sizeof(struct com_node));//????? ����frame_num��δ����
//                    if (p == NULL)
//                    {
//                        continue;
//                    }
//                }
//                else
//                {
//                    continue;
//                }
//            }

//            p->pnext = NULL;
//            
//            p->id = rx_msg.ExtId;
//            memcpy(p->buff, rx_msg.Data, 8);

//            insert_list(REC_HEAD, p);
//        }        
//    }

//}

#if 0
void USB_HP_CAN1_TX_IRQHandler(void)
{
	struct com_node *p;
//	struct com_node *q;
	
	if (CAN1->TSR & (1 << 0))			//����1�����ж�
	{
		//���ͳɹ�
		if (CAN1->TSR & (1 << 1))
		{
			//ΪӦ��֡���ͳɹ�����������Ƴ�
			//Ϊ����֡���¼���ͳɹ����ʱ��ֵ
//			q = SEND_HEAD;
			p = SEND_HEAD->pnext;
			
			while (p != NULL)
			{
				if (p->status == COM_SENDING)
				{
					p->timer  = GetSysTickCount();
					p->status = COM_WAIT_ANSWER;
					break;
				}

				if (p->status == COM_COMFIRM_SEND)
				{
					p->status = COM_END;
					break;
				}

//				q = p;
				p = p->pnext;
			}
		}

		/*
		//����ʧ��
		if (CAN1->TSR & (1 << 3))
		{
		}
		//�ٲ�ʧ��
		if (CAN1->TSR & (1 << 2))
		{

		}*/
		CAN1->TSR |= (1 << 0);

	}
}

/*
void CAN1_SCE_IRQHandler(void)
{
	if (CAN1->MSR & (1 << 2))
	{
		CAN1->MSR |= 1 << 2;

		if (CAN1->ESR & (1 << 2))			//���߱�־
		{
			//���ͽڵ�ͨ�Ų��ȶ�����
		}

		if (CAN1->ESR & (1 << 0))			//���󱻶���־
		{
			//���ͽڵ�ͨ�Ų��ȶ�����
		}
	}
}
*/
#endif

//BOOL is_send_buff_full(void)
//{
//	return send_buff_full;
//}
//void upload_loop_device_type(void)
//{
//	if (upload_stat)
//	{
//		upload_device++;
//		
//		send_buff[0] = CMD_LOOP_ALL_CONFIG;
//		send_buff[1] = upload_device;
//		send_buff[2] = ALLDevices[upload_loop][upload_device].DeviceCondition;
////		send_buff[3] = type_to_gb(ALLDevices[upload_loop][upload_device].DeviceType);
//		send_buff[3] = ALLDevices[upload_loop][upload_device].DeviceType;
//		send_buff[4] = ALLDevices[upload_loop][upload_device].DeviceZone;
//		send_buff[5] = ALLDevices[upload_loop][upload_device].DeviceFloor;
//		send_buff[6] = ALLDevices[upload_loop][upload_device].DeviceBuilding;
//		send_buff[7] = ALLDevices[upload_loop][upload_device].DeviceBuilding >> 8;
//	
//		can_send(1, dev_addr + upload_loop, 3, 8, send_buff); //����0��·1�豸����

//		if (upload_device == 255)
//		{
//			upload_loop++;
//			upload_device = 0;
//			if (upload_loop == 2)
//			{
//				upload_stat = FALSE;
//			}	
//		}
//	}
//}
void communication_handle(void)
{
//	u16 i;
	struct com_node temp;
    u8 Cmd;
//    u16 crc;
//	static BOOL first_config = TRUE;
//	uc8 stop_model[6] = {0x03, 0x00, 0x00, 0x00, 0x18, 0x40};
//	can_tx_msg tx;
	
	disable_can_interrutp();	
	
	if (get_can_buff(&temp) == FALSE)
	{
		enable_can_interrupt();
		return;
	}
	
	//���ճɹ�����ִ�гɹ�
	if (((temp.buff[0] >= 1) && (temp.buff[0] <= 4)) && ((temp.id & 0x3ff) != 0))
	{
		confirm_send();
//		upload_loop_device_type();		//�յ�ȷ�Ϻ���һ�豸��Ϣ���⻺����
		enable_can_interrupt();
		return;
	}

	//��ȡCAN ���ݰ�??????

	enable_can_interrupt();
	
	Cmd = temp.buff[0];
	//LN  = temp.id & 0x01?1:0; 		// ��·��

	//��������
	switch (Cmd)
	{


	}
}

