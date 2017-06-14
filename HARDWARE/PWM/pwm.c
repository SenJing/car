#include "pwm.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//PWM  ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/12/03
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  


//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
///////////////////////////////////////////////////////////////////////////////

//PA0��PA1�ֱ����pwm  PA0���� �üĴ���TIM_SetCompare1(TIM5,ռ�ձ�);PA1������TIM_SetCompare2(TIM5,ռ�ձ�);

//	TIM_SetCompare1(TIM5,250);	TIM_SetCompare2(TIM5,230);ֱ��

//PA2 PA3���Ʒ���



/////////////////////////////////////////////////////////////////////////////////////
u16 MAX_PWM=0;
void PWM_Init(u16 arr,u16 psc)//PWM��ʼ������
{ 
	GPIO_InitTypeDef GPIO_InitStructure;						//���ͨ����GPIO�ܽŽṹ
	TIM_OCInitTypeDef  TIM_OCInitStructure;					//ͨ�������ʼ���ṹ	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	//��ʱ����ʼ���ṹ
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //ʹ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //ʹ��AFIO 
	
	//����PWM���ͨ����ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);    										//ʹ��PORTAʱ��    	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; 	//ѡ��PA
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 													//�ܽ�Ƶ��Ϊ50MHZ
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 														//ģʽΪ�������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);   																		//��ʼ��GPIOA�Ĵ���
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 |GPIO_Pin_3;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 
	GPIO_SetBits(GPIOA,GPIO_Pin_2 |GPIO_Pin_3);		
 
	//(1)TIM5��ʼ��,ʱ�������
	TIM_TimeBaseStructure.TIM_Period =arr;          						//����0��arr
  TIM_TimeBaseStructure.TIM_Prescaler = psc;          				//ʱ�ӷ�Ƶpsc
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;       					//ʱ�ӷָ�
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//����ģʽ
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);   					//������ʼ��
  	
	//(2)TIM5ͨ����ʼ�� ��CCP����
  TIM_OCStructInit(& TIM_OCInitStructure);                      //Ĭ�ϲ���

	// 1ͨ��  PA0
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                //����״̬
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //�趨Ϊ�������ҪPWM�������Ҫ���д���
  TIM_OCInitStructure.TIM_Pulse =0;                           //ռ�ճ���
//  	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    //�ߵ�ƽ
 	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;         //�ߵ�ƽ	
  TIM_OC1Init(TIM5, &TIM_OCInitStructure);           				
	// 2ͨ��  PA1
 	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                //����״̬
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //�趨Ϊ�������ҪPWM�������Ҫ���д���
  TIM_OCInitStructure.TIM_Pulse =0;                            //ռ�ճ���
//  	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    //�ߵ�ƽ
 	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;         //�ߵ�ƽ	
  TIM_OC2Init(TIM5, &TIM_OCInitStructure);      

	TIM_OC1PreloadConfig(TIM5,TIM_OCPreload_Enable); 	//Ԥװ��ʹ��
	TIM_CtrlPWMOutputs(TIM5,ENABLE);

	TIM_ARRPreloadConfig(TIM5, ENABLE);                   
	TIM_Cmd(TIM5, ENABLE);                   					//ʹ��TIM3��������TIM�����������   
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
