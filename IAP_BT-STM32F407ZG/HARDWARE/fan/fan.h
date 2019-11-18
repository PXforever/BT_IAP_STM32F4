#ifndef FAN_H
#define FAN_H

#include "pwm.h"

#define N1 PBout(13)
#define N2 PBout(15)

void fan_Init(u8 fre);//风扇电机初始化函数
void fan(int button,int aspect);//风扇电机功能实现
#endif

