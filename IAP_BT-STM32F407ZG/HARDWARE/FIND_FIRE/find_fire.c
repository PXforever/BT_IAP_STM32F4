#include "find_fire.h"
#include "led.h"


void Set_Motor(int peed,int speed)
{
	if(peed >= 0)
			SetLeftWhell(1,peed);//正转
	else if(peed <0) 
		  SetLeftWhell(0,-peed);
	
	if(speed >= 0)
		  SetRightWhell(0,speed);//正转
	else if(speed <0) 
		  SetRightWhell(1,-speed);
	
}

int NOrY()
{
	if(L11 == 1 || L12 == 1 || L13 == 1 || L14 == 1 || L15 == 1 || L16 == 1 || L17 == 1 || L18 == 1)
		return 1;
	else 
		return 0;
}
void find_fire()
{
	while(fire_count())
	{
		if(L11 == 1 && L12 == 1)		{Set_Motor(-50,50);}
		else if(L12 == 1 && L13 == 1)	{Set_Motor(-35,35);}
		else if(L13 == 1 && L14 == 1)	{Set_Motor(-25,25);}
		else if(L14 == 1 && L15 == 1)	
		{
			Set_Motor(0,0);
			delay_ms(10);
			if(L14 == 1 && L15 == 1)
				break;
		}
		
		else if(L17 == 1 && L18 == 1)	{Set_Motor(50,-50);}
		else if(L16 == 1 && L17 == 1)	{Set_Motor(35,-35);}
		else if(L15 == 1 && L16 == 1)	{Set_Motor(25,-25);}
		else if(L14 == 1 && L15 == 1)	
		{
			Set_Motor(0,0);
			delay_ms(10);
			if(L14 == 1 && L15 == 1)
				break;
		}
		
		else if(L11 == 1)	{Set_Motor(-50,50);}
		else if(L12 == 1)	{Set_Motor(-35,35);}
		else if(L13 == 1)	{Set_Motor(-25,25);}
		
		else if(L18 == 1)	{Set_Motor(50,-50);}
		else if(L17 == 1)	{Set_Motor(35,-35);}
		else if(L16 == 1)	{Set_Motor(25,-25);}
		
		else if(L14 == 1||L15 == 1)	
		{
			Set_Motor(0,0);
			delay_ms(10);
			if(L14 == 1||L15 == 1)
				break;
		}
		
//		else if(L16 == 1)	{Set_Motor(25,-25);}
//		else if(L17 == 1)	{Set_Motor(35,-35);}
//		else if(L18 == 1)	{Set_Motor(50,-50);}
		
		else Set_Motor(0,0);


	}
}


void go_fire(int fir_num)
{
		switch(fir_num)
		{
			case 1: fire_OneOrTwo();break;
			case 2: fire_OneOrTwo();break;
			case 3: fire_Three();break;
			case 4:	fire_Four();break;
			case 5:	fire_Five();break;
		}
		
}

void fire_OneOrTwo()
{
	if(L11 == 1 && L12 == 1)	Set_Motor(-70,70); 
	else if(L12 == 1 && L13 == 1)	Set_Motor(-50,50);
	else if(L13 == 1 && L14 == 1)	Set_Motor(25,35);
	
	else if(L14 == 1 && L15 == 1)	Set_Motor(70,70);
	
	else if(L15 == 1 && L16 == 1)	Set_Motor(35,25);
	else if(L16 == 1 && L17 == 1)	Set_Motor(35,-35);
	else if(L17 == 1 && L18 == 1)	Set_Motor(70,-70);
	
	else if(L11 == 1)	Set_Motor(-50,50);
	else if(L12 == 1)	Set_Motor(-35,35);
	else if(L13 == 1)	Set_Motor(-15,25);
	
	else if(L14 == 1)	Set_Motor(35,40);
	else if(L15 == 1)	Set_Motor(40,35);
	
	else if(L16 == 1)	Set_Motor(25,-15);
	else if(L17 == 1)	Set_Motor(35,-35);
	else if(L18 == 1)	Set_Motor(50,-50);
	
	else Set_Motor(50,50);
}

void fire_Three()
{
	if(L12 == 1 && L13 == 1 && L14 == 1)	Set_Motor(-45,45);
	
	else if(L13 == 1 && L14 == 1 && L15 == 1)	Set_Motor(35,50);
	else if(L14 == 1 && L15 == 1 && L16 == 1)	Set_Motor(50,35);
	
	else if(L15 == 1 && L16 == 1 && L17 == 1)	Set_Motor(35,-35);
	
	else Set_Motor(50,50);
}
void fire_Four()
{
	if(L12 == 1 && L13 == 1 && L14 == 1 && L15 == 1)	Set_Motor(-35,35);
	
	else if(L13 == 1 && L14 == 1 && L15 == 1 && L16 == 1)	Set_Motor(30,30);
	
	else if(L14 == 1 && L15 == 1 && L16 == 1 && L17 == 1)	Set_Motor(35,-35);
	
	else Set_Motor(30,30);
}
void fire_Five()
{
	if(L12 == 1 && L13 == 1 && L14 == 1 && L15 == 1 && L16 == 1)	Set_Motor(20,27);
	
	else if(L13 == 1 && L14 == 1 && L15 == 1 && L16 == 1 && L17 == 1)	Set_Motor(27,20);
	
	else Set_Motor(27,27);
}

int fire_count()
{
	int number;
	number = (int)L11+(int)L12+(int)L13+(int)L14+(int)L15+(int)L16+(int)L17+(int)L18;
	return number;
}

