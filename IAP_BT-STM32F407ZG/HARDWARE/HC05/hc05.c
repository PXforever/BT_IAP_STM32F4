#include "delay.h" 			 
#include "usart.h" 			 
#include "usart6.h" 			 
#include "hc05.h" 
#include "led.h" 
#include "string.h"	 
#include "math.h"

//��ʼ��ATK-HC05ģ��
//����ֵ:0,�ɹ�;1,ʧ��.
u8 HC05_Init(void)
{
	u8 retry=15,t;	  		 
	u8 temp=1;

	
	GPIO_InitTypeDef GPIO_InitStructure;
 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��GPIOGʱ��
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //LED��Ӧ����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��GPIOC0
	//GPIO_SetBits(GPIOG,GPIO_Pin_7);
 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//KEY��Ӧ����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOG, &GPIO_InitStructure); //�����趨������ʼ��PF6

	GPIO_SetBits(GPIOG,GPIO_Pin_8);
 	
	usart6_init(9600);	//��ʼ������3Ϊ:9600,������
	delay_ms(300);
	
	while(retry--)
	{
		HC05_KEY=1;					//KEY�ø�,����ATģʽ
		delay_ms(10);
		u3_printf("AT\r\n");		//����AT����ָ��
		HC05_KEY=0;					//KEY����,�˳�ATģʽ
		for(t=0;t<10;t++) 			//��ȴ�50ms,������HC05ģ��Ļ�Ӧ
		{
			if(USART6_RX_STA&0X8000) break;
			delay_ms(5);
		}		
		if(USART6_RX_STA&0X8000)	//���յ�һ��������
		{
			temp = USART6_RX_STA&0X7FFF;	//�õ����ݳ���
			USART6_RX_STA=0;			 
			if(temp == 4 && USART6_PART_BUF[0] == 'O' && USART6_PART_BUF[1] == 'K')
			{
				temp=0;//���յ�OK��Ӧ
				break;
			}
		}			    		
	}		    
	if(retry==0)temp=1;	//���ʧ��
	return temp;	 
}	 
//��ȡATK-HC05ģ��Ľ�ɫ
//����ֵ:0,�ӻ�;1,����;0XFF,��ȡʧ��.							  
u8 HC05_Get_Role(void)
{	 		    
	u8 retry=0X0F;
	u8 temp,t;
	while(retry--)
	{
		HC05_KEY=1;					//KEY�ø�,����ATģʽ
		delay_ms(10);
		u3_printf("AT+ROLE?\r\n");	//��ѯ��ɫ
		for(t=0;t<20;t++) 			//��ȴ�200ms,������HC05ģ��Ļ�Ӧ
		{
			delay_ms(10);
			if(USART6_RX_STA&0X8000)break;
		}		
		HC05_KEY=0;					//KEY����,�˳�ATģʽ
		if(USART6_RX_STA&0X8000)	//���յ�һ��������
		{
			temp=USART6_RX_STA&0X7FFF;	//�õ����ݳ���
			USART6_RX_STA=0;			 
			if(temp==13&&USART6_RX_BUF[0]=='+')//���յ���ȷ��Ӧ����
			{
				temp=USART6_RX_BUF[6]-'0';//�õ�����ģʽֵ
				break;
			}
		}		
	}
	if(retry==0)temp=0XFF;//��ѯʧ��.
	return temp;
} 							   
//ATK-HC05��������
//�˺�����������ATK-HC05,�����ڽ�����OKӦ���ATָ��
//atstr:ATָ�.����:"AT+RESET"/"AT+UART=9600,0,0"/"AT+ROLE=0"���ַ���
//����ֵ:0,���óɹ�;����,����ʧ��.							  
u8 HC05_Set_Cmd(u8* atstr)
{	 		    
	u8 retry = 25;
	u8 temp,t;
	while(retry--)
	{
		HC05_KEY=1;					//KEY�ø�,����ATģʽ
		delay_ms(10);
		u3_printf("%s\r\n",atstr);	//����AT�ַ���
		delay_us(100);	
		HC05_KEY=0;					//KEY����,�˳�ATģʽ
		for(t=0;t<20;t++) 			//��ȴ�100ms,������HC05ģ��Ļ�Ӧ
		{
			if(USART6_RX_STA&0X8000)break;
			delay_ms(5);
		}		
		if(USART6_RX_STA&0X8000)	//���յ�һ��������
		{
			temp=USART6_RX_STA&0X7FFF;	//�õ����ݳ���
			USART6_RX_STA=0;			 
			if(temp==4&&USART6_RX_BUF[0]=='O')//���յ���ȷ��Ӧ����
			{			
				temp=0;
				break;			 
			}
		}	
	}
	if(retry==0)temp=0XFF;//����ʧ��.
	return temp;
} 
///////////////////////////////////////////////////////////////////////////////////////////////////
//ͨ���ú���,��������USMART,���Խ��ڴ���3�ϵ�ATK-HC05ģ��
//str:���.(����ע�ⲻ����Ҫ������س���)
void HC05_CFG_CMD(u8 *str)
{					  
	u8 temp;
	u8 t;		  
	HC05_KEY=1;						//KEY�ø�,����ATģʽ
	delay_ms(10);
	u3_printf("%s\r\n",(char*)str); //����ָ��
	for(t=0;t<50;t++) 				//��ȴ�500ms,������HC05ģ��Ļ�Ӧ
	{
		if(USART6_RX_STA&0X8000)break;
		delay_ms(10);
	}									    
	HC05_KEY=0;						//KEY����,�˳�ATģʽ
	if(USART6_RX_STA&0X8000)		//���յ�һ��������
	{
		temp=USART6_RX_STA&0X7FFF;	//�õ����ݳ���
		USART6_RX_STA=0;
		USART6_RX_BUF[temp]=0;		//�ӽ�����		 
		printf("\r\n%s",USART6_RX_BUF);//���ͻ�Ӧ���ݵ�����1
	} 				 
}



///////////////////////////////////////////////////////////////////////////////////////////////////
//ͨ���ú���,��������USMART,���Խ��ڴ���3�ϵ�ATK-HC05ģ��
//str:���.(����ע�ⲻ����Ҫ������س���)
u8 HC05_SET_MODE1(void)
{		
	u8 role = 4;
	u3_printf("AT+NAME=IAP TEST BT\r\n");
	delay_ms(400);	
	u3_printf("AT+ROLE=0\r\n");
	delay_ms(300);
	role = HC05_Get_Role();
	delay_ms(300);
	if(role == 0)
	{
		//printf("Slave Mode\r\n");
		return 0;
	}
	else if(role == 1)
	{
		//printf("Master Mode\r\n");
		return 1;
	}
	else 
		;
		//printf("error%d\r\n", role);
	return 4;
		
}










