#include "DIO.h"
#include "demo_DIO.h"
#include "led.h"

/*
*********************************************************************************************************
*	函数功能说明：灰度功能初始化
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Gray_Init(void)
{
	RCC->AHB1ENR|=1<<3; //使能PORTD时钟
	RCC->AHB1ENR|=1<<4;//使能PORTE时钟
	GPIO_Set(GPIOD,PIN2,GPIO_MODE_IN,0,0,GPIO_PUPD_PU); //PD2设置上拉输入
	GPIO_Set(GPIOE,PIN0,GPIO_MODE_IN,0,0,GPIO_PUPD_PU);//PE0上拉输入
	Gray1=1;
	Gray2=1;
}

/*
*********************************************************************************************************
*	函数功能说明：红外传感器初始化
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Infrared_sensor_Init(void)
{
	RCC->AHB1ENR|=1<<0;//使能PORTA时钟
	RCC->AHB1ENR|=1<<2;//使能PORTC时钟
	RCC->AHB1ENR|=1<<3;//使能PORTD时钟	
	RCC->AHB1ENR|=1<<5;//使能PORTF时钟
	GPIO_Set(GPIOA,PIN12,GPIO_MODE_IN,0,0,GPIO_PUPD_PU);//PA12上拉输入		ETR4
	GPIO_Set(GPIOC,PIN6|PIN7|PIN8,GPIO_MODE_IN,0,0,GPIO_PUPD_PU);//PC(6,7,8)上拉输入		PWM5~7
	GPIO_Set(GPIOD,PIN7|PIN11,GPIO_MODE_IN,0,0,GPIO_PUPD_PU);//PD(7,11)上拉输入	DIO2/1
	GPIO_Set(GPIOF,PIN8,GPIO_MODE_IN,0,0,GPIO_PUPD_PU);//PF(7,8)上拉输入 	DIO3/4
	I1=1;
	I2=1;
	I3=1;
	I4=1;
	I5=1;
	I6=1;
	I7=1;
	I8=1;
}

/*
*********************************************************************************************************
*	函 数 名: TIM2_Init
*	功能说明: 定时器2ETR计数初始化
*	形    参: arr（自动重装载值）
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM2_Init(u16 arr)
{
	RCC->APB1ENR|=1<<0;
	RCC->AHB1ENR|=1<<0;
	GPIO_Set(GPIOA,PIN5,GPIO_MODE_AF,0,0,GPIO_PUPD_NONE);
	GPIO_AF_Set(GPIOA,5,1); 
	TIM2->ARR=arr;
	TIM2->PSC=0;
	TIM2->SMCR &= ~(0xf<<8);//无滤波
	TIM2->SMCR &= ~(3<<12);//关闭预分频
	TIM2->SMCR |= 0<<15;//ETR不反相，高电平或上升沿沿有效
	TIM2->SMCR |= 1<<14;//使能外部时钟模式2
	TIM2->CNT = 0x0;//清零计数器
//	TIM2->CR1|=0<<4;
	TIM2->CR1 |= 1<<0;//使能定时器，开启计数。
}

/*
*********************************************************************************************************
*	函 数 名: TIM3_Init
*	功能说明: 定时器3ETR计数初始化
*	形    参: arr（自动重装载值）
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM3_Init(u16 arr)
{
	RCC->APB1ENR|=1<<1;
	RCC->AHB1ENR|=1<<3;
	GPIO_Set(GPIOD,PIN2,GPIO_MODE_AF,0,0,GPIO_PUPD_NONE);
	GPIO_AF_Set(GPIOD,2,2); 
	TIM3->ARR=arr;
	TIM3->PSC=0;
	TIM3->SMCR &= ~(0xf<<8);//无滤波
	TIM3->SMCR &= ~(3<<12);//关闭预分频
	TIM3->SMCR |= 0<<15;//ETR不反相，高电平或上升沿沿有效
	TIM3->SMCR |= 1<<14;//使能外部时钟模式2
	TIM3->CNT = 0x0;//清零计数器
	TIM3->CR1 |= 1<<0;//使能定时器，开启计数。
}

/*
*********************************************************************************************************
*	函 数 名: TIM6_Init
*	功能说明: 每0.5秒进入一次zhongduan
*	形    参: arr（自动重装载值）
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM6_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<4;	//TIM3时钟使能    
 	TIM6->ARR=arr;  	//设定计数器自动重装值   0.5s
	TIM6->PSC=psc;  	//预分频器	  
	TIM6->DIER|=1<<0;   //允许更新中断	  
	TIM6->CR1|=0x01;    //使能定时器3
	TIM6->CR1|=1<<6;
	TIM6->CNT=0;
	MY_NVIC_Init(1,3,TIM6_DAC_IRQn,2);	//抢占1，子优先级3，组2	
}

u16 l_circlenow;
u16 r_circlenow;

extern u16 set_speed;
extern u8 flag_line;
//定时器6中断服务程序	 
void TIM6_DAC_IRQHandler(void)
{ 		    		  			    
	if(TIM6->SR&0X0001)//溢出中断
	{
		l_circlenow=TIM3->CNT;
		TIM3->CNT = 0;
		
		r_circlenow=TIM2->CNT;
		TIM2->CNT = 0;
//		if(flag_line == 0)
//		{
//			go_straight_line(set_speed);
//		}
		
	}				   
	TIM6->SR&=~(1<<0);//清除中断标志位 	    
}

