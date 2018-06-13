#include <string.h>
#include "stm32f10x.h"
#include "loop_hw.h"
#include "loop.h"
#include "LoopDataStruct.h"
#include "LoopCfg.h"
#include "LoopOpt.h"
//#include "SysTick.h"
//#include "can.h"
#include "LoopDevTables.h"
//#include "EventDefine.h"
//#include "SystemCommand.h"
#include "DeviceTypeDefine.h"
//#include "communication.h"
#include "def.h"
#include "gpio_ctrl.h"
#include "event.h"
extern uint8_t fire_flag;
extern uint8_t fault_flag;
extern u16 dev_addr; 
extern  LOOPS LOOP[2];   				// �����·�ṹ�ͻ�·������
extern LOOPDevice ALLDevices[2][256]; 	// ÿ����·��256����·�����ͽṹ�ռ�
extern u8  OptingLoopNO;      			// ��ǰ���ڲ����Ļ�·��0��1
extern u32 CheckTypeTimeTicks;   		// ��·�������������ͼ�飬�˱�����������������ʱ�������
extern u8  LDList[2][256];				// ����������
extern int LDNode[2];

extern BOOL AutoRegister_flag;     		// �Զ���½ʱ��λ����½�����0
extern BOOL LoopCanScan_flag;       	// ���ƻ�·�Ƿ����ɨ��
extern u8 send_buff[8];
extern u8 auto_res_ln;

static u8 OutActType[2]; 				// ģ����������������ͣ�1-������0-�رա�2-���塣
extern  EVENT_CONST const EventConstTable[];
/******************************************************************************
* �������ͱ� �±���������룬��ֵ������������
* 0x05�ɺ��� 0x01������� 0x11��� 0x00���� 0xFF����������
* ��Ҫ��DeviceTypeDefine.h�ļ��е��豸���Ͷ�Ӧ
******************************************************************************/
const u8 DEVICE_IO_TYPE[256] = {
	0xFF,0x05,0x05,0x05,0xFF,0xFF,0xFF,0xFF,	//0
	0xFF,0xFF,0x05,0x05,0x05,0x05,0x05,0x05,	//8
	0x05,0xFF,0xFF,0xFF,0x05,0x05,0x05,0x05,	//16
	0x05,0x05,0xFF,0xFF,0xFF,0xFF,0x05,0x05,	//24
	0x05,0x05,0x05,0x05,0xFF,0xFF,0xFF,0xFF,	//32
	0x05,0x05,0x05,0x05,0xFF,0xFF,0xFF,0x05,	//40
	0xFF,0xFF,0x05,0x05,0x05,0x05,0xFF,0xFF,	//48
	0xFF,0xFF,0xFF,0xFF,0xFF,0x05,0x05,0x05,	//56
	0x05,0x05,0xFF,0xFF,0xFF,0xFF,0xFF,0x05,	//64
	0x00,0x11,0x01,0x05,0x05,0xFF,0xFF,0xFF,	//72
	0xFF,0x11,0x11,0x11,0x11,0x11,0x11,0xFF,	//80
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,	//88
	0xFF,0xFF,0xFF,0xFF,0x05,0x05,0x05,0x05,	//96
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,	//104
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,	//112
	0x05,0x05,0x05,0x05,0x05,0xFF,0xFF,0xFF,	//120
	0xFF,0xFF,0x05,0x05,0x05,0x05,0x05,0xFF,	//128
	0xFF,0xFF,0xFF,0xFF,0x05,0x05,0x05,0x05,	//136
	0x05,0x05,0xFF,0xFF,0xFF,0xFF,0x05,0x05,	//144
	0x05,0x05,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,	//152
	0x05,0x05,0x05,0x05,0x05,0x01,0x11,0x11,	//160
	0x05,0x05,0x05,0x00,0x01,0x01,0xFF,0xFF,	//168
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,	//176
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x01,	//184
	0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,	//192
	0x00,0x11,0x01,0x11,0x01,0x01,0x01,0x01,	//200
	0x01,0x01,0x01,0x05,0x01,0x01,0x01,0x01,	//208
	0x01,0x01,0x01,0x01,0x05,0xFF,0xFF,0xFF,	//216
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x05,0x05,	//224
	0x05,0x05,0x05,0x05,0x05,0x05,0x00,0x01,	//232
	0x01,0x05,0x05,0xFF,0xFF,0xFF,0xFF,0xFF,	//240
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x05,	//248
};
/***********************************************************
????:void Loop_Revice(u8* buff,u8 type)
????:??????
????:u8* buff ????,u8 type ????
************************************************************/
 void Loop_Revice(u8* buff)
{

    EventRamBuffer[EventRamCurrentPointer].EventAddress = buff[3];
    EventRamBuffer[EventRamCurrentPointer].EventCode    = buff[1]; 
    EventRamBuffer[EventRamCurrentPointer].EventDay     = Timer_value.date;
    EventRamBuffer[EventRamCurrentPointer].EventDeviceType = 0;
    EventRamBuffer[EventRamCurrentPointer].EventHour       = Timer_value.hour;
    EventRamBuffer[EventRamCurrentPointer].EventMinute    = Timer_value.min;
    EventRamBuffer[EventRamCurrentPointer].EventMonth     = Timer_value.month;
    EventRamBuffer[EventRamCurrentPointer].EventSecond    = Timer_value.sec;
    EventRamBuffer[EventRamCurrentPointer].EventVerion    = 0;
    EventRamBuffer[EventRamCurrentPointer].EventYear     = Timer_value.year-2000;

    //memcpy(&EventRamBuffer[EventRamCurrentPointer].EventVerion, &Event.EventVerion, 32);
    EventRamType[EventRamCurrentPointer] = buff[1]; //
    EventRamCurrentPointer++;
    //return;
}
/*
********************************************************************************
* ��������: static void DealPW12State( u8 LN, u8 device_Addr, u8 pw1, u8 pw2 );
* ��������: ����1,2״̬����
* ��    ��: u8 LN             : ��·��0 �� (0,1)
*           u8 device_Addr    : ������1~255
*           u8 pw1, u8 pw2 : ����1��2״̬ pw1,pw2 (pw2 == 0xaa��ʾ̽�����������ʾģ��)
* �� �� ֵ: ��
* ע    ��:
********************************************************************************
*/
static void DealPW12State( u8 LN, u8 device_Addr, u8 pw1, u8 pw2 )
{
    BOOL AnsCodeRight_flag;  //������������λ
    u8  StateTempSave;      //״̬��ʱ�洢������ͬ�¼�ʱ���ڴ洢�¼���־λ
    u8  tempVar;            //һ����ʱ��������������ʹ��
//    SYSTEM_EVENT 		 Event;
    if (AutoRegister_flag)// �Զ���¼��䲻�����¼�
		return;  
	
    if (0 == pw1)
    {
        return;
    }
    if (pw1 > 5) 
    {
        return;
    }
    if (0xaa == pw2)
    { // ̽��������
    } 
	else if (pw2 > 5) 
	{
        return;
    }

    if (0xaa == pw2)
    {/**************************************************************************
     *                               ̽��������                                *
     *        0xaa����Ϊ��ֵ�ģ���������̽������ģ��,ģ��������������        *
     **************************************************************************/
        if (1 == pw1)        // ̽��������
        {
            AnsCodeRight_flag = 1; // ����
            ALLDevices[LN][device_Addr].ReScanStat2 &= 0x07;  // ��������λ

            if (0 != ALLDevices[LN][device_Addr].ReScanStat2) // �����л𾯹��ϵ��¼�ȷ��
            {				
                if (ALLDevices[LN][device_Addr].ReScanStat5 & FireRecover) 
                {
                // ֻҪ�лָ��¼���������ʶ��һͬ������ˣ����Դ˴�����ͬʱ�ж�
                    if( ALLDevices[LN][device_Addr].ReScanStat2 & FaultStat )
                    {
                    }
                    else
                    {
                        return; // �𾯻ָ����ϻָ��󣬲������������ˡ�
                    }
                }

                if (0xf0 & ALLDevices[LN][device_Addr].Comd) 
                {
                	// �Ѿ���ʼ����ɨ��
                    ALLDevices[LN][device_Addr].ReScanCnt++;
                    if (ALLDevices[LN][device_Addr].ReScanCnt > 
                        (FFRecover_RESCAN_TIMES - RESCAN_ALLOW_ERR_TIMES)) 
                    { 	// ֻ�����й涨�δ���
                    	// �¼��ָ����ϱ�������ر�־λ
                        if (ALLDevices[LN][device_Addr].ReScanStat2 & DevIsFireStat) 
                        {
                            if(ALLDevices[LN][device_Addr].ReScanStat5 & FireRecover)
                            {
                            }
                            else
                            {
                                SetBit(ALLDevices[LN][device_Addr].ReScanStat5, FireRecover);// ���𾯻ָ�

							  	//������ȡ��
								if (DEVICE_IO_TYPE[ALLDevices[LN][device_Addr].DeviceType] == 0x01 ||
								    DEVICE_IO_TYPE[ALLDevices[LN][device_Addr].DeviceType] == 0x00 ||
								    DEVICE_IO_TYPE[ALLDevices[LN][device_Addr].DeviceType] == 0x11)								
								{
//									send_buff[0] = CMD_EVENT_TRANSFER;								   
//									send_buff[1] = EVENT_FEEDBACK_CANCLE;									   
//									send_buff[2] = DEVICE_TYPE_LOOP_CARD;
									send_buff[3] = device_Addr;
									send_buff[4] = 0;
									send_buff[5] = 0;
									send_buff[6] = 0;
//									can_send(1, dev_addr + LN, 0, 7, send_buff);
								}
                            }
                        }

                        if (ALLDevices[LN][device_Addr].ReScanStat2 & FaultStat)
                        {
                            SetBit(ALLDevices[LN][device_Addr].ReScanStat5, FaultRecover);
                            
//                          send_buff[0] = CMD_EVENT_TRANSFER;
//		                    send_buff[1] = EVENT_DEVICE_FAULT_RECOVERY;
		                    send_buff[2] = DEVICE_TYPE_LOOP_CARD;
		                    send_buff[3] = device_Addr;
		                    send_buff[4] = 0;
		                    send_buff[5] = 0;
		                    send_buff[6] = 0;
                            
//		                    can_send(1, dev_addr + LN, 0, 7, send_buff);		// �����ϻָ�
                        }
                        
                        ALLDevices[LN][device_Addr].ReScanStat1 &= FireRecover;
                        ALLDevices[LN][device_Addr].ReScanStat2 &= FireRecover;
                        ALLDevices[LN][device_Addr].ReScanCnt = 0;
                        ALLDevices[LN][device_Addr].Comd &= 0x0f; // �˴���ģ�鲻ͬ������ǻ𾯻ָ�������Ϊ0x0A
                        return;
                    }
                }
                else 
                { // ��һ��̽�⵽�ָ�������׼������ɨ��
                    ALLDevices[LN][device_Addr].Comd += (FFRecover_RESCAN_TIMES << 4);
                    ALLDevices[LN][device_Addr].ReScanCnt = 1; // ���ξ���һ��
                }
            }
            else  // ֻ�������״��¼���־��������������
            {
                ALLDevices[LN][device_Addr].ReScanStat1 = 0; // ��ر�־λ�Ȼָ�����
                ALLDevices[LN][device_Addr].Comd = XJCMD(LN, device_Addr);
            }
        }
        else if (3 == pw1)  // ̽������
        {
            if (FireRecover & ALLDevices[LN][device_Addr].ReScanStat5)  // �������𾯻ָ�
            {
                ALLDevices[LN][device_Addr].Comd &= 0x0f;
                return;  // ��������ֱ�ӷ���
            }
            AnsCodeRight_flag = 0;
            StateTempSave = FireStat; // �׻𾯺ͻ�ȷ�϶�λ��BIT0,�˴���д��Ϊ�˱�������
        } 
        else 
        { // ̽�������ֹ���
            //if( FaultRecover & ALLDevices[LN][device_Addr].ReScanStat5 ) { // ���������ϻָ�
            //    ALLDevices[LN][device_Addr].Comd &= 0x0f;
            //    return; //��������ֱ�ӷ���
            //}
            AnsCodeRight_flag = 0;
            if (4 == pw1)  			// ̽��������
            {
                StateTempSave = FaultStat; //�׹��Ϻ͹���ȷ�϶�λ��BIT1,�˴���д��Ϊ�˱�������
            } 
            else if (5 == pw1)  	// ̽�������ҹ���
            {
                StateTempSave = DustOrInShortStat; // �׻��Һͻ���ȷ�϶�λ��BIT2,�˴���д��Ϊ�˱�������
            }
        }
        /*************��ʼ����𾯻�������*************/
        if (!AnsCodeRight_flag)  // ̽�������¼����ڴ˴����д���
        {
            if (StateTempSave & ALLDevices[LN][device_Addr].ReScanStat2) // �¼��Ѿ�ȷ�ϲ�������
            {            
                return;
            } 
            else if (StateTempSave & ALLDevices[LN][device_Addr].ReScanStat1) 
            {
            	// ��ȷ�����¼�,�˴�ȷ�ϲ��ϱ��𾯻����,Ȼ������Ӧ�ı�־
                if (FireStat == StateTempSave) 
                {					
//                	send_buff[0] = CMD_EVENT_TRANSFER;
//                	
                	if (DEVICE_IO_TYPE[ALLDevices[LN][device_Addr].DeviceType] == 0x01 ||
						DEVICE_IO_TYPE[ALLDevices[LN][device_Addr].DeviceType] == 0x00 ||
						DEVICE_IO_TYPE[ALLDevices[LN][device_Addr].DeviceType] == 0x11)	
//                		send_buff[1] = EVENT_FEEDBACK;
//                	else                	
                
                    	
                    send_buff[2] = DEVICE_TYPE_LOOP_CARD;
                    send_buff[3] = device_Addr;
                    send_buff[1] = EVENT_TYPE_ALARM;
                    send_buff[4] = 0;
                    send_buff[5] = 0;
                    send_buff[6] = 0;
                   
//                    fire_flag = 1;
//                    can_send(1, dev_addr + LN, 0, 7, send_buff);
                    Loop_Revice(send_buff);//����
               		ALLDevices[LN][device_Addr].Comd = 0x2A; // ����,����ȷ������					
                } 
                else if (FaultStat == StateTempSave) 
                {
//                	send_buff[0] = CMD_EVENT_TRANSFER;
//                  send_buff[1] = EVENT_DEVICE_FAULT;
                    send_buff[2] = DEVICE_TYPE_LOOP_CARD;
                    send_buff[3] = device_Addr;
                    send_buff[4] = 0;
                    send_buff[5] = 0;
                    send_buff[6] = 0;
//                    
//                    can_send(1, dev_addr + LN, 0, 7, send_buff);	// ������
                } 
                else if (DustOrInShortStat == StateTempSave) 
                {
                		send_buff[0] = 0x03;
                   		send_buff[1] = LN;
                        send_buff[2] = device_Addr;
//                    		can_send(1, dev_addr, 0, 3, send_buff);????? // �����ҹ���
                }

                SetBit(ALLDevices[LN][device_Addr].ReScanStat2, StateTempSave);
            } 
            else 
            {
            	//��û��ȷ�����¼�
                if (FireStat == StateTempSave) 
                {
                    tempVar = FIRE_RESCAN_TIMES;  // ����ɨ����
                } 
                else 
                {
                    tempVar = FAULT_RESCAN_TIMES; // ������ɨ����
                }

                if (0 == (0xf0 & ALLDevices[LN][device_Addr].Comd)) 
                {
                // ��һ��̽�⵽���¼�,׼������ظ�ɨ��
                    ALLDevices[LN][device_Addr].Comd += ( tempVar << 4 );
                    ALLDevices[LN][device_Addr].ReScanCnt = 1;  // ���ξ���һ��
                } 
                else 
                {
                    ALLDevices[LN][device_Addr].ReScanCnt++;
                    if (ALLDevices[LN][device_Addr].ReScanCnt >= (tempVar - RESCAN_ALLOW_ERR_TIMES)) 
                    { // ֻ�����й涨�δ���
                        SetBit(ALLDevices[LN][device_Addr].ReScanStat1, StateTempSave);
                        ALLDevices[LN][device_Addr].Comd &= 0x0f;
                        ALLDevices[LN][device_Addr].ReScanCnt = 0;
                        LOOP[LN].NeedCheckAddr = device_Addr;
                    }
                }
            }//̽�����Ƿ��Ѿ�ȷ�����¼����䴦��--���
        }//̽�������¼�����--���
    } //������Ϊ̽�����Ĵ���--���
    /***************************************************************************/
    /****************************************************************************
    *                               ̽�����������                              *
    *        ���¿�ʼ����ģ��Ļ��룬���ȴ���ģ������ڵ�                       *
    ****************************************************************************/
    else
    {   // PW1 ����ӵ�״̬
        if (1 == pw1)  // ����ӵ�����
        {
            AnsCodeRight_flag = 1; // ����
            tempVar = ALLDevices[LN][device_Addr].ReScanStat2 & 0x0f;  // ��������ڵ��־

            if ( 0 != tempVar) 
            {
            	// ����ڵ������л𾯹��ϵ��¼�ȷ��
                if (ALLDevices[LN][device_Addr].ReScanStat5 & FireRecover ) 
                {
                    // �ֱ�,����˨��ð�ť,CDI����ʽ��̽������ͬ���𾯻ָ��������������ˡ�
                    if ((ALLDevices[LN][device_Addr].DeviceType == DEVICE_HAND_FIRE_DETECTOR) ||
                        (ALLDevices[LN][device_Addr].DeviceType == DEVICE_FIRE_COCK_BUTTON)   ||
                        (ALLDevices[LN][device_Addr].DeviceType == DEVICE_RELAY_MODULE)) 
                    {
                         if (ALLDevices[LN][device_Addr].ReScanStat2 &
                             (FaultStat | DustOrInShortStat | InOpenStat))
                         {
                         }
                         else
                         {
                             return;
                         }
                    }
                }
                // ����ģ����Է����ϱ��𾯡�����
                if (0xf0 & ALLDevices[LN][device_Addr].Comd)  // �Ѿ���ʼ����ɨ��
                {
                    if((ALLDevices[LN][device_Addr].ReScanCnt & 0x0f) < 0x0f) 
                    {
                        ALLDevices[LN][device_Addr].ReScanCnt++;  // ����λ��
                    }

                    if ((ALLDevices[LN][device_Addr].ReScanCnt & 0x0f) > 
                        ( FFRecover_RESCAN_TIMES - RESCAN_ALLOW_ERR_TIMES)) 
                    {
                    	//ֻ�����й涨�δ�������ڵ��¼��ָ����ϱ�������ر�־λ
                        if (ALLDevices[LN][device_Addr].ReScanStat2 & DevIsFireStat) 
                        {
                        	if ((ALLDevices[LN][device_Addr].ReScanStat5 & FireRecover) &&
                                 ((ALLDevices[LN][device_Addr].DeviceType == DEVICE_HAND_FIRE_DETECTOR)|| 
                                 (ALLDevices[LN][device_Addr].DeviceType == DEVICE_FIRE_COCK_BUTTON)|| 
                                 (ALLDevices[LN][device_Addr].DeviceType == DEVICE_RELAY_MODULE)))
                            {
                            }
                            else
                            {
                                SetBit(ALLDevices[LN][device_Addr].ReScanStat5, FireRecover);// ���𾯻ָ�
                                
                                //LoopSendEvent2Main( 0x40, 5, 0x3A, LN, device_Addr, 0); 

								if (DEVICE_IO_TYPE[ALLDevices[LN][device_Addr].DeviceType] == 0x01 ||	//������ȡ��
									DEVICE_IO_TYPE[ALLDevices[LN][device_Addr].DeviceType] == 0x00 ||
								    DEVICE_IO_TYPE[ALLDevices[LN][device_Addr].DeviceType] == 0x11)
							    {
//									send_buff[0] = CMD_EVENT_TRANSFER;								   
//									send_buff[1] = EVENT_FEEDBACK_CANCLE;									   
									send_buff[2] = DEVICE_TYPE_LOOP_CARD;
									send_buff[3] = device_Addr;
									send_buff[4] = 0;
									send_buff[5] = 0;
									send_buff[6] = 0;
//									can_send(1, dev_addr + LN, 0, 7, send_buff);
							    }
                            }
                        }
                        
                        if (ALLDevices[LN][device_Addr].ReScanStat2 & (FaultStat | DustOrInShortStat | InOpenStat)) 
                        {
                            SetBit(ALLDevices[LN][device_Addr].ReScanStat5, FaultRecover);             				
             				
//		                	send_buff[0] = CMD_EVENT_TRANSFER;
//		                	send_buff[1] = EVENT_LINE_FAULT_RECOVERY;			                	
		                    send_buff[2] = DEVICE_TYPE_LOOP_CARD;
		                    send_buff[3] = device_Addr;
		                    send_buff[4] = 0;
		                    send_buff[5] = 0;
		                    send_buff[6] = 0;
//		                    can_send(1, dev_addr + LN, 0, 7, send_buff);		// �����ϻָ�
							
							ALLDevices[LN][device_Addr].ReScanStat2 = 0;
                        }

                        ALLDevices[LN][device_Addr].ReScanCnt &= 0xf0;
                        if ((ALLDevices[LN][device_Addr].DeviceType == DEVICE_RELAY_MODULE)||   		// CDIģ���̽����һ������
                            (ALLDevices[LN][device_Addr].DeviceType== DEVICE_FIRE_COCK_BUTTON)||   		// ����˨��ð�ť��̽����һ������
                            (ALLDevices[LN][device_Addr].DeviceType == DEVICE_HAND_FIRE_DETECTOR))   	// �ֱ���̽����һ������
                        {
                            ALLDevices[LN][device_Addr].ReScanStat1 &= (0xf0|FireRecover); // ��ҪӰ������ڵ�
                            ALLDevices[LN][device_Addr].ReScanStat2 &= (0xf0|FireRecover); // ��ҪӰ������ڵ�
                        } 
                        else 
                        {
                            ALLDevices[LN][device_Addr].ReScanStat1 &= 0xf0; // ��ҪӰ������ڵ�
                            ALLDevices[LN][device_Addr].ReScanStat2 &= 0xf0; // ��ҪӰ������ڵ�
                        }
                        ALLDevices[LN][device_Addr].Comd = XJCMD(LN, device_Addr); //�˴���̽������ͬ
                        return;
                    }
                }
                else 
                {
                    //070426������������ʼ�޸ģ���ʧǰ�ش𣬻ָ����ȱ��ش������ֱ��ش�
                    if (ALLDevices[LN][device_Addr].OtherFlags.FlagBit.DisLoseErrAns_flag)
					{
                        return;
                    } 
					else 
					{
                        ALLDevices[LN][device_Addr].Comd += (FFRecover_RESCAN_TIMES << 4);
                        ALLDevices[LN][device_Addr].ReScanCnt &= 0xf0;
                        ALLDevices[LN][device_Addr].ReScanCnt += 1; //���ξ���һ��
                    }
                    //�޸Ľ���
                }
            }
            else 
            {
            // ֻ�������״��¼���־��������������
                ALLDevices[LN][device_Addr].ReScanStat1 &= 0xf0;//��ҪӰ������ڵ�
                if (1 == pw2) 
                {
                    ALLDevices[LN][device_Addr].Comd &= 0xf0;   //���������������������
                    ALLDevices[LN][device_Addr].Comd += XJCMD(LN, device_Addr);
                }
            }
        }
        else if (4 == pw1)  // ����ӵ��
        {
            if( ALLDevices[LN][device_Addr].ReScanStat5 & FireRecover ) 
            {
                // �ֱ�,CDI����ʽ��̽������ͬ���𾯻ָ��������������ˡ�
                if ((ALLDevices[LN][device_Addr].DeviceType == DEVICE_HAND_FIRE_DETECTOR)||
                    (ALLDevices[LN][device_Addr].DeviceType == DEVICE_FIRE_COCK_BUTTON)||
                    (ALLDevices[LN][device_Addr].DeviceType == DEVICE_RELAY_MODULE)) 
                {
                     return;
                }
            }
            AnsCodeRight_flag = 0;
            StateTempSave = FireStat; // �׻𾯺ͻ�ȷ�϶�λ��BIT0,�˴���д��Ϊ�˱�������
        } 
        else  // ����ӵ���ֹ���
        {
            AnsCodeRight_flag = 0;
            if (2 == pw1)     // ����ӵ㿪·
            {    
                StateTempSave = InOpenStat;
            } 
            else if (3 == pw1)  // ����ӵ��·
            {
                StateTempSave = DustOrInShortStat;
            } 
            else              // ģ�����
            {   
                StateTempSave = FaultStat;
            }
        }
        /*************��ʼ����𾯻�������*************/
        if (!AnsCodeRight_flag )  // ģ������ڵ����¼����ڴ˴����д���
        {
            if (StateTempSave & ALLDevices[LN][device_Addr].ReScanStat2) 
            {
            //����ڵ��¼��Ѿ�ȷ����,��������
                return;
            } 
            else if (StateTempSave & ALLDevices[LN][device_Addr].ReScanStat1) 
            {
            // ��ȷ������ڵ����¼�
                // �˴�ȷ�ϻ𾯻���ϲ��ϱ�,Ȼ������Ӧ�ı�־
                if (FireStat == StateTempSave) 
                {                    
//                	send_buff[0] = CMD_EVENT_TRANSFER;

                    if (DEVICE_IO_TYPE[ALLDevices[LN][device_Addr].DeviceType] == 0x01 ||
						DEVICE_IO_TYPE[ALLDevices[LN][device_Addr].DeviceType] == 0x00 ||
						DEVICE_IO_TYPE[ALLDevices[LN][device_Addr].DeviceType] == 0x11)
//                		send_buff[1] = EVENT_FEEDBACK;
//                	else                	
//                    	send_buff[1] = EVENT_FIRE;
                    	
                    send_buff[2] = DEVICE_TYPE_LOOP_CARD;
                    send_buff[3] = device_Addr;
                    send_buff[4] = 0;
                    send_buff[5] = 0;
                    send_buff[6] = 0;
//                    can_send(1, dev_addr + LN, 0, 7, send_buff);

                    ALLDevices[LN][device_Addr].Comd = 0x2A;	 // ����,�����                    
                } 
                else if (FaultStat == StateTempSave) 
                {              
                	if ((ALLDevices[LN][device_Addr].ReScanStat2 & 0x6E) == 0)
                	{
//	                	send_buff[0] = CMD_EVENT_TRANSFER;
//	                    send_buff[1] = EVENT_LINE_FAULT;
	                    send_buff[2] = DEVICE_TYPE_LOOP_CARD;
	                    send_buff[3] = device_Addr;
	                    send_buff[4] = 0;
	                    send_buff[5] = 0;
	                    send_buff[6] = 0;
//	                    can_send(1, dev_addr + LN, 0, 7, send_buff); 		// ������,����־����

	                    ALLDevices[LN][device_Addr].OtherFlags.FlagBit.CircuitERR_flag = 1;
                    }
                } 
                else if (DustOrInShortStat == StateTempSave) 
                {	
                	if ((ALLDevices[LN][device_Addr].ReScanStat2 & 0x6E) == 0)
                	{
//						send_buff[0] = CMD_EVENT_TRANSFER;
//						send_buff[1] = EVENT_LINE_FAULT;
						send_buff[2] = DEVICE_TYPE_LOOP_CARD;
						send_buff[3] = device_Addr;
						send_buff[4] = 0;
						send_buff[5] = 0;
						send_buff[6] = 0;
//						can_send(1, dev_addr + LN, 0, 7, send_buff);	// ������ģ���·
						
						ALLDevices[LN][device_Addr].OtherFlags.FlagBit.CircuitERR_flag = 1;
                	}
                } 
                else if (InOpenStat == StateTempSave) 
				{
				    if ((ALLDevices[LN][device_Addr].ReScanStat2 & 0x6E) == 0)
                	{
//						send_buff[0] = CMD_EVENT_TRANSFER;
//						send_buff[1] = EVENT_LINE_FAULT;
						send_buff[2] = DEVICE_TYPE_LOOP_CARD;
						send_buff[3] = device_Addr;
						send_buff[4] = 0;
						send_buff[5] = 0;
						send_buff[6] = 0;
//						can_send(1, dev_addr + LN, 0, 7, send_buff);// ������ģ�鿪·
						
	                    ALLDevices[LN][device_Addr].OtherFlags.FlagBit.CircuitERR_flag = 1;
                    }
                }
                //��ȷ��λ
                SetBit(ALLDevices[LN][device_Addr].ReScanStat2, StateTempSave);
            } 
            else 
            {
 	           //��û��ȷ�����¼�
                if (FireStat == StateTempSave) 
                {
                    tempVar = FIRE_RESCAN_TIMES;  // ����ɨ����
                } 
                else 
                {
                    tempVar = FAULT_RESCAN_TIMES; // ������ɨ����
                }

                if (0 == (0xf0 & ALLDevices[LN][device_Addr].Comd)) 
                {
                // ��һ��̽�⵽���¼�,׼������ظ�ɨ��
                    ALLDevices[LN][device_Addr].Comd += (tempVar << 4);
                    ALLDevices[LN][device_Addr].ReScanCnt &= 0xf0;
                    ALLDevices[LN][device_Addr].ReScanCnt += 1; // ���ξ���һ��
                }
                else 
                {
                    if( (ALLDevices[LN][device_Addr].ReScanCnt & 0x0f) < 0x0f ) 
                    {
                        ALLDevices[LN][device_Addr].ReScanCnt++;
                    }

                    if ((ALLDevices[LN][device_Addr].ReScanCnt & 0x0f) >= (tempVar - RESCAN_ALLOW_ERR_TIMES)) 
                    { // ֻ�����й涨�δ���
                        SetBit(ALLDevices[LN][device_Addr].ReScanStat1, StateTempSave);
                        ALLDevices[LN][device_Addr].Comd &= 0x0f;
                        LOOP[LN].NeedCheckAddr = device_Addr;
                    }
                }
            }//ģ������ڵ��Ƿ��Ѿ�ȷ�����¼����䴦��--���
        }//ģ������ڵ����¼�����--���
        /****************************************************************************
	        *                         ģ������ڵ㴦�����                              *
	        *        ���¿�ʼ����ģ������ڵ�                                           *
	        ****************************************************************************/
        if (1 == pw2) // ����ӵ�����
        {// PW2 ����ӵ�״̬
            AnsCodeRight_flag = 1; // ����
            tempVar = ALLDevices[LN][device_Addr].ReScanStat2 & 0x70; // ��������ڵ��־

            if (0 != tempVar)  // ����ڵ������л𾯹��ϵ��¼�ȷ��
            {
                if (0xf0 & ALLDevices[LN][device_Addr].Comd) 
				{
                	// �Ѿ���ʼ����ɨ��
                    if((ALLDevices[LN][device_Addr].ReScanCnt & 0xf0) < 0xf0) 
                    {
                        ALLDevices[LN][device_Addr].ReScanCnt += 0x10; //����λ��
                    }

                    if (((ALLDevices[LN][device_Addr].ReScanCnt & 0xf0) >> 4)
                        > (FFRecover_RESCAN_TIMES - RESCAN_ALLOW_ERR_TIMES)) 
                    {
                    //ֻ�����й涨�δ���
                        //����ڵ��¼��ָ����˴����IIC�ϱ�����
                        if (ALLDevices[LN][device_Addr].ReScanStat2 & DevIsOutFire) 
                        {
                            SetBit(ALLDevices[LN][device_Addr].ReScanStat5, FireRecover);

                            //LoopSendEvent2Main( 0x40, 5, 0x3A, LN, device_Addr, 0); // ���𾯻ָ�

							if (DEVICE_IO_TYPE[ALLDevices[LN][device_Addr].DeviceType] == 0x01 ||	//������ȡ��
								DEVICE_IO_TYPE[ALLDevices[LN][device_Addr].DeviceType] == 0x00 ||
								DEVICE_IO_TYPE[ALLDevices[LN][device_Addr].DeviceType] == 0x11)
						   	{
//								send_buff[0] = CMD_EVENT_TRANSFER;								   
//								send_buff[1] = EVENT_FEEDBACK_CANCLE;									   
								send_buff[2] = DEVICE_TYPE_LOOP_CARD;
								send_buff[3] = device_Addr;
								send_buff[4] = 0;
								send_buff[5] = 0;
								send_buff[6] = 0;
//								can_send(1, dev_addr + LN, 0, 7, send_buff);
						   	}
                        }
                        if (ALLDevices[LN][device_Addr].ReScanStat2 & (DevIsOutShort | DevIsOutOpen)) 
                        {
                            SetBit(ALLDevices[LN][device_Addr].ReScanStat5, FaultRecover);
                    		                    		
//		                	send_buff[0] = CMD_EVENT_TRANSFER;
//		                    send_buff[1] = EVENT_LINE_FAULT_RECOVERY;
		                    send_buff[2] = DEVICE_TYPE_LOOP_CARD;
		                    send_buff[3] = device_Addr;
		                    send_buff[4] = 0;
		                    send_buff[5] = 0;
		                    send_buff[6] = 0;
//		                    can_send(1, dev_addr + LN, 0, 7, send_buff); 		 // �����ϻָ�
							
							ALLDevices[LN][device_Addr].ReScanStat2 = 0;
                        }
                        ALLDevices[LN][device_Addr].ReScanCnt   &= 0x0f;
                        ALLDevices[LN][device_Addr].ReScanStat1 &= 0x0f; // ��ҪӰ������ڵ�
                        ALLDevices[LN][device_Addr].ReScanStat2 &= 0x0f; // ��ҪӰ������ڵ�
                        ALLDevices[LN][device_Addr].Comd = XJCMD(LN, device_Addr); // �˴���̽������ͬ
                        return;
                    }
                }
                else 
                {
                    ALLDevices[LN][device_Addr].Comd += (FFRecover_RESCAN_TIMES << 4);
                    ALLDevices[LN][device_Addr].ReScanCnt &= 0x0f;
                    ALLDevices[LN][device_Addr].ReScanCnt += 0x10; // ���ξ���һ��
                }
            } 
			else 
			{
            	//ֻ�������״��¼���־��������������
                ALLDevices[LN][device_Addr].ReScanStat1 &= 0x0f; // ��ҪӰ������ڵ�
                if (1 == pw1) 
				{
                    ALLDevices[LN][device_Addr].Comd &= 0xf0;    // ���������������������
                    ALLDevices[LN][device_Addr].Comd += XJCMD(LN, device_Addr);
                }
            }
        }
        else if (4 == pw2)      // ����ӵ��
        {
            AnsCodeRight_flag = 0;
            StateTempSave = OutFireStat;
        } 
        else                     // ����ڵ����
        {
            AnsCodeRight_flag = 0;
            
            if (2 == pw2)        // ����ӵ㿪· 
            {
                StateTempSave = OutOpenStat;
            } 
            else if (3 == pw2)  // ����ӵ��·
            {
                StateTempSave = OutShortStat;
            } 
            else                // ����ӵ㶯�����ⵥ������
            {
                return;
            }
        }
        /*************��ʼ����𾯻�������*************/
        if (!AnsCodeRight_flag)  // ģ������ڵ����¼����ڴ˴����д���
        {
            if (StateTempSave & ALLDevices[LN][device_Addr].ReScanStat2) 
            { // ����ڵ��¼��Ѿ�ȷ���ˣ���������           
                return;
            } 
            else if (StateTempSave & ALLDevices[LN][device_Addr].ReScanStat1) 
            {
            	// ��ȷ������ڵ����¼�
                // �˴�ȷ�ϻ𾯻���ϲ��ϱ�,Ȼ������Ӧ�ı�־
                if (OutFireStat == StateTempSave) 
                {                   
//                	send_buff[0] = CMD_EVENT_TRANSFER;

                	if (DEVICE_IO_TYPE[ALLDevices[LN][device_Addr].DeviceType] == 0x01 ||
						DEVICE_IO_TYPE[ALLDevices[LN][device_Addr].DeviceType] == 0x00 ||
						DEVICE_IO_TYPE[ALLDevices[LN][device_Addr].DeviceType] == 0x11)
//                		send_buff[1] = EVENT_FEEDBACK;
//                	else                	
//                    	send_buff[1] = EVENT_FIRE;
                    	
                    send_buff[2] = DEVICE_TYPE_LOOP_CARD;
                    send_buff[3] = device_Addr;
                    send_buff[4] = 0;
                    send_buff[5] = 0;
                    send_buff[6] = 0;
//                    can_send(1, dev_addr + LN, 0, 7, send_buff);

                    ALLDevices[LN][device_Addr].Comd = 0x2A ;// ����,����ȷ������   
                } 
                else if (OutShortStat == StateTempSave) 
                {		
                	if ((ALLDevices[LN][device_Addr].ReScanStat2 & 0x6E) == 0)
                	{
//						send_buff[0] = CMD_EVENT_TRANSFER;
//						send_buff[1] = EVENT_LINE_FAULT;
						send_buff[2] = DEVICE_TYPE_LOOP_CARD;
						send_buff[3] = device_Addr;
						send_buff[4] = 0;
						send_buff[5] = 0;
						send_buff[6] = 0;
//						can_send(1, dev_addr + LN, 0, 7, send_buff); // �����ģ���·
						
	                    ALLDevices[LN][device_Addr].OtherFlags.FlagBit.CircuitERR_flag = 1;
                    }
                } 
                else if (OutOpenStat  == StateTempSave) 
                {		
                	if ((ALLDevices[LN][device_Addr].ReScanStat2 & 0x6E) == 0)
                	{
//						send_buff[0] = CMD_EVENT_TRANSFER;
//						send_buff[1] = EVENT_LINE_FAULT;
						send_buff[2] = DEVICE_TYPE_LOOP_CARD;
						send_buff[3] = device_Addr;
						send_buff[4] = 0;
						send_buff[5] = 0;
						send_buff[6] = 0;
//						can_send(1, dev_addr + LN, 0, 7, send_buff);// �����ģ�鿪·
						
						ALLDevices[LN][device_Addr].OtherFlags.FlagBit.CircuitERR_flag = 1;
                	}
                }

                SetBit(ALLDevices[LN][device_Addr].ReScanStat2, StateTempSave);// ��ȷ��λ
            }
            else 
            {
            	//��û��ȷ�����¼�
                if (OutFireStat == StateTempSave) 
                {
                    tempVar = FIRE_RESCAN_TIMES;  // ����ɨ����
                } 
                else 
                {
                    tempVar = FAULT_RESCAN_TIMES; // ������ɨ����
                }

                if (0 == (0xf0 & ALLDevices[LN][device_Addr].Comd)) 
                {
                	// ��һ��̽�⵽���¼�,׼������ظ�ɨ��
                    ALLDevices[LN][device_Addr].Comd += ( tempVar <<4 );
                    ALLDevices[LN][device_Addr].ReScanCnt &= 0x0f;
                    ALLDevices[LN][device_Addr].ReScanCnt += 0x10; // ���ξ���һ��
                } 
                else 
                {
                    if ((ALLDevices[LN][device_Addr].ReScanCnt & 0xf0) < 0xf0) 
                    {
                        ALLDevices[LN][device_Addr].ReScanCnt += 0x10;
                    }

                    if (((ALLDevices[LN][device_Addr].ReScanCnt & 0xf0) >> 4)
                        >= ( tempVar - RESCAN_ALLOW_ERR_TIMES ) ) 
                    { // ֻ�����й涨�δ���
                        SetBit(ALLDevices[LN][device_Addr].ReScanStat1, StateTempSave);
                        ALLDevices[LN][device_Addr].Comd &= 0x0f;
                        LOOP[LN].NeedCheckAddr = device_Addr; // 070413
                    }
                }
            }//ģ������ڵ��Ƿ��Ѿ�ȷ�����¼����䴦��--���
        }//ģ������ڵ����¼�����--���
    }//������Ϊģ��Ĵ���--���

    return;
}

/*
********************************************************************************
* ��������: static void DealLoopOtherFault( u8 LN, u8 device_Addr, u8 fault_Type );
* ��������: ��·������������״̬����,��Щ״̬�Ǵ�̽������ģ��״̬�����в鲻�����ģ�
*           ��������Ӧ�����룬���ʹ����,�ڶ�δ�ѭ����ɨ�裬ÿ��ѭ��ֻɨһ�Ρ�
* ��    ��: u8 LN          : ��·��0��(0��1)
*           u8 device_Addr : ������(1~255)
*           u8 fault_Type  : ��������(��LoopOpt.h)
* �� �� ֵ: ��
* ע    ��: GK603����������޸ĵĻ�·������ʽ��ͬ����ǰ�ǼƴΣ���ʱ��׼ȷ������
*           ��Ϊ�ɱ���ǰ���ñ���ʱ�䣬��ʧ����ʱ�ڼ䲻�ָ��ͱ�����
********************************************************************************
*/
static void DealLoopOtherFault( u8 LN, u8 device_Addr, u8 fault_Type )
{
    u32 temp;

    if (AutoRegister_flag)
    	return;  // �Զ���¼��䲻�����¼�
    /*******************************����Ӧ����*********************************/
    if (NoRespStat == fault_Type) 
    {
        if (ALLDevices[LN][device_Addr].ReScanStat3 & NoRespStat) 
        {	
        	// ��ǰ�Ѿ���⵽��ʧ			
            if( ALLDevices[LN][device_Addr].ReScanStat4 & NoRespStat) 
            {
                return; // �Ѿ�ȷ�ϲ��ϱ���ʧ��ֱ���˳�����
            }

            temp = GetTickCount();
            if ((temp - ALLDevices[LN][device_Addr].LoseCntTicks) >
                ((LOSE_REPORT_TIME * 1000) / MSPERTICK))  // ��ʧʱ��ﵽ�涨ֵ
            {
                // �����������ö�ʧȷ��λ
                if ( 1 ) 	// ��������
                {					
//                	send_buff[0] = CMD_EVENT_TRANSFER;
                    send_buff[1] = EVENT_TYPE_FAULT;
                    send_buff[2] = DEVICE_TYPE_LOOP_CARD;
                    send_buff[3] = device_Addr;
                    send_buff[4] = 0;
                    send_buff[5] = 0;
                    send_buff[6] = 0;
//                    LED_FAULT_ON;
//                    fault_flag = 1;
//                    can_send(1, dev_addr + LN, 0, 7, send_buff);
                    Loop_Revice(send_buff);
                    SetBit(ALLDevices[LN][device_Addr].ReScanStat4, NoRespStat);
                    ClrBit(ALLDevices[LN][device_Addr].ReScanStat3, NoRespRcvStat); // ���������ϵ�ʱ��Ҳ�����ظ�����ʧ�ָ�
                    ClrBit(ALLDevices[LN][device_Addr].ReScanStat4, NoRespRcvStat);
                    ALLDevices[LN][device_Addr].OtherFlags.FlagBit.DisLoseErrAns_flag = 1;
                    
                    return;
                }
            }
        }
        else 
        {
        	// ��һ�μ�⵽��ʧ���ñ�־λ�����涪ʧʱ��
            SetBit(ALLDevices[LN][device_Addr].ReScanStat3, NoRespStat);
            ALLDevices[LN][device_Addr].LoseCntTicks = GetTickCount();
        }

        return;
    } 
    else 
    {
        if ((ALLDevices[LN][device_Addr].ReScanStat3 & NoRespStat) && 
            (0 == (ALLDevices[LN][device_Addr].ReScanStat4 & NoRespStat)))
        {
             ClrBit(ALLDevices[LN][device_Addr].ReScanStat3, NoRespStat);
             ALLDevices[LN][device_Addr].OtherFauCnt  = 0;
             ALLDevices[LN][device_Addr].LoseCntTicks = 0;
        }
    }

    /****************************�����������**********************************/
    if (ALLDevices[LN][device_Addr].ReScanStat3 & fault_Type) 
    { 	// �ϴξ����������
        if (ALLDevices[LN][device_Addr].ReScanStat4 & fault_Type) 
        {
        	// �ù����Ѿ���ȷ�ϲ����˴���
            // �Ƿ񱨹�����Ӧ�������������Ҫʹ��������Ӧ�ָ�
            if (NoRespStat & ALLDevices[LN][device_Addr].ReScanStat4) 
            { // ���������ϵ�ʱ��Ҳ�����ظ�����ʧ�ָ�
                if (ALLDevices[LN][device_Addr].ReScanStat3 & NoRespRcvStat) 
                {
                    if (ALLDevices[LN][device_Addr].ReScanStat4 & NoRespRcvStat) 
                    {
                        ALLDevices[LN][device_Addr].OtherFlags.FlagBit.DisLoseErrAns_flag = 0;
                        return; // �Ѿ���������Ӧ�ָ�
                    } 
                    else 
                    {
                        ALLDevices[LN][device_Addr].OtherFauCnt ++;
                        if (ALLDevices[LN][device_Addr].OtherFauCnt >= DA_TE_CNT_TIMES)
                        {
                            //������Ӧ�ָ����ö�Ӧ��־λ
                            SetBit(ALLDevices[LN][device_Addr].ReScanStat4, NoRespRecover);
                            //SetBit(ALLDevices[LN][device_Addr].ReScanStat5, NoRespRecover);
					
//		                	send_buff[0] = CMD_EVENT_TRANSFER;
//		                    send_buff[1] = EVENT_DEVICE_FAULT_RECOVERY;
		                    send_buff[2] = DEVICE_TYPE_LOOP_CARD;
		                    send_buff[3] = device_Addr;
		                    send_buff[4] = 0;
		                    send_buff[5] = 0;
		                    send_buff[6] = 0;
		                
//		                    can_send(1, dev_addr + LN, 0, 7, send_buff);	

                            ClrBit(ALLDevices[LN][device_Addr].ReScanStat3, NoRespStat);
                            ClrBit(ALLDevices[LN][device_Addr].ReScanStat4, NoRespStat);
                            ALLDevices[LN][device_Addr].OtherFauCnt = 0;
                            ALLDevices[LN][device_Addr].OtherFlags.FlagBit.DisLoseErrAns_flag = 0;
                        }
                    }
                }
                else 
                {
                    SetBit(ALLDevices[LN][device_Addr].ReScanStat3, NoRespRcvStat);
                    ALLDevices[LN][device_Addr].OtherFauCnt = 0;  // Ȼ�����¼���
                }
            }

            return;  // �Ѿ�ȷ�ϲ��ϱ�������쳣�ˣ���������
        }

        if (ALLDevices[LN][device_Addr].OtherFauCnt >= DA_TE_CNT_TIMES) 
        {
            if (NoOtherFauStat == fault_Type )  // ���ɨ��û�й���
            {
                if (NoRespStat & ALLDevices[LN][device_Addr].ReScanStat4) 
                {
                    //������Ӧ�ָ����ö�Ӧ��־λ           				
//                	send_buff[0] = CMD_EVENT_TRANSFER;
                    send_buff[1] = EVENT_TYPE_FAULT_RESUME;
                    send_buff[2] = DEVICE_TYPE_LOOP_CARD;
                    send_buff[3] = device_Addr;
                    send_buff[4] = 0;
                    send_buff[5] = 0;
                    send_buff[6] = 0;
                       
//                    can_send(1, dev_addr + LN, 0, 7, send_buff);	
                     Loop_Revice(send_buff);
                    ClrBit(ALLDevices[LN][device_Addr].ReScanStat4, NoRespStat);
                    ALLDevices[LN][device_Addr].OtherFlags.FlagBit.DisLoseErrAns_flag = 0;
                }
                // ���Ը��Ҳ���Դ˴���������������ͻָ����¼��ϱ�
                ALLDevices[LN][device_Addr].ReScanStat3 = NoOtherFauStat;
                // ReScanStat4���ĳ����ȷ��λ��ù��Ͽ��Է����ϱ�
            } 
            else if (DupAddrStat == fault_Type) 
            {
                // ����ַ���벢�ö�Ӧ��־λ
                SetBit(ALLDevices[LN][device_Addr].ReScanStat4, fault_Type);
            } 
            else if (TypeErrStat == fault_Type) 
            {
                // �����ʹ����ö�Ӧ��־λ					
//            	send_buff[0] = CMD_EVENT_TRANSFER;
//                send_buff[1] = EVENT_DEVICE_FAULT;
                send_buff[2] = DEVICE_TYPE_LOOP_CARD;
                send_buff[3] = device_Addr;
                send_buff[4] = 0;
                send_buff[5] = 0;
                send_buff[6] = 0;
                
//                can_send(1, dev_addr + LN, 0, 7, send_buff);	

                SetBit(ALLDevices[LN][device_Addr].ReScanStat4, fault_Type);
                ALLDevices[LN][device_Addr].Comd =  XJCMD(LN, device_Addr);
            } 
            else if (SendErrStat == fault_Type) 
            {
                // �����͹��ϲ��ö�Ӧ��־λ					
//            	send_buff[0] = CMD_EVENT_TRANSFER;
//                send_buff[1] = EVENT_DEVICE_FAULT;
                send_buff[2] = DEVICE_TYPE_LOOP_CARD;
                send_buff[3] = device_Addr;
                send_buff[4] = 0;
                send_buff[5] = 0;
                send_buff[6] = 0;
                
//                can_send(1, dev_addr + LN, 0, 7, send_buff);	

                SetBit(ALLDevices[LN][device_Addr].ReScanStat4, fault_Type);
            }

            ALLDevices[LN][device_Addr].OtherFauCnt = 0;
            return;
        } 
        else 
        {
            ALLDevices[LN][device_Addr].OtherFauCnt++;  // �쳣�������ۼ�
        }
    }
    else 
    { 	
    	// �µĹ���
        ALLDevices[LN][device_Addr].ReScanStat3 = 0;
        SetBit( ALLDevices[LN][device_Addr].ReScanStat3, fault_Type );
        ALLDevices[LN][device_Addr].OtherFauCnt = 0;     // Ȼ�����¼���
    }

    return;
}

/*
********************************************************************************
* ��������: void AnsCodeDeal(u8 LN, u8 device_Addr);
* ��������: ��·�������ݴ���
* ��    ��: u8 LN          : ��·��0 �� (0,1)
*           u8 device_Addr : ������1~255
* �� �� ֵ: ��
* ע    ��: Ϊ�˼ӿ촦���ٶȣ�0us = 0; 300us = 1; 600us = 2; 900us = 3,1200us = 4,1500us = 5
********************************************************************************
*/
void AnsCodeDeal(u8 LN, u8 device_Addr)
{
    u8 	 acodes[5];    //�����ڴ�ž��������� answer codes
    u32  temp, i;
    BOOL DevIsOldMod; //�������ģ��Ҫ��λ�����ͼ��ʱ��ʹ�ã�����ʱ���������û����
    
    memset(acodes, 0, 5);
    
    /**************************����״̬�������� *****************************/
    i = 0; // ����һ�£�����ָʾ��������

    if (LOOP[LN].OptStatFlags.StateBit.SendAddr0_flag || 
        (0 == device_Addr) ||
        LOOP[LN].OptStatFlags.StateBit.OldJmpLedSolution_flag ||  
        LOOP[LN].OptStatFlags.StateBit.InterruptByBroadCastCmd_flag) 
    {
        return; // �����������������·��0�롢�����������תʱ����ʱ�䳤������ʾ�̵Ĺ㲥����
    } 
    else if (0 == LOOP[LN].PWn[0])  // ��������Ӧ�������Ǽ�������Ӧ�����
    {  
    	// LoopCard_LED( LN, LEDY, LED_ON);
        // BOOL�������ã���Ϊ�������ʽ̫����
        DevIsOldMod = ((0x06 == (ALLDevices[LN][device_Addr].Comd & 0x0f)) ||
                       (0x07 == (ALLDevices[LN][device_Addr].Comd & 0x0f)) ||
                       (0x08 == (ALLDevices[LN][device_Addr].Comd & 0x0f)) ||
                       (0x09 == (ALLDevices[LN][device_Addr].Comd & 0x0f)));
        // �����ϼ�������ʱ���Զ���½ʱ���ϵ��ģ��ʱ�����벻���κδ���
        if ((DevIsOldMod && LOOP[LN].OptStatFlags.StateBit.RstCloseModu_flag) || AutoRegister_flag) 
        {
        } 
        else if (ALLDevices[LN][device_Addr].OtherFlags.FlagBit.CircuitERR_flag) 
        {
            //��������ֻ��������Ѳ������Ļ���
            if ((0x00 == (0x0f & ALLDevices[LN][device_Addr].Comd)) ||
                (0x0d == (0x0f & ALLDevices[LN][device_Addr].Comd))) 
            {
                DealLoopOtherFault(LN, device_Addr, NoRespStat);
            } 
            else 
            {
                ALLDevices[LN][device_Addr].Comd = 0x00;
                LDList[LN][device_Addr]          = 0; // ��ֹ��������һ��������·���ϵ�ģ��
            }

            return;
        } 
        else   //���������Ҫ������Ӧ����
        {
            DealLoopOtherFault(LN, device_Addr, NoRespStat);
            return;
        }
    } 
    else if ((0 != LOOP[LN].LoopAnsStat) && 	// ����ʱ����(��ʱ)
             (0x05 != (0x0f & ALLDevices[LN][device_Addr].Comd)))
    {
        // ���Ͳ���ʱ������Ϊ�¾������Ĳ�ͬ�������Ļ��볬ʱ��ʱ������Ϊ�Ǵ���
        if (!AutoRegister_flag) 
        {
            DealLoopOtherFault(LN, device_Addr, SendErrStat);      //���͹���
            return;
        }
    } 
    else  // �����ϴ���
    {
        i++;
    }

    if (ALLDevices[LN][device_Addr].OtherFlags.FlagBit.IsDuAddr_flag)  // ��·��������
    {
        ALLDevices[LN][device_Addr].OtherFlags.FlagBit.IsDuAddr_flag = 0;
        DealLoopOtherFault(LN, device_Addr, DupAddrStat);      // ���봦��
    } 
    else  // �����ϴ���
    {
        i++;
    }

    if (2 == i) // û�������κδ���
    {
        // ֻ�в������ͼ��ʱ������Ϊ�޴���,��Ϊ���ͼ����������
        if (0x05 != (ALLDevices[LN][device_Addr].Comd & 0x0f)) 
        {
            //�޹��Ϻ�����Ӧ�ָ��ϱ����������ϻָ����ϱ�
            DealLoopOtherFault(LN, device_Addr, NoOtherFauStat);
        }
    }

    /********************************������������ *****************************/
    //��һ�����������
    if (LOOP[LN].PWn[0] > TIME_600US) 
    {
    	temp = TIME_900US; 					// PW0����600usΪ900us����
    }
    else                                    // PW0С��600usΪ300us����������
    { 
        temp = TIME_300US;
        ALLDevices[LN][device_Addr].OtherFlags.FlagBit.IsOldDev_flag = 1; //������������һ����־λ
    }
    
    for (i = 1; i < 5; i++) 				// ����PW0����ÿһ������
    {
        if (LOOP[LN].PWn[i] > 0)
        	LOOP[LN].PWn[i] = (LOOP[LN].PWn[i] + temp) - LOOP[LN].PWn[0];
        else    
        	break;
    }
    
    LOOP[LN].PWn[0] = temp;

    /**************************************************************************/
    //�ڶ������ó���ȷ������Ϣ
    for (i = 0; i < 5; i++)
    {
        temp = LOOP[LN].PWn[i];
        
        if (temp > 0) 
        {
            if ((2 == i) &&  
                ((0x0f & ALLDevices[LN][device_Addr].Comd) < 0x04) &&
                is_detector(ALLDevices[LN][device_Addr].DeviceType))		//̽����
            {
                //����0û����һλ������1��2��3����һλ��ģ����ֵ
            	if (temp >= TIME_1758US) // ������ֵ
            	{
            	    acodes[2] = 200;
            	}
            	else //����ֵTC1758US_CNTʱ��Ӧ200����ôС��TC1758US_CNT��ֵ��Ӧ���٣��˴�������
            	{
            	    temp = temp * 200 + TIME_1758US / 2;
            	    temp = temp / TIME_1758US;
            	    acodes[2] = (u8)temp;
            	    i = 8;
            	}
            } 
            else  // ��ģ����
            {
                if (((TIME_300US + TIME_150US) > temp) &&   //20180418�޸�
                    ((TIME_300US - TIME_150US) < temp))
                {
                    acodes[i] = 1;  //300us
                } 
                else if(((TIME_600US + TIME_150US) > temp) &&
                        ((TIME_600US - TIME_150US) < temp)) 
                {
                    acodes[i] = 2;  //600us
                } 
                else if(( ( TIME_900US + TIME_150US) > temp ) &&
                        ( ( TIME_900US - TIME_150US) < temp ) )
                {
                    acodes[i] = 3;  //900us
                } 
                else if( ( ( TIME_1200US + TIME_150US) > temp ) &&
                        ( ( TIME_1200US - TIME_150US) < temp ) ) 
                {
                    acodes[i] = 4; //1200us
                } 
                else if( ( ( TIME_1500US + TIME_150US) > temp ) &&
                        ( ( TIME_1500US - TIME_150US) < temp ) ) 
                {
                    acodes[i] = 5; //1500us
                } 
                else 
                {
                    acodes[i] = 0; //������������
                }
            }//��ģ�����������
        }
        else 
        {
            break; 
        }
    }

    /**************************************************************************/
    //�����������ݲ�ͬ������������Ϣ
    /*******************************
    *    ���Ͳ������������      *
    *******************************/
    if (0x05 == (0x0f & ALLDevices[LN][device_Addr].Comd)) 
    {
        if (((device_Addr < 128) && (1 == acodes[0])) || //̽����
            ((device_Addr > 127) && (3 == acodes[0]))) 
        { 
            i = DetTypePW3PW2[acodes[3]][acodes[2]]; //ע������ӦʱiΪ0xff
        }
        else									  //ģ��
        {
            if (3 == acodes[0]) 
            {
                DevIsOldMod = 0;
            } 
            else 
            {
                DevIsOldMod = 1;
            }
            
            i = ModTypePW4PW3[acodes[4]][acodes[3]];//ע������ӦʱiΪ0xff
        }
        // ���ϻ�ȡ�������ͣ������ж������Ƿ����ʼ��������ͬ
        if (i != ALLDevices[LN][device_Addr].DeviceType) // �������ʼ�����Ͳ�ͬ�Ĵ���
        {
            if (AutoRegister_flag) 		// �Զ���½����¼��������
            {
                if (((NM_SB == ALLDevices[LN][device_Addr].DeviceType) || (NM_QB == ALLDevices[LN][device_Addr].DeviceType)) && 
                    ((NM_SR == i) && DevIsOldMod)) /*�������ϼ��ݣ��ɵ��ֱ������ð�ť����Ϊ0x10ʱ��Ҫ������ǰType�������*/
                {
                	return;
                }
                
                ALLDevices[LN][device_Addr].Comd = 0x15;  // ����һ������ɨ��

				if (i == 0xff)
				{
					if (ALLDevices[LN][device_Addr].DeviceCondition != 1)
					{
						ALLDevices[LN][device_Addr].DeviceCondition = 0;	
						ALLDevices[LN][device_Addr].DeviceType      = 0xff; 
					}
				}
				else			
				{
					ALLDevices[LN][device_Addr].DeviceCondition = 1;
                	ALLDevices[LN][device_Addr].DeviceType      = i;     // ���汾�ν��,����ӦҲһ������
                }
                	
                return;
            } 
            else 
            {
            	//�������ͼ�������ֻ�����ʹ����ǲ�����ԭ��������
                if (0xff == i) 														// �˴�û������������δ֪
                { 
                    ALLDevices[LN][device_Addr].Comd = XJCMD(LN, device_Addr); 		//�������ɨһ�ξ͵���
                    return;
                }
                if (is_detector(ALLDevices[LN][device_Addr].DeviceType) || is_detector(i))  // ̽������
                {
                    DealLoopOtherFault( LN, device_Addr, TypeErrStat);
                    return;   //���Ͷ������ˣ�û�б�Ҫ�����������봦����
                }
                else if ((NM_SB == ALLDevices[LN][device_Addr].DeviceType) || (NM_QB == ALLDevices[LN][device_Addr].DeviceType)) 
                {
                    if ((NM_SR == i) && DevIsOldMod) 
                    {
                    	/*�������ϼ��ݣ��ɵ��ֱ������ð�ť����Ϊ0x10ʱ��Ҫ�����ʹ���*/
                        ALLDevices[LN][device_Addr].Comd = XJCMD(LN, device_Addr); //�������ɨһ�ξ͵���
                    } 
                    else 
                    {
                        DealLoopOtherFault(LN, device_Addr, TypeErrStat);
                        return;   //���Ͷ������ˣ�û�б�Ҫ�����������봦����
                    }
                }
            }
            DevIsOldMod = 0;
        }
        else 
        { // �������ʼ��������ͬ�Ĵ���
            if (!AutoRegister_flag) 
            {
                ALLDevices[LN][device_Addr].Comd = XJCMD(LN, device_Addr); //���ɨһ�ξ͵���
            }
        }

        if (AutoRegister_flag) 
        {
            if (255 == device_Addr)  // ��ǰ��·�Զ���½���
            {
                LOOP[LN].OptStatFlags.StateBit.AutoRegFinish_flag = 1;
            }
            return;
        }
    }

    /*******************************
    *    ���Ͳ�����������      *
    *                              *
    *        ������������          *
    *******************************/
    if (is_detector(ALLDevices[LN][device_Addr].DeviceType))   // ��ǰ�豸Ϊ̽����
    {
        //��ͬ����Ĵ���
        DealPW12State(LN, device_Addr, acodes[1], 0xaa);        
    } 
    else 		 // ��ǰ�豸Ϊģ��,�����Ĳ�ͬ����Ĵ���
    {
        switch (0x0f & ALLDevices[LN][device_Addr].Comd) 
        {
            case 0x09: // ģ�鶯��ѡͨ����
                if (((5 == acodes[2]) && ((1 == OutActType[LN]) || (2 == OutActType[LN]))) ||
                    ((1 == acodes[2]) && (3 == OutActType[LN]))) 
                {
                    //�����˵Ļ��Ͳ���2��ȷ����,�����ӿ��ٶ�
                    ClrBit(ALLDevices[LN][device_Addr].ReScanStat1, FirstOutActStat);
                    ClrBit(ALLDevices[LN][device_Addr].ReScanStat2, FirstOutNoAct);
                    if (1 == OutActType[LN])         // �����������Ϊ��
                    {
                        SetBit(ALLDevices[LN][device_Addr].ReScanStat5, OutOpenAct);
                        ClrBit(ALLDevices[LN][device_Addr].ReScanStat5, OutCloseAct);
                        //������ڵ��Ѷ���
//                        send_buff[0] = CMD_EVENT_TRANSFER;
//						send_buff[1] = EVENT_START;
						send_buff[2] = DEVICE_TYPE_LOOP_CARD;
						send_buff[3] = device_Addr;
						send_buff[4] = 0;
						send_buff[5] = 0;
						send_buff[6] = 0;

						if (ALLDevices[LN][device_Addr].DeviceType != DEVICE_FIRE_COCK_BUTTON)
                        {
                       //   can_send(1, dev_addr + LN, 0, 7, send_buff);     
                        }

                    } 
                    else if (3 == OutActType[LN])  // �����������Ϊ�ر�
                    {
                        ClrBit(ALLDevices[LN][device_Addr].ReScanStat5, OutOpenAct );
                        SetBit(ALLDevices[LN][device_Addr].ReScanStat5, OutCloseAct );
                        // ���Ǹ�λʱ��ǿ�ƹرղ��ϱ��ر�
                        if (!LOOP[LN].OptStatFlags.StateBit.RstCloseModu_flag ) 
                        {
                            // ������ڵ��Ѷ���
//                            send_buff[0] = CMD_EVENT_TRANSFER;
//							send_buff[1] = EVENT_STOP;
							send_buff[2] = DEVICE_TYPE_LOOP_CARD;
							send_buff[3] = device_Addr;
							send_buff[4] = 0;
							send_buff[5] = 0;
							send_buff[6] = 0;
							
							if (ALLDevices[LN][device_Addr].DeviceType != DEVICE_FIRE_COCK_BUTTON)
                            {
         	                 //can_send(1, dev_addr + LN, 0, 7, send_buff);                       
                            }

                        }
                    } 
                    else if (2 == OutActType[LN])  // ���巽ʽ����
                    {
                        //������ڵ��Ѷ���
//                        send_buff[0] = CMD_EVENT_TRANSFER;
//						send_buff[1] = EVENT_START;
						send_buff[2] = DEVICE_TYPE_LOOP_CARD;
						send_buff[3] = device_Addr;
						send_buff[4] = 0;
						send_buff[5] = 0;
						send_buff[6] = 0;
						
						if (ALLDevices[LN][device_Addr].DeviceType != DEVICE_FIRE_COCK_BUTTON)
                        {
                          //  can_send(1, dev_addr + LN, 0, 7, send_buff);
                        }
                        	
                    }
                    /**************/
                    if (-1 == LDNode[LN]) 
                    {
                        LOOP[LN].OptStatFlags.StateBit.HaveLDDev_flag    = 0;
                        //�����������ر���ɺ����־׼��ģ��ֹͣ�¼��ϱ�
                        LOOP[LN].OptStatFlags.StateBit.RstCloseModu_flag = 0;
                    }

                    ALLDevices[LN][device_Addr].Comd = XJCMD(LN, device_Addr);
                }
                else 
                { //δ�������·���,���2�ζ�δ����,��δ����
                    ALLDevices[LN][device_Addr].Comd = XJCMD(LN, device_Addr);
                    
                    if (FirstOutNoAct & ALLDevices[LN][device_Addr].ReScanStat2) 
                    { //�ڶ���ȷ��δ����
                        ClrBit(ALLDevices[LN][device_Addr].ReScanStat1, FirstOutActStat);
                        ClrBit(ALLDevices[LN][device_Addr].ReScanStat2, FirstOutNoAct);
                        //������ڵ�δ����,��ȡ��
                        if (!LOOP[LN].OptStatFlags.StateBit.RstCloseModu_flag)  //���Ǹ�λʱ��ǿ�ƹرղ��ϱ�
                        {
                            //LoopSendEvent2Main( 0x40, 5, 0x32, LN, device_Addr, 0 );
                        }

                        if (-1 == LDNode[LN]) 
                        {
                            LOOP[LN].OptStatFlags.StateBit.HaveLDDev_flag    = 0;
                            //�����������ر���ɺ����־׼��ģ��ֹͣ�¼��ϱ�
                            LOOP[LN].OptStatFlags.StateBit.RstCloseModu_flag = 0;
                        }
                        return;
                    }

					ALLDevices[LN][device_Addr].ReScanStat1 &= ~FirstOutActStat;
					ALLDevices[LN][device_Addr].ReScanStat2 |= FirstOutNoAct;

                    if (1 == OutActType[LN])//�����������Ϊ��
                        ALLDevices[LN][device_Addr].Comd = 0x36; //��ɨ
                    else if (3 == OutActType[LN]) //����Ϊ�ر�
                        ALLDevices[LN][device_Addr].Comd = 0x37; //��ɨ
                    else if (2 == OutActType[LN]) //����Ϊ����
                        ALLDevices[LN][device_Addr].Comd = 0x38; //��ɨ
                }
                return;
            case 0x06: //ͨ����������ģ�顢�������ģ��
                OutActType[LN] = 1;
                if (ALLDevices[LN][device_Addr].Comd <= 0x16) 
                {
                    ALLDevices[LN][device_Addr].Comd = 0x19; //��Ҫִ��1��09����
                }
                return;
            case 0x07: //ͨ������ر����ģ�顢�������ģ��
                OutActType[LN] = 3;
                if (ALLDevices[LN][device_Addr].Comd <= 0x17) 
                {
                    ALLDevices[LN][device_Addr].Comd = 0x19; //��Ҫִ��1��09����
                }
                return;
            case 0x08: //ͨ���������ģ�鴦�����������ʽ
                OutActType[LN] = 2;
                if (ALLDevices[LN][device_Addr].Comd <= 0x18) 
                {
                    ALLDevices[LN][device_Addr].Comd = 0x19; //��Ҫִ��1��09����
                }
                return;
            case 0x05: //���Ͳ���
            case 0x00: //��������ģ�����Ѳ�죬����ʵ�ֶ�ģ����������
            case 0x0d: //��������ģ��������Ѳ��
            default:
                DealPW12State( LN, device_Addr, acodes[1], acodes[2] );
                break;
        }
    }//ģ��Ĵ������
    //����������
    return;
}

/*
********************************************************************************
* ��������: void AutoRegFinished();
* ��������: ��·�Զ���½��ɺ����³�ʼ��ɨ�����������ϴ���
* ��    ��: ��
* �� �� ֵ: ��
* ע    ��:
********************************************************************************
*/
void AutoRegFinished(void)
{
//	int i;
	
    LoopListInit( 0 );  // ���³�ʼ��ɨ������
    LoopListInit( 1 );  // ���³�ʼ��ɨ������

	//�������ڵĻ�·�豸���
//	if ((auto_res_ln & (1 << 0)) == 0)
//	{
//		for (i = 0; i < 256; i++)
//		{
//			ALLDevices[0][i].DeviceCondition = 0;
//			ALLDevices[0][i].DeviceType      = 0xff;
//		}
//	}

//	if ((auto_res_ln & (1 << 1)) == 0)
//	{
//		for (i = 0; i < 256; i++)
//		{			
//			ALLDevices[1][i].DeviceCondition = 0;
//			ALLDevices[1][i].DeviceType      = 0xff;
//		}
//	}

//    startup_upload_device_type();
}

