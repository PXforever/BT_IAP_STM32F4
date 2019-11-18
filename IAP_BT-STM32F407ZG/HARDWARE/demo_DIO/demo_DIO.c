#include "demo_DIO.h"
#include "fan.h"
#include "beep.h"
#include "delay.h"
#include "usart.h"
#include "usart3.h"	 
#include "led.h"
#include "wt61.h"
#include "lcd.h"
#include "drive.h"
#include "math.h"
#include "find_fire.h"

int flag;
float angle;//��¼���뷿��ǰת�ĽǶ�
/*
*********************************************************************************************************
*	�� �� ��: gostr_usix
*	����˵��: �������ֱ�߳���
*	��    ��: speed�����ٶ�goal_Angle���߽Ƕ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/

u8 Flase_Angle(u16 goal_Angle,float sta_FalseAngle)
{
	if(cos((goal_Angle-Get_Angle())/57.3) > cos(sta_FalseAngle/57.3))
	{
			return 1;
	}
	else
	{
			return 0;
	}
}

void gostr_usix(int goal_Angle,int speed)
{
	double va=0;
	double goal=0;
	va = Get_Angle()+360-goal_Angle;
	goal = fmod(va, 360.0);
	
	if(Flase_Angle(goal_Angle,6) == 1)
	{
			Set_Motor(speed,speed);
	}
	else
	{
			if(goal > 180 && Flase_Angle(goal_Angle,8) == 1)
			{
				Set_Motor(speed - 20,speed);
			}
			else if(goal < 180 && Flase_Angle(goal_Angle,8) == 1)
			{
				Set_Motor(speed,speed - 20);
			}
			
			else if(goal > 180 && Flase_Angle(goal_Angle,10) == 1)
			{
				Set_Motor(speed - 25,speed);
			}
			else if(goal < 180 && Flase_Angle(goal_Angle,10) == 1)
			{
				Set_Motor(speed,speed - 25);
			}
			
			else if(goal > 180 && Flase_Angle(goal_Angle,20) == 1)
			{
				Set_Motor(speed - 30,speed);
			}
			else if(goal < 180 && Flase_Angle(goal_Angle,20) == 1)
			{
				Set_Motor(speed,speed - 30);
			}
			
			else if(goal > 180 && Flase_Angle(goal_Angle,30) == 1)
			{
				Set_Motor(speed - 40,speed);
			}
			else if(goal < 180 && Flase_Angle(goal_Angle,30) == 1)
			{
				Set_Motor(speed,speed - 40);
			}
			
			else if(goal > 180 && Flase_Angle(goal_Angle,40) == 1)
			{
				Set_Motor(-30,speed);
			}
			else if(goal < 180 && Flase_Angle(goal_Angle,40) == 1)
			{
				Set_Motor(speed,-30);
			}
	}
}
/*
*********************************************************************************************************
*	�� �� ��: Infrared_L_Turn
*	����˵��: ������ǰ����ת�����
*	��    ��: speed�����ٶ� angle�����Ƕ� engine������ͷ
*	�� �� ֵ:��
*********************************************************************************************************
*/
void Infrared_L_Turn(u8 speed,int angle,int engine,int slow)
{
	while(1)
	{
			if(engine==0)
			{
					if((abs(angle-Get_Angle())>0&&abs(angle-Get_Angle())<5))
					{				
						CarAdvance(0);
						break;
					}
					else if(abs(angle-Get_Angle())>4&&abs(angle-Get_Angle())<14)
					{
						SetLeftWhell(0,(speed-5*slow)/100);SetRightWhell(0,speed-5*slow); //???
					}
					else if(abs(angle-Get_Angle())>14&&abs(angle-Get_Angle())<24)
					{
						SetLeftWhell(0,(speed-4*slow)/100);SetRightWhell(0,speed-4*slow); //???
					}
					else if(abs(angle-Get_Angle())>24&&abs(angle-Get_Angle())<34)
					{
						SetLeftWhell(0,(speed-3*slow)/100);SetRightWhell(0,speed-3*slow); //???
					}
					else if(abs(angle-Get_Angle())>34&&abs(angle-Get_Angle())<44)
					{
						SetLeftWhell(0,(speed-2*slow)/100);SetRightWhell(0,speed-2*slow); //???
					}
					else if(abs(angle-Get_Angle())>44&&abs(angle-Get_Angle())<54)
					{
						SetLeftWhell(0,(speed-1*slow)/100);SetRightWhell(0,speed-1*slow); //???
					}
					else
					{
						SetLeftWhell(0,speed/2);SetRightWhell(0,speed);
					}			
			}
		else if(engine==1)
		{
					if((abs(angle-Get_Angle())>0&&abs(angle-Get_Angle())<5))
					{				
						CarRetreat(0);
						break;
					}
					else if(abs(angle-Get_Angle())>4&&abs(angle-Get_Angle())<14)
					{
						SetLeftWhell(0,slow);SetRightWhell(1,speed-4*slow); //???
					}
					else if(abs(angle-Get_Angle())>14&&abs(angle-Get_Angle())<24)
					{
						SetLeftWhell(0,slow);SetRightWhell(1,speed-3*slow); //???
					}
					else if(abs(angle-Get_Angle())>24&&abs(angle-Get_Angle())<34)
					{
						SetLeftWhell(0,slow);SetRightWhell(1,speed-3*slow); //???
					}
					else if(abs(angle-Get_Angle())>34&&abs(angle-Get_Angle())<44)
					{
						SetLeftWhell(0,slow*2);SetRightWhell(1,speed-2*slow); //???
					}
					else if(abs(angle-Get_Angle())>44&&abs(angle-Get_Angle())<54)
					{
						SetLeftWhell(0,slow*2);SetRightWhell(1,speed-1*slow); //???
					}
					else
					{
						SetLeftWhell(0,slow*3);SetRightWhell(1,speed);
					}			
		}
	}
}


/*
*********************************************************************************************************
*	�� �� ��: Infrared_L_Turn1
*	����˵��: ������󻡶�ת�����
*	��    ��: speed�����ٶ�angle�����Ƕ�engine������ͷ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Infrared_L_Turn1(u8 speed,int angle,int engine,int slow)
{
	while(1)
	{
		if(engine==0)
		{
				if((abs(angle-Get_Angle())>0&&abs(angle-Get_Angle())<5))
				{				
					CarAdvance(0);
					break;
				}
				else if(abs(angle-Get_Angle())>4&&abs(angle-Get_Angle())<14)
				{
					SetLeftWhell(0,slow);SetRightWhell(0,speed-4*slow); //???
				}
				else if(abs(angle-Get_Angle())>14&&abs(angle-Get_Angle())<24)
				{
					SetLeftWhell(0,slow);SetRightWhell(0,speed-3*slow); //???
				}
				else if(abs(angle-Get_Angle())>24&&abs(angle-Get_Angle())<34)
				{
					SetLeftWhell(0,slow);SetRightWhell(0,speed-3*slow); //???
				}
				else if(abs(angle-Get_Angle())>34&&abs(angle-Get_Angle())<44)
				{
					SetLeftWhell(0,slow);SetRightWhell(0,speed-2*slow); //???
				}
				else if(abs(angle-Get_Angle())>44&&abs(angle-Get_Angle())<54)
				{
					SetLeftWhell(0,slow);SetRightWhell(0,speed-1*slow); //???
				}
				else
				{
					SetLeftWhell(0,slow*2);SetRightWhell(0,speed);
				}			
		}
		else if(engine==1)
		{
//				if(abs(angle-Get_Angle())>0&&abs(angle-Get_Angle())<9)
//				{
//					CarAdvance(0);
//					break;
//				}
//				else
//				{
//					SetLeftWhell(0,0);SetRightWhell(1,speed+slow*2); //???
//				}
					if((abs(angle-Get_Angle())>0&&abs(angle-Get_Angle())<9))
					{				
						CarRetreat(0);
						break;
					}
					else if(abs(angle-Get_Angle())>8&&abs(angle-Get_Angle())<15)
					{
						SetLeftWhell(1,slow);SetRightWhell(1,speed-5*slow); //???
					}
					else if(abs(angle-Get_Angle())>14&&abs(angle-Get_Angle())<24)
					{
						SetLeftWhell(1,slow);SetRightWhell(1,speed-4*slow); //???
					}
					else if(abs(angle-Get_Angle())>24&&abs(angle-Get_Angle())<34)
					{
						SetLeftWhell(1,slow);SetRightWhell(1,speed-3*slow); //???
					}
					else if(abs(angle-Get_Angle())>34&&abs(angle-Get_Angle())<44)
					{
						SetLeftWhell(1,slow);SetRightWhell(1,speed-2*slow); //???
					}
					else if(abs(angle-Get_Angle())>44&&abs(angle-Get_Angle())<54)
					{
						SetLeftWhell(1,slow);SetRightWhell(1,speed-1*slow); //???
					}
					else
					{
						SetLeftWhell(1,0);SetRightWhell(1,speed);
					}	
		}
	}
}


/*
*********************************************************************************************************
*	�� �� ��: Infrared_L_Turn2
*	����˵��: ����ԭ����ת�����
*	��    ��: speed�����ٶ�angle�����Ƕ�engine������ͷ
*	�� �� ֵ: ��
*********************************************************************************************************
*/

void Infrared_L_Turn2(u8 speed,int angle,int engine,int slow)
{
	while(1)
	{
		if(engine==0)
		{
			if((abs(angle-Get_Angle())>0&&abs(angle-Get_Angle())<5))
			{				
				CarAdvance(0);
				break;
			}
			else if(abs(angle-Get_Angle())>4&&abs(angle-Get_Angle())<14)
			{
				SetLeftWhell(0,speed-5*slow);SetRightWhell(0,speed-5*slow); //???
			}
			else if(abs(angle-Get_Angle())>14&&abs(angle-Get_Angle())<24)
			{
				SetLeftWhell(0,speed-4*slow);SetRightWhell(0,speed-4*slow); //???
			}
			else if(abs(angle-Get_Angle())>24&&abs(angle-Get_Angle())<34)
			{
				SetLeftWhell(0,speed-3*slow);SetRightWhell(0,speed-3*slow); //???
			}
			else if(abs(angle-Get_Angle())>34&&abs(angle-Get_Angle())<44)
			{
				SetLeftWhell(0,speed-2*slow);SetRightWhell(0,speed-2*slow); //???
			}
			else if(abs(angle-Get_Angle())>44&&abs(angle-Get_Angle())<54)
			{
				SetLeftWhell(0,speed-1*slow);SetRightWhell(0,speed-1*slow); //???
			}
			else
			{
				SetLeftWhell(0,speed);SetRightWhell(0,speed);
			}			
		}
	}		
}

/*
*********************************************************************************************************
*	�� �� ��: Infrared_L_Turn3
*	����˵��: �����󻡶�ת�����
*	��    ��: speed�����ٶ�angle�����Ƕ�engine������ͷ
*	�� �� ֵ: ��
*********************************************************************************************************
*/

void Infrared_L_Turn3(u8 speed,int angle,int engine,int slow)
{
	while(1)
	{
		if(engine==0)
		{
				if((abs(angle-Get_Angle())>0&&abs(angle-Get_Angle())<9))
				{				
					CarAdvance(0);
					break;
				}
				else if(abs(angle-Get_Angle())>8&&abs(angle-Get_Angle())<14)
				{
					SetLeftWhell(0,(speed-5*slow)/100);SetRightWhell(0,speed-4*slow); //???
				}
				else if(abs(angle-Get_Angle())>14&&abs(angle-Get_Angle())<24)
				{
					SetLeftWhell(0,(speed-4*slow)/100);SetRightWhell(0,speed-3*slow); //???
				}
				else if(abs(angle-Get_Angle())>24&&abs(angle-Get_Angle())<34)
				{
					SetLeftWhell(0,(speed-3*slow)/100);SetRightWhell(0,speed-2*slow); //???
				}
				else if(abs(angle-Get_Angle())>34&&abs(angle-Get_Angle())<44)
				{
					SetLeftWhell(0,(speed-2*slow)/100);SetRightWhell(0,speed-2*slow); //???
				}
				else if(abs(angle-Get_Angle())>44&&abs(angle-Get_Angle())<54)
				{
					SetLeftWhell(0,(speed-1*slow)/100);SetRightWhell(0,speed-1*slow); //???
				}
				else
				{
					SetLeftWhell(0,speed/4);SetRightWhell(0,speed);
				}			
		}
		else if(engine==1)
		{

				if((abs(angle-Get_Angle())>0&&abs(angle-Get_Angle())<9))
					{				
						CarRetreat(0);
						break;
					}
					else if(abs(angle-Get_Angle())>8&&abs(angle-Get_Angle())<15)
					{
						SetLeftWhell(1,slow*2);SetRightWhell(1,speed-3*slow); //???
					}
					else if(abs(angle-Get_Angle())>14&&abs(angle-Get_Angle())<24)
					{
						SetLeftWhell(1,slow);SetRightWhell(1,speed-2*slow); //???
					}
					else if(abs(angle-Get_Angle())>24&&abs(angle-Get_Angle())<34)
					{
						SetLeftWhell(1,slow);SetRightWhell(1,speed-1*slow); //???
					}
					else if(abs(angle-Get_Angle())>34&&abs(angle-Get_Angle())<44)
					{
						SetLeftWhell(1,slow);SetRightWhell(1,speed-1*slow); //???
					}
					else if(abs(angle-Get_Angle())>44&&abs(angle-Get_Angle())<54)
					{
						SetLeftWhell(1,slow);SetRightWhell(1,speed-1*slow); //???
					}
					else
					{
						SetLeftWhell(1,0);SetRightWhell(1,speed);
					}	
		}
   }			
}
/*
*********************************************************************************************************
*	�� �� ��: Infrared_R_Turn
*	����˵��: ������ǰ����ת�����
*	��    ��: speed�����ٶ�angle�����Ƕ�engine������ͷ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Infrared_R_Turn(u8 speed,int angle,int engine,int slow)
{
	while(1)
	{
	if(engine==0)
	{
			if((abs(angle-Get_Angle())>0&&abs(angle-Get_Angle())<5))
			{				
				CarAdvance(0);
				break;
			}
			else if(abs(angle-Get_Angle())>4&&abs(angle-Get_Angle())<14)
			{
				SetLeftWhell(1,speed-6*slow);SetRightWhell(1,(speed-1*slow)/100); //???
			}
			else if(abs(angle-Get_Angle())>14&&abs(angle-Get_Angle())<24)
			{
				SetLeftWhell(1,speed-4*slow);SetRightWhell(1,(speed-1*slow)/100); //???
			}
			else if(abs(angle-Get_Angle())>24&&abs(angle-Get_Angle())<34)
			{
				SetLeftWhell(1,speed-3*slow);SetRightWhell(1,(speed-1*slow)/100); //???
			}
			else if(abs(angle-Get_Angle())>34&&abs(angle-Get_Angle())<44)
			{
				SetLeftWhell(1,speed-2*slow);SetRightWhell(1,(speed-1*slow)/100); //???
			}
			else if(abs(angle-Get_Angle())>44&&abs(angle-Get_Angle())<54)
			{
				SetLeftWhell(1,speed-1*slow);SetRightWhell(1,(speed-1*slow)/100); //???
			}
			else
			{
				SetLeftWhell(1,speed);SetRightWhell(1,speed/2);
			}			
	}
	else if(engine==1)
	{
		while(1)
		{
			if((angle-(float)3 < Get_Angle()) && (Get_Angle() < angle+(float)3))
			{				
				break;
			}
			else
			{
				SetLeftWhell(0,speed/2);SetRightWhell(1,speed); //???
			}
		}
	}
	}
}
/*
*********************************************************************************************************
*	�� �� ��: Infrared_R_Turn1
*	����˵��: �����Һ󻡶�ת�����
*	��    ��: speed�����ٶ�angle�����Ƕ�engine������ͷ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Infrared_R_Turn1(u8 speed,int angle,int engine,int slow)
{
	while(1)
	{
	if(engine==0)
	{
			if((abs(angle-Get_Angle())>0&&abs(angle-Get_Angle())<5))
			{				
				CarAdvance(0);
				break;
			}
			else if(abs(angle-Get_Angle())>4&&abs(angle-Get_Angle())<14)
			{
				SetLeftWhell(0,(speed-5*slow)/100);SetRightWhell(0,speed-5*slow); //???
			}
			else if(abs(angle-Get_Angle())>14&&abs(angle-Get_Angle())<24)
			{
				SetLeftWhell(0,(speed-4*slow)/100);SetRightWhell(0,speed-4*slow); //???
			}
			else if(abs(angle-Get_Angle())>24&&abs(angle-Get_Angle())<34)
			{
				SetLeftWhell(0,(speed-3*slow)/100);SetRightWhell(0,speed-3*slow); //???
			}
			else if(abs(angle-Get_Angle())>34&&abs(angle-Get_Angle())<44)
			{
				SetLeftWhell(0,(speed-2*slow)/100);SetRightWhell(0,speed-2*slow); //???
			}
			else if(abs(angle-Get_Angle())>44&&abs(angle-Get_Angle())<54)
			{
				SetLeftWhell(0,(speed-1*slow)/100);SetRightWhell(0,speed-1*slow); //???
			}
			else
			{
				SetLeftWhell(0,speed/2);SetRightWhell(0,speed);
			}			
	}
	else if(engine==1)
	{
		while(1)
		{
			if((angle-(float)8 < Get_Angle()) && (Get_Angle() < angle+(float)8))
			{				
				break;
			}
			else
			{
				SetLeftWhell(0,speed);SetRightWhell(1,speed/2); //???
			}
		}
	}
	}
}


/*
*********************************************************************************************************
*	�� �� ��: Infrared_R_Turn2
*	����˵��: ������ԭ��ת�����
*	��    ��: speed�����ٶ�angle�����Ƕ�engine������ͷ;slowֵԽ�� ��Խƫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/

void Infrared_R_Turn2(u8 speed,int angle,int engine,int slow)
{
	while(1)
	{
		if(engine==0)
		{
				if((abs(angle-Get_Angle())>0&&abs(angle-Get_Angle())<5))
				{				
					CarAdvance(0);
					break;
				}
				else if(abs(angle-Get_Angle())>4&&abs(angle-Get_Angle())<14)
				{
					SetLeftWhell(1,speed-5*slow);SetRightWhell(1,speed-5*slow); //???
				}
				else if(abs(angle-Get_Angle())>14&&abs(angle-Get_Angle())<24)
				{
					SetLeftWhell(1,speed-4*slow);SetRightWhell(1,speed-4*slow); //???
				}
				else if(abs(angle-Get_Angle())>24&&abs(angle-Get_Angle())<34)
				{
					SetLeftWhell(1,speed-3*slow);SetRightWhell(1,speed-3*slow); //???
				}
				else if(abs(angle-Get_Angle())>34&&abs(angle-Get_Angle())<44)
				{
					SetLeftWhell(1,speed-2*slow);SetRightWhell(1,speed-2*slow); //???
				}
				else if(abs(angle-Get_Angle())>44&&abs(angle-Get_Angle())<54)
				{
					SetLeftWhell(1,speed-1*slow);SetRightWhell(1,speed-1*slow); //???
				}
				else
				{
					SetLeftWhell(1,speed);SetRightWhell(1,speed/2);
				}			
		}
		else if(engine==1)
		{
//				if((abs(angle-Get_Angle())>0&&abs(angle-Get_Angle())<9))
//				{				
//					CarAdvance(0);
//					break;
//				}
//				else
//				{
//					SetLeftWhell(0,speed+slow);SetRightWhell(0,slow*3);//�˴��ٶ�ԽС������ԽС
//				}			
				if((abs(angle-Get_Angle())>0&&abs(angle-Get_Angle())<9))
				{				
					CarAdvance(0);
					break;
				}
				else if(abs(angle-Get_Angle())>8&&abs(angle-Get_Angle())<14)
				{
					SetLeftWhell(0,speed-5*slow);SetRightWhell(0,slow); //???
				}
				else if(abs(angle-Get_Angle())>14&&abs(angle-Get_Angle())<24)
				{
					SetLeftWhell(0,speed-4*slow);SetRightWhell(0,slow); //???
				}
				else if(abs(angle-Get_Angle())>24&&abs(angle-Get_Angle())<34)
				{
					SetLeftWhell(0,speed-3*slow);SetRightWhell(0,slow); //???
				}
				else if(abs(angle-Get_Angle())>34&&abs(angle-Get_Angle())<44)
				{
					SetLeftWhell(0,speed-2*slow);SetRightWhell(0,slow); //???
				}
				else if(abs(angle-Get_Angle())>44&&abs(angle-Get_Angle())<54)
				{
					SetLeftWhell(0,speed-1*slow);SetRightWhell(0,slow); //???
				}
				else
				{
					SetLeftWhell(0,speed);SetRightWhell(0,slow*2);//�˴��ٶ�ԽС������ԽС
				}			
		}
	}		
}


/*
*********************************************************************************************************
*	�� �� ��: Infrared_R_Turn3
*	����˵��: �����һ���ת�����
*	��    ��: speed�����ٶ�angle�����Ƕ�engine������ͷ
*	�� �� ֵ: ��
*********************************************************************************************************
*/

void Infrared_R_Turn3(u8 speed,int angle,int engine,int slow)
{
	while(1)
	{
		if(engine==0)
		{
				if((abs(angle-Get_Angle())>0&&abs(angle-Get_Angle())<9))
				{				
					CarAdvance(0);
					break;
				}
				else if(abs(angle-Get_Angle())>8&&abs(angle-Get_Angle())<14)
				{
					SetLeftWhell(1,speed-5*slow);SetRightWhell(0,slow*2); //???
				}
				else if(abs(angle-Get_Angle())>14&&abs(angle-Get_Angle())<24)
				{
					SetLeftWhell(1,speed-4*slow);SetRightWhell(0,slow); //???
				}
				else if(abs(angle-Get_Angle())>24&&abs(angle-Get_Angle())<34)
				{
					SetLeftWhell(1,speed-3*slow);SetRightWhell(1,slow); //???
				}
				else if(abs(angle-Get_Angle())>34&&abs(angle-Get_Angle())<44)
				{
					SetLeftWhell(1,speed-2*slow);SetRightWhell(1,slow); //???
				}
				else if(abs(angle-Get_Angle())>44&&abs(angle-Get_Angle())<54)
				{
					SetLeftWhell(1,speed-1*slow);SetRightWhell(1,slow); //???
				}
				else
				{
					SetLeftWhell(1,speed);SetRightWhell(1,slow);
				}			
		}
		else if(engine==1)
		{
				if((abs(angle-Get_Angle())>0&&abs(angle-Get_Angle())<9))
				{				
					CarAdvance(0);
					break;
				}
				else if(abs(angle-Get_Angle())>8&&abs(angle-Get_Angle())<14)
				{
					SetLeftWhell(0,speed-5*slow);SetRightWhell(1,(speed-1*slow)/100); //???
				}
				else if(abs(angle-Get_Angle())>14&&abs(angle-Get_Angle())<24)
				{
					SetLeftWhell(0,speed-4*slow);SetRightWhell(1,(speed-1*slow)/100); //???
				}
				else if(abs(angle-Get_Angle())>24&&abs(angle-Get_Angle())<34)
				{
					SetLeftWhell(0,speed-3*slow);SetRightWhell(1,(speed-1*slow)/100); //???
				}
				else if(abs(angle-Get_Angle())>34&&abs(angle-Get_Angle())<44)
				{
					SetLeftWhell(0,speed-2*slow);SetRightWhell(1,(speed-1*slow)/100); //???
				}
				else if(abs(angle-Get_Angle())>44&&abs(angle-Get_Angle())<54)
				{
					SetLeftWhell(0,speed-1*slow);SetRightWhell(1,(speed-1*slow)/100); //???
				}
				else
				{
					SetLeftWhell(0,speed);SetRightWhell(1,speed/3);//�˴��ٶ�ԽС������ԽС
				}			
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: Infrared_R_Turn3
*	����˵��: �����һ���ת�����
*	��    ��: speed�����ٶ�angle�����Ƕ�engine������ͷ
*	�� �� ֵ: ��
*********************************************************************************************************
*/

void Infrared_R_Turn4(u8 speed,int angle,int engine,int slow)
{
	while(1)
	{
		if(engine==0)
		{
				if((abs(angle-Get_Angle())>0&&abs(angle-Get_Angle())<9))
				{				
					CarAdvance(0);
					break;
				}
				else if(abs(angle-Get_Angle())>8&&abs(angle-Get_Angle())<14)
				{
					SetLeftWhell(1,speed-5*slow);SetRightWhell(0,(speed-1*slow)/100); //???
				}
				else if(abs(angle-Get_Angle())>14&&abs(angle-Get_Angle())<24)
				{
					SetLeftWhell(1,speed-4*slow);SetRightWhell(0,(speed-1*slow)/100); //???
				}
				else if(abs(angle-Get_Angle())>24&&abs(angle-Get_Angle())<34)
				{
					SetLeftWhell(1,speed-3*slow);SetRightWhell(0,(speed-1*slow)/100); //???
				}
				else if(abs(angle-Get_Angle())>34&&abs(angle-Get_Angle())<44)
				{
					SetLeftWhell(1,speed-2*slow);SetRightWhell(0,(speed-1*slow)/100); //???
				}
				else if(abs(angle-Get_Angle())>44&&abs(angle-Get_Angle())<54)
				{
					SetLeftWhell(1,speed-1*slow);SetRightWhell(0,(speed-1*slow)/100); //???
				}
				else
				{
					SetLeftWhell(1,speed);SetRightWhell(0,speed/4);
				}			
		}
		else if(engine==1)
		{
				if((abs(angle-Get_Angle())>0&&abs(angle-Get_Angle())<9))
				{				
					CarAdvance(0);
					break;
				}
				else if(abs(angle-Get_Angle())>8&&abs(angle-Get_Angle())<14)
				{
					SetLeftWhell(0,speed-5*slow);SetRightWhell(1,(speed-1*slow)/100); //???
				}
				else if(abs(angle-Get_Angle())>14&&abs(angle-Get_Angle())<24)
				{
					SetLeftWhell(0,speed-4*slow);SetRightWhell(1,(speed-1*slow)/100); //???
				}
				else if(abs(angle-Get_Angle())>24&&abs(angle-Get_Angle())<34)
				{
					SetLeftWhell(0,speed-3*slow);SetRightWhell(1,(speed-1*slow)/100); //???
				}
				else if(abs(angle-Get_Angle())>34&&abs(angle-Get_Angle())<44)
				{
					SetLeftWhell(0,speed-2*slow);SetRightWhell(1,(speed-1*slow)/100); //???
				}
				else if(abs(angle-Get_Angle())>44&&abs(angle-Get_Angle())<54)
				{
					SetLeftWhell(0,speed-1*slow);SetRightWhell(1,(speed-1*slow)/100); //???
				}
				else
				{
					SetLeftWhell(0,speed);SetRightWhell(1,speed/3);//�˴��ٶ�ԽС������ԽС
				}			
		}
	}
}
/*
*********************************************************************************************************
*	�� �� ��: scanf_blaze
*	����˵��: ��ⷿ���Ƿ���ڻ�Դ
*	��    ��: speed�����ٶ�engine������ͷ
*	�� �� ֵ: ��
*********************************************************************************************************
*/

int Speed;
int Gray;

int scanf_blaze(u8 engine)
{
//			find_fire();         //�����Ƕ�
//	
//			while(fire_count())	//ǰ��������ǰ
//			{
//				LED0=~LED0;
//				go_fire(fire_count());
//				if(fire_count()>5)
//					break;
//			}
//			
//			while(fire_count())	//���
//			{
//				LED1=~LED1;
//				Stop(1);
////				while(fire_count())
////				{
////					fan(0,0);
////				}
//				fan(1,0);
//				return 1;
//			}
//			
//			return 0;

		
		if(Laser_scan(engine)==0x00)
			return 0;
		else
		{
			LED0=0;LED1=0;BEEP=1;
			if(engine)
			{
				Gray=Gray1;
			}
			else
			{
				Gray=Gray2;
			}
			while(Laser_scan(engine))
			{
				while(Laser_scan(engine)&&Gray1&&Gray2)
				{
					Speed = 0;
					Laser_Scan(engine);
					Speed = 15+7*abs(Laser_Scan(engine));
					if(Laser_Scan(engine)>1)
						{SetLeftWhell(0,Speed); SetRightWhell(0,Speed);}//��ת
					if(Laser_Scan(engine)<-1)
						{SetLeftWhell(1,Speed); SetRightWhell(1,Speed);}//��ת
					if(Laser_Scan(engine)>-2&&Laser_Scan(engine)<2)
						{
							if(engine)
							{
								CarRetreat(Speed+10);
							}
							else
							{
								CarAdvance(Speed+10);
							}
							
						}
//					LCD_ShowxNum(0,45,Laser_Scan(engine),7,12,0x80);
				}
				CarRetreat(0);
				while(!Gray1&&!Gray2&&Laser_scan(engine))
				{
					Stop(1);
					while(1)
					{
						fan(0,engine);
						while(Laser_scan(engine)==0x00)
						{
							delay_ms(100);
							while(Laser_scan(engine)==0x00)
							{							
								fan(1,engine);
								LED0=1;LED1=1;BEEP=0;
								break;
							}							
						}
					}
					return 1;
				}
			}
		}
		
		
}


/*
*********************************************************************************************************
*	�� �� ��: Hunt_door
*	����˵��: Ѱ�ҷ���
*	��    ��: speed�����ٶ�angle��¼���ӽ��ŽǶ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
float end,out;
void Hunt_door(u8 speed,float angle)
{
	end=((int)angle-90)%360;//β��׼��������
	out=((int)angle+180)%360;//���ŽǶ�
	
	Infrared_R_Turn2(speed+5,end,0,6);
	Stop(1);
	while(I6)
	{
		gostr_usix(end,-speed);
	}
	Infrared_L_Turn2(speed+5,angle,0,6);
	Stop(1);
	while(I6)
	{
		gostr_usix(angle,-speed);
	}
}
/*
*********************************************************************************************************
*	�� �� ��: start1
*	����˵��: ǰ����һ����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void start1()
{
	int sign=0;
	
		while(!I3)
		{
			gostr_usix(1,85);
		}
		
/****ǰ����һ������ĵ�һ��ֱ��*****/			
		while(!I8)
		{
			gostr_usix(1,80);
			
		}
		while(I3)
		{
			gostr_usix(1,75);
			
			if(!I8)
			{				
				sign=1;
				break;
			}
		}
		while(sign==1&&!I8)
		{
			gostr_usix(1,60);
			if(!I1)
			{
				Stop(1);
				break;
			}
		}
		while(!I3)
		{
			gostr_usix(1,60);
			
			if(!I1)
			{
				Stop(1);
				break;
			}
		}
		while(I1)
		{
			gostr_usix(1,60);
		}
		if(!I1)
			Stop(1);
		Infrared_R_Turn2(40,270,0,5);
		while(I8)
		{
			gostr_usix(270,25);
		}
//		while(1)   //���Դ����
//		{
//			LCD_ShowNum(30+48+8,240,Get_Angle(),4,16);
//			Stop(0);
//		}	
}
/*
*********************************************************************************************************
*	�� �� ��: start2
*	����˵��: ǰ���ڶ�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void start2()
{		
		while(Gray1)
		{
			if(!I4)
			{
				break;
			}
			else
			{
				gostr_usix(270,-25);
			}
		}
		Infrared_R_Turn2(45,1,1,5);
			
		Infrared_L_Turn1(45,270,1,6);
		
		while(I8)
		{
			gostr_usix(270,-25);
		}
//		while(1)   //���Դ����
//		{
//			LCD_ShowNum(30+48+8,240,Get_Angle(),4,16);
//			Stop(0);
//		}
}

/*
*********************************************************************************************************
*	�� �� ��: start3
*	����˵��: ǰ����������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void start3()
{
		while(!I8)
		{
			gostr_usix(270,30);
		}
		
		Infrared_R_Turn3(45,180,0,4);
		
		Infrared_R_Turn4(45,90,0,4);
		
		Infrared_L_Turn1(45,180,0,2);
//		while(1)   //���Դ����
//		{
//			LCD_ShowNum(30+48+8,240,Get_Angle(),4,16);
//			Stop(0);
//		}
}


/*
*********************************************************************************************************
*	�� �� ��: start4
*	����˵��: ǰ�����ķ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void start4()
{
		Infrared_L_Turn3(45,90,1,5);
		
		while(!I8)
		{
			gostr_usix(90,-70);
			LCD_ShowNum(30+48+8,100,1,4,16);
		}
		while(I8)
		{
			gostr_usix(90,-60);
			LCD_ShowNum(30+48+8,120,2,4,16);
		}
		while(I4)
		{
			gostr_usix(90,-40);
			if(I5)
			{
				break;
			}
			LCD_ShowNum(30+48+8,140,3,4,16);
		}
		
		Infrared_L_Turn(40,1,1,2);
		while(1)   //���Դ����
		{
			LCD_ShowNum(30+48+8,240,Get_Angle(),4,16);
			Stop(0);
		}
}


/*
*********************************************************************************************************
*	�� �� ��: Back_home1
*	����˵��: ��һ����ؼ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Back_home1()
{
		Get_Angle();
		angle=Get_Angle();
		Hunt_door(30,angle);
		
		Infrared_R_Turn2(35,90,0,6);
}


/*
*********************************************************************************************************
*	�� �� ��: Back_home2
*	����˵��: �ڶ�����ؼ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Back_home2()
{
		Get_Angle();
		angle=Get_Angle();
		Hunt_door(30,angle);
		
		Infrared_R_Turn2(35,180,0,6);
}


/*
*********************************************************************************************************
*	�� �� ��: Back_home3
*	����˵��: ��������ؼ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Back_home3()
{
		Get_Angle();
		angle=Get_Angle()-180;
		Hunt_door(30,angle);
		
		Infrared_L_Turn2(35,180,0,6);
}


/*
*********************************************************************************************************
*	�� �� ��: Back_home4
*	����˵��: ���ķ���ؼ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Back_home4()
{
		Get_Angle();
		angle=Get_Angle();
		Hunt_door(30,angle);
		
		Infrared_L_Turn2(35,270,0,6);
}


/*
*********************************************************************************************************
*	�� �� ��: Accident
*	����˵��: δ���ֻ�Դ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Accident()
{
	
}

