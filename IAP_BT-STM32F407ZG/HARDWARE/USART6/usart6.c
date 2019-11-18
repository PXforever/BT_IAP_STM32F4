#include "delay.h"
#include "usart6.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	  
#include "timer.h"
#include "usart.h"
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
   

//���ڷ��ͻ����� 	
__align(8) u8 USART6_TX_BUF[USART6_MAX_SEND_LEN]; 	//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
#ifdef USART6_RX_EN   								//���ʹ���˽���   	  
//���ڽ��ջ����� 	
u8 USART6_RX_BUF[USART6_MAX_RECV_LEN] __attribute__ ((at(0X20001000)));//���ջ���,���USART_REC_LEN���ֽ�,��ʼ��ַΪ0X20001000. ; 	
u8 USART6_PART_BUF[USART6_RX_PART_LEN] = {0};


//ͨ���жϽ�������2���ַ�֮���ʱ������100ms�������ǲ���һ������������.
//���2���ַ����ռ������100ms,����Ϊ����1����������.Ҳ���ǳ���100msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
u16 USART6_RX_STA=0;
u32 USART6_RX_COUNT = 0;

u32 APP_ID=0;
u32 APP_N=0;
u32 APP_LEN=0;

void iap_package_process(void)
{
	int length = 0;
	int i;
//	printf("receiver a part , processing this data\r\n");
//	printf("[%x][%x]\r\n", USART6_PART_BUF[0], USART6_PART_BUF[1]);
//	printf("[%x][%x][%x][%x]\r\n", USART6_PART_BUF[2], USART6_PART_BUF[3], USART6_PART_BUF[4], USART6_PART_BUF[5]);
//	printf("[%x][%x][%x][%x]\r\n", USART6_PART_BUF[6], USART6_PART_BUF[7], USART6_PART_BUF[8], USART6_PART_BUF[9]);
//	printf("[%x][%x][%x][%x]\r\n", USART6_PART_BUF[10], USART6_PART_BUF[11], USART6_PART_BUF[12], USART6_PART_BUF[13]);
//	printf("enter %s,re:%d\r\n", __FUNCTION__, USART6_RX_COUNT);
	//Ϊ������һ�Σ������д���������һ��ֻ�����ֿ��ܣ�1�����޶����ȣ�2��ʱδ�ӵ�����
	if(USART6_RX_STA == 0) { return;}
	//δ�ﵽ�����ݳ��ȣ��жϲ�Ϊ��
	if((USART6_RX_COUNT < USART6_RX_PART_MIN_LEN) | ((USART6_PART_BUF[0] != 0xff))) 
	{
		USART6_RX_COUNT = 0; 
		USART6_RX_STA = 0;
		//printf("it is not a package\r\n");
		return;
	}
	
	if(USART6_PART_BUF[0]==0xff)
	{
			if(USART6_PART_BUF[1]==0xf0)
			{
					APP_N = 0;
					APP_N |= USART6_PART_BUF[2] << 24;
					APP_N |= USART6_PART_BUF[3] << 16;
					APP_N |= USART6_PART_BUF[4] <<8;
					APP_N |= USART6_PART_BUF[5];

					APP_ID = 0;
					APP_ID |= USART6_PART_BUF[6] << 24;
					APP_ID |= USART6_PART_BUF[7] << 16;
					APP_ID |= USART6_PART_BUF[8] <<8;
					APP_ID |= USART6_PART_BUF[9];

					length = 0;
					length |= USART6_PART_BUF[10] << 24;
					length |= USART6_PART_BUF[11] << 16;
					length |= USART6_PART_BUF[12] <<8;
					length |= USART6_PART_BUF[13];

					//printf("legth:%d\r\n", length);
					//printf("data part :%d, APP_ID:%d, LEN:%d\r\n", APP_N, APP_ID, length);	
					
					for(i = 0;i<length;i++)
					{
						USART6_RX_BUF[i+APP_LEN] = USART6_PART_BUF[i+14];
						USART6_RX_COUNT++;				
					}
					
					APP_LEN += length;
					//		for(i=0;i<length+11;i++)
					//		{
					//			USART6_PART_BUF[i]=0;
					//		}
					memset(USART6_PART_BUF, 0, USART6_RX_PART_LEN);
					USART6_RX_COUNT = 0;
					USART6_RX_STA = 0;
					//A ACK, C check, ON!
					u3_printf("A|C|ON!\xff"); 											
			}
	}
}


void USART6_IRQHandler(void)
{	
	u8 res;
	//static u16 i = 0;
	
#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
	OSIntEnter();    
#endif	
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)//���յ�����
	{	 
		//printf("enter %s\r\n", __FUNCTION__);
		res =USART_ReceiveData(USART6);
		//printf("%d ", res);		
		if((USART6_RX_STA&(1 << 15))==0 )//�������һ������,��û�б�����,���ٽ�����������
		{ 
			if(USART6_RX_COUNT < USART6_RX_PART_LEN)		//�����Խ�������
			{
				TIM_SetCounter(TIM6,0);//���������        				 
				if(USART6_RX_STA == 0)		
					TIM_Cmd(TIM6, ENABLE);  //ʹ�ܶ�ʱ��6
				USART6_PART_BUF[USART6_RX_COUNT++] = res;		//��¼���յ���ֵ	
			}else 
			{
				USART6_RX_STA |= 1 << 15;					//ǿ�Ʊ�ǽ������
			} 
		} 
	}

#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
	OSIntExit();    			//�˳��ж�	   
#endif	
}  
#endif	
//��ʼ��IO ����3
//bound:������	  
void usart6_init(u32 bound)
{  
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
 
	USART_DeInit(USART6);  //��λ����6
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE); //ʹ��GPIOGʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//ʹ��USART6ʱ��
	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14; //GPIOG9��GPIOG14��ʼ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOG,&GPIO_InitStructure); //��ʼ��GPIOB11����GPIOB10
	
	
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART6); //GPIOG9����ΪUSART6
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6); //GPIOG14����ΪUSART6	  
	
	USART_InitStructure.USART_BaudRate = bound;//������һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART6, &USART_InitStructure); //��ʼ������3
	
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//�����ж�  
		
	USART_Cmd(USART6, ENABLE);                    //ʹ�ܴ��� 
	
 
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	TIM6_Int_Init(100-1,8400-1);	//10ms�ж�һ��
	
  TIM_Cmd(TIM6, DISABLE); //�رն�ʱ��7
	
	USART6_RX_STA=0;				//���� 
}

//����3,printf ����
//ȷ��һ�η������ݲ�����USART3_MAX_SEND_LEN�ֽ�
void u3_printf(char* fmt,...)  
{  
	u16 i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART6_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART6_TX_BUF);//�˴η������ݵĳ���
	for(j=0;j<i;j++)//ѭ����������
	{
	  while(USART_GetFlagStatus(USART6,USART_FLAG_TC)==RESET);  //�ȴ��ϴδ������ 
		USART_SendData(USART6,(uint8_t)USART6_TX_BUF[j]); 	 //�������ݵ�����3 
	}
	
}
void clear_send_flags()
{
	APP_N = 0;
	APP_ID = 0;
	APP_LEN = 0;
	USART6_RX_COUNT = 0;
	USART6_RX_STA = 0;
}

//void is_dev_online()
//{
//	
//}
 
 
 











