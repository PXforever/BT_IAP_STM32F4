#ifndef __US_100_H
#define __US_100_H
#include "sys.h"
//���ڴ�����ʽ
//
//���벶��ʵ����ĵõ���
//Trig ��  PB(12)
//Echo ��  PB(14)----TIM12-Ch1
// VCC 2.4-5.5V
// GND

#define Trig PBout(12)	// US-100

void us_100_Init(void);
void TIM12_CH1_Cap_Init(u32 arr,u16 psc);


float get_distance1();

#endif























