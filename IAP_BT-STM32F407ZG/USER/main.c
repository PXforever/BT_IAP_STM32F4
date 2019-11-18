#include "usart.h"
#include "delay.h"
#include "demo_DIO.h"
#include "beep.h"
#include "led.h"
#include "demo_motor.h"
#include "fan.h"
#include "wt61.h"
#include "usart3.h"
#include "lcd.h"
#include "drive.h"
#include "noisebridle.h"
#include "math.h"
#include "find_fire.h"
#include "fan.h"
#include "key.h"
#include "iap.h"
#include "hc05.h" 
#include "usart6.h"

void HC05_Sta_Show(void);

int main(void)
{	
	u8 t;
	u8 key;
	u16 applenth=0;				//接收到的app代码长度
	u8 clearflag=0;
	u8 role = 1;
	u8 HC05_satae = 1;
  	
	Stm32_Clock_Init(336,8,2,7); //系统时钟设置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);	  //延时函数初始化
	uart_init(115200);	//串口1初始化函数
	delay_ms(800);
	
	LED_Init ();//小灯初始化函数
	BEEP_Init();
	KEY_Init();
	HC05_satae = HC05_Init();	//蓝牙初始化
	delay_ms(800);
	
	LCD_Init();   //显示屏初始化函数
	delay_ms(900);
		
	POINT_COLOR=RED;//设置字体为红色	
	LCD_ShowString(30,260,200,16,16,"BT:");
		
	if(HC05_satae == 1) 
	{
		POINT_COLOR=BLACK;
		LCD_ShowString(60,260,200,16,16,"CLOSE");
		printf("HC05 connect failed...\r\n");
	}
	else
	{
		POINT_COLOR=GREEN;
		LCD_ShowString(60,260,200,16,16,"OPEN");
		printf("HC05 init success!\r\n");
	}
	
	role = HC05_SET_MODE1();
	if(role == 0) 
	{
		POINT_COLOR=RED;//设置字体为红色
		LCD_ShowString(100,260,200,16,16,"SLAVE");
	}
	else if(role == 1)
	{
		POINT_COLOR=RED;//设置字体为红色
		LCD_ShowString(100,260,200,16,16,"MASTER");
	}
	else ;

	POINT_COLOR=RED;//设置字体为红色
	LCD_ShowString(30,50,200,16,16,"Warship STM32");	
	LCD_ShowString(30,70,200,16,16,"IAP TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2015/1/27");  
	LCD_ShowString(30,130,200,16,16,"KEY_UP:Copy APP2FLASH");
	LCD_ShowString(30,150,200,16,16,"KEY2:Erase SRAM APP");
	LCD_ShowString(30,170,200,16,16,"KEY1:Run FLASH APP");
	LCD_ShowString(30,190,200,16,16,"KEY0:Run SRAM APP");
	POINT_COLOR=BLUE;
	
	
	//显示提示信息
	POINT_COLOR=BLUE;//设置字体为蓝色	  
	while(1)
	{
		HC05_Sta_Show();	
//		printf("APP_ID:%d \n",  APP_ID);
//		printf("APP_N:%d \n",  APP_N);
		
		//printf("RX:%d RX_STA:%d\r\n", USART6_RX_COUNT, USART6_RX_STA);
		if( USART6_RX_STA & 0x8000)
		{
			//run package dealing program;
			iap_package_process();
			if(USART6_PART_BUF[0] == 'C' 	&& USART6_PART_BUF[1] == '|' && USART6_PART_BUF[2] == 'R' \
																		&& USART6_PART_BUF[3] == '|' && USART6_PART_BUF[4] == '1' \
																		&& USART6_PART_BUF[5] == '!' )
			{
				BEEP = 1;
				delay_ms(10);
				BEEP = 0;
				delay_ms(100);
				BEEP = 1;
				delay_ms(10);
				BEEP = 0;
				delay_ms(10);
				BEEP = 1;
				delay_ms(100);
				BEEP = 0;
				delay_ms(10);
				clear_send_flags();
			}
		}
	 	if(APP_N)
		{
			if((APP_ID+1) == APP_N)//新周期内,没有收到任何数据,认为本次数据接收完成.
			{
				applenth = APP_LEN;
				APP_N = 0;
				APP_ID = 0;
				APP_LEN = 0;
				printf("用户程序接收完成!\r\n");
				printf("代码长度:%dBytes\r\n",applenth);
				//white light flash
				LED1 = 0;	
				BEEP = 1;
				delay_ms(500);
				BEEP = 0;
				LED1 = 1;
				LCD_Fill(120,22500,148+16,225+16,WHITE);
				LCD_ShowString(30,225,200,16,16,"APP RE OK, LEN: ");
				LCD_ShowxNum( 148, 225, applenth, 6, 16, 0);
			}
			else 
			{
				//程序未接受完，请等待
			}			
		}
		else if(APP_ID == 0 && USART6_RX_STA == 0)
		{
			
		}
		
		t++;
		delay_ms(10);
		if(t==30)
		{
			LED0=!LED0;
			t=0;
			if(clearflag)
			{
				clearflag--;
				if(clearflag==0)LCD_Fill(30,210,240,210+16,WHITE);//清除显示
			}
		}
	
		key=KEY_Scan(0);
		if(key==WKUP_PRES)	//WK_UP按键按下
		{
			if(applenth)
			{
				printf("开始更新固件...\r\n");	
				LCD_ShowString(30,210,200,16,16,"Copying APP2FLASH...");
 				if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
				{	 
					//iap_write_appbin(FLASH_APP1_ADDR,USART_RX_BUF,applenth);//更新FLASH代码   
					iap_write_appbin(FLASH_APP1_ADDR,USART6_RX_BUF,applenth);//更新FLASH代码
					LCD_ShowString(30,210,200,16,16,"Copy APP Successed!!");
					printf("固件更新完成!\r\n");	
				}else 
				{
					LCD_ShowString(30,210,200,16,16,"Illegal FLASH APP!  ");	   
					printf("非FLASH应用程序!\r\n");
				}
 			}else 
			{
				printf("没有可以更新的固件!\r\n");
				LCD_ShowString(30,210,200,16,16,"No APP!");
			}
			clearflag=7;//标志更新了显示,并且设置7*300ms后清除显示									 
		}
		if(key==KEY1_PRES)	//KEY1按下
		{
			if(applenth)
			{																	 
				printf("固件清除完成!\r\n");    
				LCD_ShowString(30,210,200,16,16,"APP Erase Successed!");
				applenth=0;
			}else 
			{
				printf("没有可以清除的固件!\r\n");
				LCD_ShowString(30,210,200,16,16,"No APP!");
			}
			clearflag=7;//标志更新了显示,并且设置7*300ms后清除显示									 
		}
		if(key==KEY2_PRES)	//KEY2按下
		{
			printf("开始执行FLASH用户代码!!\r\n");
			printf("addr:%x\r\n",*(vu32*)(FLASH_APP1_ADDR+4));
			if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
			{	 
				iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
			}else 
			{
				printf("非FLASH应用程序,无法执行!\r\n");
				LCD_ShowString(30,210,200,16,16,"Illegal FLASH APP!");	   
			}									 
			clearflag=7;//标志更新了显示,并且设置7*300ms后清除显示	  
		}
		if(key==KEY0_PRES)	//KEY0按下
		{
			printf("开始执行SRAM用户代码!!\r\n");
			if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x20000000)//判断是否为0X20XXXXXX.
			{	 
				iap_load_app(0X20001000);//SRAM地址
			}else 
			{
				printf("非SRAM应用程序,无法执行!\r\n");
				LCD_ShowString(30,210,200,16,16,"Illegal SRAM APP!");	   
			}									 
			clearflag=7;//标志更新了显示,并且设置7*300ms后清除显示	 
		}				   
		 
	}  		   	 
	return 0;	
}

//显示ATK-HC05模块的连接状态
void HC05_Sta_Show(void)
{												 
	if(HC05_LED)LCD_ShowString(30,240,120,16,16,"STA:Connected ");			//连接成功
	else LCD_ShowString(30,240,120,16,16,"STA:Disconnect");	 			//未连接				 
}	 




