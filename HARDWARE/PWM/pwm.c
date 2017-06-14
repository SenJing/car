#include "pwm.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//PWM  驱动代码			   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/12/03
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  


//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
///////////////////////////////////////////////////////////////////////////////

//PA0和PA1分别输出pwm  PA0左轮 用寄存器TIM_SetCompare1(TIM5,占空比);PA1右轮用TIM_SetCompare2(TIM5,占空比);

//	TIM_SetCompare1(TIM5,250);	TIM_SetCompare2(TIM5,230);直走

//PA2 PA3控制方向



/////////////////////////////////////////////////////////////////////////////////////
u16 MAX_PWM=0;
void PWM_Init(u16 arr,u16 psc)//PWM初始化函数
{ 
	GPIO_InitTypeDef GPIO_InitStructure;						//输出通道的GPIO管脚结构
	TIM_OCInitTypeDef  TIM_OCInitStructure;					//通道输出初始化结构	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	//定时器初始化结构
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //使能AFIO 
	
	//配置PWM输出通道初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);    										//使能PORTA时钟    	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; 	//选择PA
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 													//管脚频率为50MHZ
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 														//模式为推挽输出
 	GPIO_Init(GPIOA, &GPIO_InitStructure);   																		//初始化GPIOA寄存器
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 |GPIO_Pin_3;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 
	GPIO_SetBits(GPIOA,GPIO_Pin_2 |GPIO_Pin_3);		
 
	//(1)TIM5初始化,时间基部分
	TIM_TimeBaseStructure.TIM_Period =arr;          						//周期0～arr
  TIM_TimeBaseStructure.TIM_Prescaler = psc;          				//时钟分频psc
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;       					//时钟分割
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//计数模式
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);   					//基本初始化
  	
	//(2)TIM5通道初始化 ，CCP部分
  TIM_OCStructInit(& TIM_OCInitStructure);                      //默认参数

	// 1通道  PA0
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                //工作状态
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //设定为输出，需要PWM输出才需要这行代码
  TIM_OCInitStructure.TIM_Pulse =0;                           //占空长度
//  	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    //高电平
 	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;         //高电平	
  TIM_OC1Init(TIM5, &TIM_OCInitStructure);           				
	// 2通道  PA1
 	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                //工作状态
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //设定为输出，需要PWM输出才需要这行代码
  TIM_OCInitStructure.TIM_Pulse =0;                            //占空长度
//  	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    //高电平
 	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;         //高电平	
  TIM_OC2Init(TIM5, &TIM_OCInitStructure);      

	TIM_OC1PreloadConfig(TIM5,TIM_OCPreload_Enable); 	//预装载使能
	TIM_CtrlPWMOutputs(TIM5,ENABLE);

	TIM_ARRPreloadConfig(TIM5, ENABLE);                   
	TIM_Cmd(TIM5, ENABLE);                   					//使能TIM3，到此在TIM中已配置完成   
 MAX_PWM=arr;
}

void Motor(int L,int R)
{
	if(R>MAX_PWM) R=MAX_PWM;
	if(R<-MAX_PWM)R=-MAX_PWM;
	
	if(L>MAX_PWM) L=MAX_PWM;
	if(L<-MAX_PWM) L=-MAX_PWM;	
	

	
	if(R>0)
	{
		R_F=1;
		TIM_SetCompare1(TIM5,R);	
	}
	else
	{
		R=-R;

		R_F=0;
		TIM_SetCompare1(TIM5,R);				
	}

	if(L>0)
	{
		L_F=1;
		TIM_SetCompare2(TIM5,L);		
	}
	else
	{
		L=-L;

		L_F=0;
			TIM_SetCompare2(TIM5,L);				
	}	
}
