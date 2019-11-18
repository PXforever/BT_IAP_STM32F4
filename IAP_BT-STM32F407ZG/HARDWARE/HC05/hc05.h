#ifndef __HC05_H
#define __HC05_H	 
#include "sys.h"  

#define HC05_KEY  	PGout(8) 	//À¶ÑÀ¿ØÖÆKEYÐÅºÅ
#define HC05_LED  	PGin(7)		//À¶ÑÀÁ¬½Ó×´Ì¬ÐÅºÅ
  
u8 HC05_Init(void);
void HC05_CFG_CMD(u8 *str);
u8 HC05_Get_Role(void);
u8 HC05_Set_Cmd(u8* atstr);	 
u8 HC05_SET_MODE1(void);


#endif  
















