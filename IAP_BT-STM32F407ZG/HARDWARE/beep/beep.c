#include "beep.h" 

//��ʼ��PG10Ϊ�����		    
//BEEP IO��ʼ��
//4.14��
void BEEP_Init(void)
{    	 
	RCC->AHB1ENR|=1<<6;    		//ʹ��PORTGʱ�� 
	GPIO_Set(GPIOG,PIN11,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PD); //PG11����,����
	BEEP=0;						//�رշ�����	
}






