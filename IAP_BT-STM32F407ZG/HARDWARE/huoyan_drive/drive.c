#include "Drive.h"

/*
*********************************************************************************************************
*	函数功能说明：火焰传感器功能初始化
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Laser_Init(void)
{
	RCC->AHB1ENR|=1<<1;//使能PORTB时钟
	RCC->AHB1ENR|=1<<4;//使能PORTE时钟
	RCC->AHB1ENR|=1<<5;//使能PORTF时钟
	GPIO_Set(GPIOB,PIN4|PIN5|PIN6|PIN7|PIN8|PIN9,GPIO_MODE_IN,0,0,GPIO_PUPD_PD);//PB(4,5,6,7,8,9)下拉输入
	GPIO_Set(GPIOE,PIN1|PIN2|PIN5|PIN6,GPIO_MODE_IN,0,0,GPIO_PUPD_PD);//PE(1,2,5,6)下拉输入
	GPIO_Set(GPIOF,PIN0|PIN1|PIN2|PIN4|PIN5|PIN6,GPIO_MODE_IN,0,0,GPIO_PUPD_PD);//PF(0,1,2,4,5,6)下拉输入
	L01=0; L11=0;
	L02=0; L12=0;
	L03=0; L13=0;
	L04=0; L14=0;
	L05=0; L15=0;
	L06=0; L16=0;
	L07=0; L17=0;
	L08=0; L18=0;
}

/*
*********************************************************************************************************
*	函数功能说明：火焰传感器功能实现
*	形    参: aspect 0为前传感器 1为后传感器
*	返 回 值: int整型 大于0火焰在左侧 小于0火焰在右侧 
*********************************************************************************************************
*/
int Lcount,Rcount;

int Laser_Scan(int aspect)
{
	Lcount=Rcount=0;
	if(aspect==0)
	{
		if(L04==1) Lcount = 1;
		if(L05==1) Rcount = 1;
		if(L03==1) Lcount = 2;
		if(L06==1) Rcount = 2;
		if(L02==1) Lcount = 3;
		if(L07==1) Rcount = 3;
		if(L01==1) Lcount = 4;
		if(L08==1) Rcount = 4;
	}
	else if(aspect==1)
	{
		if(L14==1) Lcount = 1;
		if(L15==1) Rcount = 1;
		if(L13==1) Lcount = 2;
		if(L16==1) Rcount = 2;
		if(L12==1) Lcount = 3;
		if(L17==1) Rcount = 3;
		if(L11==1) Lcount = 4;
		if(L18==1) Rcount = 4;
	}
	
	return (Lcount-Rcount);
}

/*
*********************************************************************************************************
*	函数功能说明：火焰传感器功能实现
*	形    参: aspect 0为前传感器 1为后传感器
*	返 回 值: int整型 Acquit
*********************************************************************************************************
*/
int Acquit;
//int num;
int Laser_scan(int aspect)
{
	Acquit=0x00;
	if(aspect==0)
	{
//		if(L04==1) num++;
//		if(L05==1) num++;
//		if(L03==1) num++;
//		if(L06==1) num++;
//		if(L02==1) num++;
//		if(L07==1) num++;
//		if(L01==1) num++;
//		if(L08==1) num++;		
		if(L04==1) Acquit|=1<<3;
		if(L05==1) Acquit|=1<<4;
		if(L03==1) Acquit|=1<<2;
		if(L06==1) Acquit|=1<<5;
		if(L02==1) Acquit|=1<<1;
		if(L07==1) Acquit|=1<<6;
		if(L01==1) Acquit|=1<<0;
		if(L08==1) Acquit|=1<<7;
	}
	else if(aspect==1)
	{
//		if(L14==1) num++;
//		if(L15==1) num++;
//		if(L13==1) num++;
//		if(L16==1) num++;
//		if(L12==1) num++;
//		if(L17==1) num++;
//		if(L11==1) num++;
//		if(L18==1) num++;			
		if(L14==1) Acquit|=1<<3;
		if(L15==1) Acquit|=1<<4;
		if(L13==1) Acquit|=1<<2;
		if(L16==1) Acquit|=1<<5;
		if(L12==1) Acquit|=1<<1;
		if(L17==1) Acquit|=1<<6;
		if(L11==1) Acquit|=1<<0;
		if(L18==1) Acquit|=1<<7;
	}
	return Acquit;
//	num = 0;
}

