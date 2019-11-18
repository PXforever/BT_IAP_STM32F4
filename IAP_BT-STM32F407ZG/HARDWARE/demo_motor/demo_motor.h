#ifndef DEMO_MOTOR_H
#define DEMO_MOTOR_H

#include "pwm.h"

#define M1 PFout(14)
#define M2 PFout(15)
#define M3 PGout(0)
#define M4 PGout(1)

void  motor_configuration(u8 PWM_fre);//电机初始化函数
void  SetLeftWhell(u8 left_direction,u8 speed);//驱动小车左边轮函数                                           
void  SetRightWhell(u8 right_direction,u8 speed);//驱动小车右边轮函数
void  Stop(int engine);//小车停止
void  CarAdvance(u8 speed);//驱动小车前进函数
void  CarRetreat(u8 speed);//驱动小车后退函数
#endif

