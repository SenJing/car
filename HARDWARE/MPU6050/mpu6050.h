#ifndef GY521_H
#define GY521_H

#include "sys.h"
#include "delay.h"
//#define SDA         PBout(10)		 //SDA PB10		 
//#define SCL         PBout(11)		 //SCL PB11
//#define Read_SDA()  PBin(10)
#define SDA         PCout(11)		 //SDA PB6		 
#define SCL         PCout(12)		 //SCL PB7
#define Read_SDA()  PCin(11)

/*要更改的地方：IO口，寄存器的值，时钟使能*/

#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			  0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define SIGNAL_PATH_RESET  0x68  //Signal Path Reset 
#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I			0x75	//IIC地址寄存器(默认数值0x68，只读)
#define	SlaveAddress	0xD0	//IIC写入时的地址字节数据，+1为读取


//#define SDA_OUT() { GPIOB->CRH &= 0XFFFF0FFF ; GPIOB->CRH |= 0X00002000;}
//#define SDA_IN()  { GPIOB->CRH &= 0XFFFF0FFF ; GPIOB->CRH |= 0X00008000;}
//#define SCL_OUT() { GPIOB->CRH &= 0XFFFFF0FF ; GPIOB->CRH |= 0X00000200;}

//#define SDA_OUT() { GPIOB->CRH &= 0XFFFFF0FF ; GPIOB->CRH |= 0X00000200;}
//#define SDA_IN()  { GPIOB->CRH &= 0XFFFFF0FF ; GPIOB->CRH |= 0X00000800;}
//#define SCL_OUT() { GPIOB->CRH &= 0XFFFF0FFF ; GPIOB->CRH |= 0X00002000;}

//#define SDA_OUT() { GPIOB->CRL &= 0XF0FFFFFF ; GPIOB->CRL |= 0X02000000;}
//#define SDA_IN()  { GPIOB->CRL &= 0XF0FFFFFF ; GPIOB->CRL |= 0X08000000;}
//#define SCL_OUT() { GPIOB->CRL &= 0X0FFFFFFF ; GPIOB->CRL |= 0X20000000;}

//#define SDA_OUT() { GPIOC->CRH &= 0XFFFF0FFF ; GPIOC->CRH |= 0X00002000;}
//#define SDA_IN()  { GPIOC->CRH &= 0XFFFF0FFF ; GPIOC->CRH |= 0X00008000;}
#define SCL_OUT() { GPIOC->CRH &= 0XFFF0FFFF ; GPIOC->CRH |= 0X00020000;}

extern unsigned char TX_DATA[4];  	 
extern short T_X,T_Y,T_Z,T_T;		
extern s32 MPU_SUM,angel; 				 

static void I2C_GPIOInit(void);
static void I2C_Start(void);
static void I2C_Stop(void);
static void I2C_SendACK(u8 ack);
static void I2C_WaitACK(void);
void I2C_WriteByte(u8 data);
u8 I2C_ReadByte(void);

void DATA_printf(u8 *s,short temp_data);
void Single_WriteI2C(u8 REG_Address,u8 REG_data);
u8 Single_ReadI2C(u8 REG_Address);
void InitMPU3050(void);
s16 GetData(u8 REG_Address);
void READ_MPU3050(void);


#endif


