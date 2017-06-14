#ifndef __huidu_H
#define __huidu_H	 
#include "sys.h"
void huidu_Init(void);

#define Q1 PCin(9)      //设置PC9为输入模式，以下同理(针对MCU）
#define Q2 PBin(9)		//设置PA4 5 6 7 11，PB0 1 6 7 8 9 10 11 12 14 15，PC8 9；
#define Q3 PBin(10)
#define Q4 PBin(11)
#define Q5 PBin(12)
#define Q6 PBin(15)
#define Q7 PBin(14)
#define Q8 PCin(8)
#define H2 PAin(4)
#define H3 PAin(5)
#define H4 PAin(6)
#define H5 PAin(7)
#define H6 PBin(1)
#define H7 PAin(11)
#define ZL PBin(8)
#define ZR PBin(7)
#define GD PBin(6)
#define HIT PBin(0)

#endif
