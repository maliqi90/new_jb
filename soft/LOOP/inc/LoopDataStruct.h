/*
********************************************************************************
*                             北京国泰怡安电子有限公司
* 地    址 ： 北京市丰台区杜家坎南路8号
* 邮    编 ： 100072
* 网    址 ： Http://www.guotaiyian.com
*
* 文 件 名 :  LoopDataStruct.h
* 编    制 :  白 瑜
* 创建时间 :  2006/11/16
* 描    述 :  回路程序用到的数据结构集中在这个文件中。
*
* 注    意 ： 数据结构针对32位结构的CPU体系结构做了优化，由于指令执行速度不再是
*             瓶颈，同时CPU不具备51系列位变量区，这里将老回路卡程序的位变量整合
*             成位域，突出了数据结构的按功能分类。
********************************************************************************
*/
#ifndef __LOOPDATASTRUCT_H__
#define __LOOPDATASTRUCT_H__

/*******************************************************************************/
/********************************************************************************
*                        以下是回路上单个器件的数据结构                         *
* 火警、故障延时被取消                                                          *
********************************************************************************/
#define MASK_FLAG  	(1 << 1)  	// 屏蔽控制
#define PRIOR_FLAG 	(1 << 2)  	// 优先扫描
#define FLASH_FLAG 	(1 << 3)  	// 闪灯控制

/******************************************************************************************
** 以下定义用于对数据结构中器件或回路相关的状态位操作
*******************************************************************************************/
#define SetBit(ByteName, BitName)    (ByteName) |= (BitName)
#define ClrBit(ByteName, BitName)    (ByteName) &= (~BitName)
#define bit0  0x01
#define bit1  0x02
#define bit2  0x04
#define bit3  0x08
#define bit4  0x10
#define bit5  0x20
#define bit6  0x40
#define bit7  0x80

////LOOPDevice-->OtherFlags：D7() + D6(进入) + D5(进入火警、故障延时)

////LOOPDevice-->ReScanStat1：首次标志   ReScanStat2：确认标志
////             ReScanStat3: 首次标志   ReScanStat4：确认标志
////                          ReScanStat5 恢复标志

                                       //《探测器》        《模块》       《ReScanStat?》
#define  FireStat          (1 << 0)    //                                      TYPE
#define  FirstFireStat     (1 << 0)    //  首火警        输入节点首火警          1
#define  DevIsFireStat     (1 << 0)    // 火警确认      输入节点火警确认         2
                          
#define  FaultStat         (1 << 1)    //                                      TYPE
#define  FirstFaultStat    (1 << 1)    //  首故障           首故障               1
#define  DevIsFaultStat    (1 << 1)    // 故障确认         故障确认              2
                        
#define  DustOrInShortStat (1 << 2)    //                                      TYPE
#define  FirstDustStat     (1 << 2)    // 积灰故障        输入节点短路           1
#define  DevIsDustStat     (1 << 2)    // 积灰确认      输入节点短路确认         2
                        
#define  InOpenStat        (1 << 3)    //                                      TYPE
#define  FirstInOpenStat   (1 << 3)    //                 输入节点开路           1
#define  DevIsInOpen       (1 << 3)    //               输入节点开路确认         2
                         
#define  OutFireStat       (1 << 4)    //                                      TYPE
#define  FirstOutFireStat  (1 << 4)    //                 输出节点首火警         1
#define  DevIsOutFire      (1 << 4)    //                输出节点火警确认        2
                         
#define  OutShortStat      (1 << 5)    //                                      TYPE
#define  FirstOutShortStat (1 << 5)    //                 输出节点短路           1
#define  DevIsOutShort     (1 << 5)    //                输出节点短路确认        2
                          
#define  OutOpenStat       (1 << 6)     //                                      TYPE
#define  FirstOutOpenStat  (1 << 6)     //                 输出节点开路           1
#define  DevIsOutOpen      (1 << 6)     //               输出节点开路确认         2
                         
#define  FirstOutActStat   (1 << 7)    //                 输出节点首次动作        1  //输出节点动作另外处理
#define  FirstOutNoAct     (1 << 7)    //                 输出节点首次未动作      2  //不要和其他的一样处理
/////////////////////////////////////
//其它错误类型

#define  DupAddrStat       (1 << 0)    //                                      TYPE
#define  FirstDupAddrStat  (1 << 0)    //   重码             重码                3
#define  DevIsDupAddrStat  (1 << 0)    // 重码确认         重码确认              4
                         
#define  TypeErrStat       (1 << 1)    //                                      TYPE
#define  FirstTypeErrStat  (1 << 1)    //  类型错误        类型错误              3
#define  DevIsTypeErrStat  (1 << 1)    // 类型错误确认   类型错误确认            4
                         
#define  NoRespStat        (1 << 2)    //                                      TYPE
#define  FirstNoRespStat   (1 << 2)    //   无响应         无响应                3
#define  DevIsNoRespStat   (1 << 2)    // 无响应确认      无响应确认             4
#define  SendErrStat       (1 << 3)    //                                      TYPE
#define  FirstSendErrStat  (1 << 3)    //   发送故障       发送故障              3
#define  DevIsSendErrStat   (1 << 3)    // 发送故障确认    发送故障确认           4
#define  NoRespRcvStat      (1 << 4)   //                                      TYPE
#define  FirstNoRespRcvStat (1 << 4)   // 无响应恢复       无响应恢复            3
#define  DevIsNoRespRcvStat (1 << 4)   // 无响应恢复确认  无响应恢复确认         4
  
//#define  NRepHaveReported  bit6    // 无响应已上报
#define  NoOtherFauStat    (1 << 7)    //没有其他错误    没有其他错误          TYPE
/////////////////////////////////////
//火警故障恢复标志
#define  FireRecover       (1 << 0)    // 火警恢复          火警恢复             5
#define  FaultRecover      (1 << 1)    // 故障恢复          故障恢复             5
#define  NoRespRecover     (1 << 2)    // 无响应恢复      无响应恢复             5
//#define  NoRespMustReport  bit3  // 无响应需要上报  无响应需要上报         5
//#define  FaultRecover2   bit4    //            已取消了                    5
#define  OutOpenAct        (1 << 5)    //                 输出节点打开已动作     5
#define  OutCloseAct       (1 << 6)    //                 输出节点关闭已动作     5
/************************************************************************************************/
#endif


