#ifndef __US_101_H
#define __US_101_H
#include "sys.h"

//���ڴ�����ʽ
//
//���벶��ʵ����ĵõ���
//Trig ��  PF(4)----ISP
//Echo ��  PA(0)----TIM5-Ch1----DIO3
// VCC 2.4-5.5V
// GND

#define Trig1 PGout(13)	// US-101

void us_101_Init(void);
void TIM5_CH1_Cap_Init(u32 arr,u16 psc);

float get_distance2();

#endif























