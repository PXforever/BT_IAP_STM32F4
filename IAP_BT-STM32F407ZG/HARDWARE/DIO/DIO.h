#ifndef DIO_H
#define DIO_H
#include "sys.h"

void Gray_Init(void);//�Ҷȳ�ʼ������
void Infrared_sensor_Init(void);//���⴫������ʼ������
void TIM2_Init(u16 arr);//��ʱ��2��ʼ��
void TIM3_Init(u16 arr);//��ʱ��3��ʼ��
void TIM6_Init(u16 arr,u16 psc);//��ʱ��3��ʼ��

#endif

