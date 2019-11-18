#include "us_101.h"
#include "usart.h"
#include "math.h"
#include "delay.h"
#include "lcd.h"
//����״̬
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽵͵�ƽ;1,�Ѿ����񵽵͵�ƽ��.
//[5:0]:����͵�ƽ������Ĵ���(����32λ��ʱ����˵,1us��������1,���ʱ��:4294��)
u8  TIM11CH1_CAPTURE_STA=0;	//���벶��״̬		    				
u32	TIM11CH1_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)


float get_distance2()
{
    int temp=0;
	u8 complete = 1;
    TIM5_CH1_Cap_Init(0XFFFFFFFF,168-1);
	if(complete == 1)
	{
		Trig1 = 1;
		delay_us(30);
		Trig1 = 0;
		complete = 0;
	}
	while(complete == 0)
	{
	
		if(TIM11CH1_CAPTURE_STA&0X80)        //�ɹ�������һ�θߵ�ƽ
		{
			temp=TIM11CH1_CAPTURE_STA&0X3F; 
			temp*=0XFFFFFFFF;		 		         //���ʱ���ܺ�
			temp+=TIM11CH1_CAPTURE_VAL;		   //�õ��ܵĸߵ�ƽʱ��
			//Ҫ��ʾ����ʱ������ע��
			temp = temp * 17/1000;               //����õ�����
			//printf("HIGH:%f cm\r\n",temp); //��ӡ����
		    // printf("t2.txt=\"dist:%f\"%c%c%c",temp,0xFF,0xFF,0xFF);//HMI��ָ��
			TIM11CH1_CAPTURE_STA=0;			     //������һ�β���
			complete = 1;
		}
		//break;
	}
	LCD_ShowxNum(0,80,temp,5,12,0x80);
    return temp;
}

void us_101_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��GPIOFʱ��

	//GPIOF9,F10��ʼ������
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��

	GPIO_ResetBits(GPIOG,GPIO_Pin_13);//��ʼ��GPIOF1Ϊ�͵�ƽ
	}

	//��ʱ��5ͨ��1���벶������
	//arr���Զ���װֵ(TIM2,TIM5��32λ��!!)
	//psc��ʱ��Ԥ��Ƶ��
	void TIM5_CH1_Cap_Init(u32 arr,u16 psc)
	{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM11_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11,ENABLE);  	//TIM5ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 	//ʹ��PORTAʱ��	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //����
	GPIO_Init(GPIOF,&GPIO_InitStructure); //��ʼ��PA0

	GPIO_PinAFConfig(GPIOF,GPIO_PinSource7,GPIO_AF_TIM11); //PA0����λ��ʱ��5

	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 

	TIM_TimeBaseInit(TIM11,&TIM_TimeBaseStructure);


	//��ʼ��TIM5���벶�����
	TIM11_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM11_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM11_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM11_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
	TIM11_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM11, &TIM11_ICInitStructure);
		
	TIM_ITConfig(TIM11,TIM_IT_Update|TIM_IT_CC1,ENABLE);//��������ж� ,����CC1IE�����ж�	

	TIM_Cmd(TIM11,ENABLE ); 	//ʹ�ܶ�ʱ��5


	NVIC_InitStructure.NVIC_IRQChannel = TIM1_TRG_COM_TIM11_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
}

//��ʱ��5�жϷ������	 
void TIM1_TRG_COM_TIM11_IRQHandler(void)
{ 	
 	if((TIM11CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{
		if(TIM_GetITStatus(TIM11, TIM_IT_Update) != RESET)//���
		{	     
			if(TIM11CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM11CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM11CH1_CAPTURE_STA|=0X80;		//��ǳɹ�������һ��
					TIM11CH1_CAPTURE_VAL=0XFFFFFFFF;
				}
				else TIM11CH1_CAPTURE_STA++;
			}	 
		}
		if(TIM_GetITStatus(TIM11, TIM_IT_CC1) != RESET)//����1���������¼�
		{	
			if(TIM11CH1_CAPTURE_STA&0X40)		//����һ���½��� 		
			{	  			
				TIM11CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
				TIM11CH1_CAPTURE_VAL=TIM_GetCapture1(TIM11);//��ȡ��ǰ�Ĳ���ֵ.
	 			TIM_OC1PolarityConfig(TIM11,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
			}
			else  								//��δ��ʼ,��һ�β���������
			{
				TIM11CH1_CAPTURE_STA=0;			//���
				TIM11CH1_CAPTURE_VAL=0;
				TIM11CH1_CAPTURE_STA|=0X40;		//��ǲ�����������
				TIM_Cmd(TIM11,DISABLE ); 	//�رն�ʱ��5
	 			TIM_SetCounter(TIM11,0);
	 			TIM_OC1PolarityConfig(TIM11,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
				TIM_Cmd(TIM11,ENABLE ); 	//ʹ�ܶ�ʱ��5
			}		    
		}			     	    					   
 	}
	TIM_ClearITPendingBit(TIM11, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
	
	//LCD_ShowxNum(0,80,get_distance2(),5,12,0x80);
}

