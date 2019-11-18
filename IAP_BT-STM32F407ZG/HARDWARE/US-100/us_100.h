#ifndef __US_100_H
#define __US_100_H
#include "sys.h"
//串口触发方式
//
//输入捕获实验更改得到，
//Trig 接  PB(12)
//Echo 接  PB(14)----TIM12-Ch1
// VCC 2.4-5.5V
// GND

#define Trig PBout(12)	// US-100

void us_100_Init(void);
void TIM12_CH1_Cap_Init(u32 arr,u16 psc);


float get_distance1();

#endif























