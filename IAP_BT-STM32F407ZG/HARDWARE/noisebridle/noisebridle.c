#include "noisebridle.h"

/*
*********************************************************************************************************
*	函数功能说明：火焰传感器功能初始化
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void noise_Init(void)
{
	RCC->AHB1ENR|=1<<5;//使能PORTF时钟
	GPIO_Set(GPIOF,PIN3,GPIO_MODE_IN,0,0,GPIO_PUPD_PD);//PF(3)下拉输入
	Bridle=0;
}

