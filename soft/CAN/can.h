#ifndef __CAN_H__
#define __CAN_H__

#include "stm32f10x.h"
#include "def.h"
//#define CANINITFAILED               0x00
//#define CANINITOK                   0x01
//#define CAN_FilterFIFO0             0x00
//#define CAN_FilterFIFO1             0x01
//#define CAN_ID_STD                  0x00           
//#define CAN_ID_EXT                  0x04
//#define CAN_RTR_DATA                0x00000000         
//#define CAN_RTR_REMOTE              0x00000002
//#define CANTXFAILE                  0x00
//#define CANTXOK                     0x01
//#define CANTXPENDING                0x02
//#define CAN_NO_MB                   0x04
//#define CANSLEEPFAILED              0x00
//#define CANSLEEPOK                  0x01
//#define CANWAKEUPFAILED             0x00        
//#define CANWAKEUPOK                 0x01        


#define CAN_BUFF_SIZE 				8//65
//#define COM_TIMEOUT		200		// 2s超时

typedef struct
{
  u32 StdId;  /*!< Specifies the standard identifier.
                        This parameter can be a value between 0 to 0x7FF. */

  u32 ExtId;  /*!< Specifies the extended identifier.
                        This parameter can be a value between 0 to 0x1FFFFFFF. */

  u8 IDE;     /*!< Specifies the type of identifier for the message that 
                        will be transmitted. This parameter can be a value 
                        of @ref CAN_identifier_type */

  u8 RTR;     /*!< Specifies the type of frame for the message that will 
                        be transmitted. This parameter can be a value of 
                        @ref CAN_remote_transmission_request */

  u8 DLC;     /*!< Specifies the length of the frame that will be 
                        transmitted. This parameter can be a value between 
                        0 to 8 */

  u8 Data[8]; /*!< Contains the data to be transmitted. It ranges from 0 
                        to 0xFF. */
} can_tx_msg;

typedef struct
{
  u32 StdId;  /*!< Specifies the standard identifier.
                        This parameter can be a value between 0 to 0x7FF. */

  u32 ExtId;  /*!< Specifies the extended identifier.
                        This parameter can be a value between 0 to 0x1FFFFFFF. */

  u8 IDE;     /*!< Specifies the type of identifier for the message that 
                        will be received. This parameter can be a value of 
                        @ref CAN_identifier_type */

  u8 RTR;     /*!< Specifies the type of frame for the received message.
                        This parameter can be a value of 
                        @ref CAN_remote_transmission_request */

  u8 DLC;     /*!< Specifies the length of the frame that will be received.
                        This parameter can be a value between 0 to 8 */

  u8 Data[8]; /*!< Contains the data to be received. It ranges from 0 to 
                        0xFF. */

  u8 FMI;     /*!< Specifies the index of the filter the message stored in 
                        the mailbox passes through. This parameter can be a 
                        value between 0 to 0xFF */
} can_rx_msg;

#pragma pack(1)
struct com_node
{
	struct com_node *pnext;
	u8  pos;
	u8  len;
//	u8  packet_no;
//	u8  frame_no;
	u32 id;
	u32 timer;					//时间
	u8  status; 
	u8  buff[CAN_BUFF_SIZE];	//第一字节为数据长度
};
#pragma pack()

//void can_transmit(can_tx_msg * tx);
extern struct com_node *SEND_HEAD ;	//发送数据
extern struct com_node *REC_HEAD ;	//接收数据
void enable_can_interrupt(void);

void disable_can_interrutp(void);

void can_init(void);

void can_send(u16 des_addr, u16 src_addr, u8 pro, u8 len, u8 *data);

void can_send_confirm(u16 des_addr, u16 src_addr, u8 pro, u8 len, u8 *data);

BOOL get_can_buff(struct com_node *buff);

void allow_active_send(void);

void prohibit_active_send(void);

void confirm_send(void);

u16 get_loop_addr(void);

BOOL is_send_buff_full(void);

void can_transmit(can_tx_msg * tx);
void can_rec(can_rx_msg * rx);
void CAN1_CONFIG(void);
void CAN1_CONFIG1(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);
void communication_handle(void);
void insert_list(struct com_node * head, struct com_node *pnode);
#endif

