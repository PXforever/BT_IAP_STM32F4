#ifndef __USART6_H
#define __USART6_H	 
#include "sys.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����3��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/29
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	  



#define USART6_RX_PART_LEN				1400
#define USART6_RX_PART_MIN_LEN		13
#define USART6_MAX_RECV_LEN		120*1024					//�����ջ����ֽ���
#define USART6_MAX_SEND_LEN		400					//����ͻ����ֽ���
#define USART6_RX_EN 			1					//0,������;1,����.

extern u8  USART6_PART_BUF[USART6_RX_PART_LEN];		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern u8  USART6_RX_BUF[USART6_MAX_RECV_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern u8  USART6_TX_BUF[USART6_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern u16 USART6_RX_STA;   						//��������״̬
extern u32 USART6_RX_COUNT;

extern u32 APP_ID;
extern u32 APP_N;
extern u32 APP_LEN;


void usart6_init(u32 bound);				//����3��ʼ�� 
void TIM7_Int_Init(u16 arr,u16 psc);
void u3_printf(char* fmt, ...);
void iap_package_process(void);
void clear_send_flags();
#endif













