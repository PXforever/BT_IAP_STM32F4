#include "wt61.h"
#include <string.h>
#include <stdio.h>
#include "led.h"


struct SAngle 	stcAngle;



void CopeSerial3Data(unsigned char ucData)
{
	static unsigned char ucRxBuffer[250];
	static unsigned char ucRxCnt = 0;	

	ucRxBuffer[ucRxCnt++]=ucData;	//���յ������ݴ��뻺������
	if (ucRxBuffer[0]!=0x55) //����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}//���ݲ���11�����򷵻�
	else
	{
		switch(ucRxBuffer[1])//�ж��������������ݣ�Ȼ���俽������Ӧ�Ľṹ���У���Щ���ݰ���Ҫͨ����λ���򿪶�Ӧ������󣬲��ܽ��յ�������ݰ�������
		{
//			case 0x50:	memcpy(&stcTime,&ucRxBuffer[2],8);break;//memcpyΪ�������Դ����ڴ濽��������������"string.h"�������ջ��������ַ����������ݽṹ�����棬�Ӷ�ʵ�����ݵĽ�����
//			case 0x51:	memcpy(&stcAcc,&ucRxBuffer[2],8);break;
//			case 0x52:	memcpy(&stcGyro,&ucRxBuffer[2],8);break;
			case 0x53:	memcpy(&stcAngle,&ucRxBuffer[2],8);break;	
		}
		
		ucRxCnt=0;//��ջ�����
	}

}


/*
*********************************************************************************************************
*	�� �� ��: Get_Angle
*	����˵��: ���ת��ĽǶ�
*	��    ��: 
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int Get_Angle(void)
{
	int a;
	a = (float)stcAngle.Angle[2]/32768*180;
	a = (float)((a + 360)%360);
//	printf("Angle:%.3f %.3f %.3f\r \n",(float)stcAngle.Angle[0]/32768*180,(float)stcAngle.Angle[1]/32768*180,(float)stcAngle.Angle[2]/32768*180);
//	USART3->CR1&=~(1<<5);
	return a;
}



/*
*********************************************************************************************************
*	�� �� ��: Get_Gesture
*	����˵��: Before and after the gesture
*	��    ��: 
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int Get_Gesture(void)
{
	int a;
	a = (int)Get_Angle();
	if(a < 0)
	{
		a = a -120;
	}else
	{
		
	}
//	printf("Angle:%.3f %.3f %.3f\r\n",(float)stcAngle.Angle[0]/32768*180,(float)stcAngle.Angle[1]/32768*180,(float)stcAngle.Angle[2]/32768*180);
	return a;
	
}
