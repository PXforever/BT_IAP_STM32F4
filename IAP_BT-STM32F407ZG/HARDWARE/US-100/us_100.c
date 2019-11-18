#include "us_100.h"
#include "usart.h"
#include "math.h"
#include "delay.h"
#include "lcd.h"
//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数(对于32位定时器来说,1us计数器加1,溢出时间:4294秒)
u8  TIM12CH1_CAPTURE_STA=0;	//输入捕获状态		    				
u32	TIM12CH1_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)

float get_distance1()
{
    int temp=0;
		u8 complete = 1;
    TIM12_CH1_Cap_Init(0XFFFFFFFF,84-1);
    if(complete == 1)
	{
		Trig = 1;
    delay_us(30);
		Trig = 0;
		complete = 0;
	}     
	while(complete == 0)
	{
	
		if(TIM12CH1_CAPTURE_STA&0X80)        //成功捕获到了一次高电平
		{
			temp=TIM12CH1_CAPTURE_STA&0X3F; 
			temp*=0XFFFFFFFF;		 		         //溢出时间总和
			temp+=TIM12CH1_CAPTURE_VAL;		   //得到总的高电平时间
			//要显示距离时，不用注释
			temp = temp * 17/1000;               //计算得到距离
			//printf("HIGH:%f cm\r\n",temp); //打印距离
			// printf("t2.txt=\"dist:%f\"%c%c%c",temp,0xFF,0xFF,0xFF);//HMI屏指令
			TIM12CH1_CAPTURE_STA=0;			     //开启下一次捕获
			complete = 1;
		 }
		//break;
	 }
	LCD_ShowxNum(0,45,temp,5,12,0x80);
	 return temp;
}

void us_100_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOF时钟

	//GPIOF9,F10初始化设置
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化

	GPIO_ResetBits(GPIOB,GPIO_Pin_12);//初始化GPIOF1为低电平
}




void TIM12_CH1_Cap_Init(u32 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM12_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);  	//TIM5时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTA时钟	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //下拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化PA0

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_TIM12); //PA0复用位定时器5
  
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM12,&TIM_TimeBaseStructure);
	

	//初始化TIM5输入捕获参数
	TIM12_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
	TIM12_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
	TIM12_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
	TIM12_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
	TIM12_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM12, &TIM12_ICInitStructure);
	
	TIM_ITConfig(TIM12,TIM_IT_Update|TIM_IT_CC1,ENABLE);//允许更新中断 ,允许CC1IE捕获中断
		
	
	TIM_Cmd(TIM12,ENABLE ); 	//使能定时器5

	NVIC_InitStructure.NVIC_IRQChannel = TIM8_BRK_TIM12_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	
	
}


//定时器5中断服务程序
void TIM8_BRK_TIM12_IRQHandler(void)
{ 		    
	if((TIM12CH1_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{
		if(TIM_GetITStatus(TIM12, TIM_IT_Update) != RESET)//溢出
		{	     
			if(TIM12CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM12CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM12CH1_CAPTURE_STA|=0X80;		//标记成功捕获了一次
					TIM12CH1_CAPTURE_VAL=0XFFFFFFFF;
				}
				else TIM12CH1_CAPTURE_STA++;
			}	 
		}
		if(TIM_GetITStatus(TIM12, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
		{	
			if(TIM12CH1_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
			{	  			
				TIM12CH1_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
				TIM12CH1_CAPTURE_VAL=TIM_GetCapture1(TIM12);//获取当前的捕获值.
				TIM_OC1PolarityConfig(TIM12,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
			}
			else  								//还未开始,第一次捕获上升沿
			{
				TIM12CH1_CAPTURE_STA=0;			//清空
				TIM12CH1_CAPTURE_VAL=0;
				TIM12CH1_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
				TIM_Cmd(TIM12,DISABLE ); 	//关闭定时器5
				TIM_SetCounter(TIM12,0);
				TIM_OC1PolarityConfig(TIM12,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
				TIM_Cmd(TIM12,ENABLE ); 	//使能定时器5
			}		    
		}			     	    					   
	}
	TIM_ClearITPendingBit(TIM12, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
	
	//LCD_ShowxNum(0,45,get_distance1(),5,12,0x80);
}


