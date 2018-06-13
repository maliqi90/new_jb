/*
********************************************************************************
*                             北京国泰怡安电子有限公司
* 地    址 ： 北京市丰台区杜家坎南路8号
* 邮    编 ： 100072
* 网    址 ： Http://www.guotaiyian.com
*
* 文 件 名 :  LoopSendOpt.c
* 编    制 :  白 瑜
* 创建时间 :  2006/11/16
* 描    述 :  回路回码操作：回码接收完毕前的所有工作在这个文件中处理。
*             主要是为了减少原来LoopOpt.c文件的行数，将原文件一分为二，
*             分为回码接收完前和接收完后，共同包含LoopOpt.h。
*             另外整合了原 mainC.c 和 linechk.c 文件中的内容。
*
* 注    意 ： 新回路程序因为硬件的不同，回码时先等到下降沿，然后是上升沿。
*             发24V时在检测管脚上的1电平表示正常，0电平表示短路。
*             发 5V时在检测管脚上的1电平表示正常，0电平表示重码。
* 修    改 ： 2008/02/25 移植到新回路卡上的修改。
*             主要改动发生在以下函数中：SendVolt();LoopShortChk()
*             LoopCardISR_Handler(); _DuplAddrChk()->del
********************************************************************************
*/
#include "loop.h"
//#include "can.h"
#include "loop_hw.h"
#include "LoopDataStruct.h"
#include "LoopOpt.h"
//#include "SysTick.h"
#include "LoopDevTables.h"
//#include "communication.h"
#include "DeviceTypeDefine.h"
#include "def.h"
#include "event.h"
/******************************************************************************/
extern u16 dev_addr; 
extern const u8 DEVICE_IO_TYPE[256];

 LOOPS LOOP[2];               			// 定义回路结构型回路变量。
LOOPDevice ALLDevices[2][256]; 			// 每个回路上256个回路器件型结构空间
u8  OptingLoopNO;                   	// 当前正在操作的回路号0，1
u32 CheckTypeTimeTicks;             	// 回路定期做器件类型检查，此变量是用于这个事情的时间计数器
u8  LDList[2][256];            			// 器件联动表
u8	auto_res_ln;						//自动登录回路号

int LDNode[2];						  	//当前联动的现场部件编号

BOOL   AutoRegister_flag = 0;         	// 自动登陆时置位，登陆完成清0
BOOL   LoopCanScan_flag  = 0;         	// 控制回路是否可以扫描

/******************************************************************************/
static u8  AnsCodeCount;     // 不同类型的器件在不同命令的情况下应该回码的个数

/*
********************************************************************************
* 函数名称: void LoopListInit( u8 LN );
* 函数功能: 回路链表的初始化，系统接收到主机相关配置或被修改相关配置后调用
* 参    数: u8 LN :回路号 0,1
* 返 回 值: 无
* 注    意:
********************************************************************************
*/
static u8 LoopListFirstInit_flag[2] ={1,1}; // 20071012增加，修改器件配置后初始化了LDlist，导致宏逻辑重复启动

void LoopListInit( u8 LN )
{
    u32 i, j;
    //为1时表示扫描配置表找到的是第一个普通链表回路中的器件
    BOOL firstFindLPAddr_flag = 1, firstFindHPAddr_flag = 1;
    u8   firstFindLPAddr = 0, 	   firstFindHPAddr;
    u8   LPAddrNum = 0, 		   HPAddrNum = 0;
    u8   temp;
    u8   InsIntv;

    // 初始化本回路的普通链表(循环结构)和优先扫描链表(单向结构)
    LOOP[LN].HPFirstAddr = 0;
    LOOP[LN].LPFirstAddr = 0;
    LOOP[LN].RNum 		 = 0;   
    LOOP[LN].TNum 		 = 0;

    for (i = 1; i <= 16; i++) 
    {
        if (ALLDevices[LN][i].DeviceCondition & (1 << 0)) 				// 器件存在 
        {
            if (!(ALLDevices[LN][i].DeviceCondition & (1 << 1)))  		// 器件没有被屏蔽
            {
                if (!(ALLDevices[LN][i].DeviceCondition & (1 << 2)))	//优先扫描
                {
                    // 使用当前器件构造普通链表
                    if (firstFindLPAddr_flag) 
                    {
                        firstFindLPAddr_flag = 0;
                        firstFindLPAddr      = i;
                        LOOP[LN].LPFirstAddr = i;
                    } 
                    else 
                    {
                        ALLDevices[LN][i].NextAddr = LOOP[LN].LPFirstAddr;
                        LOOP[LN].LPFirstAddr = i;
                    }
                    LPAddrNum ++; // 普通链表中器件的个数增加
                } 
                else 
                {
                    // 使用当前器件构造高优先级扫描链表
                    if (firstFindHPAddr_flag) 
                    {
                        firstFindHPAddr_flag = 0;
                        firstFindHPAddr = i;
                    }
                    ALLDevices[LN][i].NextAddr = LOOP[LN].HPFirstAddr;
                    LOOP[LN].HPFirstAddr = i;
                    HPAddrNum ++; // 优先扫描链表中器件的个数增加
                }
                // 当前器件的一些属性有必要在此处初始化
                //ALLDevices[LN][i].OtherFlags.Val = 0; 这里有2个标志不能随便清除
                ALLDevices[LN][i].OtherFlags.FlagBit.Go2Prior_flag = 0;
                ALLDevices[LN][i].OtherFlags.FlagBit.IsOldDev_flag = 0;
                ALLDevices[LN][i].OtherFlags.FlagBit.IsDuAddr_flag = 0;
                // 0x0a命令只有复位后才可以清0
                if (0x0a != (0x0f & ALLDevices[LN][i].Comd))
                {
                    ALLDevices[LN][i].Comd = XJCMD(LN, i);
                }// 命令都要恢复为00或0d，火警已经确认的为0A
            }// 器件没有被屏蔽
        }// 器件存在
    }// 查找所有255个器件

    /*******************用器件配置表构造扫描链表完成****************************/
    // 计算有效器件个数
    LOOP[LN].TNum  = HPAddrNum + LPAddrNum;
    LOOP[LN].RNum  = LOOP[LN].TNum;
    LOOP[LN].HPNum = HPAddrNum;
    if (LOOP[LN].TNum < MIN_DEVICE_NUM) // 器件个数太少需要插入空扫
    { 
        LOOP[LN].ScanedCount = 0;          // 此处初始化空扫计数器为0
    }

    /**************************************************************************/
    if (1 == LoopListFirstInit_flag[LN])// 程序上电第一次调用本函数时初始化
    { 
        LoopListFirstInit_flag[LN] = 0;
        LOOP[LN].OptStatFlags.Val = 0;
        LDNode[LN] = -1;  // 器件扫描链表被修改了自然联动动作链表也要被清除以免乱动做
        
        for (i = 0; i <= 255; i++) 
        {
            LDList[LN][i] = 0;
        }
    }
    /**************************************************************************/
    if (LPAddrNum > 0)  // 普通链表的器件个数不为0才有意义
    {        
        // 闭合普通链表成为环形
        ALLDevices[LN][firstFindLPAddr].NextAddr = LOOP[LN].LPFirstAddr;
        LOOP[LN].OptingAddr    = LOOP[LN].LPFirstAddr;
        LOOP[LN].OldJmpPreAddr = LOOP[LN].LPFirstAddr;
    } 
    else if (HPAddrNum > 0) // 有器件且所有器件都想优先扫描
    {   //此时LOOP[LN].LPFirstAddr = 0; 且LOOP[LN].RNUM != 0;所以优先扫描的单向
        //链表遇到NULL节点后直接转入他自己的首器件处扫描，其实是把它也给闭合成
        //一个循环链表了，此处直接闭合，去掉了单向链表结尾的NULL。
        LOOP[LN].LPFirstAddr = LOOP[LN].HPFirstAddr;
        ALLDevices[LN][firstFindHPAddr].NextAddr = LOOP[LN].HPFirstAddr;
        LOOP[LN].OptingAddr    = LOOP[LN].LPFirstAddr;
        LOOP[LN].OldJmpPreAddr = LOOP[LN].LPFirstAddr;
        LOOP[LN].HPNum = 0;  //已经成为闭环，所以认为都是普通优先级的器件

        if( LOOP[LN].Defi & (1 << 1))  // 回路被屏蔽
        {
            LOOP[LN].RNum = 0;
            LOOP[LN].OptingAddr = 0;
        }
        return; // 回路中全是高优先级器件且此处已完成扫描链表初始化
    } 
    else  
    	return; // 回路中没有器件，将来直接扫0码，不管链表

    /**************************************************************************/
    // 计算优先扫描链表在普通链表中的插入点，并在插入点处标明。
    if( LPAddrNum <= PRIOR_SCAN_TIMES )// 如果普通链表中的器件个数比优先扫描次数还少,则
    {                                  // 每个器件扫描完成后，都要转入优先扫描链表中去
        temp = LOOP[LN].LPFirstAddr;
        for( i = 0; i < LPAddrNum; i++ ) 
        {
            ALLDevices[LN][temp].OtherFlags.FlagBit.Go2Prior_flag = 1; // 置转向优先链表标志
            temp  = ALLDevices[LN][temp].NextAddr;             // 取环形普通链表下一个器件地址
        }
    } 
    else 
    {
        InsIntv = LPAddrNum / PRIOR_SCAN_TIMES;   // 插入优先扫描链表的间隔
        temp = LOOP[LN].LPFirstAddr;
        for( i = 0; i < PRIOR_SCAN_TIMES; i++) 
        {
            for( j = 0; j < InsIntv; j++) 
            {
                temp = ALLDevices[LN][temp].NextAddr;
            }
            ALLDevices[LN][temp].OtherFlags.FlagBit.Go2Prior_flag = 1;  // 置转向优先链表标志
        }
    }
	
//    if (LOOP[LN].Defi & (1 << 1)) 
//    {
//        LOOP[LN].RNum = 0;
//        LOOP[LN].OptingAddr = 0;
//        return;
//    } //回路被屏蔽

    LOOP[LN].OptingAddr = LOOP[LN].LPFirstAddr;
}

/*
********************************************************************************
* 函数名称: BOOL LoopShortChk(u8 LN);
* 函数功能: 负责回路短路后重新启动回路
* 参    数: u8 LN   : 回路号 0,1
* 返 回 值:无
* 注    意: 
********************************************************************************
*/
void LoopShortChk(u8 LN)
{
    u32 timer;
    uint8_t send_buff[7];
    if (LOOP[LN].OptStatFlags.StateBit.ShortPowerOff_flag) // 回路LN已经因短路关闭电源
    { 
        timer = GetTickCount();
        
        // 短路时间超过规定值
        if ((timer - LOOP[LN].LoopShortCntTick) > (WAIT_RECOVER_TIMERS * 1000 / MSPERTICK)) 
        {
        	LOOP[LN].OptStatFlags.StateBit.ShortPowerOff_flag = 0;   // 开始回路供电恢复过程
//            LOOP[LN].OptStatFlags.StateBit.LPLineIsShort_flag = 0;
//            LOOP[LN].LoopShortCntTick = 0;
					//LOOP[1].OptStatFlags.StateBit.ShortReported_flag = 0;
			    LOOP[LN].LoopShortCntTick = timer;
          loop_power_on(LN);		//回路上电
        }
    }
    else
    {
		 if (!LOOP[LN].OptStatFlags.StateBit.ShortPowerOff_flag && LOOP[LN].OptStatFlags.StateBit.LPLineIsShort_flag)
		{
			timer = GetTickCount();

			if ((timer - LOOP[LN].LoopShortCntTick) > (2 * WAIT_RECOVER_TIMERS * 1000 / MSPERTICK))		//20s秒
			{
				LOOP[LN].OptStatFlags.StateBit.LPLineIsShort_flag = 0;
			}
		}
    }
}

/*
********************************************************************************
* 函数名称: void SetLDList(u8 B5, u8 B6, u8 B7);
* 函数功能: 设置回路上器件联动表,参数为矩阵输出协议的字节5，6，7。
* 参    数: 输出项的字节5，6，7，字节4无用
* 返 回 值: 无
* 注    意:
********************************************************************************
*/
void SetLDList(u8 category, u16 controllerID, u8 loop, u8 address, u8 control_type)
{
    u8   temp, tempAddr, tempDevNum, HPorLP;
    int  i;
    u8   LN;
    BOOL BuildPass, TypePass, Zo_Fl_Ma_Pass;  //楼号符合、类型符合、区层控制器号符合
/*
	1	分类	LinkageOutCategory
	2	机器号/楼号/组号	LinkageOutControllerID
	3		
	4	回路号/区号/层号	LinkageOutLoop
	5	地址号/类型 LinkageOutAddress
	6	控制方式	LinkageOutControlType
*/
	if (category == 0)			//地址
	{
		if (loop == 0 && address == 0)
		{
			LDList[0][0] = (control_type & 0xc0) | 0x01;  		  //新命令，且新命令还没有执行
            LOOP[0].OptStatFlags.StateBit.HaveLDDev_flag = 1; 	  //有需要联动的器件

            LDList[1][0] = (control_type & 0xc0) | 0x01;  //新命令，且新命令还没有执行
            LOOP[1].OptStatFlags.StateBit.HaveLDDev_flag = 1; 	  //有需要联动的器件
		}
		else
		{
			if ((dev_addr - loop) == 7)
			{
				LN = 0;
			}
	
			if ((dev_addr - loop) == 6)
			{
				LN = 1;
			}
			
			if (LN == 0 || LN == 1) 	//是本回路卡的回路
			{
				if (!(ALLDevices[LN][address].DeviceCondition & (1 << 1)))
				{
					//联动表单元格式：D7(电平或脉冲)+D6(打开或关闭)+D1(联动已经执行)+D0(联动还未执行)
					temp = ALLDevices[LN][address].DeviceType;
	
					if (DEVICE_IO_TYPE[temp] == 0x01 || DEVICE_IO_TYPE[temp] == 0x11)
					{
						temp = LDList[LN][address] ^ control_type;
	
						//动作类型改变，脉冲方式电平方式处理是一致的
						if ((temp & 0x40) || (0 == LDList[LN][address]))
						{
							 LDList[LN][address] = (control_type & 0xc0) | 0x01;		// 新命令，且新命令还没有执行
							 LOOP[LN].OptStatFlags.StateBit.HaveLDDev_flag = 1; 		// 有需要联动的器件标识
						}
					}
				}
			}
		}		
	}
    else if ((0x01 == category) || (0x02 == category) || (0x03 == category))//(区号/层号/控制器) + 模块类型（多地址联动 ）
    { 
        for (LN = 0; LN < 2; LN++) //从两个回路中寻找
        {
            for (HPorLP = 0; HPorLP < 2; HPorLP++) //从当前回路高低优先级链表中寻找
            {
                if (0 == HPorLP) //先扫描高优先级链表
                {
                    tempAddr   = LOOP[LN].HPFirstAddr; 
                    tempDevNum = LOOP[LN].HPNum;
                }
                else              //再扫描普通链表
                {
                    tempAddr   = LOOP[LN].LPFirstAddr; 
                    tempDevNum = LOOP[LN].TNum - LOOP[LN].HPNum;
                }

                for (i = 0; i < tempDevNum; i++)//从某优先级链表中寻找
                {
                    BuildPass = 0;   
                    TypePass = 0;   
                    Zo_Fl_Ma_Pass = 0;
                    
                    //   楼号为0    或  楼号相符
                    if ((controllerID == 0) || (controllerID == ALLDevices[LN][tempAddr].DeviceBuilding))
                    { 
                        BuildPass = 1;
                    }
                    //区层控制器号也一样，为0或相符则表示符合条件
                    if (0x01 == category)       //区号
                    {
                        if ((loop == 0) || (loop == ALLDevices[LN][tempAddr].DeviceZone))
                        	Zo_Fl_Ma_Pass = 1;
                    }
                    else if (0x02 == category)  //层号
                    {
                        if ((loop == 0) || (loop == ALLDevices[LN][tempAddr].DeviceFloor))
                        	Zo_Fl_Ma_Pass = 1;
                    }
                    else                     //控制器号
                    {
                        Zo_Fl_Ma_Pass = 1;
                    }
                    //然后是类型检查
                    temp = ALLDevices[LN][tempAddr].DeviceType;
                    
                    if (is_detector(temp))		//不是模块不理会
                    { 
                    }                               
                    else if (ALLDevices[LN][tempAddr].DeviceCondition & (1 << 1)) //被屏蔽不理会
                    {
                    }
                    else if (temp == address)   	//类型相符
                    {   
                    	TypePass = 1;   
                    }
                    else if (address == DEVICE_MODULE)	//类型不符但是命令指定所有输出模块
                    {
                        if (DEVICE_IO_TYPE[temp] == 0x01 || DEVICE_IO_TYPE[temp] == 0x11)
                        {   
                        	TypePass = 1;   
                    	}  
                    }
                   
                    if (BuildPass && TypePass && Zo_Fl_Ma_Pass) //各种条件都符合
                    {
                        //联动表单元格式：D7(电平或脉冲)+D6(打开或关闭)+D1(联动已经执行)+D0(联动还未执行)
                        temp = LDList[LN][tempAddr] ^ control_type;

                        if ((temp & 0x40) || (0 == LDList[LN][tempAddr]))//动作类型改变，脉冲方式电平方式处理是一致的
                        {
                            LDList[LN][tempAddr] = (control_type & 0xc0) | 0x01;  	//新命令，且新命令还没有执行
                            LOOP[LN].OptStatFlags.StateBit.HaveLDDev_flag = 1; 		//有需要联动的器件
                        }
                    }

                    tempAddr = ALLDevices[LN][tempAddr].NextAddr;
                }
            }
        }
    }
    else
    {   
    	return;   
    }

    /**************联动表已经根据命令重置，最后再做一下统计*****************/
    //联动表每一维数组的第0个元素存放联动表指针，从255递减，为0表示当前
    //回路没有器件联动，如果2个回路都没有器件联动就要清HaveLDDev_flag
    if (LOOP[0].OptStatFlags.StateBit.HaveLDDev_flag || LOOP[1].OptStatFlags.StateBit.HaveLDDev_flag)
    {
        for (LN = 0; LN < 2; LN++)
        {
            for (i = 255; i >= 0; i--) //从高地址向低地址查找还没有执行的第一个联动器件
            {
                //D7(电平或脉冲)+D6(打开或关闭)+D1(联动已经执行)+D0(联动还未执行)
                if (LDList[LN][i] & bit0)    //D0(联动还未执行),
                {
                    LDNode[LN] = i;  
                    break;
                } 
            }

            if (i < 0)
            {
				LDNode[LN] = -1;
            }
        }
    }

    /*********最后如果某个回路被屏蔽，那么联动指针为0，不指向任何器件*********/
    for (LN = 0; LN < 2; LN++) 
    {
        if (LOOP[LN].Defi & (1 << 1)) 
        {
            LDNode[LN] = -1;
            
            for (i = 255; i >= 0; i--) 
            {
                LDList[LN][i] &= 0xc0;
            }
        }
    }
}

/*
********************************************************************************
* 函数名称: static u8 _FindLDListNext( u8 LN );
* 函数功能: 查找某回路联动表中下一个要联动的器件
* 参    数:LN 回路号0，1   addr:返回有联动的部件地址
* 返 回 值:0表示没有器件要联动1表示有器件要联动
* 注    意:
********************************************************************************
*/
static bool FindLDListNext(u8 LN, u8 *addr)
{
    int temp;
    
    if (LDNode[LN] != -1) 
    {
		temp = LDNode[LN];
		
        for ( ; temp >= 0; temp--) 
        {
            LDNode[LN] = temp - 1;   //下次进来从这个地址的前一个地址开始判断
            if (temp == -1)
            	LDNode[LN] = 255;
            	
            //D7(电平或脉冲)+D6(打开或关闭)+D1(联动已经执行)+D0(联动还未执行)
            if (LDList[LN][temp] & bit0)  //D0(联动还未执行)
            {
                //为了容错，再次判断获取的地址处有没有器件，有没有被屏蔽
                if ((ALLDevices[LN][temp].DeviceCondition & (1 << 0)) && (!(ALLDevices[LN][temp].DeviceCondition & (1 << 1))))
                {
                    if (LDList[LN][temp] & bit7)  //输出脉冲
                    {
                        if (LDList[LN][temp] & bit6) //关闭
                        {
                            ALLDevices[LN][temp].Comd = 0x37; // 本次就算一次，所以不应为3次
                        } 
                        else             //打开
                        {           
                            ALLDevices[LN][temp].Comd = 0x38;
                        }
                    } 
                    else 
                    {                        //输出电平
                        if (LDList[LN][temp] & bit6) //关闭
                        {                        
                            ALLDevices[LN][temp].Comd = 0x37;
                        } 
                        else                     //打开
                        {   
                            ALLDevices[LN][temp].Comd = 0x36;
                        }
                    }
                    
                    ClrBit(LDList[LN][temp], bit0);  //转去执行之前置联动已执行标志
                    SetBit(LDList[LN][temp], bit1);  //即使没有执行成功也不会再执行了

                    *addr = temp;
                    
                    return TRUE;
                }
            }
        }
        return FALSE;
    }
    else 
    {
        LOOP[LN].OptStatFlags.StateBit.HaveLDDev_flag = 0;

        return FALSE;
    }
}

/*
********************************************************************************
* 函数名称: static u8 GetNextDevAddr( u8 LN );
* 函数功能: 获得某回路链表中下一个即将扫描的器件的地址
* 参    数: u8 LN :回路号 0,1
* 返 回 值: 器件地址（不为0）或0(无器件可扫或空扫）
* 注    意:
********************************************************************************
*/
static u8 GetNextDevAddr(u8 LN)
{
    u8 tempAddr;

    // 首先处理以下几种特殊情况：
    /************************************************/
    // A:回路上有老器件存在时，在255点版本中扫描地址跨越128时，
    //   需要专门扫描0或128地址该器件的灯才熄灭，否则长时间亮。
    if (LOOP[LN].OptStatFlags.StateBit.OldJmpLedSolution_flag && (!AutoRegister_flag))  
    {
    	  LOOP[LN].OptStatFlags.StateBit.OldJmpLedSolution_flag = 0;
    	  
    	  if( LOOP[LN].OptStatFlags.StateBit.OldJmpLedGoNxt_flag ) 
          {
    	  	  LOOP[LN].OptStatFlags.StateBit.OldJmpLedGoNxt_flag = 0;
    	  	  return( LOOP[LN].OldJmpNxdAddr );
    	  }
    	  LOOP[LN].OptStatFlags.StateBit.OldJmpLedGoNxt_flag = 0;
    }
    /************************************************/
    //B: 回路上没有器件或回路被屏蔽后这个值被清0
    if (LOOP[LN].RNum == 0)    
    	return 0;   //发0码空扫
    /************************************************/
    //C: 回路上有器件
    tempAddr = LOOP[LN].OptingAddr;
    //0，是否扫描被发给显示盘的广播命令打断过 2007-8-28 add
    if (LOOP[LN].OptStatFlags.StateBit.InterruptByBroadCastCmd_flag)  
    {
        LOOP[LN].OptStatFlags.StateBit.InterruptByBroadCastCmd_flag = 0;
        return tempAddr;
    }
    
    //1，要先判断有没有器件要求重复扫描
    if (ALLDevices[LN][tempAddr].Comd & 0xf0)  
    {
        //要求的重新扫描次数不为0，则回去重扫当前器件
        ALLDevices[LN][tempAddr].Comd -=0x10;
        return tempAddr;
    }
    
    //2,是否是自动登陆状态
    if (AutoRegister_flag) 
    {    	
        LOOP[LN].OptingAddr =  ALLDevices[LN][tempAddr].NextAddr;
        return( LOOP[LN].OptingAddr );
    }
    
    //3,是否有器件需要联动
    if( 0 == LOOP[LN].OptStatFlags.StateBit.HaveLDDev_flag )
    {
        GetLoopLDCmdAndExe();
    }
    if ( LOOP[LN].OptStatFlags.StateBit.HaveLDDev_flag ) 
    {
        if (FindLDListNext(LN, &tempAddr))		//有联动 
        {
            LOOP[LN].OptingAddr = tempAddr;
            return tempAddr;
        }
        else 
        {
        	LOOP[LN].OptStatFlags.StateBit.HaveLDDev_flag = 0;
        }
    }

    if( LOOP[LN].OptStatFlags.StateBit.RstCloseModu_flag ) 
    {
        if( !LOOP[LN].OptStatFlags.StateBit.HaveLDDev_flag ) 
        {
            LOOP[LN].OptStatFlags.StateBit.RstCloseModu_flag = 0;
        } // 如果没有器件需要联动说明当前回路上电关闭模块的动作已完成
    }
    
    //4,每隔500ms检查有没有处于火警故障延时的器件，如果有,检查事件延时是否到，到了就要转去处理
    // 没有等待确认火警、故障的器件，转回扫描链表继续扫描
    // 注意：新版程序中取消了火警故障延时。
    if( (0 != LOOP[LN].NeedCheckAddr) && (!AutoRegister_flag) ) 
    {  
        tempAddr = LOOP[LN].NeedCheckAddr;
        LOOP[LN].NeedCheckAddr = 0;
        return tempAddr;
    }
    
    //5,每隔一定时间就进行类型检测，程序走到此处应该是以上各种情况都处理完后才会过来
    if ((GetTickCount() - CheckTypeTimeTicks) > (600000 / MSPERTICK)) // 每隔10分钟做一次类型检查
    {
        CheckTypeTimeTicks = GetTickCount();
        
        for (tempAddr = 255; tempAddr > 0; tempAddr-- ) //类型检测命令
        {
            ALLDevices[0][tempAddr].Comd = 0x15 + (DA_TE_CNT_TIMES << 4);
            ALLDevices[1][tempAddr].Comd = 0x15 + (DA_TE_CNT_TIMES << 4);
        }
    }
    
    //6,处理回路上器件总数太少的情况，此时扫一圈时间很短，器件看起来像常亮。
    //  次处人为的插入0码，这样使扫一圈的时间不是太短。
    if (LOOP[LN].RNum < MIN_DEVICE_NUM) 
    {
        // 这个计数 = 空扫次数 + 器件扫描的次数(正常情况下就是已扫描的器件个数)，
        // 他大于MIN_DEVICE_NUM后，计数重来。
        LOOP[LN].ScanedCount ++;
        if(LOOP[LN].ScanedCount >= LOOP[LN].RNum)  // 扫描了全部器件，开始插入空扫
        {
            if( LOOP[LN].ScanedCount >= MIN_DEVICE_NUM ) 
            {
                LOOP[LN].ScanedCount = 0;
            } 
            else 
            {
                // 如果器件不足MIN_DEVICE_NUM个,且最后一个为老模块,解决点灯时间长
                if (LOOP[LN].ScanedCount == LOOP[LN].RNum) 
                {
                    tempAddr = LOOP[LN].OptingAddr;
                    if ((0x08 <= ALLDevices[LN][tempAddr].DeviceType) && (tempAddr > 128)) 
                    { // 此时有可能是老模块
                        LOOP[LN].OptStatFlags.StateBit.OldJmpLedSolution_flag = 1;
                        LOOP[LN].OptStatFlags.StateBit.OldJmpLedGoNxt_flag    = 0;
                        return 128; //返回旧模块的0地址
                    }
                } // 点灯解决完成
                return 0; //空扫
            }
        }
    } // 否则应该扫描回路上存在的器件，往下走
    /**************************************************************************/
    /* 以上为非正常扫描，不修改LOOP[LN].OptingAddr，扫描完毕后还从            */
    /* LOOP[LN].OptingAddr处开始扫描                                          */
    /*******************以下为正常扫描地址获取*********************************/
    tempAddr = LOOP[LN].OptingAddr;
    tempAddr = ALLDevices[LN][tempAddr].NextAddr;

    if (0 == tempAddr) // NULL,说明是单向链表的结尾
    { 
        LOOP[LN].OptingAddr = LOOP[LN].LPFirstAddr;  //转回循环链表断点处
        return ( LOOP[LN].OptingAddr );
    } 
    else //除单向链表结尾外的其他链表节点，该节点指向的下一个器件地址有效 
    {
        tempAddr = LOOP[LN].OptingAddr;
        // 判断是否从此处转入单向链表(高优先级器件链表)中扫描
        if (ALLDevices[LN][tempAddr].OtherFlags.FlagBit.Go2Prior_flag) 
        {
            if (LOOP[LN].HPNum > 0) // 高优先级的器件存在
            { 
                LOOP[LN].LPFirstAddr = ALLDevices[LN][tempAddr].NextAddr;  //保存从单向链表中返回的器件地址
                LOOP[LN].OptingAddr  = LOOP[LN].HPFirstAddr;
                return ( LOOP[LN].OptingAddr );
            }
        }

        LOOP[LN].OptingAddr  = ALLDevices[LN][tempAddr].NextAddr;
        
        return LOOP[LN].OptingAddr;
    }
}

/*
********************************************************************************
* 函数名称: void DevOptInitAndRun(u8 LN);
* 函数功能: 某回路中下一个器件操作之前的过程变量等的初始化和启动操作
* 参    数: u8 LN ：回路号 0,1
* 返 回 值: 无
* 注    意:
********************************************************************************
*/
void DevOptInitAndRun(u8 LN)
{
    u8 j,tempAddr;
    u8 addr, cmd;

    /**********************************************/
    OptingLoopNO = LN;                 // 要操作的回路号
    tempAddr     = GetNextDevAddr(LN); // 要操作的地址号
    /**********************************************/
    if(tempAddr == 6)
    {
        j = 6;
    }
    // 开始解决旧器件闪灯问题
    if ((LDNode[LN] != -1) || AutoRegister_flag)// 不做闪灯问题的处理的情况 
    { 
        LOOP[LN].OldJmpPreAddr = tempAddr;
    } 
    else 
    {
        j = LOOP[LN].OldJmpPreAddr;
        if (((j < 128) && (tempAddr> 128)) || ((j > 128) && (tempAddr < 128))) // 高低地址跳转
        { 
            if (ALLDevices[LN][j].OtherFlags.FlagBit.IsOldDev_flag) // 且前一个器件是老器件
            { 
                LOOP[LN].OptStatFlags.StateBit.OldJmpLedSolution_flag = 1;
                LOOP[LN].OptStatFlags.StateBit.OldJmpLedGoNxt_flag    = 1;
                LOOP[LN].OldJmpNxdAddr = tempAddr; // 保存待会儿要接着扫描的器件地址

                if (j > 128) 
                {
                    tempAddr = 128;                // 发旧模块空扫
                    LOOP[LN].OldJmpPreAddr = 0;    // 下次不再进来
                } 
                else 
                {
                    tempAddr = 0;                  // 发旧探测器空扫
                    LOOP[LN].OldJmpPreAddr = 129;  // 下次不再进来
                }
            }
        } 
        else  
        {
        	LOOP[LN].OldJmpPreAddr = tempAddr;
        }
    } // 旧器件点灯解决完毕

    /**********************************************/
    // 点灯测试处理，旧程序一个回路卡允许点一个灯，
    //此处允许一个回路点一个灯
    // 判断被点灯测试的器件是否是当前器件
    if ((0 != LOOP[LN].DevLedTestAddr ) && (LOOP[LN].DevLedTestAddr == tempAddr)) 
    {
        ALLDevices[LN][tempAddr].Comd &= 0xf0;
        ALLDevices[LN][tempAddr].Comd += 0x0a; //发点灯命令
    }
    /**********************************************/
    LOOP[LN].RealOptingAddr = tempAddr;
    LOOP[LN].OptStatFlags.StateBit.OptFinish_flag = 0;
    LOOP[LN].LoopAnsStat = 0;  // 清回码状态字节

    if (0 == tempAddr) 
    	LOOP[LN].OptStatFlags.StateBit.SendAddr0_flag = 1; // 指示发的是0码
    else                
    	LOOP[LN].OptStatFlags.StateBit.SendAddr0_flag = 0;

    addr = tempAddr;
    cmd  = ALLDevices[LN][tempAddr].Comd & 0x0f;

    // 需插入发给总线火显的广播命令
    if( LOOP[LN].OptStatFlags.StateBit.SendBroadCastCmdToDisp_flag )
    {
        if ((LOOP[LN].CmdBeforeBroadCmd >= 0x09) ||     // 非动作命令，或一个动作命令刚执行完
            (LOOP[LN].CmdBeforeBroadCmd <  0x06)) 	// 此时可以发广播命令
        { 
            if ( 0 == ( LOOP[LN].BroadCmdSendToDisp >> 6 ) )
            { // 校时发小时
                j = LOOP[LN].BroadTimeHourVal;
                LOOP[LN].BroadCmdSendToDisp = 1 << 6;
                LOOP[LN].OptStatFlags.StateBit.SendBroadCastCmdToDisp_flag = 1; // 准备送分
            } 
            else if( 1 == ( LOOP[LN].BroadCmdSendToDisp >> 6 ) ) 
            { // 校时发分钟
                j = ( 1 << 6 ) + LOOP[LN].BroadTimeMinuteVal;
                LOOP[LN].OptStatFlags.StateBit.SendBroadCastCmdToDisp_flag  = 0;
            } 
            else 
            { // 其他广播命令，消音自检
                j = LOOP[LN].BroadCmdSendToDisp;
                LOOP[LN].OptStatFlags.StateBit.SendBroadCastCmdToDisp_flag  = 0;                
            }
            //FillLoopBit( LN, j, 0x0f ); // 广播=0x0f+val
            addr = j;
            cmd  = 0x0f;
            LOOP[LN].OptStatFlags.StateBit.InterruptByBroadCastCmd_flag = 1;
        } 
        else if(((ALLDevices[LN][tempAddr].Comd & 0x0f) >= 0x06)&&((ALLDevices[LN][tempAddr].Comd & 0x0f) <= 0x09))
        { // 动作类命令不得打断
            LOOP[LN].CmdBeforeBroadCmd = ALLDevices[LN][tempAddr].Comd & 0x0f;
        }
    } 
    else    
    	LOOP[LN].CmdBeforeBroadCmd = ALLDevices[LN][tempAddr].Comd & 0x0f;

    for (j = 0; j < 5; j++) 
    {
        LOOP[LN].PWn[j] = 0;
    }

    //获得不同类型的器件在不同命令的情况下应该回码的个数
    if (is_detector(ALLDevices[LN][tempAddr].DeviceType))  					 // 探测器
    {
        AnsCodeCount = AnsCodeNUM[0][0x0f & ALLDevices[LN][tempAddr].Comd];  //回码计数
    }
    else 											// 模块 
    {
         AnsCodeCount = AnsCodeNUM[1][0x0f & ALLDevices[LN][tempAddr].Comd];  //回码计数
    }
//    if (LN == 1)
//    {
//        LN = 1;
//        addr = 40;
//        tempAddr = 40;
//        AnsCodeCount = 4;
//        ALLDevices[LN][tempAddr].Comd = 0x05;
//        loop_send(1,40,0x05,4);    
//    }
//    else
        loop_send(LN, addr, cmd, AnsCodeCount);
    
    return;
}

/*
********************************************************************************
* 函数名称: AutoRegInit
* 函数功能: 回路自动登陆前初始化工作。
* 参    数: ln_no:BIT0 本卡第一回路BIT1 本卡第二回路
* 返 回 值: 无
********************************************************************************
*/
void AutoRegInit(u8 ln_no)
{
    u32 i, j;

    auto_res_ln = ln_no;

    for (j = 0; j < 2; j++) 
    {
        for (i = 255; i > 0; i--) 
        {
            ALLDevices[j][i].DeviceCondition = 0;  //重新登陆后,屏蔽,优先,灭灯无效
            ALLDevices[j][i].DeviceType      = 0xff;
            //ALLDevices[j][i].OtherFlags.Val = 0; 这里有2个标志不能随便清除
            ALLDevices[j][i].OtherFlags.FlagBit.Go2Prior_flag = 0;
            ALLDevices[j][i].OtherFlags.FlagBit.IsOldDev_flag = 0;
            ALLDevices[j][i].OtherFlags.FlagBit.IsDuAddr_flag = 0;
            ALLDevices[j][i].Comd           = 0x05;
            ALLDevices[j][i - 1].NextAddr   = i;
            ALLDevices[j][i].ReScanStat3    = 0;
            ALLDevices[j][i].ReScanStat4    = 0;
            ALLDevices[j][i].ReScanStat5   &= 0xf3;
        }
        
        ALLDevices[j][255].NextAddr = 1;
        ALLDevices[j][0].Comd       = 0x05;

        LOOP[j].OptingAddr = 0;
        LOOP[j].RNum       = 255;
        LOOP[j].TNum       = 255;
    }
}

/*
********************************************************************************
* 函数名称: u8  XJCMD(u8 LN, u8 Addr);
* 函数功能: 获取适用于某个回路上某个器件的巡检命令。
* 参    数: u8  LN   : 0,1
*           u8  Addr : 1~255
* 返 回 值: 1个字节的巡检命令。如果报过火警   返回0x0a
*                              没有火警且灭灯 返回0x0d
*                              其它正常情况   返回0x00
********************************************************************************
*/
u8  XJCMD(u8 LN, u8 Addr)
{
    if (ALLDevices[LN][Addr].ReScanStat2 & (DevIsFireStat | DevIsOutFire))
        return 0x0a;
    else
        return ((LOOP[LN].OptStatFlags.StateBit.LoopLedFlashOff_flag ||
                (ALLDevices[LN][Addr].DeviceCondition & (1 << 3)))?0x0d:0x00);
}
void light_dev(uint8_t dat)
{
    LOOP[1].DevLedTestAddr = dat;
}

