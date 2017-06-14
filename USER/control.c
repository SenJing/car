#include "stm32f10x.h"
#include "control.h"
#include "delay.h"
#include "pwm.h"
#include "huidu.h"
#include "led.h"

#include "mpu6050.h"
int sum=0;
int L=235,R=220;    //初始值： L=472,R=440;可以上坡的调试结果L=350,R=327;(235,230)back函数适用Motor(-707,-728);
int H_L=707,H_R=728;//初始值： Motor(-707,-728);
void Go_straight(void)
{
  Motor(L,R);
}
void Turn_left(void)
{
	Motor(-475,270);    
}
void Turn_right(void)
{
	Motor(240,-520);   //760
}
void Stop(void)
{
	Motor(1,1);
	//delay_ms(500);
}
void back( )
{

	if((H4==0)&&(H5==0)) //白色为0  直走
	{
		Motor(-H_L,-H_R);		
	}
	else if	(H5==0) //向右微调
	{
		Motor(-H_L*0.8,-H_R*1.2);
	
	}
	else if	(H4==0) //向左微调
	{
		Motor(-H_L*1.2,-H_R*0.8);
	 
	}	  
	else if((H6==0)||(H7==0))
	{
		Motor(-H_L*0.3,-H_R*1.6);
		
	}
	else if((H3==0)||(H2==0))
	{
		Motor(-H_L*1.6,-H_R*0.3);
	}
	else if((H2==1)&&(H3==1)&&(H4==1)&&(H5==1)&&(H6==1)&&(H7==1))Stop();
	else 
		Motor(-H_L,-H_R);
}
void Straight(void)   //Motor()的参数越大越快
{ 
//	if((Q4==1)&&(Q5==1)&&(Q3==1)&&(Q6==1)&&(Q2==1)&&(Q7==1))
//	{
//		if((H4==0)||(H5==0)||(H3==0)||(H6==0)||(H2==0)||(H7==0))
//			Motor(L,R);
//	}
		if((Q4==0)&&(Q5==0)) //白色为0  直走
	{
		Motor(L,R);
	}
	else if	(Q5==0) //向右微调
	{
		Motor(L*1.2,R*0.8);  //1.2 0.75
	}
	else if	(Q4==0) //向左微调
	{
		Motor(L*0.8,R*1.2);
	}	  
	else	if((Q6==0)||(Q7==0))
	{
		Motor(L*1.3,R*0.9);   //2.0 0.4
	}
	else	if((Q3==0)||(Q2==0))
	{
		Motor(L*0.9,R*1.3);
	}
	else if(Q8==0)//Q8==0||Q7==1
	{
		Motor(L*2.2,R*0.2);   //2.2 0.2
	}
	
	else if(Q1==0)//Q1==0&&Q2==1
	{
		Motor(L*0.2,R*2.2);
	}
	else 
	{
		Go_straight();
	}
}


void TurnLeftxxx(void)		  //TurnRightxxx(-2880);陀螺仪
{

	while(1)
	{
	  if(sum>-7800)
	  {
			Turn_left();
			READ_MPU3050();
			sum+=T_Z;
			//printf("%d\n",sum);
			delay_ms(60);
	  }
		else if(sum<=-7800)
	  {
			//printf("finish");
			break;		
    }
  }
	  Go_straight();
	  delay_ms(285);
    sum=0;
}

void TurnLeftxxx_final_1(int l,int r,int num)		  //TurnRightxxx(-2880);陀螺仪
{
	while(1)
	{
	  if(sum>-num)
	  {
			Motor(l,r);
			READ_MPU3050();
			sum+=T_Z;
	
		//   printf("%d",sum);
			delay_ms(60);
	  }
		else if(sum<=-num)
	  {
			break;		
    }
  }
    sum=0;
}
void TurnRightxxx_final_1(int l,int r,int num)		  //TurnRightxxx(-2880);陀螺仪
{
	while(1)
	{
		LED11=1;
	  if(sum<=num)
	  {
			Motor(l,r);	delay_ms(60);
			READ_MPU3050();
			sum-=T_Z;
		
	  }
		else if(sum>num)
	  {
			break;		
    }
  }
  sum=0;
}
