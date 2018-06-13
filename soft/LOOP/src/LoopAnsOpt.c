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
extern  LOOPS LOOP[2];   				// 定义回路结构型回路变量。
extern LOOPDevice ALLDevices[2][256]; 	// 每个回路上256个回路器件型结构空间
extern u8  OptingLoopNO;      			// 当前正在操作的回路号0，1
extern u32 CheckTypeTimeTicks;   		// 回路定期做器件类型检查，此变量是用于这个事情的时间计数器
extern u8  LDList[2][256];				// 器件联动表
extern int LDNode[2];

extern BOOL AutoRegister_flag;     		// 自动登陆时置位，登陆完成清0
extern BOOL LoopCanScan_flag;       	// 控制回路是否可以扫描
extern u8 send_buff[8];
extern u8 auto_res_ln;

static u8 OutActType[2]; 				// 模块输出动作命令类型：1-启动。0-关闭。2-脉冲。
extern  EVENT_CONST const EventConstTable[];
/******************************************************************************
* 器件类型表 下标代表类型码，数值代表器件类型
* 0x05可忽略 0x01输入输出 0x11输出 0x00输入 0xFF器件不存在
* 需要与DeviceTypeDefine.h文件中的设备类型对应
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
* 函数名称: static void DealPW12State( u8 LN, u8 device_Addr, u8 pw1, u8 pw2 );
* 函数功能: 回码1,2状态处理
* 参    数: u8 LN             : 回路号0 或 (0,1)
*           u8 device_Addr    : 器件号1~255
*           u8 pw1, u8 pw2 : 回码1，2状态 pw1,pw2 (pw2 == 0xaa表示探测器，否则表示模块)
* 返 回 值: 无
* 注    意:
********************************************************************************
*/
static void DealPW12State( u8 LN, u8 device_Addr, u8 pw1, u8 pw2 )
{
    BOOL AnsCodeRight_flag;  //回码正常则置位
    u8  StateTempSave;      //状态临时存储，处理不同事件时用于存储事件标志位
    u8  tempVar;            //一个临时变量，可以任意使用
//    SYSTEM_EVENT 		 Event;
    if (AutoRegister_flag)// 自动登录其间不处理事件
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
    { // 探测器回码
    } 
	else if (pw2 > 5) 
	{
        return;
    }

    if (0xaa == pw2)
    {/**************************************************************************
     *                               探测器处理                                *
     *        0xaa是人为赋值的，用于区别探测器和模块,模块的这个参数有用        *
     **************************************************************************/
        if (1 == pw1)        // 探测器正常
        {
            AnsCodeRight_flag = 1; // 正常
            ALLDevices[LN][device_Addr].ReScanStat2 &= 0x07;  // 屏蔽无用位

            if (0 != ALLDevices[LN][device_Addr].ReScanStat2) // 曾经有火警故障等事件确认
            {				
                if (ALLDevices[LN][device_Addr].ReScanStat5 & FireRecover) 
                {
                // 只要有恢复事件这两个标识就一同被清除了，所以此处可以同时判断
                    if( ALLDevices[LN][device_Addr].ReScanStat2 & FaultStat )
                    {
                    }
                    else
                    {
                        return; // 火警恢复故障恢复后，不做其它处理了。
                    }
                }

                if (0xf0 & ALLDevices[LN][device_Addr].Comd) 
                {
                	// 已经开始重新扫描
                    ALLDevices[LN][device_Addr].ReScanCnt++;
                    if (ALLDevices[LN][device_Addr].ReScanCnt > 
                        (FFRecover_RESCAN_TIMES - RESCAN_ALLOW_ERR_TIMES)) 
                    { 	// 只允许有规定次错误
                    	// 事件恢复，上报并置相关标志位
                        if (ALLDevices[LN][device_Addr].ReScanStat2 & DevIsFireStat) 
                        {
                            if(ALLDevices[LN][device_Addr].ReScanStat5 & FireRecover)
                            {
                            }
                            else
                            {
                                SetBit(ALLDevices[LN][device_Addr].ReScanStat5, FireRecover);// 报火警恢复

							  	//报反馈取消
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
                            
//		                    can_send(1, dev_addr + LN, 0, 7, send_buff);		// 报故障恢复
                        }
                        
                        ALLDevices[LN][device_Addr].ReScanStat1 &= FireRecover;
                        ALLDevices[LN][device_Addr].ReScanStat2 &= FireRecover;
                        ALLDevices[LN][device_Addr].ReScanCnt = 0;
                        ALLDevices[LN][device_Addr].Comd &= 0x0f; // 此处与模块不同，如果是火警恢复，命令为0x0A
                        return;
                    }
                }
                else 
                { // 第一次探测到恢复正常，准备重新扫描
                    ALLDevices[LN][device_Addr].Comd += (FFRecover_RESCAN_TIMES << 4);
                    ALLDevices[LN][device_Addr].ReScanCnt = 1; // 本次就算一次
                }
            }
            else  // 只是置了首次事件标志，或者器件正常
            {
                ALLDevices[LN][device_Addr].ReScanStat1 = 0; // 相关标志位等恢复正常
                ALLDevices[LN][device_Addr].Comd = XJCMD(LN, device_Addr);
            }
        }
        else if (3 == pw1)  // 探测器火警
        {
            if (FireRecover & ALLDevices[LN][device_Addr].ReScanStat5)  // 发生过火警恢复
            {
                ALLDevices[LN][device_Addr].Comd &= 0x0f;
                return;  // 不做处理直接返回
            }
            AnsCodeRight_flag = 0;
            StateTempSave = FireStat; // 首火警和火警确认都位于BIT0,此处单写是为了避免歧义
        } 
        else 
        { // 探测器各种故障
            //if( FaultRecover & ALLDevices[LN][device_Addr].ReScanStat5 ) { // 发生过故障恢复
            //    ALLDevices[LN][device_Addr].Comd &= 0x0f;
            //    return; //不做处理直接返回
            //}
            AnsCodeRight_flag = 0;
            if (4 == pw1)  			// 探测器故障
            {
                StateTempSave = FaultStat; //首故障和故障确认都位于BIT1,此处单写是为了避免歧义
            } 
            else if (5 == pw1)  	// 探测器积灰故障
            {
                StateTempSave = DustOrInShortStat; // 首积灰和积灰确认都位于BIT2,此处单写是为了避免歧义
            }
        }
        /*************开始处理火警或故障相关*************/
        if (!AnsCodeRight_flag)  // 探测器有事件则在此处集中处理
        {
            if (StateTempSave & ALLDevices[LN][device_Addr].ReScanStat2) // 事件已经确认不做处理
            {            
                return;
            } 
            else if (StateTempSave & ALLDevices[LN][device_Addr].ReScanStat1) 
            {
            	// 已确认首事件,此处确认并上报火警或故障,然后置相应的标志
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
                    Loop_Revice(send_buff);//报火警
               		ALLDevices[LN][device_Addr].Comd = 0x2A; // 报火警,发火警确认命令					
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
//                    can_send(1, dev_addr + LN, 0, 7, send_buff);	// 报故障
                } 
                else if (DustOrInShortStat == StateTempSave) 
                {
                		send_buff[0] = 0x03;
                   		send_buff[1] = LN;
                        send_buff[2] = device_Addr;
//                    		can_send(1, dev_addr, 0, 3, send_buff);????? // 报积灰故障
                }

                SetBit(ALLDevices[LN][device_Addr].ReScanStat2, StateTempSave);
            } 
            else 
            {
            	//还没有确认首事件
                if (FireStat == StateTempSave) 
                {
                    tempVar = FIRE_RESCAN_TIMES;  // 火警重扫次数
                } 
                else 
                {
                    tempVar = FAULT_RESCAN_TIMES; // 故障重扫次数
                }

                if (0 == (0xf0 & ALLDevices[LN][device_Addr].Comd)) 
                {
                // 第一次探测到有事件,准备多次重复扫描
                    ALLDevices[LN][device_Addr].Comd += ( tempVar << 4 );
                    ALLDevices[LN][device_Addr].ReScanCnt = 1;  // 本次就算一次
                } 
                else 
                {
                    ALLDevices[LN][device_Addr].ReScanCnt++;
                    if (ALLDevices[LN][device_Addr].ReScanCnt >= (tempVar - RESCAN_ALLOW_ERR_TIMES)) 
                    { // 只允许有规定次错误
                        SetBit(ALLDevices[LN][device_Addr].ReScanStat1, StateTempSave);
                        ALLDevices[LN][device_Addr].Comd &= 0x0f;
                        ALLDevices[LN][device_Addr].ReScanCnt = 0;
                        LOOP[LN].NeedCheckAddr = device_Addr;
                    }
                }
            }//探测器是否已经确认了事件及其处理--完成
        }//探测器有事件处理--完成
    } //该器件为探测器的处理--完成
    /***************************************************************************/
    /****************************************************************************
    *                               探测器处理完成                              *
    *        以下开始处理模块的回码，首先处理模块输入节点                       *
    ****************************************************************************/
    else
    {   // PW1 输入接点状态
        if (1 == pw1)  // 输入接点正常
        {
            AnsCodeRight_flag = 1; // 正常
            tempVar = ALLDevices[LN][device_Addr].ReScanStat2 & 0x0f;  // 屏蔽输出节点标志

            if ( 0 != tempVar) 
            {
            	// 输入节点曾经有火警故障等事件确认
                if (ALLDevices[LN][device_Addr].ReScanStat5 & FireRecover ) 
                {
                    // 手报,消火栓起泵按钮,CDI处理方式和探测器相同：火警恢复后不做其它处理了。
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
                // 其它模块可以反复上报火警、故障
                if (0xf0 & ALLDevices[LN][device_Addr].Comd)  // 已经开始重新扫描
                {
                    if((ALLDevices[LN][device_Addr].ReScanCnt & 0x0f) < 0x0f) 
                    {
                        ALLDevices[LN][device_Addr].ReScanCnt++;  // 低四位加
                    }

                    if ((ALLDevices[LN][device_Addr].ReScanCnt & 0x0f) > 
                        ( FFRecover_RESCAN_TIMES - RESCAN_ALLOW_ERR_TIMES)) 
                    {
                    	//只允许有规定次错误，输入节点事件恢复，上报并置相关标志位
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
                                SetBit(ALLDevices[LN][device_Addr].ReScanStat5, FireRecover);// 报火警恢复
                                
                                //LoopSendEvent2Main( 0x40, 5, 0x3A, LN, device_Addr, 0); 

								if (DEVICE_IO_TYPE[ALLDevices[LN][device_Addr].DeviceType] == 0x01 ||	//报反馈取消
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
//		                    can_send(1, dev_addr + LN, 0, 7, send_buff);		// 报故障恢复
							
							ALLDevices[LN][device_Addr].ReScanStat2 = 0;
                        }

                        ALLDevices[LN][device_Addr].ReScanCnt &= 0xf0;
                        if ((ALLDevices[LN][device_Addr].DeviceType == DEVICE_RELAY_MODULE)||   		// CDI模块和探测器一样处理
                            (ALLDevices[LN][device_Addr].DeviceType== DEVICE_FIRE_COCK_BUTTON)||   		// 消火栓起泵按钮和探测器一样处理
                            (ALLDevices[LN][device_Addr].DeviceType == DEVICE_HAND_FIRE_DETECTOR))   	// 手报和探测器一样处理
                        {
                            ALLDevices[LN][device_Addr].ReScanStat1 &= (0xf0|FireRecover); // 不要影响输出节点
                            ALLDevices[LN][device_Addr].ReScanStat2 &= (0xf0|FireRecover); // 不要影响输出节点
                        } 
                        else 
                        {
                            ALLDevices[LN][device_Addr].ReScanStat1 &= 0xf0; // 不要影响输出节点
                            ALLDevices[LN][device_Addr].ReScanStat2 &= 0xf0; // 不要影响输出节点
                        }
                        ALLDevices[LN][device_Addr].Comd = XJCMD(LN, device_Addr); //此处与探测器不同
                        return;
                    }
                }
                else 
                {
                    //070426晚于沈阳所开始修改，丢失前回答，恢复后先报回答撤消后又报回答
                    if (ALLDevices[LN][device_Addr].OtherFlags.FlagBit.DisLoseErrAns_flag)
					{
                        return;
                    } 
					else 
					{
                        ALLDevices[LN][device_Addr].Comd += (FFRecover_RESCAN_TIMES << 4);
                        ALLDevices[LN][device_Addr].ReScanCnt &= 0xf0;
                        ALLDevices[LN][device_Addr].ReScanCnt += 1; //本次就算一次
                    }
                    //修改结束
                }
            }
            else 
            {
            // 只是置了首次事件标志，或者器件正常
                ALLDevices[LN][device_Addr].ReScanStat1 &= 0xf0;//不要影响输出节点
                if (1 == pw2) 
                {
                    ALLDevices[LN][device_Addr].Comd &= 0xf0;   //输入正常不等于输出正常
                    ALLDevices[LN][device_Addr].Comd += XJCMD(LN, device_Addr);
                }
            }
        }
        else if (4 == pw1)  // 输入接点火警
        {
            if( ALLDevices[LN][device_Addr].ReScanStat5 & FireRecover ) 
            {
                // 手报,CDI处理方式和探测器相同：火警恢复后不做其它处理了。
                if ((ALLDevices[LN][device_Addr].DeviceType == DEVICE_HAND_FIRE_DETECTOR)||
                    (ALLDevices[LN][device_Addr].DeviceType == DEVICE_FIRE_COCK_BUTTON)||
                    (ALLDevices[LN][device_Addr].DeviceType == DEVICE_RELAY_MODULE)) 
                {
                     return;
                }
            }
            AnsCodeRight_flag = 0;
            StateTempSave = FireStat; // 首火警和火警确认都位于BIT0,此处单写是为了避免歧义
        } 
        else  // 输入接点各种故障
        {
            AnsCodeRight_flag = 0;
            if (2 == pw1)     // 输入接点开路
            {    
                StateTempSave = InOpenStat;
            } 
            else if (3 == pw1)  // 输入接点短路
            {
                StateTempSave = DustOrInShortStat;
            } 
            else              // 模块故障
            {   
                StateTempSave = FaultStat;
            }
        }
        /*************开始处理火警或故障相关*************/
        if (!AnsCodeRight_flag )  // 模块输入节点有事件则在此处集中处理
        {
            if (StateTempSave & ALLDevices[LN][device_Addr].ReScanStat2) 
            {
            //输入节点事件已经确认了,不做处理
                return;
            } 
            else if (StateTempSave & ALLDevices[LN][device_Addr].ReScanStat1) 
            {
            // 已确认输入节点首事件
                // 此处确认火警或故障并上报,然后置相应的标志
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

                    ALLDevices[LN][device_Addr].Comd = 0x2A;	 // 报火警,并点灯                    
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
//	                    can_send(1, dev_addr + LN, 0, 7, send_buff); 		// 报故障,并标志出来

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
//						can_send(1, dev_addr + LN, 0, 7, send_buff);	// 报输入模块短路
						
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
//						can_send(1, dev_addr + LN, 0, 7, send_buff);// 报输入模块开路
						
	                    ALLDevices[LN][device_Addr].OtherFlags.FlagBit.CircuitERR_flag = 1;
                    }
                }
                //置确认位
                SetBit(ALLDevices[LN][device_Addr].ReScanStat2, StateTempSave);
            } 
            else 
            {
 	           //还没有确认首事件
                if (FireStat == StateTempSave) 
                {
                    tempVar = FIRE_RESCAN_TIMES;  // 火警重扫次数
                } 
                else 
                {
                    tempVar = FAULT_RESCAN_TIMES; // 故障重扫次数
                }

                if (0 == (0xf0 & ALLDevices[LN][device_Addr].Comd)) 
                {
                // 第一次探测到有事件,准备多次重复扫描
                    ALLDevices[LN][device_Addr].Comd += (tempVar << 4);
                    ALLDevices[LN][device_Addr].ReScanCnt &= 0xf0;
                    ALLDevices[LN][device_Addr].ReScanCnt += 1; // 本次就算一次
                }
                else 
                {
                    if( (ALLDevices[LN][device_Addr].ReScanCnt & 0x0f) < 0x0f ) 
                    {
                        ALLDevices[LN][device_Addr].ReScanCnt++;
                    }

                    if ((ALLDevices[LN][device_Addr].ReScanCnt & 0x0f) >= (tempVar - RESCAN_ALLOW_ERR_TIMES)) 
                    { // 只允许有规定次错误
                        SetBit(ALLDevices[LN][device_Addr].ReScanStat1, StateTempSave);
                        ALLDevices[LN][device_Addr].Comd &= 0x0f;
                        LOOP[LN].NeedCheckAddr = device_Addr;
                    }
                }
            }//模块输入节点是否已经确认了事件及其处理--完成
        }//模块输入节点有事件处理--完成
        /****************************************************************************
	        *                         模块输入节点处理完成                              *
	        *        以下开始处理模块输出节点                                           *
	        ****************************************************************************/
        if (1 == pw2) // 输出接点正常
        {// PW2 输出接点状态
            AnsCodeRight_flag = 1; // 正常
            tempVar = ALLDevices[LN][device_Addr].ReScanStat2 & 0x70; // 屏蔽输入节点标志

            if (0 != tempVar)  // 输出节点曾经有火警故障等事件确认
            {
                if (0xf0 & ALLDevices[LN][device_Addr].Comd) 
				{
                	// 已经开始重新扫描
                    if((ALLDevices[LN][device_Addr].ReScanCnt & 0xf0) < 0xf0) 
                    {
                        ALLDevices[LN][device_Addr].ReScanCnt += 0x10; //高四位加
                    }

                    if (((ALLDevices[LN][device_Addr].ReScanCnt & 0xf0) >> 4)
                        > (FFRecover_RESCAN_TIMES - RESCAN_ALLOW_ERR_TIMES)) 
                    {
                    //只允许有规定次错误
                        //输出节点事件恢复，此处添加IIC上报代码
                        if (ALLDevices[LN][device_Addr].ReScanStat2 & DevIsOutFire) 
                        {
                            SetBit(ALLDevices[LN][device_Addr].ReScanStat5, FireRecover);

                            //LoopSendEvent2Main( 0x40, 5, 0x3A, LN, device_Addr, 0); // 报火警恢复

							if (DEVICE_IO_TYPE[ALLDevices[LN][device_Addr].DeviceType] == 0x01 ||	//报反馈取消
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
//		                    can_send(1, dev_addr + LN, 0, 7, send_buff); 		 // 报故障恢复
							
							ALLDevices[LN][device_Addr].ReScanStat2 = 0;
                        }
                        ALLDevices[LN][device_Addr].ReScanCnt   &= 0x0f;
                        ALLDevices[LN][device_Addr].ReScanStat1 &= 0x0f; // 不要影响输入节点
                        ALLDevices[LN][device_Addr].ReScanStat2 &= 0x0f; // 不要影响输入节点
                        ALLDevices[LN][device_Addr].Comd = XJCMD(LN, device_Addr); // 此处与探测器不同
                        return;
                    }
                }
                else 
                {
                    ALLDevices[LN][device_Addr].Comd += (FFRecover_RESCAN_TIMES << 4);
                    ALLDevices[LN][device_Addr].ReScanCnt &= 0x0f;
                    ALLDevices[LN][device_Addr].ReScanCnt += 0x10; // 本次就算一次
                }
            } 
			else 
			{
            	//只是置了首次事件标志，或者器件正常
                ALLDevices[LN][device_Addr].ReScanStat1 &= 0x0f; // 不要影响输入节点
                if (1 == pw1) 
				{
                    ALLDevices[LN][device_Addr].Comd &= 0xf0;    // 输出正常不等于输入正常
                    ALLDevices[LN][device_Addr].Comd += XJCMD(LN, device_Addr);
                }
            }
        }
        else if (4 == pw2)      // 输出接点火警
        {
            AnsCodeRight_flag = 0;
            StateTempSave = OutFireStat;
        } 
        else                     // 输出节点故障
        {
            AnsCodeRight_flag = 0;
            
            if (2 == pw2)        // 输出接点开路 
            {
                StateTempSave = OutOpenStat;
            } 
            else if (3 == pw2)  // 输出接点短路
            {
                StateTempSave = OutShortStat;
            } 
            else                // 输出接点动作另外单独处理
            {
                return;
            }
        }
        /*************开始处理火警或故障相关*************/
        if (!AnsCodeRight_flag)  // 模块输出节点有事件则在此处集中处理
        {
            if (StateTempSave & ALLDevices[LN][device_Addr].ReScanStat2) 
            { // 输出节点事件已经确认了，不做处理           
                return;
            } 
            else if (StateTempSave & ALLDevices[LN][device_Addr].ReScanStat1) 
            {
            	// 已确认输出节点首事件
                // 此处确认火警或故障并上报,然后置相应的标志
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

                    ALLDevices[LN][device_Addr].Comd = 0x2A ;// 报火警,发火警确认命令   
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
//						can_send(1, dev_addr + LN, 0, 7, send_buff); // 报输出模块短路
						
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
//						can_send(1, dev_addr + LN, 0, 7, send_buff);// 报输出模块开路
						
						ALLDevices[LN][device_Addr].OtherFlags.FlagBit.CircuitERR_flag = 1;
                	}
                }

                SetBit(ALLDevices[LN][device_Addr].ReScanStat2, StateTempSave);// 置确认位
            }
            else 
            {
            	//还没有确认首事件
                if (OutFireStat == StateTempSave) 
                {
                    tempVar = FIRE_RESCAN_TIMES;  // 火警重扫次数
                } 
                else 
                {
                    tempVar = FAULT_RESCAN_TIMES; // 故障重扫次数
                }

                if (0 == (0xf0 & ALLDevices[LN][device_Addr].Comd)) 
                {
                	// 第一次探测到有事件,准备多次重复扫描
                    ALLDevices[LN][device_Addr].Comd += ( tempVar <<4 );
                    ALLDevices[LN][device_Addr].ReScanCnt &= 0x0f;
                    ALLDevices[LN][device_Addr].ReScanCnt += 0x10; // 本次就算一次
                } 
                else 
                {
                    if ((ALLDevices[LN][device_Addr].ReScanCnt & 0xf0) < 0xf0) 
                    {
                        ALLDevices[LN][device_Addr].ReScanCnt += 0x10;
                    }

                    if (((ALLDevices[LN][device_Addr].ReScanCnt & 0xf0) >> 4)
                        >= ( tempVar - RESCAN_ALLOW_ERR_TIMES ) ) 
                    { // 只允许有规定次错误
                        SetBit(ALLDevices[LN][device_Addr].ReScanStat1, StateTempSave);
                        ALLDevices[LN][device_Addr].Comd &= 0x0f;
                        LOOP[LN].NeedCheckAddr = device_Addr; // 070413
                    }
                }
            }//模块输出节点是否已经确认了事件及其处理--完成
        }//模块输出节点有事件处理--完成
    }//该器件为模块的处理--完成

    return;
}

/*
********************************************************************************
* 函数名称: static void DealLoopOtherFault( u8 LN, u8 device_Addr, u8 fault_Type );
* 函数功能: 回路器件其他故障状态处理,这些状态是从探测器或模块状态回码中查不出来的，
*           包括无响应，重码，类型错误等,在多次大循环中扫描，每次循环只扫一次。
* 参    数: u8 LN          : 回路号0或(0，1)
*           u8 device_Addr : 器件号(1~255)
*           u8 fault_Type  : 故障类型(见LoopOpt.h)
* 返 回 值: 无
* 注    意: GK603改造过程中修改的回路报丢方式不同，以前是计次，定时不准确，现在
*           改为可编译前配置报丢时间，丢失后延时期间不恢复就报丢。
********************************************************************************
*/
static void DealLoopOtherFault( u8 LN, u8 device_Addr, u8 fault_Type )
{
    u32 temp;

    if (AutoRegister_flag)
    	return;  // 自动登录其间不处理事件
    /*******************************无响应处理*********************************/
    if (NoRespStat == fault_Type) 
    {
        if (ALLDevices[LN][device_Addr].ReScanStat3 & NoRespStat) 
        {	
        	// 先前已经检测到丢失			
            if( ALLDevices[LN][device_Addr].ReScanStat4 & NoRespStat) 
            {
                return; // 已经确认并上报丢失，直接退出即可
            }

            temp = GetTickCount();
            if ((temp - ALLDevices[LN][device_Addr].LoseCntTicks) >
                ((LOSE_REPORT_TIME * 1000) / MSPERTICK))  // 丢失时间达到规定值
            {
                // 报丢，并设置丢失确认位
                if ( 1 ) 	// 单个报丢
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
                    ClrBit(ALLDevices[LN][device_Addr].ReScanStat3, NoRespRcvStat); // 有其他故障的时候也可以重复报丢失恢复
                    ClrBit(ALLDevices[LN][device_Addr].ReScanStat4, NoRespRcvStat);
                    ALLDevices[LN][device_Addr].OtherFlags.FlagBit.DisLoseErrAns_flag = 1;
                    
                    return;
                }
            }
        }
        else 
        {
        	// 第一次检测到丢失，置标志位并保存丢失时间
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

    /****************************其它情况处理**********************************/
    if (ALLDevices[LN][device_Addr].ReScanStat3 & fault_Type) 
    { 	// 上次就是这个故障
        if (ALLDevices[LN][device_Addr].ReScanStat4 & fault_Type) 
        {
        	// 该故障已经被确认并作了处理
            // 是否报过无响应，如果报过，需要使他报无响应恢复
            if (NoRespStat & ALLDevices[LN][device_Addr].ReScanStat4) 
            { // 有其他故障的时候也可以重复报丢失恢复
                if (ALLDevices[LN][device_Addr].ReScanStat3 & NoRespRcvStat) 
                {
                    if (ALLDevices[LN][device_Addr].ReScanStat4 & NoRespRcvStat) 
                    {
                        ALLDevices[LN][device_Addr].OtherFlags.FlagBit.DisLoseErrAns_flag = 0;
                        return; // 已经报过无响应恢复
                    } 
                    else 
                    {
                        ALLDevices[LN][device_Addr].OtherFauCnt ++;
                        if (ALLDevices[LN][device_Addr].OtherFauCnt >= DA_TE_CNT_TIMES)
                        {
                            //报无响应恢复并置对应标志位
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
                    ALLDevices[LN][device_Addr].OtherFauCnt = 0;  // 然后重新计数
                }
            }

            return;  // 已经确认并上报了这个异常了，不做处理
        }

        if (ALLDevices[LN][device_Addr].OtherFauCnt >= DA_TE_CNT_TIMES) 
        {
            if (NoOtherFauStat == fault_Type )  // 多次扫描没有故障
            {
                if (NoRespStat & ALLDevices[LN][device_Addr].ReScanStat4) 
                {
                    //报无响应恢复并置对应标志位           				
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
                // 如果愿意也可以此处添加其他故障类型恢复的事件上报
                ALLDevices[LN][device_Addr].ReScanStat3 = NoOtherFauStat;
                // ReScanStat4清除某故障确认位则该故障可以反复上报
            } 
            else if (DupAddrStat == fault_Type) 
            {
                // 报地址重码并置对应标志位
                SetBit(ALLDevices[LN][device_Addr].ReScanStat4, fault_Type);
            } 
            else if (TypeErrStat == fault_Type) 
            {
                // 报类型错误并置对应标志位					
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
                // 报发送故障并置对应标志位					
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
            ALLDevices[LN][device_Addr].OtherFauCnt++;  // 异常计数器累加
        }
    }
    else 
    { 	
    	// 新的故障
        ALLDevices[LN][device_Addr].ReScanStat3 = 0;
        SetBit( ALLDevices[LN][device_Addr].ReScanStat3, fault_Type );
        ALLDevices[LN][device_Addr].OtherFauCnt = 0;     // 然后重新计数
    }

    return;
}

/*
********************************************************************************
* 函数名称: void AnsCodeDeal(u8 LN, u8 device_Addr);
* 函数功能: 回路回码数据处理
* 参    数: u8 LN          : 回路号0 或 (0,1)
*           u8 device_Addr : 器件号1~255
* 返 回 值: 无
* 注    意: 为了加快处理速度，0us = 0; 300us = 1; 600us = 2; 900us = 3,1200us = 4,1500us = 5
********************************************************************************
*/
void AnsCodeDeal(u8 LN, u8 device_Addr)
{
    u8 	 acodes[5];    //将用于存放经过处理后的 answer codes
    u32  temp, i;
    BOOL DevIsOldMod; //如果是老模块要置位，类型检查时才使用，其他时候这个变量没有用
    
    memset(acodes, 0, 5);
    
    /**************************回码状态及错误处理 *****************************/
    i = 0; // 借用一下，用于指示错误数量

    if (LOOP[LN].OptStatFlags.StateBit.SendAddr0_flag || 
        (0 == device_Addr) ||
        LOOP[LN].OptStatFlags.StateBit.OldJmpLedSolution_flag ||  
        LOOP[LN].OptStatFlags.StateBit.InterruptByBroadCastCmd_flag) 
    {
        return; // 不处理回码的情况：回路发0码、处理旧器件跳转时闪灯时间长、发显示盘的广播命令
    } 
    else if (0 == LOOP[LN].PWn[0])  // 器件无响应，以下是几种无响应的情况
    {  
    	// LoopCard_LED( LN, LEDY, LED_ON);
        // BOOL变量借用，因为条件表达式太长，
        DevIsOldMod = ((0x06 == (ALLDevices[LN][device_Addr].Comd & 0x0f)) ||
                       (0x07 == (ALLDevices[LN][device_Addr].Comd & 0x0f)) ||
                       (0x08 == (ALLDevices[LN][device_Addr].Comd & 0x0f)) ||
                       (0x09 == (ALLDevices[LN][device_Addr].Comd & 0x0f)));
        // 发以上几个命令时、自动登陆时、上电关模块时不回码不作任何处理
        if ((DevIsOldMod && LOOP[LN].OptStatFlags.StateBit.RstCloseModu_flag) || AutoRegister_flag) 
        {
        } 
        else if (ALLDevices[LN][device_Addr].OtherFlags.FlagBit.CircuitERR_flag) 
        {
            //器件故障只处理正常巡检命令的回码
            if ((0x00 == (0x0f & ALLDevices[LN][device_Addr].Comd)) ||
                (0x0d == (0x0f & ALLDevices[LN][device_Addr].Comd))) 
            {
                DealLoopOtherFault(LN, device_Addr, NoRespStat);
            } 
            else 
            {
                ALLDevices[LN][device_Addr].Comd = 0x00;
                LDList[LN][device_Addr]          = 0; // 防止反复启动一个发生电路故障的模块
            }

            return;
        } 
        else   //其他情况都要做无响应处理
        {
            DealLoopOtherFault(LN, device_Addr, NoRespStat);
            return;
        }
    } 
    else if ((0 != LOOP[LN].LoopAnsStat) && 	// 操作时出错(超时)
             (0x05 != (0x0f & ALLDevices[LN][device_Addr].Comd)))
    {
        // 类型测试时可能因为新旧器件的不同出现最后的回码超时此时不能认为是错误。
        if (!AutoRegister_flag) 
        {
            DealLoopOtherFault(LN, device_Addr, SendErrStat);      //发送故障
            return;
        }
    } 
    else  // 无以上错误
    {
        i++;
    }

    if (ALLDevices[LN][device_Addr].OtherFlags.FlagBit.IsDuAddr_flag)  // 回路器件重码
    {
        ALLDevices[LN][device_Addr].OtherFlags.FlagBit.IsDuAddr_flag = 0;
        DealLoopOtherFault(LN, device_Addr, DupAddrStat);      // 重码处理
    } 
    else  // 无以上错误
    {
        i++;
    }

    if (2 == i) // 没有以上任何错误
    {
        // 只有不是类型检测时才能认为无错了,因为类型检测在这句后面
        if (0x05 != (ALLDevices[LN][device_Addr].Comd & 0x0f)) 
        {
            //无故障和无响应恢复上报，其他故障恢复不上报
            DealLoopOtherFault(LN, device_Addr, NoOtherFauStat);
        }
    }

    /********************************回码正常处理 *****************************/
    //第一步：脉宽调整
    if (LOOP[LN].PWn[0] > TIME_600US) 
    {
    	temp = TIME_900US; 					// PW0大于600us为900us脉宽
    }
    else                                    // PW0小于600us为300us脉宽，老器件
    { 
        temp = TIME_300US;
        ALLDevices[LN][device_Addr].OtherFlags.FlagBit.IsOldDev_flag = 1; //是老器件，置一个标志位
    }
    
    for (i = 1; i < 5; i++) 				// 根据PW0补偿每一个回码
    {
        if (LOOP[LN].PWn[i] > 0)
        	LOOP[LN].PWn[i] = (LOOP[LN].PWn[i] + temp) - LOOP[LN].PWn[0];
        else    
        	break;
    }
    
    LOOP[LN].PWn[0] = temp;

    /**************************************************************************/
    //第二步：得出正确回码信息
    for (i = 0; i < 5; i++)
    {
        temp = LOOP[LN].PWn[i];
        
        if (temp > 0) 
        {
            if ((2 == i) &&  
                ((0x0f & ALLDevices[LN][device_Addr].Comd) < 0x04) &&
                is_detector(ALLDevices[LN][device_Addr].DeviceType))		//探测器
            {
                //命令0没有这一位，命令1，2，3的这一位是模拟量值
            	if (temp >= TIME_1758US) // 最大计数值
            	{
            	    acodes[2] = 200;
            	}
            	else //计数值TC1758US_CNT时对应200，那么小于TC1758US_CNT的值对应多少？此处求出结果
            	{
            	    temp = temp * 200 + TIME_1758US / 2;
            	    temp = temp / TIME_1758US;
            	    acodes[2] = (u8)temp;
            	    i = 8;
            	}
            } 
            else  // 非模拟量
            {
                if (((TIME_300US + TIME_150US) > temp) &&   //20180418修改
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
                    acodes[i] = 0; //回码宽度有问题
                }
            }//非模拟量处理结束
        }
        else 
        {
            break; 
        }
    }

    /**************************************************************************/
    //第三步：根据不同的命令处理回码信息
    /*******************************
    *    类型测试命令单独处理      *
    *******************************/
    if (0x05 == (0x0f & ALLDevices[LN][device_Addr].Comd)) 
    {
        if (((device_Addr < 128) && (1 == acodes[0])) || //探测器
            ((device_Addr > 127) && (3 == acodes[0]))) 
        { 
            i = DetTypePW3PW2[acodes[3]][acodes[2]]; //注意无响应时i为0xff
        }
        else									  //模块
        {
            if (3 == acodes[0]) 
            {
                DevIsOldMod = 0;
            } 
            else 
            {
                DevIsOldMod = 1;
            }
            
            i = ModTypePW4PW3[acodes[4]][acodes[3]];//注意无响应时i为0xff
        }
        // 以上获取器件类型，下面判断类型是否与初始化类型相同
        if (i != ALLDevices[LN][device_Addr].DeviceType) // 类型与初始化类型不同的处理
        {
            if (AutoRegister_flag) 		// 自动登陆，记录器件类型
            {
                if (((NM_SB == ALLDevices[LN][device_Addr].DeviceType) || (NM_QB == ALLDevices[LN][device_Addr].DeviceType)) && 
                    ((NM_SR == i) && DevIsOldMod)) /*考虑新老兼容，旧的手报和启泵按钮类型为0x10时不要覆盖先前Type里的配置*/
                {
                	return;
                }
                
                ALLDevices[LN][device_Addr].Comd = 0x15;  // 再来一次类型扫描

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
                	ALLDevices[LN][device_Addr].DeviceType      = i;     // 保存本次结果,无响应也一样处理
                }
                	
                return;
            } 
            else 
            {
            	//器件类型检查错误处理，只报类型错误但是不覆盖原来的类型
                if (0xff == i) 														// 此处没有器件或类型未知
                { 
                    ALLDevices[LN][device_Addr].Comd = XJCMD(LN, device_Addr); 		//这种情况扫一次就得了
                    return;
                }
                if (is_detector(ALLDevices[LN][device_Addr].DeviceType) || is_detector(i))  // 探测器类
                {
                    DealLoopOtherFault( LN, device_Addr, TypeErrStat);
                    return;   //类型都错误了，没有必要进行其他回码处理了
                }
                else if ((NM_SB == ALLDevices[LN][device_Addr].DeviceType) || (NM_QB == ALLDevices[LN][device_Addr].DeviceType)) 
                {
                    if ((NM_SR == i) && DevIsOldMod) 
                    {
                    	/*考虑新老兼容，旧的手报和启泵按钮类型为0x10时不要报类型错误*/
                        ALLDevices[LN][device_Addr].Comd = XJCMD(LN, device_Addr); //这种情况扫一次就得了
                    } 
                    else 
                    {
                        DealLoopOtherFault(LN, device_Addr, TypeErrStat);
                        return;   //类型都错误了，没有必要进行其他回码处理了
                    }
                }
            }
            DevIsOldMod = 0;
        }
        else 
        { // 类型与初始化类型相同的处理
            if (!AutoRegister_flag) 
            {
                ALLDevices[LN][device_Addr].Comd = XJCMD(LN, device_Addr); //相等扫一次就得了
            }
        }

        if (AutoRegister_flag) 
        {
            if (255 == device_Addr)  // 当前回路自动登陆完成
            {
                LOOP[LN].OptStatFlags.StateBit.AutoRegFinish_flag = 1;
            }
            return;
        }
    }

    /*******************************
    *    类型测试命令处理完成      *
    *                              *
    *        处理其它命令          *
    *******************************/
    if (is_detector(ALLDevices[LN][device_Addr].DeviceType))   // 当前设备为探测器
    {
        //不同命令的处理
        DealPW12State(LN, device_Addr, acodes[1], 0xaa);        
    } 
    else 		 // 当前设备为模块,对它的不同命令的处理
    {
        switch (0x0f & ALLDevices[LN][device_Addr].Comd) 
        {
            case 0x09: // 模块动作选通命令
                if (((5 == acodes[2]) && ((1 == OutActType[LN]) || (2 == OutActType[LN]))) ||
                    ((1 == acodes[2]) && (3 == OutActType[LN]))) 
                {
                    //动作了的话就不发2遍确认了,这样加快速度
                    ClrBit(ALLDevices[LN][device_Addr].ReScanStat1, FirstOutActStat);
                    ClrBit(ALLDevices[LN][device_Addr].ReScanStat2, FirstOutNoAct);
                    if (1 == OutActType[LN])         // 输出动作类型为打开
                    {
                        SetBit(ALLDevices[LN][device_Addr].ReScanStat5, OutOpenAct);
                        ClrBit(ALLDevices[LN][device_Addr].ReScanStat5, OutCloseAct);
                        //报输出节点已动作
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
                    else if (3 == OutActType[LN])  // 输出动作类型为关闭
                    {
                        ClrBit(ALLDevices[LN][device_Addr].ReScanStat5, OutOpenAct );
                        SetBit(ALLDevices[LN][device_Addr].ReScanStat5, OutCloseAct );
                        // 不是复位时的强制关闭才上报关闭
                        if (!LOOP[LN].OptStatFlags.StateBit.RstCloseModu_flag ) 
                        {
                            // 报输出节点已动作
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
                    else if (2 == OutActType[LN])  // 脉冲方式动作
                    {
                        //报输出节点已动作
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
                        //开机后器件关闭完成后清标志准许模块停止事件上报
                        LOOP[LN].OptStatFlags.StateBit.RstCloseModu_flag = 0;
                    }

                    ALLDevices[LN][device_Addr].Comd = XJCMD(LN, device_Addr);
                }
                else 
                { //未动作重新发送,如果2次都未动作,报未动作
                    ALLDevices[LN][device_Addr].Comd = XJCMD(LN, device_Addr);
                    
                    if (FirstOutNoAct & ALLDevices[LN][device_Addr].ReScanStat2) 
                    { //第二次确定未动作
                        ClrBit(ALLDevices[LN][device_Addr].ReScanStat1, FirstOutActStat);
                        ClrBit(ALLDevices[LN][device_Addr].ReScanStat2, FirstOutNoAct);
                        //报输出节点未动作,拟取消
                        if (!LOOP[LN].OptStatFlags.StateBit.RstCloseModu_flag)  //不是复位时的强制关闭才上报
                        {
                            //LoopSendEvent2Main( 0x40, 5, 0x32, LN, device_Addr, 0 );
                        }

                        if (-1 == LDNode[LN]) 
                        {
                            LOOP[LN].OptStatFlags.StateBit.HaveLDDev_flag    = 0;
                            //开机后器件关闭完成后清标志准许模块停止事件上报
                            LOOP[LN].OptStatFlags.StateBit.RstCloseModu_flag = 0;
                        }
                        return;
                    }

					ALLDevices[LN][device_Addr].ReScanStat1 &= ~FirstOutActStat;
					ALLDevices[LN][device_Addr].ReScanStat2 |= FirstOutNoAct;

                    if (1 == OutActType[LN])//输出动作类型为打开
                        ALLDevices[LN][device_Addr].Comd = 0x36; //重扫
                    else if (3 == OutActType[LN]) //类型为关闭
                        ALLDevices[LN][device_Addr].Comd = 0x37; //重扫
                    else if (2 == OutActType[LN]) //类型为脉冲
                        ALLDevices[LN][device_Addr].Comd = 0x38; //重扫
                }
                return;
            case 0x06: //通过命令打开输出模块、输入输出模块
                OutActType[LN] = 1;
                if (ALLDevices[LN][device_Addr].Comd <= 0x16) 
                {
                    ALLDevices[LN][device_Addr].Comd = 0x19; //还要执行1个09命令
                }
                return;
            case 0x07: //通过命令关闭输出模块、输入输出模块
                OutActType[LN] = 3;
                if (ALLDevices[LN][device_Addr].Comd <= 0x17) 
                {
                    ALLDevices[LN][device_Addr].Comd = 0x19; //还要执行1个09命令
                }
                return;
            case 0x08: //通过命令控制模块处于脉冲输出方式
                OutActType[LN] = 2;
                if (ALLDevices[LN][device_Addr].Comd <= 0x18) 
                {
                    ALLDevices[LN][device_Addr].Comd = 0x19; //还要执行1个09命令
                }
                return;
            case 0x05: //类型测试
            case 0x00: //控制器对模块进行巡检，用来实现对模块的正常监控
            case 0x0d: //控制器对模块进行灭灯巡检
            default:
                DealPW12State( LN, device_Addr, acodes[1], acodes[2] );
                break;
        }
    }//模块的处理完成
    //第三步结束
    return;
}

/*
********************************************************************************
* 函数名称: void AutoRegFinished();
* 函数功能: 回路自动登陆完成后，重新初始化扫描链表，数据上传。
* 参    数: 无
* 返 回 值: 无
* 注    意:
********************************************************************************
*/
void AutoRegFinished(void)
{
//	int i;
	
    LoopListInit( 0 );  // 重新初始化扫描链表
    LoopListInit( 1 );  // 重新初始化扫描链表

	//将不存在的回路设备清除
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

