#ifndef _FIND_FIRE_H
#define _FIND_FIRE_H

#include "demo_motor.h"
#include "drive.h"
#include "delay.h"
/*******************************************************
�������ͣ�int
������null
������NOrY
���ã�ȷ���Ƿ��л�
����ֵ��1(��) 0(��)
********************************************************/
int NOrY(void);


/*******************************************************
�������ͣ�void
������null
������find_fire
���ã�Ѱ�һ�Դ���̶��Ƕ�
����Ա��xds
ʱ�䣺2018��10��12�� 20:04:40
����ֵ��null
********************************************************/
void find_fire(void);


/*******************************************************
�������ͣ�void
���������洫�������ĸ���
������go_fire
���ã����ߵ������λ��
����Ա��xds
ʱ�䣺2018��10��12�� 20:04:36
����ֵ��null
********************************************************/
void go_fire(int fir_num);


/*******************************************************
�������ͣ�void
������null
������fire_OneOrTwo
���ã���ֻ��һ������������������ʱ�����߷�ʽ
����Ա��xds
ʱ�䣺2018��10��12�� 20:05:35
����ֵ��null
********************************************************/
void fire_OneOrTwo(void);


/*******************************************************
�������ͣ�void
������null
������fire_OneOrTwo
���ã������������洫������ʱ�����߷�ʽ
����Ա��xds
ʱ�䣺2018��10��12�� 20:05:35
����ֵ��null
********************************************************/
void fire_Three(void);


/*******************************************************
�������ͣ�void
������null
������fire_OneOrTwo
���ã������ĸ���������ʱ�����߷�ʽ
����Ա��xds
ʱ�䣺2018��10��12�� 20:05:35
����ֵ��null
********************************************************/
void fire_Four(void);


/*******************************************************
�������ͣ�void
������null
������fire_Fire
���ã����������������ʱ�����߷�ʽ
����Ա��xds
ʱ�䣺2018��10��12�� 20:05:35
����ֵ��null
********************************************************/
void fire_Five(void);


/*******************************************************
�������ͣ�int
������null
������fire_count
���ã������洫���������ĸ���
����Ա��xds
ʱ�䣺2018��10��12�� 20:05:35
����ֵ��number
********************************************************/
int fire_count(void);


void Set_Motor(int peed,int speed);

#endif

