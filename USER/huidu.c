#include "huidu.h"

///////////////////////////////////////////////
///�Ҷȴ�����ֻ����2-7�Ʒֱ��Ӧ��PB9-PB14/////
///////////////////////////////////////////////
void huidu_Init()
{
 GPIO_InitTypeDef  GPIO_InitStructure;
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable);
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PA,PB��PC�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15|GPIO_Pin_14|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_1;				 //PB �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //ģʽ����Ϊ��������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;				 //PA�˿����ã�����PA8����LED0��LED0-->PA.8 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //ģʽ����Ϊ��������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);		//�����趨������ʼ��GPIOA
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_8;				//PC�˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //ģʽ����Ϊ��������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);		//�����趨������ʼ��GPIOA.8
 
}

