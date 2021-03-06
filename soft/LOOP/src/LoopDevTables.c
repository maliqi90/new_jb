/*
********************************************************************************
*                             北京国泰怡安电子有限公司
* 地    址 ： 北京市丰台区杜家坎南路8号
* 邮    编 ： 100072
* 网    址 ： Http://www.guotaiyian.com
*
* 文 件 名 :  LoopDevTables.c
* 编    制 :  石甫
* 创建时间 :  2010/05/05
* 描    述 :  回路器件类型的判别
*
* 注    意 ： 
********************************************************************************
*/
//#include "DeviceTypeDefine.h"
#include "stm32f10x.h"
#include "LoopDevTables.h"

/*******************************************************************************
*      该数组表示了探测器和模块分别在不同控制命令(B0-B3)时的回码的个数。  
*      注意为了可以检测出类型错误命令 0x05统一认为有5个回码  
*******************************************************************************/ 
const unsigned char AnsCodeNUM[2][16] = {
//控制命令：00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
           { 2, 3, 3, 0, 0, 5, 0, 0, 0, 0, 3, 0, 0, 2, 0, 0}, //探测器
           { 3, 0, 0, 0, 0, 5, 3, 3, 3, 3, 3, 0, 0, 3, 0, 0}  //模块
 };  
  
/*****************************************************************************
-------------------  数字和时间长短对照表   ----------------------------
【0x00】                【000us 】
【0x01】                【300us 】             
【0x02】                【600us 】
【0x03】                【900us 】
【0x04】                【1200us】
【0x05】                【1500us】
******************************************************************************/

/*******************************************************************************
【0x05】 类型检测命令 【探测器类】  回码 PW0 PW1 PW2 PW3(老器件没有)
        说明: 新老器件的判断 根据有没有PW3回码，没有PW3则为老器件，有为新器件 
        类型根据地址 和 PW0 回码宽度判别，满足以下条件就是探测器类类
                                   地址为 1  - 127    PW0 回码 300us;
                                   地址为 128- 254    PW0 回码 900us    
*******************************************************************************/

#if 0
//探测器类型        代码
#define D_NONE      0x1f     //不存在的一个值，PW2 = 0时

#define D_GD        0x00     //光电探测器     <----老
#define D_CDW       0x01     //差定温探测器   <----老
#define D_DW        0x02     //没有定温探测器     <----老
#define D_YWFH      0x03     //烟温复合探测器 <----老
#define D_KRQT      0x04     //可燃气体探测器 <----老

#define ND_GD       0x00     //新光电探测器				23	
#define ND_CDW      0x01     //新差定温探测器			31
#define ND_DW       0x02     //没有新定温探测器		31
#define ND_YWFH     0x03     //新烟温复合探测器		50
#define ND_KRQT     0x04     //新可燃气体探测器		102
#define ND_ZWHY     0x05     //新紫外火焰探测器		40
#define ND_XCHJ     0x07     //新现场环境探测器		235
#define ND_HWHY     0x05     //新红外火焰探测器		40
#define ND_GLMD     0x07     //探测器类						241
#endif


/*********************  探测器类型对照表 **************************************/
const unsigned char DetTypePW3PW2[6][6] = {
//|  PW2   |  PW2   |  PW2    | PW2    |  PW2    |  PW2    |
//|0(0us)  |1(300us)| 2(600us)|3(900us)|4(1200us)|4(1500us)|
	{D_NONE,  D_GD,     D_CDW,    D_DW,    D_YWFH,   D_KRQT }, //PW3 = 0(0us   )【老】
	{D_NONE,  ND_GD,    ND_CDW,   ND_DW,   ND_YWFH,  ND_KRQT}, //PW3 = 1(300us )【新】
	{D_NONE,  ND_ZWHY,  ND_XCHJ,  ND_HWHY, ND_GLMD,  D_NONE }, //PW3 = 2(600us )【新】
	{D_NONE,  D_NONE,   D_NONE,   D_NONE,  D_NONE,   D_NONE }, //PW3 = 3(900us )【新】
	{D_NONE,  D_NONE,   D_NONE,   D_NONE,  D_NONE,   D_NONE }, //PW3 = 4(1200us)【新】
	{D_NONE,  D_NONE,   D_NONE,   D_NONE,  D_NONE,   D_NONE }  //PW3 = 5(1500us)【新】
};

/*******************************************************************************
【0x05】 类型检测命令 【模块类】    回码 PW0 PW1 PW2 PW3(老器件没有)
         说明: 新老器件的判断 根据有没有PW3回码，没有PW3则为老器件，有为新器件 
               类型根据地址 和 PW0 回码宽度判别，满足以下条件就是模块类
                                   地址为 1  - 127    PW0 回码 900us;
                                   地址为 128- 254    PW0 回码 300us    
*******************************************************************************/

#if 0
//模块类型          代码
#define M_NONE      0x1f     //不存在的一个值，PW3 = 0时

#define M_SRSBQB    0x10     //输入模块、手动报警按钮、消火拴启泵按钮    <----老
#define M_SC        0x18     //输出模块                                  <----老
#define M_SRSC      0x19     //输入输出模块                              <----老
#define M_BZXXQ     0x1a     //编址讯响器                                <----老

#define NM_SR       0x10     //新输入模块					72
#define NM_SB       0x11     //新手动报警按钮			61
#define NM_QB       0x12     //新消火拴启泵按钮		62
#define NM_SC       0x18     //新输出模块					73
#define NM_SRSC     0x19     //新输入输出模块			74
#define NM_BZXXQ    0x1a     //新编址讯响器				82
#define NM_CDI      0x13     //新CDI模块						75
#define NM_ZYSR     0x10     //新专用输入模块			72
#define NM_MD       0x19     //门灯060430归入输入输出	74
#define NM_GBQH     0x08     //广播切换模块				166
#define NM_HZXS     0x0f     //火灾显示盘					10
#define NM_JJQT     0x20     //紧急启停按钮				65
#define NM_ZDZH     0x21     //自动转换开关				242
#endif


/**************************** 模块类型对照表  *********************************/
const unsigned char ModTypePW4PW3[6][5] = {
//     |  PW3   |  PW3    |  PW3     | PW3    |  PW3    |
//     |0(0us)  |1(300us) | 2(600us) |3(900us)|4(1200us)|
	{M_NONE,   M_SRSBQB, M_SC,     M_SRSC,  M_BZXXQ }, //PW4 = 0(0us   )【老】
	{M_NONE,   NM_SR,    NM_SC,    NM_SRSC, NM_BZXXQ}, //PW4 = 1(300us )【新】
	{M_NONE,   NM_SB,    NM_JJQT,  M_NONE,  NM_MD   }, //PW4 = 2(600us )【新】
	{M_NONE,   NM_QB,    NM_GBQH,  M_NONE,  M_NONE  }, //PW4 = 3(900us )【新】
	{M_NONE,   NM_CDI,   NM_ZDZH,  M_NONE,  NM_HZXS }, //PW4 = 4(1200us)【新】
	{M_NONE,   NM_ZYSR,  M_NONE,   M_NONE,  M_NONE  }, //PW4 = 5(1500us)【新】
};

uc8 detector_bit_map[32] = {
	0x00, 0x00, 0xF0, 0xC3, 0x0F, 0x0F, 0x3C, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x02, 0x00
};

uc8 module_bit_map[32] = {
	0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60,
	0x00, 0x0F, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x20, 0x00, 0x00, 0x00, 0x40, 0x10, 0x00, 0x80,
	0x48, 0xA4, 0x45, 0x00, 0x00, 0xC0, 0x01, 0x00
};

/*
u8 type_from_gb(u8 type)
{
	u8 temp;
	
	switch (type)
	{
	case 23:
		temp = 0x00;
		break;
	case 31:
		temp = 0x01;
		break;
	case 50:
		temp = 0x03;
		break;
	case 102:
		temp = 0x04;
		break;
	case 40:
		temp = 0x05;
		break;
	case 235:
		temp = 0x07;
		break;
	case 166:
		temp = 0x08;
		break;
	case 10:
		temp = 0x0f;
		break;
	case 72:
		temp = 0x10;
		break;
	case 61:
		temp = 0x11;
		break;
	case 62:
		temp = 0x12;
		break;
	case 75:
		temp = 0x13;
		break;
	case 73:
		temp = 0x18;
		break;
	case 74:
		temp = 0x19;
		break;
	case 82:
		temp = 0x1a;
		break;
	case 65:
		temp = 0x20;
		break;
	case 236:
		temp = 0x21;
		break;
	default:
		temp = 0x1f;
		break;	
	}

	return temp;
}


u8 type_to_gb(u8 type)
{
	u8 temp;
	
	switch (type)
	{
	case 0x00:
		temp = 23;
		break;
	case 0x01:
	case 0x02:
		temp = 31;
		break;
	case 0x03:
		temp = 50;
		break;
	case 0x04:
		temp = 102;
		break;
	case 0x05:
		temp = 40;
		break;
	case 0x07:
		temp = 241;
		break;
	case 0x08:
		temp = 166;
		break;
	case 0x0f:
		temp = 10;
		break;
	case 0x10:
		temp = 72;
		break;
	case 0x11:
		temp = 61;
		break;
	case 0x12:
		temp = 62;
		break;
	case 0x13:
		temp = 75;
		break;
	case 0x18:
		temp = 73;
		break;
	case 0x19:
		temp = 74;
		break;
	case 0x1a:
		temp = 82;
		break;
	case 0x20:
		temp = 65;
		break;
	case 0x21:
		temp = 242;
		break;
	default:
		temp = 0xff;
		break;	
	}

	return temp;
}
*/

/*
  *   是探测器
 */
BOOL is_detector(u8 type)
{
	u8 byte = type / 8;
	u8 bit  = type % 8;

	if (detector_bit_map[byte] & (1 << bit))
		return TRUE;
	else
		return FALSE;
}

/******************************************************************************
                       探测器类的命令详解
 ******************************************************************************

0. 【PW0】 探测器类型判断     地址为1-127，PW0回码300us 和 地址为128-254，PW0回码900us为探测器
                              地址为1-127，PW0回码900us 和 地址为128-254，PW0回码300us为模块
   【PW1】 探测器状态标志     300us :探测器正常；600us : 探测器预警
                              900us :探测器火警；1200us:探测器故障
                              1500us:探测器积灰故障。
         
1. 巡检：控制器对探测器进行巡检，用来实现对探测器的正常监控。 
         ●该控制位为 00。 
         ●探测器接收到该命令后，依次回传 PW0、PW1。  
         ●PW1 为探测器状态标志。         
2. 模拟量监测：查看探测器曲线时观察模拟量值的变化。 
         ●该控制位为 01。 
         ●探测器接收到该命令后，依次回传 PW0、PW1、PW2。 
         ●PW1 为探测器状态标志，
         ●PW2 为探测器模拟量值，范围不超过两毫秒（200×8.79uS） 。 
3. 主机监测：用来传送查看探测器曲线时的报警阈值。 
         ●该控制位为 02。 
         ●探测器接收到该命令后，依次回传 PW0、PW1、PW2。 
         ●PW1 为探测器状态标志，
         ●PW2 为报警阈值，范围不超过两毫秒（对感温探测器 103×8.79uS） 。 
4. 火警确认：探测器检测到火警、控制器确认后，控制器不停发送该命令，保持巡检灯在恒亮状态；
             若火警恢复，探测器确认灯处于一秒亮，一秒灭状态。探测器接收到巡检命令后，清除报警灯，恢复正常状态。 
         ●该控制位为 0AH。 
         ●探测器接收到该命令后，依次回传 PW0、PW1、PW2。 
         ●PW1 为探测器状态标志，
         ●PW2 为 300μS，探测器未确认火警，PW2 为 600μS，探测器确认火警。 
5. 观察背景光值：观察探测器的背景光，以确认探测电路的一致性。 
         ●该控制位为 03。 
         ●探测器接收到该命令后，依次回传 PW0、PW1、PW2。 
         ●PW1 为探测器状态标志，
         ●PW2 为背景光值（大约 20×8.79uS） 。 
6. 改背景光值：此项功能允许控制器更改探测器的背景光值。 
         ●该控制位为 04。 
         ●探测器接收到该命令后，依次回传 PW0、PW1、PW2。 
         ●PW1 为探测器状态标志，
         ●PW2 如果写入背景光值失败回码脉宽为 300μS；如果写入背景光值成功回码脉宽为 600μS； 
7. 通过命令对器件进行类型测试： 
         ●该控制位为 05。 
         ●探测器接收到该命令后，依次回传 PW0、PW1、PW2，PW3(新器件回码，旧器件不回) 
         ●PW1 为探测器状态标志， 
         ●PW2 (只有PW2)表示不同的探测器 300μS 表示光电，600μS 表示差定温，900μS 表示定温探测器，1200μS
             表示烟温复合探测器，1500 可燃气体探测器。
         ●PW2 PW3（PW3有回码） 详见上探测器类型对照表
8.灭灯巡检指令：用该指令巡检现场设备时，现场设备灯灭（现在指令和00命令一样，不能用做别的用途）
         ●该控制位为 0D。
         ●探测器接收到该命令后，依次回传 PW0、PW1、PW2。
9．(扩展用)通过命令对探测器灵敏度进行调整 
         ●该控制位为 0BH。 
         ●探测器接收到该命令后，依次回传 PW0、PW1、PW2。 
         ●PW1 为探测器状态标志，PW2 为 300μS，灵敏度未调整；PW2 为 600μS，灵敏度已调整。 
10. (扩展用)通过命令对器件进行远程测试 
         ●该控制位为 0C。 
         ●探测器接收到该命令后，依次回传 PW0、PW1、PW2。 
         ●PW1 为探测器状态标志，PW2 为 300μS，测试未打开；PW2 为 600μS，测试打开。

******************************************************************************
                       模块类的命令详解
*******************************************************************************
0. 【PW0】 模块类型判断     地址为1-127，PW0回码300us 和 地址为128-254，PW0回码900us为探测器
                            地址为1-127，PW0回码900us 和 地址为128-254，PW0回码300us为模块
   【PW1】 输入节点监视。300us正常, 600us输入节点开路, 900us输入节点短路, 1200us火警。
   【PW2】 输出节点监视，300us正常, 600us输出节点开路, 900us输出节点短路, 1200us火警, 1500us输出节点动作。

1. 巡检：控制器对模块进行巡检，用来实现对模块的正常监控。 
        ●该控制位为 00。 
        ●模块接收到该命令后，依次回传 PW0、PW1、PW2；各位定义同上  
        输入模块: PW1输入节点监视如前所示，PW2回码脉宽固定为300us
        输出模块: PW1回码脉宽固定为300us， PW2输入节点监视如前所示，
        输出输入模块: PW1输入节点监视如前所示，PW2输入节点监视如前所示，在输出节点动作时，输出节点监视应有变化。

2. 通过命令打开输出模块、输入输出模块 
        ●该控制位为 06。 
        ●模块接收到该命令后，依次回传 PW0、PW1、PW2，各位定义同上 
3. 通过命令关闭输出模块、输入输出模块 
        ●该控制位为 07。 
        ●模块接收到该命令后，依次回传 PW0、PW1、PW2，各位定义同上 
4. 通过命令使控制模块处于脉冲输出方式，即触点处于一秒开一秒闭状态 
        ●该控制位为 08。 
        ●模块接收到该命令后，依次回传 PW0、PW1、PW2。各位定义同上  
5．模块动作选通命令： 
        ●该控制位为 09。 
        ●模块接收到该命令后，依次回传 PW0、PW1、PW2、PW3；远程测试是仿 真验证输
          入输出节点短路时，PW1、PW2 是否变为短路状态，PW3 是确认当前状态为远程测
          试，脉宽为 900μS 
       如使输出设备动作，模块在连续接收两次同样动作指令的情况下，若第三组命令为
       选通指令才能动作，输出节点动作监视只有在第三次命令时才起变化。 
6.  通过命令对器件进行类型测试： 
        ●该控制位为 05。 
        ●模块接收到该命令后，依次回传 PW0、PW1、PW2、PW3、PW4(新器件回码，旧器件不回)
        ●PW0、PW1、PW2。各位定义同上  
        ●PW3(只有PW3)不同值表示不同的模块。300us表示监视模块（输入模块、手动报警按钮、消火栓起泵按钮）
                                            600us控制模块，900us表示输入输出模块。 
        ●PW3 PW4（PW4有回码） 详见上模块类型对照表 
7。灭灯巡检指令：用该指令巡检现场设备时，现场设备灯灭（现在指令和00命令一样，不能用做别的用途）
         ●该控制位为 0D。
         ●模块接收到该命令后，依次回传 PW0、PW1、PW2。

*******************************************************************************/
