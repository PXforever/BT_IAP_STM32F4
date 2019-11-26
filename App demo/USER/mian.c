#include <stdio.h>
#include "i2c.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"

typedef  u8 (*key_func_point)(u8);
typedef  void (*iapfun)(void);	
typedef int (*printf_point)(char *fmt,...);

int main()
{
	key_func_point func_key_sacn;
	printf_point my_printf;
	iapfun iap_main;
	iapfun current_code;
	u8 addr = 0x11;
	u8 message[17] = "I Love you, honey";
	u8 key = 0;
	//uart_init(72, 115200);
	
	//Stm32_Clock_Init(336,8,2,7);//����ʱ��,168Mhz
	delay_init(168);		//��ʼ����ʱ����
	

	current_code = (iapfun)* (vu32 *)0x08010004;
	func_key_sacn =(key_func_point) 0x08002025;	//ɨ�谴��
	my_printf =(printf_point)(vu32 *) 0x08000321;
	iap_main = (iapfun)* (vu32 *)0x08000004;			//IAP������
	//delay_us(10000);
	//iap_main();
	//func_key_sacn(1);
	my_printf("enter 2 app\r\n");
	//my_printf("hello\r\n");	

	
	i2c_init();
	//delay_ms(500);
	//i2c_write( addr, message, 0, 17);
	
	while(1)
	{	
		my_printf("hello\r\n");
		key=func_key_sacn(0);
		
		//i2c_write( addr, message, 0, 17);
		//delay_ms(1000);	
		if(key==4)	//WK_UP��������
		{
			my_printf("key 4 press\r\n");
			iap_main();
			break;	 
		}
		if(key==1)	//KEY1����
		{
			current_code();
							 
		}
		if(key==2)	//KEY2����
		{

		}
		if(key==3)	//KEY0����
		{
			//printf("��ʼִ��SRAM�û�����!!\r\n");

		}			
	} 
		
		
		
	
	return 1;
}

