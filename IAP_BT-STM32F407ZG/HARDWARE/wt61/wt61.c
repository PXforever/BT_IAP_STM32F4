#include "wt61.h"
#include <string.h>
#include <stdio.h>
#include "led.h"


struct SAngle 	stcAngle;



void CopeSerial3Data(unsigned char ucData)
{
	static unsigned char ucRxBuffer[250];
	static unsigned char ucRxCnt = 0;	

	ucRxBuffer[ucRxCnt++]=ucData;	//将收到的数据存入缓冲区中
	if (ucRxBuffer[0]!=0x55) //数据头不对，则重新开始寻找0x55数据头
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}//数据不满11个，则返回
	else
	{
		switch(ucRxBuffer[1])//判断数据是哪种数据，然后将其拷贝到对应的结构体中，有些数据包需要通过上位机打开对应的输出后，才能接收到这个数据包的数据
		{
//			case 0x50:	memcpy(&stcTime,&ucRxBuffer[2],8);break;//memcpy为编译器自带的内存拷贝函数，需引用"string.h"，将接收缓冲区的字符拷贝到数据结构体里面，从而实现数据的解析。
//			case 0x51:	memcpy(&stcAcc,&ucRxBuffer[2],8);break;
//			case 0x52:	memcpy(&stcGyro,&ucRxBuffer[2],8);break;
			case 0x53:	memcpy(&stcAngle,&ucRxBuffer[2],8);break;	
		}
		
		ucRxCnt=0;//清空缓存区
	}

}


/*
*********************************************************************************************************
*	函 数 名: Get_Angle
*	功能说明: 获得转弯的角度
*	形    参: 
*	返 回 值: 无
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
*	函 数 名: Get_Gesture
*	功能说明: Before and after the gesture
*	形    参: 
*	返 回 值: 无
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
