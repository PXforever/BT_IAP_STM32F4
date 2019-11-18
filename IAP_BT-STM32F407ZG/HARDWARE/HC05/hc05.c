#include "delay.h" 			 
#include "usart.h" 			 
#include "usart6.h" 			 
#include "hc05.h" 
#include "led.h" 
#include "string.h"	 
#include "math.h"

//初始化ATK-HC05模块
//返回值:0,成功;1,失败.
u8 HC05_Init(void)
{
	u8 retry=15,t;	  		 
	u8 temp=1;

	
	GPIO_InitTypeDef GPIO_InitStructure;
 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOG时钟
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //LED对应引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化GPIOC0
	//GPIO_SetBits(GPIOG,GPIO_Pin_7);
 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//KEY对应引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOG, &GPIO_InitStructure); //根据设定参数初始化PF6

	GPIO_SetBits(GPIOG,GPIO_Pin_8);
 	
	usart6_init(9600);	//初始化串口3为:9600,波特率
	delay_ms(300);
	
	while(retry--)
	{
		HC05_KEY=1;					//KEY置高,进入AT模式
		delay_ms(10);
		u3_printf("AT\r\n");		//发送AT测试指令
		HC05_KEY=0;					//KEY拉低,退出AT模式
		for(t=0;t<10;t++) 			//最长等待50ms,来接收HC05模块的回应
		{
			if(USART6_RX_STA&0X8000) break;
			delay_ms(5);
		}		
		if(USART6_RX_STA&0X8000)	//接收到一次数据了
		{
			temp = USART6_RX_STA&0X7FFF;	//得到数据长度
			USART6_RX_STA=0;			 
			if(temp == 4 && USART6_PART_BUF[0] == 'O' && USART6_PART_BUF[1] == 'K')
			{
				temp=0;//接收到OK响应
				break;
			}
		}			    		
	}		    
	if(retry==0)temp=1;	//检测失败
	return temp;	 
}	 
//获取ATK-HC05模块的角色
//返回值:0,从机;1,主机;0XFF,获取失败.							  
u8 HC05_Get_Role(void)
{	 		    
	u8 retry=0X0F;
	u8 temp,t;
	while(retry--)
	{
		HC05_KEY=1;					//KEY置高,进入AT模式
		delay_ms(10);
		u3_printf("AT+ROLE?\r\n");	//查询角色
		for(t=0;t<20;t++) 			//最长等待200ms,来接收HC05模块的回应
		{
			delay_ms(10);
			if(USART6_RX_STA&0X8000)break;
		}		
		HC05_KEY=0;					//KEY拉低,退出AT模式
		if(USART6_RX_STA&0X8000)	//接收到一次数据了
		{
			temp=USART6_RX_STA&0X7FFF;	//得到数据长度
			USART6_RX_STA=0;			 
			if(temp==13&&USART6_RX_BUF[0]=='+')//接收到正确的应答了
			{
				temp=USART6_RX_BUF[6]-'0';//得到主从模式值
				break;
			}
		}		
	}
	if(retry==0)temp=0XFF;//查询失败.
	return temp;
} 							   
//ATK-HC05设置命令
//此函数用于设置ATK-HC05,适用于仅返回OK应答的AT指令
//atstr:AT指令串.比如:"AT+RESET"/"AT+UART=9600,0,0"/"AT+ROLE=0"等字符串
//返回值:0,设置成功;其他,设置失败.							  
u8 HC05_Set_Cmd(u8* atstr)
{	 		    
	u8 retry = 25;
	u8 temp,t;
	while(retry--)
	{
		HC05_KEY=1;					//KEY置高,进入AT模式
		delay_ms(10);
		u3_printf("%s\r\n",atstr);	//发送AT字符串
		delay_us(100);	
		HC05_KEY=0;					//KEY拉低,退出AT模式
		for(t=0;t<20;t++) 			//最长等待100ms,来接收HC05模块的回应
		{
			if(USART6_RX_STA&0X8000)break;
			delay_ms(5);
		}		
		if(USART6_RX_STA&0X8000)	//接收到一次数据了
		{
			temp=USART6_RX_STA&0X7FFF;	//得到数据长度
			USART6_RX_STA=0;			 
			if(temp==4&&USART6_RX_BUF[0]=='O')//接收到正确的应答了
			{			
				temp=0;
				break;			 
			}
		}	
	}
	if(retry==0)temp=0XFF;//设置失败.
	return temp;
} 
///////////////////////////////////////////////////////////////////////////////////////////////////
//通过该函数,可以利用USMART,调试接在串口3上的ATK-HC05模块
//str:命令串.(这里注意不再需要再输入回车符)
void HC05_CFG_CMD(u8 *str)
{					  
	u8 temp;
	u8 t;		  
	HC05_KEY=1;						//KEY置高,进入AT模式
	delay_ms(10);
	u3_printf("%s\r\n",(char*)str); //发送指令
	for(t=0;t<50;t++) 				//最长等待500ms,来接收HC05模块的回应
	{
		if(USART6_RX_STA&0X8000)break;
		delay_ms(10);
	}									    
	HC05_KEY=0;						//KEY拉低,退出AT模式
	if(USART6_RX_STA&0X8000)		//接收到一次数据了
	{
		temp=USART6_RX_STA&0X7FFF;	//得到数据长度
		USART6_RX_STA=0;
		USART6_RX_BUF[temp]=0;		//加结束符		 
		printf("\r\n%s",USART6_RX_BUF);//发送回应数据到串口1
	} 				 
}



///////////////////////////////////////////////////////////////////////////////////////////////////
//通过该函数,可以利用USMART,调试接在串口3上的ATK-HC05模块
//str:命令串.(这里注意不再需要再输入回车符)
u8 HC05_SET_MODE1(void)
{		
	u8 role = 4;
	u3_printf("AT+NAME=IAP TEST BT\r\n");
	delay_ms(400);	
	u3_printf("AT+ROLE=0\r\n");
	delay_ms(300);
	role = HC05_Get_Role();
	delay_ms(300);
	if(role == 0)
	{
		//printf("Slave Mode\r\n");
		return 0;
	}
	else if(role == 1)
	{
		//printf("Master Mode\r\n");
		return 1;
	}
	else 
		;
		//printf("error%d\r\n", role);
	return 4;
		
}










