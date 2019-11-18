#ifndef DEMO_MOTOR_H
#define DEMO_MOTOR_H

#include "pwm.h"

#define M1 PFout(14)
#define M2 PFout(15)
#define M3 PGout(0)
#define M4 PGout(1)

void  motor_configuration(u8 PWM_fre);//�����ʼ������
void  SetLeftWhell(u8 left_direction,u8 speed);//����С������ֺ���                                           
void  SetRightWhell(u8 right_direction,u8 speed);//����С���ұ��ֺ���
void  Stop(int engine);//С��ֹͣ
void  CarAdvance(u8 speed);//����С��ǰ������
void  CarRetreat(u8 speed);//����С�����˺���
#endif

