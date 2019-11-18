#ifndef __USART6_H
#define __USART6_H	 
#include "sys.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口3驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/29
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	  



#define USART6_RX_PART_LEN				1400
#define USART6_RX_PART_MIN_LEN		13
#define USART6_MAX_RECV_LEN		120*1024					//最大接收缓存字节数
#define USART6_MAX_SEND_LEN		400					//最大发送缓存字节数
#define USART6_RX_EN 			1					//0,不接收;1,接收.

extern u8  USART6_PART_BUF[USART6_RX_PART_LEN];		//接收缓冲,最大USART3_MAX_RECV_LEN字节
extern u8  USART6_RX_BUF[USART6_MAX_RECV_LEN]; 		//接收缓冲,最大USART3_MAX_RECV_LEN字节
extern u8  USART6_TX_BUF[USART6_MAX_SEND_LEN]; 		//发送缓冲,最大USART3_MAX_SEND_LEN字节
extern u16 USART6_RX_STA;   						//接收数据状态
extern u32 USART6_RX_COUNT;

extern u32 APP_ID;
extern u32 APP_N;
extern u32 APP_LEN;


void usart6_init(u32 bound);				//串口3初始化 
void TIM7_Int_Init(u16 arr,u16 psc);
void u3_printf(char* fmt, ...);
void iap_package_process(void);
void clear_send_flags();
#endif













