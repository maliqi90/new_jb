#ifndef _DATA_STRUCT_H_
#define _DATA_STRUCT_H_
#include "stdint.h"
/**************************器件类型***********************************************************/
#define DEVICE_POINT_PY_SMOKE_FIRE_DETECTOR 23 //点型光电感烟火灾探测器



/****************************事件类型*********************************************************************/
#define EVENT_POWER_FAULT                      82 //主电故障
#define EVENT_BATTERY_FAULT                    83 //备电故障
#define EVENT_DEVICE_FAULT                     86 //部件故障

#define EVENT_BATTERY_LOWER                    93 //备电欠压
#define EVENT_POWER_LOWER                      94 //主电欠压

#define EVENT_DISABLE                          72 //屏蔽
#define EVENT_DISABLE_REMOVE                   73 //屏蔽解除
#define EVENT_COMMUNICATION_FAULT_RECOVERY     110 //通讯故障恢复
#define EVENT_POWER_FAULT_RECOVERY             111 //主电故障恢复
#define EVENT_BATTERY_FAULT_RECOVERY           112 //备电欠压恢复

#define EVENT_DEVICE_FAULT_RECOVERY            115 //部件故障恢复
#define EVENT_BATTERY_LOWER_RECOVERY           122  //备电欠压恢复           
#define EVENT_POWER_LOWER_RECOVERY             123  //主电欠压恢复

#define EVENT_POWER_ON                         138 //开机
#define EVENT_POWER_OFF                        139 //关机
#define EVENT_RESET                            140 //复位
#define EVENT_SELF_TEST                        141 //自检
#define EVENT_SELF_TEST_FAILURE                142 //自检失败
#define EVENT_SILENCE                          145 //消音
//设备配置数据结构体
#pragma pack(1)
typedef struct
{
	
	uint8_t  DeviceCondition;//状态，按位定义：BIT0,0-不存在，1-存在；BIT1,1-屏蔽，0正常；BIT2:1-亮灯巡检，0-灭灯巡检
	uint8_t  DeviceType;     //设备类型
	uint8_t  DeviceCaption;  //位置信息
    uint8_t  DeviceZone;
    uint8_t  DeviceFloor;
    uint16_t DeviceBuilding;
	uint8_t reserve;     //预留
	
	
	
} DEVICE_CONFIG;
typedef struct
{
    uint8_t type;
    char caption[10];
    uint8_t tepe_num;
}DEVICE_TYPE;
//事件信息数据结构体
typedef struct
{
	uint8_t EventVerion;     //事件版本号；本版本值为1；
	uint8_t EventCode;       //事件代码
	uint8_t EventDeviceType;//设备类型
    uint8_t  EventAddress;
    uint8_t  EventCaption;
    uint8_t Zone;
	uint8_t	EventYear;       	//年                      						//0 ~ 255 
	uint8_t	EventMonth;      	//月                      						//1 ~ 12
	uint8_t	EventDay;        	//日                     						//1 ~ 31  
	
	uint8_t	EventHour;       	//时                     						//0 ~ 23  
	uint8_t	EventMinute;     	//分                     						//0 ~ 59  
	uint8_t	EventSecond;     	//秒                      						//0 ~ 59 
	 
	uint8_t reserve[20];      //预留
	
} SYSTEM_EVENT;
#pragma pack(1)
typedef struct
{
    char Caption[10]; //事件名称
    char Date[12];    //日期
    char Time[12];    //时间
    char Location[4];//位置
    char Zone;        //区位
    char address;
    char DeviceName[10];//设备名称
    
    
}EVENT_INF;

































#endif

