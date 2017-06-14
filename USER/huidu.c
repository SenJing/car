#include "huidu.h"

///////////////////////////////////////////////
///灰度传感器只用了2-7灯分别对应是PB9-PB14/////
///////////////////////////////////////////////
void huidu_Init()
{
 GPIO_InitTypeDef  GPIO_InitStructure;
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable);
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);	 //使能PA,PB，PC端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15|GPIO_Pin_14|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_1;				 //PB 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //模式设置为上拉输入
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;				 //PA端口配置，其中PA8控制LED0，LED0-->PA.8 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //模式设置为上拉输入
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);		//根据设定参数初始化GPIOA
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_8;				//PC端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //模式设置为上拉输入
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);		//根据设定参数初始化GPIOA.8
 
}

