/*
********************************************************************************
*                             ������̩�����������޹�˾
* ��    ַ �� �����з�̨���żҿ���·8��
* ��    �� �� 100072
* ��    ַ �� Http://www.guotaiyian.com
*
* �� �� �� :  LoopDataStruct.h
* ��    �� :  �� �
* ����ʱ�� :  2006/11/16
* ��    �� :  ��·�����õ������ݽṹ����������ļ��С�
*
* ע    �� �� ���ݽṹ���32λ�ṹ��CPU��ϵ�ṹ�����Ż�������ָ��ִ���ٶȲ�����
*             ƿ����ͬʱCPU���߱�51ϵ��λ�����������ｫ�ϻ�·�������λ��������
*             ��λ��ͻ�������ݽṹ�İ����ܷ��ࡣ
********************************************************************************
*/
#ifndef __LOOPDATASTRUCT_H__
#define __LOOPDATASTRUCT_H__

/*******************************************************************************/
/********************************************************************************
*                        �����ǻ�·�ϵ������������ݽṹ                         *
* �𾯡�������ʱ��ȡ��                                                          *
********************************************************************************/
#define MASK_FLAG  	(1 << 1)  	// ���ο���
#define PRIOR_FLAG 	(1 << 2)  	// ����ɨ��
#define FLASH_FLAG 	(1 << 3)  	// ���ƿ���

/******************************************************************************************
** ���¶������ڶ����ݽṹ���������·��ص�״̬λ����
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

////LOOPDevice-->OtherFlags��D7() + D6(����) + D5(����𾯡�������ʱ)

////LOOPDevice-->ReScanStat1���״α�־   ReScanStat2��ȷ�ϱ�־
////             ReScanStat3: �״α�־   ReScanStat4��ȷ�ϱ�־
////                          ReScanStat5 �ָ���־

                                       //��̽������        ��ģ�顷       ��ReScanStat?��
#define  FireStat          (1 << 0)    //                                      TYPE
#define  FirstFireStat     (1 << 0)    //  �׻�        ����ڵ��׻�          1
#define  DevIsFireStat     (1 << 0)    // ��ȷ��      ����ڵ��ȷ��         2
                          
#define  FaultStat         (1 << 1)    //                                      TYPE
#define  FirstFaultStat    (1 << 1)    //  �׹���           �׹���               1
#define  DevIsFaultStat    (1 << 1)    // ����ȷ��         ����ȷ��              2
                        
#define  DustOrInShortStat (1 << 2)    //                                      TYPE
#define  FirstDustStat     (1 << 2)    // ���ҹ���        ����ڵ��·           1
#define  DevIsDustStat     (1 << 2)    // ����ȷ��      ����ڵ��·ȷ��         2
                        
#define  InOpenStat        (1 << 3)    //                                      TYPE
#define  FirstInOpenStat   (1 << 3)    //                 ����ڵ㿪·           1
#define  DevIsInOpen       (1 << 3)    //               ����ڵ㿪·ȷ��         2
                         
#define  OutFireStat       (1 << 4)    //                                      TYPE
#define  FirstOutFireStat  (1 << 4)    //                 ����ڵ��׻�         1
#define  DevIsOutFire      (1 << 4)    //                ����ڵ��ȷ��        2
                         
#define  OutShortStat      (1 << 5)    //                                      TYPE
#define  FirstOutShortStat (1 << 5)    //                 ����ڵ��·           1
#define  DevIsOutShort     (1 << 5)    //                ����ڵ��·ȷ��        2
                          
#define  OutOpenStat       (1 << 6)     //                                      TYPE
#define  FirstOutOpenStat  (1 << 6)     //                 ����ڵ㿪·           1
#define  DevIsOutOpen      (1 << 6)     //               ����ڵ㿪·ȷ��         2
                         
#define  FirstOutActStat   (1 << 7)    //                 ����ڵ��״ζ���        1  //����ڵ㶯�����⴦��
#define  FirstOutNoAct     (1 << 7)    //                 ����ڵ��״�δ����      2  //��Ҫ��������һ������
/////////////////////////////////////
//������������

#define  DupAddrStat       (1 << 0)    //                                      TYPE
#define  FirstDupAddrStat  (1 << 0)    //   ����             ����                3
#define  DevIsDupAddrStat  (1 << 0)    // ����ȷ��         ����ȷ��              4
                         
#define  TypeErrStat       (1 << 1)    //                                      TYPE
#define  FirstTypeErrStat  (1 << 1)    //  ���ʹ���        ���ʹ���              3
#define  DevIsTypeErrStat  (1 << 1)    // ���ʹ���ȷ��   ���ʹ���ȷ��            4
                         
#define  NoRespStat        (1 << 2)    //                                      TYPE
#define  FirstNoRespStat   (1 << 2)    //   ����Ӧ         ����Ӧ                3
#define  DevIsNoRespStat   (1 << 2)    // ����Ӧȷ��      ����Ӧȷ��             4
#define  SendErrStat       (1 << 3)    //                                      TYPE
#define  FirstSendErrStat  (1 << 3)    //   ���͹���       ���͹���              3
#define  DevIsSendErrStat   (1 << 3)    // ���͹���ȷ��    ���͹���ȷ��           4
#define  NoRespRcvStat      (1 << 4)   //                                      TYPE
#define  FirstNoRespRcvStat (1 << 4)   // ����Ӧ�ָ�       ����Ӧ�ָ�            3
#define  DevIsNoRespRcvStat (1 << 4)   // ����Ӧ�ָ�ȷ��  ����Ӧ�ָ�ȷ��         4
  
//#define  NRepHaveReported  bit6    // ����Ӧ���ϱ�
#define  NoOtherFauStat    (1 << 7)    //û����������    û����������          TYPE
/////////////////////////////////////
//�𾯹��ϻָ���־
#define  FireRecover       (1 << 0)    // �𾯻ָ�          �𾯻ָ�             5
#define  FaultRecover      (1 << 1)    // ���ϻָ�          ���ϻָ�             5
#define  NoRespRecover     (1 << 2)    // ����Ӧ�ָ�      ����Ӧ�ָ�             5
//#define  NoRespMustReport  bit3  // ����Ӧ��Ҫ�ϱ�  ����Ӧ��Ҫ�ϱ�         5
//#define  FaultRecover2   bit4    //            ��ȡ����                    5
#define  OutOpenAct        (1 << 5)    //                 ����ڵ���Ѷ���     5
#define  OutCloseAct       (1 << 6)    //                 ����ڵ�ر��Ѷ���     5
/************************************************************************************************/
#endif


