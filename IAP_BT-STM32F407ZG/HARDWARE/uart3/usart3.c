#include "sys.h"
#include "usart3.h"	  
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"
//#include "timer.h"
#include "led.h"

 
//��ʼ��IO ����3
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������ 
void usart3_init(u32 pclk1,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;   
	temp=(float)(pclk1*1000000)/(bound*16);//�õ�USARTDIV,OVER8����Ϊ0
	mantissa=temp;				 	//�õ���������
	fraction=(temp-mantissa)*16; 	//�õ�С������,OVER8����Ϊ0	 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->AHB1ENR|=1<<1;   			//ʹ��PORTB��ʱ��  
	RCC->APB1ENR|=1<<18;  			//ʹ�ܴ���3ʱ�� 
	GPIO_Set(GPIOB,PIN10|PIN11,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);//PB10,PB11,���ù���,�������
 	GPIO_AF_Set(GPIOB,10,7);		//PB10,AF7
	GPIO_AF_Set(GPIOB,11,7);		//PB11,AF7  	   
	//����������
 	USART3->BRR=mantissa; 			// ����������	  

	USART3->CR1|=1<<3;  			//���ڷ���ʹ��  
	USART3->CR1|=1<<2;  			//���ڽ���ʹ��
	USART3->CR1|=1<<5;    			//���ջ������ǿ��ж�ʹ��	
	USART3->CR1|=1<<13;  			//����ʹ��  
	MY_NVIC_Init(0,0,USART3_IRQn,2);//��2�����ȼ�0,0,������ȼ� 
	USART3_RX_STA=0;				//���� 
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
			CopeSerial3Data((unsigned char)USART3->DR);//��������
		}
		

} 

#endif

