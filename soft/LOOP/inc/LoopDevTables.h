#ifndef __LOOPDEVTABLE_H__
#define __LOOPDEVTABLE_H__

#include "stm32f10x.h"
#include "def.h"
//̽��������        ����
#define D_NONE      0xff     //�����ڵ�һ��ֵ��PW2 = 0ʱ

#define D_GD        23     	//���̽����     <----��
#define D_CDW       31     	//���̽����   <----��
#define D_DW        31     	//û�ж���̽����     <----��
#define D_YWFH      50     	//���¸���̽���� <----��
#define D_KRQT      102     //��ȼ����̽���� <----��

#define ND_GD       23     	//�¹��̽����				23	
#define ND_CDW      31     	//�²��̽����			31
#define ND_DW       31     	//û���¶���̽����		31
#define ND_YWFH     50     	//�����¸���̽����		50
#define ND_KRQT     102     //�¿�ȼ����̽����		102
#define ND_ZWHY     40     	//���������̽����		40
#define ND_XCHJ     235     //���ֳ�����̽����		235
#define ND_HWHY     40     	//�º������̽����		40
#define ND_GLMD     241     //̽������					241

//ģ������          ����
#define M_NONE      0xff     //�����ڵ�һ��ֵ��PW3 = 0ʱ

#define M_SRSBQB    0x10     //����ģ�顢�ֶ�������ť������˩���ð�ť    <----��
#define M_SC        73     //���ģ��                                  <----��
#define M_SRSC      74     //�������ģ��                              <----��
#define M_BZXXQ     82     //��ַѶ����                                <----��

#define NM_SR       72     //������ģ��					72
#define NM_SB       61     //���ֶ�������ť				61
#define NM_QB       62     //������˩���ð�ť			62
#define NM_SC       73     //�����ģ��					73
#define NM_SRSC     74     //���������ģ��				74
#define NM_BZXXQ    82     //�±�ַѶ����					82
#define NM_CDI      75     //��CDIģ��						75
#define NM_ZYSR     72     //��ר������ģ��				72
#define NM_MD       74     //�ŵ�060430�����������	74
#define NM_GBQH     166     //�㲥�л�ģ��				166
#define NM_HZXS     10     //������ʾ��					10
#define NM_JJQT     65     //������ͣ��ť					65
#define NM_ZDZH     242     //�Զ�ת������				242


extern const unsigned char AnsCodeNUM[2][16];  
  
extern const unsigned char DetTypePW3PW2[6][6];

extern const unsigned char ModTypePW4PW3[6][5];

BOOL is_detector(u8 type);


#endif

