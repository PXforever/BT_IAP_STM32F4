#include "timer.h"
#include "usart6.h"
#include "led.h"

//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif

extern u16 USART6_RX_STA;

//��ʱ��7�жϷ������		    
void TIM6_DAC_IRQHandler(void)
{ 	
#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
	OSIntEnter();    
#endif
	
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)//�Ǹ����ж�
	{	 			   
		USART6_RX_STA|=1<<15;	//��ǽ������
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );  //���TIM6�����жϱ�־    
		TIM_Cmd(TIM6, DISABLE);  //�ر�TIM6
		//iap_package_process();
	}
	
#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
	OSIntExit();    	//�˳��ж�	   
#endif	
}
 
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��		 
void TIM6_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);//TIM7ʱ��ʹ��    
	
	//��ʱ��TIM7��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM7�ж�,��������ж�
   
	TIM_Cmd(TIM6,ENABLE);//ʹ�ܶ�ʱ��7
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
}
	 
