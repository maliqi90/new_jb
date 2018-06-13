#define W24_COMMUNICATION_WINDOW_WIN_PRIVATE
#include "include.h"
extern uint8_t         WinIndex;
static uint8_t sell = 0,sel_numm = 0;
static uint16_t canaddr = 0,busaddr = 0;
Communication_Set comm_set;
uint8_t busaddr_flag = 0;//BUS地址更改标志位
static void Dis_All(void)
{
	GUI_SetColor(GUI_BLACK);
	GUI_SetBkColor(GUI_WHITE);
    GUI_SetFont(GUI_FONT_HZ24);
	if(comm_set.Can_Set == 1)
	{
       GUI_DispStringAt("打开",120,50); 
	}
	else
	{
	   GUI_DispStringAt("关闭",120,50);
	}
    if(comm_set.Bus_Set == 1)
	{
       GUI_DispStringAt("打开",120,100); 
	}
	else
	{
	   GUI_DispStringAt("关闭",120,100);
	}
	GUI_DispDecAt(canaddr,250,50,3);
	GUI_DispDecAt(busaddr,250,100,3);
	
}
static void Dis_(uint8_t sel,uint8_t sel_num)
{
	uint8_t temp1,temp2,temp3;
	switch(sel)
	{
		case 0: //CAN端口开关
			comm_set.Can_Set = sel_num;
		    Dis_All();
			GUI_SetColor(GUI_WHITE);
	        GUI_SetBkColor(GUI_BLUE);
            GUI_SetFont(GUI_FONT_HZ24);
			if(comm_set.Can_Set == 1)
	         {
               GUI_DispStringAt("打开",120,50); 
	         }
	         else
	         {
	           GUI_DispStringAt("关闭",120,50);
	         }
			break;
		case 1://CAN地址百位数
			 temp1 = canaddr/100;
		     temp2 = (canaddr/10)%10;
		     temp3 = canaddr%10;
		     temp1 = sel_num;
		     canaddr = temp1*100+temp2*10+temp3;
		     Dis_All();
			GUI_SetColor(GUI_WHITE);
	        GUI_SetBkColor(GUI_BLUE);
            GUI_SetFont(GUI_FONT_HZ24);
		    GUI_DispDecAt(temp1,250,50,1);
			break;
		case 2://CAN地址十位数
		    temp1 = canaddr/100;
		     temp2 = (canaddr/10)%10;
		     temp3 = canaddr%10;
		     temp2 = sel_num;
		     canaddr = temp1*100+temp2*10+temp3;
		     Dis_All();
		     GUI_SetColor(GUI_WHITE);
	         GUI_SetBkColor(GUI_BLUE);
             GUI_SetFont(GUI_FONT_HZ24);
		     GUI_DispDecAt(temp2,262,50,1);
			break;
		case 3://CAN地址个位数
		     temp1 = canaddr/100;
		     temp2 = (canaddr/10)%10;
		     temp3 = canaddr%10;
		     temp3 = sel_num;
		     canaddr = temp1*100+temp2*10+temp3;
		     Dis_All();
		     GUI_SetColor(GUI_WHITE);
	         GUI_SetBkColor(GUI_BLUE);
             GUI_SetFont(GUI_FONT_HZ24);
		     GUI_DispDecAt(temp3,274,50,1);
			break;
		case 4://BUS端口开关
		    comm_set.Bus_Set = sel_num;
		    Dis_All();
			GUI_SetColor(GUI_WHITE);
	        GUI_SetBkColor(GUI_BLUE);
            GUI_SetFont(GUI_FONT_HZ24);
			if(comm_set.Bus_Set == 1)
	         {
               GUI_DispStringAt("打开",120,100); 
	         }
	         else
	         {
	           GUI_DispStringAt("关闭",120,100);
	         }
			break;
		case 5: //BUS地址百位
			 temp1 = busaddr/100;
		     temp2 = (busaddr/10)%10;
		     temp3 = busaddr%10;
		     temp1 = sel_num;
		     busaddr = temp1*100+temp2*10+temp3;
		     Dis_All();
			 GUI_SetColor(GUI_WHITE);
	         GUI_SetBkColor(GUI_BLUE);
             GUI_SetFont(GUI_FONT_HZ24);
		     GUI_DispDecAt(temp1,250,100,1);
			break;
		case 6://BUS地址十位
			 temp1 = busaddr/100;
		     temp2 = (busaddr/10)%10;
		     temp3 = busaddr%10;
		     temp2 = sel_num;
		     busaddr = temp1*100+temp2*10+temp3;
		     Dis_All();
			 GUI_SetColor(GUI_WHITE);
	         GUI_SetBkColor(GUI_BLUE);
             GUI_SetFont(GUI_FONT_HZ24);
		     GUI_DispDecAt(temp2,262,100,1);
			break;
		case 7://BUS地址个位
			 temp1 = busaddr/100;
		     temp2 = (busaddr/10)%10;
		     temp3 = busaddr%10;
		     temp3 = sel_num;
		     busaddr = temp1*100+temp2*10+temp3;
		     Dis_All();
			 GUI_SetColor(GUI_WHITE);
	         GUI_SetBkColor(GUI_BLUE);
             GUI_SetFont(GUI_FONT_HZ24);
		     GUI_DispDecAt(temp3,274,100,1);
			break;
		default:
			break;
	}
}
void W24_CommunicationWindowDisplay(void)
{
	sell = 0;
	
    norflash_read_comm_set((uint8_t*)&comm_set);
    sel_numm = comm_set.Can_Set;
    if(comm_set.Can_Addr == 0xff)
    {
         canaddr = 0 ;
    }
    else
    {
         canaddr = comm_set.Can_Addr ;
    }
    if(comm_set.Bus_Addr == 0xff)
    {
         busaddr = 0 ;
    }
    else
    {
          busaddr  =  comm_set.Bus_Addr;
    }
    //canaddr = comm_set.Can_Addr ;
   
    Dis_(sell,sel_numm);
}
void W24_CommunicationWindowProcess(void)
{
    switch (WinKeyValue )
    {
        case KEY_OK:
             if(sell < 7)
			 {
				 sell++;
				 
			 }
             else if(sell == 7)
             {
                 sell = 0;
                 comm_set.Can_Addr = canaddr;
                 comm_set.Bus_Addr = busaddr;
                 norflash_write_comm_set((uint8_t *)&comm_set);
                 busaddr_flag = 1;
//             if(comm_set.Can_Set == 1)
//             {
//                 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, DISABLE);
//                 CAN1_CONFIG();
//                 can_init();  
//             }
//             else
//             {
//                CAN_DeInit(CAN1);
//                Set_System();
//                Set_USBClock();
//                USB_Interrupts_Config();
//                USB_Init();                       
//             }
                 Message_("数据保存成功！",10,216);
             }

			 switch(sell)
			 {
				 case 0:
					 sel_numm = comm_set.Can_Set;
					 break;
				 case 1:
					 sel_numm = canaddr/100;
					 break;
				 case 2:
					 sel_numm = (canaddr/10)%10;
					 break;
				 case 3:
					 sel_numm = canaddr%10;
					 break;
				 case 4:
					 sel_numm = comm_set.Bus_Set;
					 break;
				 case 5:
					  sel_numm = busaddr/100;
					 break;
				 case 6:
					 sel_numm = (busaddr/10)%10;
					 break;
				 case 7:
					 sel_numm = busaddr%10;
					 break;
			 }
			 Dis_(sell,sel_numm);
            break;

        case KEY_UP:
             if((sell == 0)||(sell == 4))
			 {
				 if(sel_numm == 0)
				 {
					 sel_numm = 1;
					 
				 }
				 else
				 {
					 sel_numm = 0;
				 }
			 }
			 else
			 {
				 if(sel_numm < 9)
				 {
					 sel_numm++;
				 }
				 else
				 {
					 sel_numm = 0;
				 }
			 }
			  Dis_(sell,sel_numm);
            break;

        case KEY_DOWN:
             if((sell == 0)||(sell == 4))
			 {
				 if(sel_numm == 0)
				 {
					 sel_numm = 1;
					 
				 }
				 else
				 {
					 sel_numm = 0;
				 }
			 }
			 else
			 {
				 if(sel_numm > 0)
				 {
					 sel_numm--;
				 }
				 else
				 {
					 sel_numm = 9;
				 }
			 }
			  Dis_(sell,sel_numm);
            break;

        case KEY_SLIENCE:

            break;

        case KEY_FIRE:

            break;

        default:

            break;





    }
}

