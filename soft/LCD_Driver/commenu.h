#ifndef _COMMENU_H_
#define _COMMENU_H_

#define LCD_TIME    10  //待机10s息屏
extern volatile unsigned int lcd_time;
#define PIC_STAY1   1//待机图标
#define PIC_LOGO    2// LOGO图标
#define PIC_FIRE    4//火警图标
#define PIC_FAULT   3//故障图标
#define PIC_SLIENCE 5//消音图标

#define DEVICE_CAPTION_NUM  5
#define DEIVCE_TYPE_NUM     4
void emWin_CreateBMPPicture(uint8_t windex);
extern uint8_t slienceflag;
extern uint8_t voice_flag;
//显示国泰怡安图片 相关文字
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
void time_show(void);  //时间显示
void Save_Time_Bkp(void); //保存时间到BKP寄存器
void Read_Time_Bkp(void); //读取BKP寄存器内保存的时间
void Write_Time_RTC(void);//保存时间到RTC寄存器
//void set_time(void);  //设置时间
void TimeSet_Show(char num);//时间设置光标
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

