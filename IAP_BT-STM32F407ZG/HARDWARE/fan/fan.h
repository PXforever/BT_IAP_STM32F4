#ifndef FAN_H
#define FAN_H

#include "pwm.h"

#define N1 PBout(13)
#define N2 PBout(15)

void fan_Init(u8 fre);//���ȵ����ʼ������
void fan(int button,int aspect);//���ȵ������ʵ��
#endif

