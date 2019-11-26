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
	
	//Stm32_Clock_Init(336,8,2,7);//设置时钟,168Mhz
	delay_init(168);		//初始化延时函数
	

	current_code = (iapfun)* (vu32 *)0x08010004;
	func_key_sacn =(key_func_point) 0x08002025;	//扫描按键
	my_printf =(printf_point)(vu32 *) 0x08000321;
	iap_main = (iapfun)* (vu32 *)0x08000004;			//IAP主程序
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
		if(key==4)	//WK_UP按键按下
		{
			my_printf("key 4 press\r\n");
			iap_main();
			break;	 
		}
		if(key==1)	//KEY1按下
		{
			current_code();
							 
		}
		if(key==2)	//KEY2按下
		{

		}
		if(key==3)	//KEY0按下
		{
			//printf("开始执行SRAM用户代码!!\r\n");

		}			
	} 
		
		
		
	
	return 1;
}

