#include "led.h"
#include "delay.h"
#include "sys.h"
#include "pwm.h"
#include "huidu.h"
#include "control.h"
#include "key.h"
#include "i2c.h"
#include "mpu6050.h"
#include "sys.h"
#include "usart.h"
//ALIENTEK Mini STM32�����巶������8
//PWM���ʵ��   
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾
 int main(void)
 {	extern int sum;	
	 extern int L,R;
//	u16 led0pwmval=0;    
//	u8 dir=1;
	u8 t=0;	
	// huidu_Init();
	delay_init();	    	 //��ʱ������ʼ��	  
	 huidu_Init();
	wheel_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	 KEY_Init(); 
	 uart_init(9600);	 //���ڳ�ʼ��Ϊ9600	
	PWM_Init(899,0);//����Ƶ��PWMƵ��=72000/(899+1)=80Khz 	
	  IIC_Init();
	InitMPU3050();
	delay_ms(500);
	IIC_Init();
	InitMPU3050();
	delay_ms(500);
//	 while(1)
//	 {	
//		Straight(); 
//    if(ZL==0)	t++;
//		 delay_ms(2000);
//    if(t==3)
//		
//Motor(-400,-400);

	TurnRightxxx_final_1(500,-500,9800);//4600
		Straight();
		delay_ms(2000);
		Stop();
		
		
		delay_ms(100);

} 


