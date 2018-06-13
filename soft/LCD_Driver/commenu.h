#ifndef _COMMENU_H_
#define _COMMENU_H_

#define LCD_TIME    10  //����10sϢ��
extern volatile unsigned int lcd_time;
#define PIC_STAY1   1//����ͼ��
#define PIC_LOGO    2// LOGOͼ��
#define PIC_FIRE    4//��ͼ��
#define PIC_FAULT   3//����ͼ��
#define PIC_SLIENCE 5//����ͼ��

#define DEVICE_CAPTION_NUM  5
#define DEIVCE_TYPE_NUM     4
void emWin_CreateBMPPicture(uint8_t windex);
extern uint8_t slienceflag;
extern uint8_t voice_flag;
//��ʾ��̩����ͼƬ �������
void began(void);
void DemoProgBar(void);
void Stay_Flag(void);
void menu(uint8_t pointer);
void menudis(void);
void set(uint8_t pointer);
void setdis(void);
void inquirydis(void);
void inquiry(uint8_t pointer);
void chekself(void);
void qjdd_dis(void);
void tim_set(void);
void qjbj_dis(void);
void set_phone_num(void);
void pbjcpb_dis(void);
void guzhang(void);
void huojing(void);
void time_show(void);  //ʱ����ʾ
void Save_Time_Bkp(void); //����ʱ�䵽BKP�Ĵ���
void Read_Time_Bkp(void); //��ȡBKP�Ĵ����ڱ����ʱ��
void Write_Time_RTC(void);//����ʱ�䵽RTC�Ĵ���
//void set_time(void);  //����ʱ��
void TimeSet_Show(char num);//ʱ�����ù��
void GUI_LCD_delay(int ms);
void Dis_ClearFlash(void);
void Dis_Power(uint8_t num,GUI_COLOR color);
void dis_fireevent(void);
void dis_faultevent(void);
void dis_sheildevent(void);
void dis_devent(void);
void Shield(void);
void Dis_Light_Set(void);
void phone_windows(void);
 void LoopLogin(void);
 void Info(void);
 void communcition(void);
 void DeviceSet(void);
 void PasswordLogin(void);
void qjbjdis(void);     
void Dis_Signal(uint8_t num,GUI_COLOR color);
void Message_(char *str,uint16_t x,uint16_t y);
#endif

