#ifndef DIO_H
#define DIO_H
#include "sys.h"

void Gray_Init(void);//灰度初始化函数
void Infrared_sensor_Init(void);//红外传感器初始化函数
void TIM2_Init(u16 arr);//定时器2初始化
void TIM3_Init(u16 arr);//定时器3初始化
void TIM6_Init(u16 arr,u16 psc);//定时器3初始化

#endif

