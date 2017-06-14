#include "mpu6050.h"
#include "i2c.h"
unsigned char TX_DATA[4];  	 //显示据缓存区
short T_X,T_Y,T_Z,T_T;		 //X,Y,Z轴，温度
s32 MPU_SUM=0,angel; 				 //积分得到的数据

void DATA_printf(u8 *s,short temp_data)
{
	if(temp_data<0){
	temp_data=-temp_data;
    *s='-';
	}
	else *s=' ';
		*++s =temp_data/1000+0x30;
		temp_data%=1000;
    *++s =temp_data/100+0x30;
    temp_data=temp_data%100;     //取余运算
    *++s =temp_data/10+0x30;
    temp_data=temp_data%10;      //取余运算
    *++s =temp_data+0x30;
		*++s=0; 	
}

static void I2C_GPIOInit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	SDA_OUT();
	SCL_OUT();
  SCL=1;
  SDA=1;
}

static void I2C_Start(void)
{
  SDA_OUT();
  SDA=1;
  delay_us(2);
  SCL=1;
  delay_us(2);
  SDA=0;  //Start:SCL为高时，SDA由高变低
  delay_us(2);
  SCL=0;  //钳住I2C总线，准备发送或接收数据
}          
static void I2C_Stop(void)
{
  SDA_OUT();
  SDA=0;
  delay_us(2);
  SCL=1;
  delay_us(2);   //Stop:SCL为高时，SDA由低变高
  SDA=1;
  delay_us(2);
  SCL=0;
}
static void I2C_WaitACK(void)
{
  u8 i;
  SDA=1; delay_us(2);  
  SDA_IN();         //SDA置为输入
  SCL=1; delay_us(2);
  while(Read_SDA()==1 && i<250) //第九个时钟周期，SDA变低是应答信号，i应该需要好好调整??
  {
    i++;
  }
  SCL=0;   
  delay_us(2);
}

//send ack 1:nack,0:ack
static void I2C_SendACK(u8 ack)
{
  if(ack)
		SDA=1;
	else
		SDA=0;
  SCL=1;                    //拉高时钟线
  delay_us(2);                 //延时
  SCL=0;                    //拉低时钟线
  delay_us(2);                 //延时
}

//发送一个字节
void I2C_WriteByte(u8 data)
{
  u8 i;
  SDA_OUT();
  SCL=0;       //SCL置低，准备发送数据
  for(i=0;i<8;i++)
  {
    if(data & 0x80)
      SDA=1;
    else
      SDA=0;
    //SDA = (data & 0x80)>>7;
    data <<= 1;
    delay_us(2);
    SCL=1;    //SCL拉高准备写数据
    delay_us(2);
    SCL=0;    //SCL拉低数据写完
    delay_us(2);   //三个延时必不可少
  }
	I2C_WaitACK();
}

//读一个字节
u8 I2C_ReadByte(void)
{
  u8 i,data_Receive=0;
  SDA_IN();    //SDA设置为输入
  for(i=0;i<8;i++)
  {
    SCL=0;   
    delay_us(2);
    SCL=1;     //SCL由低到高后，可以从SDA读取数据
    delay_us(2);
    if(Read_SDA()) 
      data_Receive |= (1<<(7-i));
    else
      data_Receive |= (0<<(7-i));
    //data_Receive <<= 1;
    delay_us(2);    
  }  
  return data_Receive;
}

//**************************************
//向I2C设备写入一个字节数据
//**************************************
void Single_WriteI2C(u8 REG_Address,u8 REG_data)
{
  I2C_Start();                  //起始信号
  I2C_WriteByte(SlaveAddress);   //发送设备地址+写信号
  I2C_WriteByte(REG_Address);    //内部寄存器地址，
  I2C_WriteByte(REG_data);       //内部寄存器数据，
  I2C_Stop();                   //发送停止信号
}
//**************************************
//从I2C设备读取一个字节数据
//**************************************
u8 Single_ReadI2C(u8 REG_Address)
{
	u8 REG_data;
	I2C_Start();                   //起始信号
	I2C_WriteByte(SlaveAddress);    //发送设备地址+写信号
	I2C_WriteByte(REG_Address);     //发送存储单元地址，从0开始	
	I2C_Start();                   //起始信号
	I2C_WriteByte(SlaveAddress+1);  //发送设备地址+读信号
	REG_data=I2C_ReadByte();       //读出寄存器数据
	I2C_SendACK(1);                //应答信号
	I2C_Stop();                    //停止信号
	return REG_data;
}
//**************************************
//初始化MPU3050
//**************************************
void InitMPU3050(void)
{
	I2C_GPIOInit();
	Single_WriteI2C(PWR_MGMT_1,0x80);
	Single_WriteI2C(SMPLRT_DIV,0x07);
	Single_WriteI2C(CONFIG,0x06);
	Single_WriteI2C(GYRO_CONFIG,0x18);		//2000deg/s
	Single_WriteI2C(ACCEL_CONFIG,0x09);		//4g 5Hz
	Single_WriteI2C(PWR_MGMT_1,0x00);	    //解除休眠状态
	Single_WriteI2C(SIGNAL_PATH_RESET,0x07);
}
//**************************************
//读取并合成数据
//**************************************
s16 GetData(u8 REG_Address)
{
	s16 read=0;
	char H,L;
	H=Single_ReadI2C(REG_Address);
	L=Single_ReadI2C(REG_Address+1);
	read=(H<<8)+L;
	read/=16;
	return read;   //合成数据
}
//******读取MPU3050数据****************************************
void READ_MPU3050(void)
{
	u8 BUF[12];                   //接收数据缓存区

	BUF[0]=Single_ReadI2C(GYRO_XOUT_L); 
	BUF[1]=Single_ReadI2C(GYRO_XOUT_H);
	T_X=(BUF[1]<<8)|BUF[0];
	T_X/=16; 						   		//读取计算角速度X轴数据
	
	BUF[2]=Single_ReadI2C(GYRO_YOUT_L);
	BUF[3]=Single_ReadI2C(GYRO_YOUT_H);
	T_Y=(BUF[3]<<8)|BUF[2];
	T_Y/=16;									//读取计算角速度Y轴数据
					   
	BUF[4]=Single_ReadI2C(GYRO_ZOUT_L);
	BUF[5]=Single_ReadI2C(GYRO_ZOUT_H);
	T_Z=(BUF[5]<<8)|BUF[4];
	T_Z/=16; 					       	//读取计算角速度Z轴数据

//	BUF[6]=Single_ReadI2C(ACCEL_XOUT_L);
//	BUF[7]=Single_ReadI2C(ACCEL_XOUT_H);
//	A_X=(BUF[7]<<8)|BUF[6];
//	A_X/=16; 					       	//读取计算加速度X轴数据
//
//	BUF[8]=Single_ReadI2C(ACCEL_YOUT_L);
//	BUF[9]=Single_ReadI2C(ACCEL_YOUT_H);
//	A_Y=(BUF[9]<<8)|BUF[8];
//	A_Y/=16; 					       	//读取计算加速度Y轴数据
//
//	BUF[10]=Single_ReadI2C(ACCEL_ZOUT_L);
//	BUF[11]=Single_ReadI2C(ACCEL_ZOUT_H);
//	A_Z=(BUF[11]<<8)|BUF[10];
//	A_Z/=16; 					       	//读取计算加速度Z轴数据
}
















