#include "beep.h" 

//初始化PG10为输出口		    
//BEEP IO初始化
//4.14改
void BEEP_Init(void)
{    	 
	RCC->AHB1ENR|=1<<6;    		//使能PORTG时钟 
	GPIO_Set(GPIOG,PIN11,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PD); //PG11设置,下拉
	BEEP=0;						//关闭蜂鸣器	
}






