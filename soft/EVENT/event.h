#ifndef _EVENT_H_
#define _EVENT_H_
#include "stdint.h"
#include "datastruct.h"
#include "include.h"
#ifdef EVENT_PRIVATE
	#define EVENT_PUBLIC     
#else
	#define EVENT_PUBLIC extern
#endif

//extern uint8_t bat_flag;
extern volatile  uint8_t power_flag;    
    
    
#define EVENT_ALL_COUNT   4096 //最大事件存储数量
#define EVENT_ALARM_COUNT 128   // 最大火警存储数量
#define EVENT_FAULT_COUNT 512   //最大故障存储数量

#define EVENT_ALL_COUNTRAM 32 //事件在RAM中最大存储数量
#define EVENT_ALL_ALARM_FAULT      16
extern  const char event__type[23][16];

//事件类型
#define EVENT_INVALID_CODE		 				 0xff                  
#define EVENT_NORMAL                             1    //正常                  
#define EVENT_FIRE                               2    //火警                  
#define EVENT_ELECTRICAL_FIRE_ALARM              3    //电气火灾报警          
#define EVENT_COMBUSTIBLE_GAS_LOV_ALARM          4    //可燃气体低限报警      
#define EVENT_COMBUSTIBLE_GAS_HIGH_ALARM         5    //可燃气体高限报警      
#define EVENT_COMBUSTIBLE_GAS_EXCESS_ALARM       6    //可燃气体超量程报警    
#define EVENT_ELECTRICAL_FIRE_EARLYWARNING       7    //电气火灾预警          
//#define EVENT_                                 8    //预留                  
//#define EVENT_                                 9    //预留                  
//#define EVENT_                                 10   //预留                  
//#define EVENT_                                 11   //预留                  
//#define EVENT_                                 12   //预留                  
//#define EVENT_                                 13   //预留                  
//#define EVENT_                                 14   //预留                  
//#define EVENT_                                 15   //预留                  
//#define EVENT_                                 16   //预留                  
//#define EVENT_                                 17   //预留                  
//#define EVENT_                                 18   //预留                  
//#define EVENT_                                 19   //预留                  
#define EVENT_START                              20   //启动                
#define EVENT_AUTO_START                         21   //自动启动             
#define EVENT_MANUL_START                        22   //手动启动              
#define EVENT_FIELD_EMERGENT_START               23   //现场急启              
#define EVENT_GAS_BEGAIN_DELAY                   24   //气体灭火开始延时      
#define EVENT_GAS_SPRAY                          25   //气体喷洒              
#define EVENT_FEEDBACK                           26   //反馈
#define EVENT_SPRAY_FEEDBACK                     27   //喷洒反馈              
#define EVENT_FEEDBACK_CANCLE                    28   //反馈撤销              
#define EVENT_STOP                               29   //停止
#define EVENT_FIELD_EMERGENT_STOP                30   //现场急停              
//#define EVENT_                                 31   //预留                  
//#define EVENT_                                 32   //预留                  
//#define EVENT_                                 33   //预留                  
//#define EVENT_                                 34   //预留                  
//#define EVENT_                                 35   //预留                  
//#define EVENT_                                 36   //预留                  
//#define EVENT_                                 37   //预留                  
//#define EVENT_                                 38   //预留                  
//#define EVENT_                                 39   //预留                  
#define EVENT_EMERGENCY                          40   //应急                  
#define EVENT_MONTHLY_TEST                       41   //月检                  
#define EVENT_ANNUAL                             42   //年检                  
#define EVENT_MASK_LAMPS_CHANGE_DIRECTION        43   //标志灯具改变方向      
#define EVENT_LIFT_DOWN                          44   //电梯迫降              
#define EVENT_SHUTTER_HALF_DOWN                  45   //卷帘半降              
#define EVENT_SHUTTER_ALL_DOWN                   46   //卷帘全降              
#define EVENT_TALK                               47   //呼叫                  
#define EVENT_CALL                               48   //通话      

#define EVENT_FIRE_EQUIPMENT_NO_POWER            49   //消防设备电源失电      
#define EVENT_FIRE_EQUIPMENT_UNDER_POWER         50   //消防设备电源欠压      
#define EVENT_FIRE_EQUIPMENT_OVER_POWER          51   //消防设备电源过压      
#define EVENT_FIRE_EQUIPMENT_POWER               52   //消防设备电源过载      
#define EVENT_FIRE_EQUIPMENT_OVERDRIVE_POWER     53   //消防设备电源缺相      
#define EVENT_FIRE_EQUIPMENT_MISPHASE_POWER      54   //消防设备电源错相      
#define EVENT_FIRE_WATER_TANK_LOW                55   //消防水箱（池）水位低  
#define EVENT_FIRE_LIFT_NON_USE                  56   //消防电梯停用          
//#define EVENT_                                 57   //预留                  
//#define EVENT_                                 58   //预留                  
//#define EVENT_                                 59   //预留                  
//#define EVENT_                                 60   //预留                  
//#define EVENT_                                 61   //预留                  
//#define EVENT_                                 62   //预留                  
//#define EVENT_                                 63   //预留                  
//#define EVENT_                                 64   //预留                  
//#define EVENT_                                 65   //预留                  
//#define EVENT_                                 66   //预留                  
//#define EVENT_                                 67   //预留                  
//#define EVENT_                                 68   //预留                  
//#define EVENT_                                 69   //预留                  
#define EVENT_SUPERVISE                          70   //监管                  
#define EVENT_SUPERVISE_REMOVE                   71   //监管解除              
#define EVENT_DISABLE                            72   //屏蔽                  
#define EVENT_DISABLE_REMOVE                     73   //屏蔽解除              
//#define EVENT_                                 74   //预留                  
//#define EVENT_                                 75   //预留                  
//#define EVENT_                                 76   //预留                  
//#define EVENT_                                 77   //预留                  
//#define EVENT_                                 78   //预留                  
//#define EVENT_                                 79   //预留                  
#define EVENT_FAULT                            80   //故障                  
#define EVENT_COMMUNICATION_FAULT              81   //通讯故障              
#define EVENT_POWER_FAULT                      82   //主电故障              
#define EVENT_BATTERY_FAULT                    83   //备电故障              
#define EVENT_CHARGE_FAULT                     84   //充电故障              
#define EVENT_LOOP_FAULT                       85   //回路故障              
#define EVENT_DEVICE_FAULT                     86   //部件故障              
#define EVENT_LINE_FAULT                       87   //线路故障              
#define EVENT_GROUND_FAULT                     88   //接地故障              
#define EVENT_NC_FILR_DOOR_OPEN                89   //常闭防火门打开        
#define EVENT_NO_FILR_DOOR_CLOSE               90   //常开防火门关闭        
#define EVENT_PRINTER_FAULT                    91   //打印机故障                 
#define EVENT_SOUND_LIGHT_FAULT                92   //声光故障                  
#define EVENT_BATTERY_LOWER                    93   //备电欠压                  
#define EVENT_POWER_LOWER                      94   //主电欠压
#define EVENT_OVERCURRENT                      95   //过流  
#define EVENT_OVERCURRENT_PROTECT              96   //过流保护
#define EVENT_SPRAY_START_FAULT                97   //喷洒启动连线故障           
#define EVENT_SPRAY_START_SHORT                98   //喷洒启动连线短路 
#define EVENT_SPRAY_START_OPEN          	   99   //喷洒启动连线开路    
#define EVENT_SPRAY_ANSWER_FAULT               100  //喷洒回答连线故障   
#define EVENT_SPRAY_ANSWER_SHORT               101  //喷洒回答连线短路
#define EVENT_SPRAY_ANSWER_OPEN          	   102  //喷洒回答连线开路
//#define EVENT_                                 103  //预留                  
//#define EVENT_                                 104  //预留                  
//#define EVENT_                                 105  //预留  
//#define EVENT_                                 106  //预留                  
//#define EVENT_                                 107  //预留                  
//#define EVENT_                                 108  //预留  
#define EVENT_FAULT_RECOVERY                   109  //故障恢复              
#define EVENT_COMMUNICATION_FAULT_RECOVERY     110  //通讯故障恢复          
#define EVENT_POWER_FAULT_RECOVERY             111  //主电故障恢复          
#define EVENT_BATTERY_FAULT_RECOVERY           112  //备电故障恢复          
#define EVENT_CHARGE_FAULT_RECOVERY            113  //充电故障恢复          
#define EVENT_LOOP_FAULT_RECOVERY              114  //回路故障恢复          
#define EVENT_DEVICE_FAULT_RECOVERY            115  //部件故障恢复          
#define EVENT_LINE_FAULT_RECOVERY              116  //线路故障恢复          
#define EVENT_GROUND_FAULT_RECOVERY            117  //接地故障恢复          
#define EVENT_NC_FILR_DOOR_RE_CLOSE            118  //常闭防火门恢复关闭状态
#define EVENT_NO_FILR_DOOR_RE_OPEN             119  //常开防火门恢复开门状态
#define EVENT_PRINTER_FAULT_RECOVERY           120  //打印机故障恢复                
#define EVENT_SOUND_LIGHT_FAULT_RECOVERY       121  //声光故障恢复                   
#define EVENT_BATTERY_LOWER_RECOVERY           122  //备电欠压恢复            
#define EVENT_POWER_LOWER_RECOVERY             123  //主电欠压恢复  
#define EVENT_OVERCURRENT_RECOVERY             124  //过流恢复
#define EVENT_OVERCURRENT_PROTECT_RECOVERY     125  //过流保护恢复
#define EVENT_SPRAY_START_FAULT_RECOVERY       126  //喷洒启动连线故障恢复           
#define EVENT_SPRAY_START_SHORT_RECOVERY       127  //喷洒启动连线短路恢复 
#define EVENT_SPRAY_START_OPEN_RECOVERY        128  //喷洒启动连线开路恢复    
#define EVENT_SPRAY_ANSWER_FAULT_RECOVERY      129  //喷洒回答连线故障恢复   
#define EVENT_SPRAY_ANSWER_SHORT_RECOVERY      130  //喷洒回答连线短路恢复
#define EVENT_SPRAY_ANSWER_OPEN_RECOVERY       131  //喷洒回答连线开路恢复
//#define EVENT_                                 132  //预留                  
//#define EVENT_                                 133  //预留                  
//#define EVENT_                                 134  //预留            
//#define EVENT_                                 135  //预留                  
//#define EVENT_                                 136  //预留                  
//#define EVENT_                                 137  //预留   
#define EVENT_POWER_ON                         138  //开机                  
#define EVENT_POWER_OFF                        139  //关机                  
#define EVENT_RESET                            140  //复位                  
#define EVENT_SELF_TEST                        141  //自检                  
#define EVENT_SELF_TEST_FAILURE                142  //自检失败              
#define EVENT_MANULE_CONDITION                 143  //手动状态              
#define EVENT_AUTO_CONDITION                   144  //自动状态              
#define EVENT_SILENCE                          145  //确认/消音 


EVENT_PUBLIC SYSTEM_EVENT EventRamBuffer[EVENT_ALL_COUNTRAM];//事件在RAM中的存储
EVENT_PUBLIC uint8_t EventRamType[EVENT_ALL_COUNTRAM];//事件类型在RAM中的存储
//EVENT_PUBLIC U8 EventRecordBuffer[EVENT_ALL_COUNTRAM];//事件查询
EVENT_PUBLIC SYSTEM_EVENT EventFireRamBuffer[EVENT_ALL_ALARM_FAULT];
EVENT_PUBLIC SYSTEM_EVENT EventFaultRamBuffer[EVENT_ALL_ALARM_FAULT];//

//-----------------------------------------------------------------------------
EVENT_PUBLIC uint16_t EventAlarmCount;       //报警总数
EVENT_PUBLIC uint16_t EventFaultCount;       //故障总数
EVENT_PUBLIC uint16_t EventStartCount;       //启动总数
EVENT_PUBLIC uint16_t EventFeedBackCount;    //反馈总数


EVENT_PUBLIC U8 WinKeyTimer;//
EVENT_PUBLIC U8 EventSwitchTimer; //事件切换定时，30秒无按键，自动返回高优先级事件
//EVENT_PUBLIC U16 EventStartCount;       //Æô¶¯×ÜÊý
//EVENT_PUBLIC U16 EventFeedBackCount;    //·´À¡×ÜÊý
//EVENT_PUBLIC U16 LinkDelayCount;        //ÑÓÊ±×ÜÊý
EVENT_PUBLIC uint16_t EventFlashAlarmCurrentPointer;
EVENT_PUBLIC uint16_t  EventFlashCurrentPointer;
EVENT_PUBLIC uint16_t EventRamTaskPointer; //任务处理指针
EVENT_PUBLIC uint16_t EventRamCurrentPointer; //RAM记录指针
EVENT_PUBLIC uint16_t EventFireRamCurrentPointer;//火警指针
EVENT_PUBLIC uint16_t EventFaultRamCurrentPointer;//故障指针

EVENT_PUBLIC uint16_t EventFlashBasePointer;
EVENT_PUBLIC uint16_t EventFlashFaultBasePointer;
EVENT_PUBLIC uint16_t EventFlashFaultCurrentPointer;
EVENT_PUBLIC uint16_t EventFlashAlarmBasePointer;

static __inline uint16_t GetEventAlarmCount(void)
{
    return EventAlarmCount;
}
static __inline uint16_t GetEventFaultCount(void)
{
    return EventFaultCount;
}
static __inline uint16_t GetEventStartCount(void)
{
    return EventStartCount;
}
static __inline uint16_t GetEventFeedBackCount(void)
{
    return EventFeedBackCount;
}
static __inline uint16_t GetEventRamCurrentPointer(void)
{
    return EventRamCurrentPointer;
}

//CAN通讯优先级定义；V1.0    2015-8-3 16:18:06
//事件类型和优先级定义，包括CAN通讯发送的命令；事件类型决定事件处理的优先级，数字越低优先级越高。
//共16个优先级
//2016/2/19 13:07:46 启动、反馈、监管合并；增加巡检应答，定义其优先级最低
//2016/2/23 8:41:32  定义最高优先级，应答等使用最高优先级

#define T_PRIORITY_HIGHEST			    0 		//最高优先级，确认用此优先级
#define T_PRIORITY_RESERVE_2			1 		//
#define T_PRIORITY_RESERVE_3			2 		//
#define T_PRIORITY_TRANSFER_PAUSE	    3 		//传输暂停：接收方缓冲区已满80%，发送方暂停发送，等待继续发送命令
#define T_PRIORITY_PACKAGE_CONTINUE	    4 		//连续数据包：一组数据需要分包传送
#define T_PRIORITY_TRANSFER_CONTINUE	5 		//传输继续
#define T_PRIORITY_BROADCAST_CMD		6 		//广播命令：复位、消音等
#define T_PRIORITY_RESERVE_4			7 		//
#define T_PRIORITY_RESERVE_5			8 		//
#define T_PRIORITY_ALARM_EVENT		    9 		//报警事件
#define T_PRIORITY_ACT_CMD			    10		//启动命令
#define T_PRIORITY_ACT_FEEDBACK_EVENT   11		//启动、反馈事件、监管事件
#define T_PRIORITY_FAULT_EVENT    	    12		//故障事件
#define T_PRIORITY_OTHERS_EVENT		    13		//其他事件
#define T_PRIORITY_RESERVE_6  		    14		//
#define T_PRIORITY_LOWEST        	    15		//最低优先级，巡检应答用此优先级


//事件类型
#define EVENT_TYPE_ANY 			        0   //
#define EVENT_TYPE_ALARM 			    1   //报警事件
#define EVENT_TYPE_FAULT			    2   //故障
#define EVENT_TYPE_FAULT_RESUME	        3   //故障恢复
#define EVENT_TYPE_SYSTEM			    4   //系统
#define EVENT_TYPE_START			    5   //启动
#define EVENT_TYPE_STOP 			    6   //停止
#define EVENT_TYPE_FEEDBACK			    7   //反馈
#define EVENT_TYPE_FEEDBACK_CANCLE      8   //反馈撤销
#define EVENT_TYPE_DISABLE              9   //屏蔽
#define EVENT_TYPE_DISABLE_CANCLE       10  //屏蔽解除
#define EVENT_TYPE_SUPERVISE            11  //监管
#define EVENT_TYPE_SUPERVISE_CANCLE     12  //监管解除
#define EVENT_TYPE_RESET                13  //复位
#define EVENT_TYPE_KAIJI                14  //关机
#define EVENT_TYPE_GUANJI		        15	//开机
#define EVENT_TYPE_SILENCE				16	//消音
#define EVENT_TYPE_LOOP_FAULT           17  //回路短路
#define EVENT_TYPE_LOOPFAULT_RESUME     18  //回路短路恢复
#define EVENT_TYPE_POWER_FAULT          19  //主电故障
#define EVENT_TYPE_POWER_FAULT_RESUME   20  //主电故障恢复
#define EVENT_TYPE_BATPOWER_FAULT       21  //备电故障
#define EVENT_TYPE_BATPOWER_FAULT_RE    22  //备电故障恢复
#define EVENT_TYPE_SET_PHONE            23  //设置电话号码
#define EVENT_TYPE_CALL_PHONE           24  //拨打报警电话
#define EVENT_TYPE_CALL_CANCEL          25  //没有拨通电话号码
#define EVENT_TYPE_CALL_ON              26  //电话已接通



//设备类型定义

#define DEVICE_POINT_PY_SMOKE_FIRE_DETECTOR         23 //光电感烟探测器
#define DEVICE_HAND_FIRE_DETECTOR                   61 //手报
#define DEVICE_FEEL_FIRE_DETECTOR                   30 //感温探测器
typedef struct
{
	uint8_t Type;					//事件类型，报警、故障等，根据事件类型不同做不同的处理
	uint8_t TransferPriority;		//传输优先级
	uint8_t MatchEventCode;          //匹配事件码
	uint8_t ParameterNumber;		    //参数个数
	char Caption[26];			    //事件描述
}EVENT_CONST;

void EventTask(void);
void EventInit(void);
 void Loop_Revice(u8* buff);
void WriteEventToFlash(SYSTEM_EVENT *pEvent);
void EventAnalysis(SYSTEM_EVENT *pEvent);
void DateToString(char *DateString,uint8_t *pDateBuffer);
void TimeToString(char *TimeString,uint8_t *pTimeBuffer);
void event_porwer_on_off(void);
BOOL is_soft_reset(void);
#endif

