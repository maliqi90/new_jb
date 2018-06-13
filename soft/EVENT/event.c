#define EVENT_PRIVATE

//#include "event.h"
#include "datastruct.h"
#include "string.h"
#include "include.h"
//uint8_t bat_flag = 0;//备电故障标志位
volatile uint8_t power_flag = 0;//主电故障标志位
extern uint8_t         WinIndex;
uint8_t event_write_flag = 0;//事件记录循环覆盖标记位 BIT0 1 表示火警 BIT0 表示 事件
extern const DEVICE_TYPE device_type1[];
// EVENT_CONST const EventConstTable[] = 
//{
//	//事件类型                      传输优先级                        匹配事件代码                 参数    事件描述
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  "               "},	//0  
//	{EVENT_TYPE_ALARM,              T_PRIORITY_OTHERS_EVENT,        EVENT_INVALID_CODE,             1,  "正常"},	//1  
//	{EVENT_TYPE_ALARM,              T_PRIORITY_ALARM_EVENT,         EVENT_INVALID_CODE,             3,  "火警"},	//2  
//	{EVENT_TYPE_ALARM,              T_PRIORITY_ALARM_EVENT,         EVENT_INVALID_CODE,             3,  ""},	//3 电气火灾报警  
//	{EVENT_TYPE_ALARM,              T_PRIORITY_ALARM_EVENT,         EVENT_INVALID_CODE,             3,  ""},	//4 可燃气体低限报警                    
//	{EVENT_TYPE_ALARM,              T_PRIORITY_ALARM_EVENT,         EVENT_INVALID_CODE,             3,  ""},	//5  可燃气体高限报警          
//	{EVENT_TYPE_ALARM,              T_PRIORITY_ALARM_EVENT,         EVENT_INVALID_CODE,             3,  ""},	//6  可燃气体超量程报警        
//	{EVENT_TYPE_ALARM,              T_PRIORITY_ALARM_EVENT,         EVENT_INVALID_CODE,             3,  ""},	//7  电气火灾预警              
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//8  
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//9  
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//10 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//11 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//12 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//13 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//14 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//15 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//16 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//17 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//18 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//19 
//	{EVENT_TYPE_START,              T_PRIORITY_ACT_FEEDBACK_EVENT,  EVENT_INVALID_CODE,             3,  ""},	//20 启动
//	{EVENT_TYPE_START,              T_PRIORITY_ACT_FEEDBACK_EVENT,  EVENT_INVALID_CODE,             3,  ""},	//21 自动启动                  
//	{EVENT_TYPE_START,              T_PRIORITY_ACT_FEEDBACK_EVENT,  EVENT_INVALID_CODE,             3,  " "},	//22 手动启动                 
//	{EVENT_TYPE_START,              T_PRIORITY_ACT_FEEDBACK_EVENT,  EVENT_INVALID_CODE,             3,  ""},	//23 现场急启                  
//	{EVENT_TYPE_DELAY,              T_PRIORITY_ACT_FEEDBACK_EVENT,  EVENT_INVALID_CODE,             3,  ""},	//24 气体灭火开始延时          
//	{EVENT_TYPE_START,              T_PRIORITY_ACT_FEEDBACK_EVENT,  EVENT_INVALID_CODE,             3,  ""},	//25 气体喷洒                  
//	{EVENT_TYPE_FEEDBACK,           T_PRIORITY_ACT_FEEDBACK_EVENT,  EVENT_INVALID_CODE,             3,  ""},	//26 反馈                      
//	{EVENT_TYPE_FEEDBACK,           T_PRIORITY_ACT_FEEDBACK_EVENT,  EVENT_INVALID_CODE,             3,  ""},	//27 喷洒反馈                  
//	{EVENT_TYPE_FEEDBACK_CANCLE,    T_PRIORITY_ACT_FEEDBACK_EVENT,  EVENT_SPRAY_FEEDBACK,           3,  ""},	//28 反馈撤销                  
//	{EVENT_TYPE_STOP,               T_PRIORITY_ACT_FEEDBACK_EVENT,  EVENT_INVALID_CODE,             3,  ""},	//29 停止                      
//	{EVENT_TYPE_STOP,               T_PRIORITY_ACT_FEEDBACK_EVENT,  EVENT_FIELD_EMERGENT_START,     3,  ""},	//30 现场急停                  
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//31 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//32 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//33 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//34 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//35 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//36 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//37 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//38 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//39 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//40 
//	{EVENT_TYPE_OTHER,              T_PRIORITY_ACT_CMD,             EVENT_INVALID_CODE,             3,  ""},	//41 应急                      
//	{EVENT_TYPE_OTHER,              T_PRIORITY_OTHERS_EVENT,        EVENT_INVALID_CODE,             1,  " "},	//42 年检                     
//	{EVENT_TYPE_OTHER,              T_PRIORITY_ACT_CMD,             EVENT_INVALID_CODE,             3,  ""},	//43 标志灯具改变方向          
//	{EVENT_TYPE_START,              T_PRIORITY_ACT_CMD,             EVENT_INVALID_CODE,             3,  ""},	//44 电梯迫降                  
//	{EVENT_TYPE_START,              T_PRIORITY_ACT_CMD,             EVENT_INVALID_CODE,             3,  ""},	//45 卷帘半降                  
//	{EVENT_TYPE_START,              T_PRIORITY_ACT_CMD,             EVENT_INVALID_CODE,             3,  ""},	//46 卷帘全降                  
//	{EVENT_TYPE_START,              T_PRIORITY_ACT_CMD,             EVENT_INVALID_CODE,             3,  ""},	//47 呼叫                      
//	{EVENT_TYPE_START,              T_PRIORITY_ACT_CMD,             EVENT_INVALID_CODE,             3,  ""},	//48 通话                      
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_FAULT_EVENT,         EVENT_INVALID_CODE,             3,  ""},	//49 消防设备电源失电          
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_FAULT_EVENT,         EVENT_INVALID_CODE,             3,  ""},	//50 消防设备电源欠压          
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_FAULT_EVENT,         EVENT_INVALID_CODE,             3,  ""},	//51 消防设备电源过压          
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_FAULT_EVENT,         EVENT_INVALID_CODE,             3,  ""},	//52 消防设备电源过载          
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_FAULT_EVENT,         EVENT_INVALID_CODE,             3,  ""},	//53 消防设备电源缺相          
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_FAULT_EVENT,         EVENT_INVALID_CODE,             3,  ""},	//54 消防设备电源错相          
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_FAULT_EVENT,         EVENT_INVALID_CODE,             3,  ""},	//55 消防水箱（池）水位低      
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_OTHERS_EVENT,        EVENT_INVALID_CODE,             3,  ""},	//56 消防电梯停用              
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//57 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//58 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//59 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//60 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//61 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//62 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//63 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//64 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  " "},	//65 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//66 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//67 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//68 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//69 
//	{EVENT_TYPE_SUPERVISE,          T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//70 监管                      
//	{EVENT_TYPE_SUPERVISE_CANCLE,   T_PRIORITY_ACT_FEEDBACK_EVENT,  EVENT_INVALID_CODE,             3,  ""},	//71 监管解除                  
//	{EVENT_TYPE_DISABLE,            T_PRIORITY_OTHERS_EVENT,        EVENT_DISABLE,          	    3,  "屏蔽"},	//72 屏蔽
//	{EVENT_TYPE_DISABLE_CANCLE,     T_PRIORITY_OTHERS_EVENT,        EVENT_DISABLE_REMOVE,           3,  ""},	//73 屏蔽解除
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  " "},	//74 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//75 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//76 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//77 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//78 
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//79 
//	{EVENT_TYPE_FAULT,              T_PRIORITY_FAULT_EVENT,         EVENT_INVALID_CODE,             1,  ""},	//80 故障
//	{EVENT_TYPE_FAULT,              T_PRIORITY_FAULT_EVENT,         EVENT_INVALID_CODE,             1,  ""},	//81 通讯故障
//	{EVENT_TYPE_FAULT,              T_PRIORITY_FAULT_EVENT,         EVENT_INVALID_CODE,             1,  ""},	//82 主电故障
//	{EVENT_TYPE_FAULT,              T_PRIORITY_FAULT_EVENT,         EVENT_INVALID_CODE,             1,  ""},	//83 备电故障
//	{EVENT_TYPE_FAULT,              T_PRIORITY_FAULT_EVENT,         EVENT_INVALID_CODE,             1,  ""},	//84 备电欠压
//	{EVENT_TYPE_FAULT,              T_PRIORITY_FAULT_EVENT,         EVENT_INVALID_CODE,             2,  ""},	//85 回路故障
//	{EVENT_TYPE_FAULT,              T_PRIORITY_FAULT_EVENT,         EVENT_INVALID_CODE,             3,  ""},	//86 部件故障
//	{EVENT_TYPE_FAULT,              T_PRIORITY_FAULT_EVENT,         EVENT_INVALID_CODE,             2,  ""},	//87 线路故障
//	{EVENT_TYPE_FAULT,              T_PRIORITY_FAULT_EVENT,         EVENT_INVALID_CODE,             2,  ""},	//88 接地故障
//	{EVENT_TYPE_FAULT,              T_PRIORITY_FAULT_EVENT,         EVENT_INVALID_CODE,             3,  ""},	//89 常闭防火门打开
//	{EVENT_TYPE_FAULT,              T_PRIORITY_FAULT_EVENT,         EVENT_INVALID_CODE,             3,  ""},	//90 常开防火门关闭
//	{EVENT_TYPE_FAULT,              T_PRIORITY_FAULT_EVENT,         EVENT_INVALID_CODE,             1,  ""},	//91 打印机故障
//	{EVENT_TYPE_FAULT,              T_PRIORITY_FAULT_EVENT,         EVENT_INVALID_CODE,        		1,  ""},	//92 声光故障
//	{EVENT_TYPE_FAULT,              T_PRIORITY_FAULT_EVENT,         EVENT_INVALID_CODE,             1,  ""},	//93 备电欠压
//	{EVENT_TYPE_FAULT,              T_PRIORITY_FAULT_EVENT,         EVENT_INVALID_CODE,             1,  ""},	//94 主电欠压
//	{EVENT_TYPE_FAULT,              T_PRIORITY_FAULT_EVENT,         EVENT_INVALID_CODE,             2,  ""},	//95 过流 
//	{EVENT_TYPE_FAULT,              T_PRIORITY_FAULT_EVENT,         EVENT_INVALID_CODE,             3,  ""},	//96 过流保护
//	{EVENT_TYPE_FAULT,              T_PRIORITY_FAULT_EVENT,         EVENT_INVALID_CODE,             2,  ""},	//97 喷洒启动连线故障          
//	{EVENT_TYPE_FAULT,              T_PRIORITY_FAULT_EVENT,         EVENT_INVALID_CODE,             2,  ""},	//98 喷洒启动连线短路          
//	{EVENT_TYPE_FAULT,              T_PRIORITY_FAULT_EVENT,         EVENT_INVALID_CODE,             3,  ""},	//99 喷洒启动连线开路          
//	{EVENT_TYPE_FAULT,              T_PRIORITY_FAULT_EVENT,         EVENT_INVALID_CODE,             3,  ""},	//10 喷洒回答连线故障          
//	{EVENT_TYPE_FAULT,              T_PRIORITY_FAULT_EVENT,         EVENT_INVALID_CODE,             1,  ""},	//101喷洒回答连线短路          
//	{EVENT_TYPE_FAULT,              T_PRIORITY_FAULT_EVENT,         EVENT_INVALID_CODE,             1,  ""},	//102 喷洒回答连线开路           	
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  " "},	//103
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//104
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//105
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//106
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//107
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//108	
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_FAULT_EVENT,         EVENT_FAULT,                    1,  ""},	//109故障恢复                  
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_FAULT_EVENT,         EVENT_COMMUNICATION_FAULT,      1,  ""},	//110通讯故障恢复              
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_FAULT_EVENT,         EVENT_POWER_FAULT,              1,  ""},	//111主电故障恢复              
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_FAULT_EVENT,         EVENT_BATTERY_FAULT,            1,  ""},	//112备电故障恢复              
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_FAULT_EVENT,         EVENT_CHARGE_FAULT,             1,  ""},	//113充电故障恢复              
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_FAULT_EVENT,         EVENT_LOOP_FAULT,               2,  ""},	//114回路故障恢复              
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_FAULT_EVENT,         EVENT_DEVICE_FAULT,             3,  ""},	//115部件故障恢复              
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_FAULT_EVENT,         EVENT_LINE_FAULT,               2,  ""},	//116线路故障恢复              
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_FAULT_EVENT,         EVENT_GROUND_FAULT,             2,  ""},	//117接地故障恢复              
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_FAULT_EVENT,         EVENT_NC_FILR_DOOR_OPEN,        3,  ""},	//118常闭防火门恢复关闭状态    
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_FAULT_EVENT,         EVENT_NO_FILR_DOOR_CLOSE,       3,  ""},	//119常开防火门恢复开门状态    
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_FAULT_EVENT,         EVENT_PRINTER_FAULT,            1,  ""},	//120打印机故障恢复            
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_FAULT_EVENT,         EVENT_SOUND_LIGHT_FAULT,		1,  ""},	//121声光故障恢复              
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_FAULT_EVENT,         EVENT_BATTERY_LOWER,            1,  ""},	//122备电欠压恢复              
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_FAULT_EVENT,         EVENT_POWER_LOWER,              1,  ""},	//123主电欠压恢复              
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_FAULT_EVENT,         EVENT_OVERCURRENT,              2,  ""},	//124过流恢复                  
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_FAULT_EVENT,         EVENT_OVERCURRENT_PROTECT,      3,  ""},	//125过流保护恢复              
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_FAULT_EVENT,         EVENT_SPRAY_START_FAULT,        2,  ""},	//126喷洒启动连线故障恢复      
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_FAULT_EVENT,         EVENT_SPRAY_START_SHORT,        2,  ""},	//127喷洒启动连线短路恢复      
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_FAULT_EVENT,         EVENT_SPRAY_START_OPEN,         3,  ""},	//128喷洒启动连线开路恢复      
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_FAULT_EVENT,         EVENT_SPRAY_ANSWER_FAULT,       3,  ""},	//129喷洒回答连线故障恢复      
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_FAULT_EVENT,         EVENT_SPRAY_ANSWER_SHORT,       1,  ""},	//130喷洒回答连线短路恢复      
//	{EVENT_TYPE_FAULT_RESUME,       T_PRIORITY_FAULT_EVENT,         EVENT_SPRAY_ANSWER_OPEN,        1,  ""},	//131	喷洒回答连线开路恢复      
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//132
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//133
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//134
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//135
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//136
//	{EVENT_TYPE_ANY,                T_PRIORITY_LOWEST,              EVENT_INVALID_CODE,             1,  ""},	//137	
// 	{EVENT_TYPE_SYSTEM,             T_PRIORITY_OTHERS_EVENT,        EVENT_INVALID_CODE,            	1,  ""},	//138开机
//	{EVENT_TYPE_SYSTEM,             T_PRIORITY_OTHERS_EVENT,        EVENT_INVALID_CODE,            	1,  ""},	//139关机
//	{EVENT_TYPE_SYSTEM,             T_PRIORITY_OTHERS_EVENT,        EVENT_INVALID_CODE,             1,  ""},	//140 复位
//	{EVENT_TYPE_SYSTEM,             T_PRIORITY_OTHERS_EVENT,        EVENT_INVALID_CODE,            	1,  ""},	//141  自检
//	{EVENT_TYPE_SYSTEM,             T_PRIORITY_OTHERS_EVENT,        EVENT_INVALID_CODE,        		1,  ""},	//142自检失败
//	{EVENT_TYPE_SYSTEM,             T_PRIORITY_OTHERS_EVENT,        EVENT_INVALID_CODE,         	1,  ""},	//143手动状态
//	{EVENT_TYPE_SYSTEM,             T_PRIORITY_OTHERS_EVENT,        EVENT_INVALID_CODE,           	1,  ""},	//144自动状态
//	{EVENT_TYPE_SYSTEM,             T_PRIORITY_OTHERS_EVENT,        EVENT_INVALID_CODE,            	1,  ""},	//145确认/消音	

//};
//事件记录初始化，在系统上电时择机调用
void EventInit(void)
{
    SYSTEM_EVENT Event;
    norflash_read_eventflag(&event_write_flag);
    if(event_write_flag == 0xff)
    {
        event_write_flag = 0;
        norflash_write_eventflag(0);
    }
    //存储指针初始化
    for (EventFlashBasePointer = 0; EventFlashBasePointer < EVENT_ALL_COUNT; EventFlashBasePointer++)
    {
     		norflash_read_otherevent(EventFlashBasePointer,&Event);
		
		    if (Event.EventCode == EVENT_INVALID_CODE)
		    {
			    break;
		    }
           EventFlashCurrentPointer = EventFlashBasePointer+1;//事件记录指针            
    }
    
    //报警记录指针初始化
     for (EventFlashAlarmBasePointer = 0; EventFlashAlarmBasePointer < EVENT_ALARM_COUNT; EventFlashAlarmBasePointer++)
     {
         norflash_read_AlarmEvent(EventFlashAlarmBasePointer,&Event);
         if (Event.EventCode != EVENT_TYPE_ALARM)
		{
			break;
		}
     }
     //故障记录
//     for(EventFlashFaultBasePointer = 0;EventFlashFaultBasePointer < EVENT_ALL_COUNT;EventFlashFaultBasePointer++)
//     {
//         norflash_read_otherevent(EventFlashBasePointer,&Event);
//           if (Event.EventCode == EVENT_TYPE_FAULT)
//		    {
//			    break;
//		    }
//         
//     }
    EventFlashAlarmCurrentPointer = EventFlashAlarmBasePointer;//报警事件记录指针	
    EventFireRamCurrentPointer 		= 0;
	EventFaultRamCurrentPointer 	= 0;
    memset(EventRamBuffer, 0, sizeof(EventRamBuffer));
     
     if(EventFlashAlarmCurrentPointer == EVENT_ALARM_COUNT)
     {
         event_write_flag |=0x80;
     }
}












/************************************************************
函数名称：EventShiledProcess
函数功能：屏蔽事件处理
************************************************************/
void EventShiledProcess(void)
{
	uint16_t i = 0;
	SYSTEM_EVENT Event;
	for(i = 0;i < 16;i++)
	{
		norflash_read_SheildEvent(i,&Event);
	    if (Event.EventCode == 0xff)
		{
			break;
		}	
	}
	norflash_write_SheildEvent(i,(uint8_t*)&EventRamBuffer[EventRamTaskPointer]);
	LightShieldLed();
}
//开放事件处理
void EventOpenProcess(void)
{
    
}

//事件处理
void EventLedSoundDisplayProcess(uint8_t EventType)
{
    uint32_t temp;
	//点亮背光
	switch(EventType)
	{
		case EVENT_TYPE_ALARM:
             LED_ALARM_ON;
             RELAY_ON;
             Play_Voice(NORFLASH_FIRE_VOICE_BASE,2);
              voice_flag = 1;
             call_flag = 1;
          //Call_PhoneNum();
		//点火警灯
		//显示火警画面
            if(WinIndex == W12_SHIELDSET_WINDOW_WIN)
           {
               LISTVIEW_Delete(W12_hListView);
               GUI_Exec();
           }
           else if(WinIndex == W10_EDITDEVICE_WINDOW_WIN)
           {
              LISTVIEW_Delete(W10_hListView);
               GUI_Exec();               
           }
           else if(WinIndex == W19_DEVICELIGHT_WINDOW_WIN)
           {
              LISTVIEW_Delete(W19_hListView);
               GUI_Exec();     
           }
           else if(WinIndex == W22_LOOPLOGIN_WINDOW_WIN)
           {
                  LISTVIEW_Delete(W22_hListView);
                  PROGBAR_Delete(LoopBar);
                  GUI_Exec();  
           }
           else if(WinIndex == W20_LIGHT_SET_WINDOW_WIN)
           {
                 PROGBAR_Delete(slider);
           
                GUI_Exec(); 
           }
          W5_FireWindowDisplay();
		//远程拨号
		//输出火警音
		break;

        case EVENT_TYPE_POWER_FAULT:
        case EVENT_TYPE_BATPOWER_FAULT:
        case EVENT_TYPE_LOOP_FAULT:
		case EVENT_TYPE_FAULT://故障
             voice_flag = 1;
            Led_Ctrl(LEDSHIELD_OFF);
            LED_FAULT_ON;
           switch(EventType)
           {
               case EVENT_TYPE_POWER_FAULT:
                   temp = NORFLASH_POWERFAULT_VOICE_BASE;
							     
                   break;
               case EVENT_TYPE_BATPOWER_FAULT:
                   temp = NORFLASH_BATFAULT_VOICE_BASE;
                   break;
               case EVENT_TYPE_FAULT:
                   temp = NORFLASH_COMMFAULT_VOICE_BASE;
                   break;
               default:
                   break;
           }
            Play_Voice(temp,2);
           if(WinIndex == W12_SHIELDSET_WINDOW_WIN)
           {
               LISTVIEW_Delete(W12_hListView);
               GUI_Exec();
           }
           else if(WinIndex == W10_EDITDEVICE_WINDOW_WIN)
           {
               LISTVIEW_Delete(W10_hListView);
               GUI_Exec();               
           }
           else if(WinIndex == W19_DEVICELIGHT_WINDOW_WIN)
           {
              LISTVIEW_Delete(W19_hListView);
               GUI_Exec();     
           }
           else if(WinIndex == W22_LOOPLOGIN_WINDOW_WIN)
           {
                  LISTVIEW_Delete(W22_hListView);
                  GUI_Exec();  
           }
            if(GetEventAlarmCount!=0)
            {
              W5_FireWindowDisplay();
            }
            else
            {
              W4_FaultWindowDisplay();
            }
		//点故障灯
		//显示故障界面
		//输出故障声音
		break;
		case EVENT_TYPE_BATPOWER_FAULT_RE:
		case EVENT_TYPE_LOOPFAULT_RESUME:
    case EVENT_TYPE_POWER_FAULT_RESUME:
		case EVENT_TYPE_FAULT_RESUME://故障恢复
            if(GetEventFaultCount() == 0)
            {
                if(EventShieldRamCurrentPointer > 0)
                {
                   Led_Ctrl(LEDSHIELD_ON);
                }
                else
                {
                     LED_FAULT_OFF;
                }
            }
            else
            {
                
            }
            if(WinIndex == W12_SHIELDSET_WINDOW_WIN)
           {
               LISTVIEW_Delete(W12_hListView);
               GUI_Exec();
           }
           else if(WinIndex == W10_EDITDEVICE_WINDOW_WIN)
           {
               LISTVIEW_Delete(W10_hListView);
               GUI_Exec();               
           }
           else if(WinIndex == W19_DEVICELIGHT_WINDOW_WIN)
           {
              LISTVIEW_Delete(W19_hListView);
               GUI_Exec();     
           } 
           else if(WinIndex == W22_LOOPLOGIN_WINDOW_WIN)
           {
                  LISTVIEW_Delete(W22_hListView);
                  GUI_Exec();  
           }           
            if(GetEventAlarmCount!=0)
            {
              W5_FireWindowDisplay();
            }
            else
            {
              W4_FaultWindowDisplay();
            }
             //W4_FaultWindowDisplay();
		//熄灭故障灯
		//刷新故障界面
		
		break;
		
		case EVENT_TYPE_DISABLE://屏蔽
            if(GetEventFaultCount()==0)
            {
                Led_Ctrl(LEDSHIELD_ON);
                //LED_SHEILD_ON;
            }
//            if(GetEventAlarmCount!=0)
//            {
//              W5_FireWindowDisplay();
//            }
//            else if((GetEventFaultCount()!=0)&&(GetEventFaultCount()==0))
//            {
//                W4_FaultWindowDisplay();
//            }
//            else
//            {
           if(WinIndex == W12_SHIELDSET_WINDOW_WIN)
           {
               LISTVIEW_Delete(W12_hListView);
               GUI_Exec();
           }
           else if(WinIndex == W10_EDITDEVICE_WINDOW_WIN)
           {
               LISTVIEW_Delete(W10_hListView);
               GUI_Exec();               
           }
           else if(WinIndex == W19_DEVICELIGHT_WINDOW_WIN)
           {
              LISTVIEW_Delete(W19_hListView);
               GUI_Exec();     
           }
           else if(WinIndex == W22_LOOPLOGIN_WINDOW_WIN)
           {
                  LISTVIEW_Delete(W22_hListView);
                  GUI_Exec();  
           }
             W6_ShieldWindowDisplay();
          //  }
            
            
		     //显示屏蔽界面
			 //点屏蔽灯
		break;
		case EVENT_TYPE_DISABLE_CANCLE://屏蔽解除
             if(GetEventFaultCount()==0)
             {
                 Led_Ctrl(LEDSHIELD_OFF);
             }
//            if(GetEventAlarmCount!=0)
//            {
//              W5_FireWindowDisplay();
//            }
//            else if((GetEventFaultCount()!=0)&&(GetEventFaultCount()==0))
//            {
//                W4_FaultWindowDisplay();
//            }
//            else
//            {
           if(WinIndex == W12_SHIELDSET_WINDOW_WIN)
           {
               LISTVIEW_Delete(W12_hListView);
               GUI_Exec();
           }
           else if(WinIndex == W10_EDITDEVICE_WINDOW_WIN)
           {
               LISTVIEW_Delete(W10_hListView);
               GUI_Exec();               
           }
           else if(WinIndex == W19_DEVICELIGHT_WINDOW_WIN)
           {
              LISTVIEW_Delete(W19_hListView);
               GUI_Exec();     
           }
           else if(WinIndex == W22_LOOPLOGIN_WINDOW_WIN)
           {
                  LISTVIEW_Delete(W22_hListView);
                  GUI_Exec();  
           }
             W6_ShieldWindowDisplay();
//            }
            break;
		case EVENT_TYPE_SILENCE:
		     //消音处理
		break;
	}
}


//新事件处理
void EventTask(void)
{
   uint32_t i,j;
    uint8_t temp;
   if(EventRamCurrentPointer == EventRamTaskPointer)
   {
	   return;
   }
   lcd_time = LCD_TIME;//点亮背光
   	norflash_write_otherevent(EventFlashCurrentPointer,(uint8_t*)&EventRamBuffer[EventRamTaskPointer]);//写事件到flash
	EventFlashCurrentPointer++;
   if(EventFlashCurrentPointer == EVENT_ALL_COUNT)
   {
       
       EventFlashCurrentPointer -=128;
       temp = ((event_write_flag&0xFE)>>1);
       
       temp++;
 
       if(temp == 32)
       {
           temp = 0;
       }
       sFLASH_NOR_SectorErase(NORFLASH_EVENT_BASE+(temp-1)*4096);
       temp = temp<<1;
       event_write_flag &= 0x01;
       event_write_flag +=temp;
       norflash_write_eventflag(event_write_flag);
       
           
   }
   switch(EventRamType[EventRamTaskPointer])//根据事件类型分类处理
   {
	   case EVENT_TYPE_ALARM: //报警
	        EventAlarmCount++;
			memcpy(&EventFireRamBuffer[EventFireRamCurrentPointer], &EventRamBuffer[EventRamTaskPointer], 64);
            
            EventFireRamCurrentPointer++;
            if (EventFireRamCurrentPointer == EVENT_ALARM_COUNT)
            {
                EventFireRamCurrentPointer = 0;
            }
            
            norflash_write_AlarmEvent(EventFlashAlarmCurrentPointer,(uint8_t*)&EventFireRamBuffer[EventFireRamCurrentPointer-1]);//写火警事件到专门的flash区域
		    
		    EventFlashAlarmCurrentPointer++;
		
		    if (EventFlashAlarmCurrentPointer == EVENT_ALARM_COUNT)
		     {
                 event_write_flag |= 0x80;//循环覆盖标记位置1；
			    EventFlashAlarmCurrentPointer = EVENT_ALARM_COUNT-1;
		     }

		break;
         case EVENT_TYPE_BATPOWER_FAULT://主电故障
         case EVENT_TYPE_POWER_FAULT://主电故障
		 case EVENT_TYPE_LOOP_FAULT: //回路短路
		 case EVENT_TYPE_FAULT://故障
		     EventFaultCount++;
			 if(1) //主电故障/备电故障
			 {
				 
			 }
			 if(1) //现场部件故障
			 {
				 
			 }
		     memcpy(&EventFaultRamBuffer[EventFaultRamCurrentPointer], &EventRamBuffer[EventRamTaskPointer], 32);
			EventFaultRamCurrentPointer++;
			
			if (EventFaultRamCurrentPointer == EVENT_FAULT_COUNT)
			{
				EventFaultRamCurrentPointer = 0;
			}
			
		break;
		
		case EVENT_TYPE_FAULT_RESUME: //故障恢复
		     for(i = EventRamTaskPointer; i < EventRamCurrentPointer; i++)
			 {
				if (EventRamType[i] == EVENT_TYPE_FAULT_RESUME)
				{
					for (j = 0; j < i; j++)
                    {
                       if (
							(EventRamBuffer[j].EventCode		 ==	EVENT_TYPE_FAULT)  &&
							(EventRamBuffer[j].EventDeviceType	 ==	EventRamBuffer[i].EventDeviceType)   &&
							(EventRamBuffer[j].EventAddress		 == EventRamBuffer[i].EventAddress))
						{
							EventRamType[j] = 0xff;
										
							if (EventFaultCount)
								EventFaultCount--;                            
                        }                            
                    }
				}					
			 }
           EventFaultRamCurrentPointer = 0;
			
			for (i = 0; i <= EventRamTaskPointer; i++)
			{
				if ((EventRamType[i] == EVENT_TYPE_FAULT)||(EventRamType[i] == EVENT_TYPE_LOOP_FAULT)||(EventRamType[i] == EVENT_TYPE_POWER_FAULT)||(EventRamType[i] == EVENT_TYPE_BATPOWER_FAULT)||(EventRamType[i] == EVENT_TYPE_LOOP_FAULT))
				{
					memcpy(&EventFaultRamBuffer[EventFaultRamCurrentPointer], &EventRamBuffer[i], 32);
					EventFaultRamCurrentPointer++;
					
					if (EventFaultRamCurrentPointer == EVENT_FAULT_COUNT)
					{
						EventFaultRamCurrentPointer = 0;
					}
				}
			}
            if (EventFaultRamCurrentPointer != EventFaultCount)
				EventFaultCount = EventFaultRamCurrentPointer;
             
		break;
		
		case EVENT_TYPE_DISABLE://屏蔽
		     //EventShiledProcess();
		break;
		
		case EVENT_TYPE_DISABLE_CANCLE://屏蔽取消
		     //EventOpenProcess();
		break;
//        case EVENT_TYPE_POWER_FAULT://主电故障
//            
//            break;
        case EVENT_TYPE_POWER_FAULT_RESUME://主电故障取消
		     for(i = EventRamTaskPointer; i < EventRamCurrentPointer; i++)
			 {
				if (EventRamType[i] == EVENT_TYPE_POWER_FAULT_RESUME)
				{
					for (j = 0; j < i; j++)
                    {
                       if (
							(EventRamBuffer[j].EventCode		 ==	EVENT_TYPE_POWER_FAULT)  &&
							(EventRamBuffer[j].EventDeviceType	 ==	EventRamBuffer[i].EventDeviceType)   &&
							(EventRamBuffer[j].EventAddress		 == EventRamBuffer[i].EventAddress))
						{
							EventRamType[j] = 0xff;
										
							if (EventFaultCount)
								EventFaultCount--;                            
                        }                            
                    }
				}					
			 }
           EventFaultRamCurrentPointer = 0;
			
			for (i = 0; i <= EventRamTaskPointer; i++)
			{
				if ((EventRamType[i] == EVENT_TYPE_FAULT)||(EventRamType[i] == EVENT_TYPE_LOOP_FAULT)||(EventRamType[i] == EVENT_TYPE_POWER_FAULT)||(EventRamType[i] == EVENT_TYPE_BATPOWER_FAULT)||(EventRamType[i] == EVENT_TYPE_LOOP_FAULT))
				{
					memcpy(&EventFaultRamBuffer[EventFaultRamCurrentPointer], &EventRamBuffer[i], 32);
					EventFaultRamCurrentPointer++;
					
					if (EventFaultRamCurrentPointer == EVENT_FAULT_COUNT)
					{
						EventFaultRamCurrentPointer = 0;
					}
				}
			}
            if (EventFaultRamCurrentPointer != EventFaultCount)
				EventFaultCount = EventFaultRamCurrentPointer;            
        
        
            break;
//	    case EVENT_TYPE_BATPOWER_FAULT://
//            break;
        case EVENT_TYPE_BATPOWER_FAULT_RE://备电取消
             for(i = EventRamTaskPointer; i < EventRamCurrentPointer; i++)
			 {
				if (EventRamType[i] == EVENT_TYPE_BATPOWER_FAULT_RE)
				{
					for (j = 0; j < i; j++)
                    {
                       if (
							(EventRamBuffer[j].EventCode		 ==	EVENT_TYPE_BATPOWER_FAULT)  &&
							(EventRamBuffer[j].EventDeviceType	 ==	EventRamBuffer[i].EventDeviceType)   &&
							(EventRamBuffer[j].EventAddress		 == EventRamBuffer[i].EventAddress))
						{
							EventRamType[j] = 0xff;
										
							if (EventFaultCount)
								EventFaultCount--;                            
                        }                            
                    }
				}					
			 }
           EventFaultRamCurrentPointer = 0;
			
			for (i = 0; i <= EventRamTaskPointer; i++)
			{
				if ((EventRamType[i] == EVENT_TYPE_FAULT)||(EventRamType[i] == EVENT_TYPE_LOOP_FAULT)||(EventRamType[i] == EVENT_TYPE_POWER_FAULT)||(EventRamType[i] == EVENT_TYPE_BATPOWER_FAULT)||(EventRamType[i] == EVENT_TYPE_LOOP_FAULT))
				{
					memcpy(&EventFaultRamBuffer[EventFaultRamCurrentPointer], &EventRamBuffer[i], 32);
					EventFaultRamCurrentPointer++;
					
					if (EventFaultRamCurrentPointer == EVENT_FAULT_COUNT)
					{
						EventFaultRamCurrentPointer = 0;
					}
				}
			}
            if (EventFaultRamCurrentPointer != EventFaultCount)
				EventFaultCount = EventFaultRamCurrentPointer;    
            break;
		default :
		break;

   }
  EventLedSoundDisplayProcess(EventRamType[EventRamTaskPointer]);
   EventRamTaskPointer++;

}
/***********************************************************************************
函数名称：WriteEventToFlash(SYSTEM_EVENT *pEvent)
函数功能：写本机事件到FLASH
参    数：SYSTEM_EVENT *pEvent 事件结构体
*************************************************************************************/
void WriteEventToFlash(SYSTEM_EVENT *pEvent)
{
		SYSTEM_EVENT 		 Event;
//	  memset(&Event, 0, 64);
	
	 Event.EventVerion = 1;
	 if((pEvent->EventCode == EVENT_TYPE_GUANJI ||pEvent->EventCode == EVENT_TYPE_KAIJI ||pEvent->EventCode == EVENT_TYPE_RESET))
	 {
		Event.EventYear         = pEvent->EventYear;
        Event.EventMonth        = pEvent->EventMonth;
        Event.EventDay          = pEvent->EventDay;
        Event.EventHour         = pEvent->EventHour;
        Event.EventMinute       = pEvent->EventMinute;
        Event.EventSecond       = pEvent->EventSecond;
	 }
	 else
	 {
		Event.EventYear         = Timer_value.year-2000;
        Event.EventMonth        = Timer_value.month;
        Event.EventDay          = Timer_value.date;
        Event.EventHour         = Timer_value.hour;
        Event.EventMinute       = Timer_value.min;
        Event.EventSecond       = Timer_value.sec;
	 }
	 Event.EventCode        = pEvent->EventCode;
     Event.EventCaption     = pEvent->EventCaption;
     Event.EventAddress     = pEvent->EventAddress;
     Event.Zone             = pEvent->Zone;
	 	//备份到RAM内
      memcpy(&EventRamBuffer[EventRamCurrentPointer].EventVerion, &Event.EventVerion, 32);
	 EventRamType[EventRamCurrentPointer] = Event.EventCode;  
	 EventRamCurrentPointer++;
	 norflash_write_otherevent(EventFlashCurrentPointer,(uint8_t*)&Event);
	 
	 //火警事件备份到专门的FLASH区域
	 if(Event.EventCode == EVENT_TYPE_ALARM)
	 {
		 norflash_write_AlarmEvent(EventFlashAlarmCurrentPointer,(uint8_t*)&Event);
		 EventFlashAlarmCurrentPointer++;
		 if(EventFlashAlarmCurrentPointer == EVENT_ALARM_COUNT)
		 {
			 EventFlashAlarmCurrentPointer = 0;
		 }
	 }
	 
}
//-------------------------------------------------------------------------------------
//获取事件日期
void DateToString(char *DateString,uint8_t *pDateBuffer)
{
	uint8_t i = 2;


		DateString[0] = '2';
		DateString[1] = '0';


	DateString[i++] = (((uint8_t)pDateBuffer[0]) / 10) | 0x30;
	DateString[i++] = (((uint8_t)pDateBuffer[0]) % 10) | 0x30;
	DateString[i++] = '-';
	DateString[i++] = (((uint8_t)pDateBuffer[1] & 0x0f) / 10) | 0x30;
	DateString[i++] = (((uint8_t)pDateBuffer[1] & 0x0f) % 10) | 0x30;
	DateString[i++] = '-';
	DateString[i++] = (((uint8_t)pDateBuffer[2]) / 10) | 0x30;
	DateString[i++] = (((uint8_t)pDateBuffer[2]) % 10) | 0x30;

	DateString[i]   = '\0';
}

//获取事件时间
void TimeToString(char *TimeString,uint8_t *pTimeBuffer)
{
	TimeString[0] = (((uint8_t)pTimeBuffer[0]) / 10) | 0x30;
	TimeString[1] = (((uint8_t)pTimeBuffer[0]) % 10) | 0x30;
	TimeString[2] = ':';
	TimeString[3] = (((uint8_t)pTimeBuffer[1]) / 10) | 0x30;
	TimeString[4] = (((uint8_t)pTimeBuffer[1]) % 10) | 0x30;
	TimeString[5] = ':';
	TimeString[6] = (((uint8_t)pTimeBuffer[2]) / 10) | 0x30;
	TimeString[7] = (((uint8_t)pTimeBuffer[2]) % 10) | 0x30;

	TimeString[8] = '\0';
}

//获取事件日期
void DateToString16(char *DateString,uint16_t *pDateBuffer,uint8_t DateFormat)
{
	uint8_t i;

	if (DateFormat == 1)
	{
		DateString[0] = '2';
		DateString[1] = '0';
		i = 2;
	}
	else
	{
		i = 0;
	}
	DateString[i++] = (((uint8_t)pDateBuffer[0]) / 10) | 0x30;
	DateString[i++] = (((uint8_t)pDateBuffer[0]) % 10) | 0x30;
	DateString[i++] = '-';
	DateString[i++] = (((uint8_t)pDateBuffer[1] & 0x0f) / 10) | 0x30;
	DateString[i++] = (((uint8_t)pDateBuffer[1] & 0x0f) % 10) | 0x30;
	DateString[i++] = '-';
	DateString[i++] = (((uint8_t)pDateBuffer[2]) / 10) | 0x30;
	DateString[i++] = (((uint8_t)pDateBuffer[2]) % 10) | 0x30;

	DateString[i]   = '\0';
}

//获取事件时间
void TimeToString16(char *TimeString,uint16_t *pTimeBuffer)
{
	TimeString[0] = (((uint8_t)pTimeBuffer[0]) / 10) | 0x30;
	TimeString[1] = (((uint8_t)pTimeBuffer[0]) % 10) | 0x30;
	TimeString[2] = ':';
	TimeString[3] = (((uint8_t)pTimeBuffer[1]) / 10) | 0x30;
	TimeString[4] = (((uint8_t)pTimeBuffer[1]) % 10) | 0x30;
	TimeString[5] = ':';
	TimeString[6] = (((uint8_t)pTimeBuffer[2]) / 10) | 0x30;
	TimeString[7] = (((uint8_t)pTimeBuffer[2]) % 10) | 0x30;

	TimeString[8] = '\0';
}
//显示事件
//void EventAnalysis(SYSTEM_EVENT *pEvent)
//{
//    U16   CharPointer;
//    //显示
//    memset(EventInf, 0, sizeof(EVENT_INF));
//    //事件描述
//    memcpy(EventInf->Caption, EventConstTable[pEvent->EventCode].Caption, 10);
//    
//    //日期和时间
//    DateToString(&EventInf->Date[0], &pEvent->EventYear);
//    TimeToString(&EventInf->Time[0], &pEvent->EventHour);
//    //设备名称
//    strcpy(EventInf->DeviceName, (const char*)&device_type1[pEvent->EventDeviceType]);
//    //安装位置
//    memcpy(EventInf->Location, device_caption1[pEvent->EventCaption], 4);
//    EventInf->address = pEvent->EventAddress;
//}

/*******************************************************************************
* 函数名称： is soft_reset
* 函数功能：是否为软件复位
* 参数    ：无
* ·返回值： TRUE->为软件复位 FALSE->非软件复位
*******************************************************************************/
BOOL is_soft_reset(void)
{
	u32 temp = RCC->CSR & (1 << 28);
	
	RCC->CSR |= 1 << 24;
	
	if (temp != 0)
		return TRUE;
	else	
		return FALSE;
}
void event_porwer_on_off(void)
{
	SYSTEM_EVENT event_on_off;
	memset(&event_on_off.EventVerion,0,32);

	if (is_soft_reset())
	{
		
		//产生复位事件	
		event_on_off.EventCode 	 	   = EVENT_TYPE_RESET;
		
		event_on_off.EventYear 	       = Timer_value.year-2000;
		event_on_off.EventMonth 	   = Timer_value.month;
		event_on_off.EventDay 	   	   = Timer_value.date;
		
		event_on_off.EventHour 	  	   = Timer_value.hour;
		event_on_off.EventMinute 	   = Timer_value.min;
		event_on_off.EventSecond 	   = Timer_value.sec;
        event_on_off.EventAddress      = 0xff;
        event_on_off.EventCaption      = 0xff;
        event_on_off.EventDeviceType   = 0xff;
		
		
		WriteEventToFlash(&event_on_off); //复位事件
		
		return;
	}
	
//	event_on_off.EventCode 		   = EVENT_POWER_OFF;
//	
//	event_on_off.EventYear 		   = (uint8)(LPC_RTC->GPREG0>>16);      //(LPC_RTC->GPREG0>>16)%10+((LPC_RTC->GPREG0>>16)%100/10)*10;//LPC_RTC->GPREG0 ±¸·Ý¼Ä´æÆ÷
//	event_on_off.EventMonth 	   = (uint8)(LPC_RTC->GPREG0>>8);
//	event_on_off.EventDay 		   = (uint8)(LPC_RTC->GPREG0);
//	
//	event_on_off.EventHour		   = (uint8)(LPC_RTC->GPREG1>>16);
//	event_on_off.EventMinute	   = (uint8)(LPC_RTC->GPREG1>>8);
//	event_on_off.EventSecond	   = (uint8)(LPC_RTC->GPREG1);
//	
//	event_on_off.EventControllerID = sys_config.ControllerID;
//	
//	event_on_off.PanelCardType	   = DEVICE_FIRE_MCU;
//	event_on_off.EventDeviceType   = DEVICE_FIRE_MCU;
//	
//	WriteEventToFlash(&event_on_off);   //关机事件
      
	memset(&event_on_off.EventVerion, 0, 32);
	
	event_on_off.EventCode         = EVENT_TYPE_GUANJI;
	
		event_on_off.EventYear 	       = Timer_value.year-2000;
		event_on_off.EventMonth 	   = Timer_value.month;
		event_on_off.EventDay 	   	   = Timer_value.date;
		
		event_on_off.EventHour 	  	   = Timer_value.hour;
		event_on_off.EventMinute 	   = Timer_value.min;
		event_on_off.EventSecond 	   = Timer_value.sec;
	    event_on_off.EventAddress      = 0xff;
        event_on_off.EventCaption      = 0xff;
        event_on_off.EventDeviceType   = 0xff;


	WriteEventToFlash(&event_on_off); //开机事件
}
