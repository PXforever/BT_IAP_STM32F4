#ifndef DEMO_DIO
#define DEMO_DIO

#include "DIO.h"
#include "demo_motor.h"

#define Gray1	PEin(0)//灰度1输入 ETR1
#define Gray2 	PDin(2) //灰度2输入 ETR3

#define I1 PDin(11)//DIO1
#define I2 PDin(7)//DIO2
#define I3 PFin(7)//DIO3
#define I4 PFin(8)//DIO4
#define I5 PCin(6)//PWM5
#define I6 PCin(7)//PWM6
#define I7 PCin(8)//PWM7
#define I8 PAin(12)//ETR4

u8 Flase_Angle(u16 goal_Angle,float sta_FalseAngle);
void gostr_usix(int goal_Angle,int speed);
void Infrared_R_Turn(u8 speed,int angle,int engine,int slow);
void Infrared_L_Turn(u8 speed,int angle,int engine,int slow);
void Infrared_R_Turn1(u8 speed,int angle,int engine,int slow);
void Infrared_L_Turn1(u8 speed,int angle,int engine,int slow);
void Infrared_R_Turn2(u8 speed,int angle,int engine,int slow);
void Infrared_L_Turn2(u8 speed,int angle,int engine,int slow);
void Infrared_R_Turn3(u8 speed,int angle,int engine,int slow);
void Infrared_L_Turn3(u8 speed,int angle,int engine,int slow);
void Infrared_R_Turn4(u8 speed,int angle,int engine,int slow);
int scanf_blaze(u8 engine);
void Hunt_door(u8 speed,float angle);
void start1(void);
void start2(void);
void start3(void);
void start4(void);
void Back_home1(void);
void Back_home2(void);
void Back_home3(void);
void Back_home4(void);
void Accident(void);

#endif

