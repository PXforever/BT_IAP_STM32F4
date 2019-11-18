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
	u16 applenth=0;				//���յ���app���볤��
	u8 clearflag=0;
	u8 role = 1;
	u8 HC05_satae = 1;
  	
	Stm32_Clock_Init(336,8,2,7); //ϵͳʱ������
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);	  //��ʱ������ʼ��
	uart_init(115200);	//����1��ʼ������
	delay_ms(800);
	
	LED_Init ();//С�Ƴ�ʼ������
	BEEP_Init();
	KEY_Init();
	HC05_satae = HC05_Init();	//������ʼ��
	delay_ms(800);
	
	LCD_Init();   //��ʾ����ʼ������
	delay_ms(900);
		
	POINT_COLOR=RED;//��������Ϊ��ɫ	
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
		POINT_COLOR=RED;//��������Ϊ��ɫ
		LCD_ShowString(100,260,200,16,16,"SLAVE");
	}
	else if(role == 1)
	{
		POINT_COLOR=RED;//��������Ϊ��ɫ
		LCD_ShowString(100,260,200,16,16,"MASTER");
	}
	else ;

	POINT_COLOR=RED;//��������Ϊ��ɫ
	LCD_ShowString(30,50,200,16,16,"Warship STM32");	
	LCD_ShowString(30,70,200,16,16,"IAP TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2015/1/27");  
	LCD_ShowString(30,130,200,16,16,"KEY_UP:Copy APP2FLASH");
	LCD_ShowString(30,150,200,16,16,"KEY2:Erase SRAM APP");
	LCD_ShowString(30,170,200,16,16,"KEY1:Run FLASH APP");
	LCD_ShowString(30,190,200,16,16,"KEY0:Run SRAM APP");
	POINT_COLOR=BLUE;
	
	
	//��ʾ��ʾ��Ϣ
	POINT_COLOR=BLUE;//��������Ϊ��ɫ	  
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
			if((APP_ID+1) == APP_N)//��������,û���յ��κ�����,��Ϊ�������ݽ������.
			{
				applenth = APP_LEN;
				APP_N = 0;
				APP_ID = 0;
				APP_LEN = 0;
				printf("�û�����������!\r\n");
				printf("���볤��:%dBytes\r\n",applenth);
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
				//����δ�����꣬��ȴ�
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
				if(clearflag==0)LCD_Fill(30,210,240,210+16,WHITE);//�����ʾ
			}
		}
	
		key=KEY_Scan(0);
		if(key==WKUP_PRES)	//WK_UP��������
		{
			if(applenth)
			{
				printf("��ʼ���¹̼�...\r\n");	
				LCD_ShowString(30,210,200,16,16,"Copying APP2FLASH...");
 				if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
				{	 
					//iap_write_appbin(FLASH_APP1_ADDR,USART_RX_BUF,applenth);//����FLASH����   
					iap_write_appbin(FLASH_APP1_ADDR,USART6_RX_BUF,applenth);//����FLASH����
					LCD_ShowString(30,210,200,16,16,"Copy APP Successed!!");
					printf("�̼��������!\r\n");	
				}else 
				{
					LCD_ShowString(30,210,200,16,16,"Illegal FLASH APP!  ");	   
					printf("��FLASHӦ�ó���!\r\n");
				}
 			}else 
			{
				printf("û�п��Ը��µĹ̼�!\r\n");
				LCD_ShowString(30,210,200,16,16,"No APP!");
			}
			clearflag=7;//��־��������ʾ,��������7*300ms�������ʾ									 
		}
		if(key==KEY1_PRES)	//KEY1����
		{
			if(applenth)
			{																	 
				printf("�̼�������!\r\n");    
				LCD_ShowString(30,210,200,16,16,"APP Erase Successed!");
				applenth=0;
			}else 
			{
				printf("û�п�������Ĺ̼�!\r\n");
				LCD_ShowString(30,210,200,16,16,"No APP!");
			}
			clearflag=7;//��־��������ʾ,��������7*300ms�������ʾ									 
		}
		if(key==KEY2_PRES)	//KEY2����
		{
			printf("��ʼִ��FLASH�û�����!!\r\n");
			printf("addr:%x\r\n",*(vu32*)(FLASH_APP1_ADDR+4));
			if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
			{	 
				iap_load_app(FLASH_APP1_ADDR);//ִ��FLASH APP����
			}else 
			{
				printf("��FLASHӦ�ó���,�޷�ִ��!\r\n");
				LCD_ShowString(30,210,200,16,16,"Illegal FLASH APP!");	   
			}									 
			clearflag=7;//��־��������ʾ,��������7*300ms�������ʾ	  
		}
		if(key==KEY0_PRES)	//KEY0����
		{
			printf("��ʼִ��SRAM�û�����!!\r\n");
			if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x20000000)//�ж��Ƿ�Ϊ0X20XXXXXX.
			{	 
				iap_load_app(0X20001000);//SRAM��ַ
			}else 
			{
				printf("��SRAMӦ�ó���,�޷�ִ��!\r\n");
				LCD_ShowString(30,210,200,16,16,"Illegal SRAM APP!");	   
			}									 
			clearflag=7;//��־��������ʾ,��������7*300ms�������ʾ	 
		}				   
		 
	}  		   	 
	return 0;	
}

//��ʾATK-HC05ģ�������״̬
void HC05_Sta_Show(void)
{												 
	if(HC05_LED)LCD_ShowString(30,240,120,16,16,"STA:Connected ");			//���ӳɹ�
	else LCD_ShowString(30,240,120,16,16,"STA:Disconnect");	 			//δ����				 
}	 




