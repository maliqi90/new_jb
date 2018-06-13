#ifndef __LOOPDEVTABLE_H__
#define __LOOPDEVTABLE_H__

#include "stm32f10x.h"
#include "def.h"
//探测器类型        代码
#define D_NONE      0xff     //不存在的一个值，PW2 = 0时

#define D_GD        23     	//光电探测器     <----老
#define D_CDW       31     	//差定温探测器   <----老
#define D_DW        31     	//没有定温探测器     <----老
#define D_YWFH      50     	//烟温复合探测器 <----老
#define D_KRQT      102     //可燃气体探测器 <----老

#define ND_GD       23     	//新光电探测器				23	
#define ND_CDW      31     	//新差定温探测器			31
#define ND_DW       31     	//没有新定温探测器		31
#define ND_YWFH     50     	//新烟温复合探测器		50
#define ND_KRQT     102     //新可燃气体探测器		102
#define ND_ZWHY     40     	//新紫外火焰探测器		40
#define ND_XCHJ     235     //新现场环境探测器		235
#define ND_HWHY     40     	//新红外火焰探测器		40
#define ND_GLMD     241     //探测器类					241

//模块类型          代码
#define M_NONE      0xff     //不存在的一个值，PW3 = 0时

#define M_SRSBQB    0x10     //输入模块、手动报警按钮、消火拴启泵按钮    <----老
#define M_SC        73     //输出模块                                  <----老
#define M_SRSC      74     //输入输出模块                              <----老
#define M_BZXXQ     82     //编址讯响器                                <----老

#define NM_SR       72     //新输入模块					72
#define NM_SB       61     //新手动报警按钮				61
#define NM_QB       62     //新消火拴启泵按钮			62
#define NM_SC       73     //新输出模块					73
#define NM_SRSC     74     //新输入输出模块				74
#define NM_BZXXQ    82     //新编址讯响器					82
#define NM_CDI      75     //新CDI模块						75
#define NM_ZYSR     72     //新专用输入模块				72
#define NM_MD       74     //门灯060430归入输入输出	74
#define NM_GBQH     166     //广播切换模块				166
#define NM_HZXS     10     //火灾显示盘					10
#define NM_JJQT     65     //紧急启停按钮					65
#define NM_ZDZH     242     //自动转换开关				242


extern const unsigned char AnsCodeNUM[2][16];  
  
extern const unsigned char DetTypePW3PW2[6][6];

extern const unsigned char ModTypePW4PW3[6][5];

BOOL is_detector(u8 type);


#endif

