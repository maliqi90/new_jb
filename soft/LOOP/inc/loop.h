#ifndef __LOOP_H__
#define __LOOP_H__
#define  MSPERTICK    			10 
#include "stm32f10x.h"
#include "def.h"
typedef struct __loop
{
    u8 Defi;          // 回路定义表（详细见数据结构.DOC）
    u8 TNum;          // 该回路上的器件总数 (Total number)
    u8 RNum;          // 实际上该回路上的器件数(Real number)
    u8 HPNum;         // 要求优先扫描的器件个数(High Priority device number)
    /********************/
    u8 HPFirstAddr;   // 当前回路高优先级链表(单向)的首器件地址：初始化后就不再变化，直到下次重新初始化。
    u8 LPFirstAddr;   // 当前回路低优先级链表(循环)的首器件地址：转向高优先级器件时，这个变量被修改。
    u8 OldJmpNxdAddr; // 旧器件跳转点指向的下一个要扫的器件的地址
    u8 OldJmpPreAddr; // 旧器件跳转点刚刚扫完的这个器件的地址
    /********************/
    u8 OptingAddr;    // 当前回路上正在操作的器件号1~255。（直到下一个器件马上开始操作之前，这个值才可被修改）
    u8 RealOptingAddr;// 实际发送到回路线上的地址
    u8 DevLedTestAddr;// 器件点灯测试的地址
    u8 ScanedCount;   // 已经扫描的次数, 需要插入空扫时计数用，其它情况下不需要
    /********************/
    u8 LoopAnsStat;   // 回路回码状态指示
    u8 NeedCheckAddr; // 火警故障预警后需要重扫确认的器件地址 070413
    
    u8 BroadTimeHourVal;   // 待发给显示盘的时
    u8 BroadTimeMinuteVal; // 待发给显示盘的分
    u8 BroadCmdSendToDisp; // 待发往回路上显示盘的广播命令内容（0x0f后头的内容的头2位）
    u8 CmdBeforeBroadCmd;  // 广播命令之前的那条命令，必须记录待查，因为模块动作命令顺序不允许打断
    union {              // 器件操作过程中使用的一些状态标志
        u32 Val;
        struct {
        u32  RstCloseModu_flag  : 1;  // 上电时需要先给所有模块发关闭命令,此时本位为1
        u32  HaveLDDev_flag     : 1;  // 当前回路有模块需要联动时为1

        u32  AutoRegFinish_flag : 1;  // 自动登陆完成置位
        u32  AutoRegInited_flag : 1;  // 自动登陆初始化完成置位

        u32  LoopLedFlashOff_flag   : 1; // 整个回路如果是灭灯巡检则该位为1
        u32  OldJmpLedSolution_flag : 1; // 老模块向小于128的地址跳跃或者老探测器向大于128的
                                            // 地址跳跃时为1，否则为0。用于解决此时led闪灯时间长的问题
        u32  OldJmpLedGoNxt_flag: 1;     // 解决完点灯 1:返回下一个要发的地址; 0:还是不做理会

        u32  LPLineIsShort_flag : 1;  // 回路短路
        u32  ShortReported_flag : 1;  // 回路短路已上报
        u32  ShortPowerOff_flag : 1;  // 回路因短路关闭电源

        u32  OptFinish_flag : 1;     // 收发码操作结束标志
        u32  SendAddr0_flag : 1;     // 发0码时置1，指示后续程序不用处理回码结果
        u32  AnsCodeDealOk_flag : 1; // 回码处理结束标志
        u32  BeginCharge_flag   : 1; // 系统中只有一个回路时两次扫描之间需要充电10ms,开始充电标志
        u32  ChargeOK_flag      : 1; // 充电完成标志
                
        u32  SendBroadCastCmdToDisp_flag : 1; // 给本回路的显示盘发送广播命令，由主CPU
                                                 // 给回路发相关命令导致该位置位,命令执行完成后回路自己清位
        u32  InterruptByBroadCastCmd_flag: 1; // 如果回路打断扫描进程，则置该位通知扫描流程控制相关函数
        u32  LoopLoseReportType_flag: 1;      // 回路报丢失方式，为1时单个报无响应，为0时全部部分丢
            
        u32  NeedDownloadCfg_flag : 1;   // 回路需要被配置则置1，上电后置1，如果是软复位则清0 
        }StateBit;
    }OptStatFlags;
    /********************/
    u32 PWn[5];           // 保存码宽
    u32 SendBitsStream;   // 发码时的码流
    u32 LoopShortCntTick; // 短路后计算恢复时间使用，它直接保存系统tick值，通过减法判断时间
    /********************/
} LOOPS;

typedef struct __device
{   // 系统要求的数据结构
	u8 DeviceCondition;		//状态,按位定义：BIT0,1-存在，0-不存在；BIT1,1-隔离，0-正常；
							//BIT2:1-优先扫描0-不优先扫描BIT3:1-灭灯巡检0-亮灯巡检   
	u8 DeviceType;         // 器件类型
	u8 DeviceZone;         // 器件所在的区
	u8 DeviceFloor;        // 器件所在的层
	u16 DeviceBuilding;      // 器件所在的楼号    
	//	u16 DeviceAlarmValue;	//报警值,扩展值 					//		   
    /*******************/
    // 以下为自定义
    u8 Comd;         // 器件操作命令重新执行次数(D7~D4通常为0) + 命令内容(D3~D0)
    u8 NextAddr;     // 下一个操作的器件地址（同一回路的）。
    union {             // 操作过程中用到的一些状态标志
        u8 Val;
        struct {
        u8  Go2Prior_flag     : 1; // 下个操作转入优先链表
        u8  CircuitERR_flag   : 1; // 如果器件报了电路故障则置位，不处理它的回码
        u8  IsOldDev_flag     : 1; // 如果器件是老器件则置位
        u8  IsDuAddr_flag     : 1; // 当前器件重码
        u8  DisLoseErrAns_flag: 1; // 丢失恢复正常后请0
        } FlagBit;
    }OtherFlags;

    u8 ReScanStat1;   //器件重扫状态（各种重扫异常）
    u8 ReScanStat2;   //器件重扫状态（各种异常确认）
    u8 ReScanStat3;   //器件重扫状态（各种重扫异常）
    u8 ReScanStat4;   //器件重扫状态（各种异常确认）
	u8 ReScanStat5;   //    恢复标志
    u8 ReScanCnt;     //器件相同事件重扫次数计数。注意如果器件为模块则分为2个计数器高4作为输出模块
                      //的计数，低4作为输出模块的计数，所以为了避免计数溢出，要求重扫次数不得大于F

    u8  OtherFauCnt;  // 重码、类型错误计数，每个大循环扫描计数加一次，如果超过规定次数就确认并上报
    u32 LoseCntTicks; // 丢失不再使用以前的计次，而是使用计时的办法，这样报丢的时间很好控制，直接保存系统滴答
}LOOPDevice;

void LoopRstInit( void );

u16 get_loop_check(u8 loop);

void LoopCfgSaveToFlash(void);

/*
********************************************************************************
* 函数名称: LoopInSafeMode
* 函数功能: 回路进入安全模式，即停止扫描，停止对回路供电
* 参    数: 无
* 返 回 值: 无
********************************************************************************
*/
void LoopInSafeMode(BOOL stat);

/*
********************************************************************************
* 函数名称:LoopIsSafeMode
* 函数功能:是否为安全模式
* 参    数: 无
* 返 回 值: TRUE->安全模式FALSE->非安全模式
********************************************************************************
*/
BOOL LoopIsSafeMode(void);
void LoopLDCmdBufAdd(u8 buff[6]);
u8 GetLoopLDCmdAndExe(void);
void loop_handle(void);
extern uint8_t loop_login_flag;
extern void light_dev(uint8_t dat);
#endif

