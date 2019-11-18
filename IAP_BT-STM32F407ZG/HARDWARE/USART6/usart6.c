#include "delay.h"
#include "usart6.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	  
#include "timer.h"
#include "usart.h"
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
   

//串口发送缓存区 	
__align(8) u8 USART6_TX_BUF[USART6_MAX_SEND_LEN]; 	//发送缓冲,最大USART3_MAX_SEND_LEN字节
#ifdef USART6_RX_EN   								//如果使能了接收   	  
//串口接收缓存区 	
u8 USART6_RX_BUF[USART6_MAX_RECV_LEN] __attribute__ ((at(0X20001000)));//接收缓冲,最大USART_REC_LEN个字节,起始地址为0X20001000. ; 	
u8 USART6_PART_BUF[USART6_RX_PART_LEN] = {0};


//通过判断接收连续2个字符之间的时间差不大于100ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过100ms,则认为不是1次连续数据.也就是超过100ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
u16 USART6_RX_STA=0;
u32 USART6_RX_COUNT = 0;

u32 APP_ID=0;
u32 APP_N=0;
u32 APP_LEN=0;

void iap_package_process(void)
{
	int length = 0;
	int i;
//	printf("receiver a part , processing this data\r\n");
//	printf("[%x][%x]\r\n", USART6_PART_BUF[0], USART6_PART_BUF[1]);
//	printf("[%x][%x][%x][%x]\r\n", USART6_PART_BUF[2], USART6_PART_BUF[3], USART6_PART_BUF[4], USART6_PART_BUF[5]);
//	printf("[%x][%x][%x][%x]\r\n", USART6_PART_BUF[6], USART6_PART_BUF[7], USART6_PART_BUF[8], USART6_PART_BUF[9]);
//	printf("[%x][%x][%x][%x]\r\n", USART6_PART_BUF[10], USART6_PART_BUF[11], USART6_PART_BUF[12], USART6_PART_BUF[13]);
//	printf("enter %s,re:%d\r\n", __FUNCTION__, USART6_RX_COUNT);
	//为接受完一段，不进行处理，接收完一段只有两种可能：1闯过限定长度，2超时未接到数据
	if(USART6_RX_STA == 0) { return;}
	//未达到包数据长度，判断不为包
	if((USART6_RX_COUNT < USART6_RX_PART_MIN_LEN) | ((USART6_PART_BUF[0] != 0xff))) 
	{
		USART6_RX_COUNT = 0; 
		USART6_RX_STA = 0;
		//printf("it is not a package\r\n");
		return;
	}
	
	if(USART6_PART_BUF[0]==0xff)
	{
			if(USART6_PART_BUF[1]==0xf0)
			{
					APP_N = 0;
					APP_N |= USART6_PART_BUF[2] << 24;
					APP_N |= USART6_PART_BUF[3] << 16;
					APP_N |= USART6_PART_BUF[4] <<8;
					APP_N |= USART6_PART_BUF[5];

					APP_ID = 0;
					APP_ID |= USART6_PART_BUF[6] << 24;
					APP_ID |= USART6_PART_BUF[7] << 16;
					APP_ID |= USART6_PART_BUF[8] <<8;
					APP_ID |= USART6_PART_BUF[9];

					length = 0;
					length |= USART6_PART_BUF[10] << 24;
					length |= USART6_PART_BUF[11] << 16;
					length |= USART6_PART_BUF[12] <<8;
					length |= USART6_PART_BUF[13];

					//printf("legth:%d\r\n", length);
					//printf("data part :%d, APP_ID:%d, LEN:%d\r\n", APP_N, APP_ID, length);	
					
					for(i = 0;i<length;i++)
					{
						USART6_RX_BUF[i+APP_LEN] = USART6_PART_BUF[i+14];
						USART6_RX_COUNT++;				
					}
					
					APP_LEN += length;
					//		for(i=0;i<length+11;i++)
					//		{
					//			USART6_PART_BUF[i]=0;
					//		}
					memset(USART6_PART_BUF, 0, USART6_RX_PART_LEN);
					USART6_RX_COUNT = 0;
					USART6_RX_STA = 0;
					//A ACK, C check, ON!
					u3_printf("A|C|ON!\xff"); 											
			}
	}
}


void USART6_IRQHandler(void)
{	
	u8 res;
	//static u16 i = 0;
	
#if SYSTEM_SUPPORT_OS  //使用UCOS操作系统
	OSIntEnter();    
#endif	
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)//接收到数据
	{	 
		//printf("enter %s\r\n", __FUNCTION__);
		res =USART_ReceiveData(USART6);
		//printf("%d ", res);		
		if((USART6_RX_STA&(1 << 15))==0 )//接收完的一批数据,还没有被处理,则不再接收其他数据
		{ 
			if(USART6_RX_COUNT < USART6_RX_PART_LEN)		//还可以接收数据
			{
				TIM_SetCounter(TIM6,0);//计数器清空        				 
				if(USART6_RX_STA == 0)		
					TIM_Cmd(TIM6, ENABLE);  //使能定时器6
				USART6_PART_BUF[USART6_RX_COUNT++] = res;		//记录接收到的值	
			}else 
			{
				USART6_RX_STA |= 1 << 15;					//强制标记接收完成
			} 
		} 
	}

#if SYSTEM_SUPPORT_OS  //使用UCOS操作系统
	OSIntExit();    			//退出中断	   
#endif	
}  
#endif	
//初始化IO 串口3
//bound:波特率	  
void usart6_init(u32 bound)
{  
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
 
	USART_DeInit(USART6);  //复位串口6
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE); //使能GPIOG时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//使能USART6时钟
	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14; //GPIOG9和GPIOG14初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOG,&GPIO_InitStructure); //初始化GPIOB11，和GPIOB10
	
	
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART6); //GPIOG9复用为USART6
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6); //GPIOG14复用为USART6	  
	
	USART_InitStructure.USART_BaudRate = bound;//波特率一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART6, &USART_InitStructure); //初始化串口3
	
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//开启中断  
		
	USART_Cmd(USART6, ENABLE);                    //使能串口 
	
 
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	TIM6_Int_Init(100-1,8400-1);	//10ms中断一次
	
  TIM_Cmd(TIM6, DISABLE); //关闭定时器7
	
	USART6_RX_STA=0;				//清零 
}

//串口3,printf 函数
//确保一次发送数据不超过USART3_MAX_SEND_LEN字节
void u3_printf(char* fmt,...)  
{  
	u16 i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART6_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART6_TX_BUF);//此次发送数据的长度
	for(j=0;j<i;j++)//循环发送数据
	{
	  while(USART_GetFlagStatus(USART6,USART_FLAG_TC)==RESET);  //等待上次传输完成 
		USART_SendData(USART6,(uint8_t)USART6_TX_BUF[j]); 	 //发送数据到串口3 
	}
	
}
void clear_send_flags()
{
	APP_N = 0;
	APP_ID = 0;
	APP_LEN = 0;
	USART6_RX_COUNT = 0;
	USART6_RX_STA = 0;
}

//void is_dev_online()
//{
//	
//}
 
 
 











