#include "us_100.h"
#include "usart.h"
#include "math.h"
#include "delay.h"
#include "lcd.h"
//����״̬
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽵͵�ƽ;1,�Ѿ����񵽵͵�ƽ��.
//[5:0]:����͵�ƽ������Ĵ���(����32λ��ʱ����˵,1us��������1,���ʱ��:4294��)
u8  TIM12CH1_CAPTURE_STA=0;	//���벶��״̬		    				
u32	TIM12CH1_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)

float get_distance1()
{
    int temp=0;
		u8 complete = 1;
    TIM12_CH1_Cap_Init(0XFFFFFFFF,84-1);
    if(complete == 1)
	{
		Trig = 1;
    delay_us(30);
		Trig = 0;
		complete = 0;
	}     
	while(complete == 0)
	{
	
		if(TIM12CH1_CAPTURE_STA&0X80)        //�ɹ�������һ�θߵ�ƽ
		{
			temp=TIM12CH1_CAPTURE_STA&0X3F; 
			temp*=0XFFFFFFFF;		 		         //���ʱ���ܺ�
			temp+=TIM12CH1_CAPTURE_VAL;		   //�õ��ܵĸߵ�ƽʱ��
			//Ҫ��ʾ����ʱ������ע��
			temp = temp * 17/1000;               //����õ�����
			//printf("HIGH:%f cm\r\n",temp); //��ӡ����
			// printf("t2.txt=\"dist:%f\"%c%c%c",temp,0xFF,0xFF,0xFF);//HMI��ָ��
			TIM12CH1_CAPTURE_STA=0;			     //������һ�β���
			complete = 1;
		 }
		//break;
	 }
	LCD_ShowxNum(0,45,temp,5,12,0x80);
	 return temp;
}

void us_100_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOFʱ��

	//GPIOF9,F10��ʼ������
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��

	GPIO_ResetBits(GPIOB,GPIO_Pin_12);//��ʼ��GPIOF1Ϊ�͵�ƽ
}




void TIM12_CH1_Cap_Init(u32 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM12_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);  	//TIM5ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//ʹ��PORTAʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��PA0

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_TIM12); //PA0����λ��ʱ��5
  
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM12,&TIM_TimeBaseStructure);
	

	//��ʼ��TIM5���벶�����
	TIM12_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM12_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM12_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM12_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
	TIM12_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM12, &TIM12_ICInitStructure);
	
	TIM_ITConfig(TIM12,TIM_IT_Update|TIM_IT_CC1,ENABLE);//��������ж� ,����CC1IE�����ж�
		
	
	TIM_Cmd(TIM12,ENABLE ); 	//ʹ�ܶ�ʱ��5

	NVIC_InitStructure.NVIC_IRQChannel = TIM8_BRK_TIM12_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
	
}


//��ʱ��5�жϷ������
void TIM8_BRK_TIM12_IRQHandler(void)
{ 		    
	if((TIM12CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{
		if(TIM_GetITStatus(TIM12, TIM_IT_Update) != RESET)//���
		{	     
			if(TIM12CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM12CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM12CH1_CAPTURE_STA|=0X80;		//��ǳɹ�������һ��
					TIM12CH1_CAPTURE_VAL=0XFFFFFFFF;
				}
				else TIM12CH1_CAPTURE_STA++;
			}	 
		}
		if(TIM_GetITStatus(TIM12, TIM_IT_CC1) != RESET)//����1���������¼�
		{	
			if(TIM12CH1_CAPTURE_STA&0X40)		//����һ���½��� 		
			{	  			
				TIM12CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
				TIM12CH1_CAPTURE_VAL=TIM_GetCapture1(TIM12);//��ȡ��ǰ�Ĳ���ֵ.
				TIM_OC1PolarityConfig(TIM12,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
			}
			else  								//��δ��ʼ,��һ�β���������
			{
				TIM12CH1_CAPTURE_STA=0;			//���
				TIM12CH1_CAPTURE_VAL=0;
				TIM12CH1_CAPTURE_STA|=0X40;		//��ǲ�����������
				TIM_Cmd(TIM12,DISABLE ); 	//�رն�ʱ��5
				TIM_SetCounter(TIM12,0);
				TIM_OC1PolarityConfig(TIM12,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
				TIM_Cmd(TIM12,ENABLE ); 	//ʹ�ܶ�ʱ��5
			}		    
		}			     	    					   
	}
	TIM_ClearITPendingBit(TIM12, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
	
	//LCD_ShowxNum(0,45,get_distance1(),5,12,0x80);
}


