#ifndef __WT61_H
#define __WT61_H
#include "stdio.h"	
//#include "stm32f4xx_conf.h"




struct SAngle
{
	short Angle[3];
	short T;
};


void CopeSerial3Data(unsigned char ucData);
int Get_Angle(void);
int Get_Gesture(void);


#endif

