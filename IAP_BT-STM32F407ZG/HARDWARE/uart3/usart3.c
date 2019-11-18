#include "sys.h"
#include "usart3.h"	  
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"
//#include "timer.h"
#include "led.h"

 
//初始化IO 串口3
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率 
void usart3_init(u32 pclk1,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;   
	temp=(float)(pclk1*1000000)/(bound*16);//得到USARTDIV,OVER8设置为0
	mantissa=temp;				 	//得到整数部分
	fraction=(temp-mantissa)*16; 	//得到小数部分,OVER8设置为0	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->AHB1ENR|=1<<1;   			//使能PORTB口时钟  
	RCC->APB1ENR|=1<<18;  			//使能串口3时钟 
	GPIO_Set(GPIOB,PIN10|PIN11,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);//PB10,PB11,复用功能,上拉输出
 	GPIO_AF_Set(GPIOB,10,7);		//PB10,AF7
	GPIO_AF_Set(GPIOB,11,7);		//PB11,AF7  	   
	//波特率设置
 	USART3->BRR=mantissa; 			// 波特率设置	  

	USART3->CR1|=1<<3;  			//串口发送使能  
	USART3->CR1|=1<<2;  			//串口接收使能
	USART3->CR1|=1<<5;    			//接收缓冲区非空中断使能	
	USART3->CR1|=1<<13;  			//串口使能  
	MY_NVIC_Init(0,0,USART3_IRQn,2);//组2，优先级0,0,最高优先级 
	USART3_RX_STA=0;				//清零 
}



#ifdef USART3_RX_EN  
vu16 USART3_RX_STA=0;   

static unsigned char TxBuffer[256];
static unsigned char TxCounter=0;
static unsigned char count=0; 
extern void CopeSerial3Data(unsigned char ucData);


void USART3_IRQHandler(void)
{
		if(USART3->SR&(1<<5))
		{
			CopeSerial3Data((unsigned char)USART3->DR);//处理数据
		}
		

} 

#endif

