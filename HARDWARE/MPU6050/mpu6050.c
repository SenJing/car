#include "mpu6050.h"
#include "i2c.h"
unsigned char TX_DATA[4];  	 //��ʾ�ݻ�����
short T_X,T_Y,T_Z,T_T;		 //X,Y,Z�ᣬ�¶�
s32 MPU_SUM=0,angel; 				 //���ֵõ�������

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
    temp_data=temp_data%100;     //ȡ������
    *++s =temp_data/10+0x30;
    temp_data=temp_data%10;      //ȡ������
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
  SDA=0;  //Start:SCLΪ��ʱ��SDA�ɸ߱��
  delay_us(2);
  SCL=0;  //ǯסI2C���ߣ�׼�����ͻ��������
}          
static void I2C_Stop(void)
{
  SDA_OUT();
  SDA=0;
  delay_us(2);
  SCL=1;
  delay_us(2);   //Stop:SCLΪ��ʱ��SDA�ɵͱ��
  SDA=1;
  delay_us(2);
  SCL=0;
}
static void I2C_WaitACK(void)
{
  u8 i;
  SDA=1; delay_us(2);  
  SDA_IN();         //SDA��Ϊ����
  SCL=1; delay_us(2);
  while(Read_SDA()==1 && i<250) //�ھŸ�ʱ�����ڣ�SDA�����Ӧ���źţ�iӦ����Ҫ�úõ���??
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
  SCL=1;                    //����ʱ����
  delay_us(2);                 //��ʱ
  SCL=0;                    //����ʱ����
  delay_us(2);                 //��ʱ
}

//����һ���ֽ�
void I2C_WriteByte(u8 data)
{
  u8 i;
  SDA_OUT();
  SCL=0;       //SCL�õͣ�׼����������
  for(i=0;i<8;i++)
  {
    if(data & 0x80)
      SDA=1;
    else
      SDA=0;
    //SDA = (data & 0x80)>>7;
    data <<= 1;
    delay_us(2);
    SCL=1;    //SCL����׼��д����
    delay_us(2);
    SCL=0;    //SCL��������д��
    delay_us(2);   //������ʱ�ز�����
  }
	I2C_WaitACK();
}

//��һ���ֽ�
u8 I2C_ReadByte(void)
{
  u8 i,data_Receive=0;
  SDA_IN();    //SDA����Ϊ����
  for(i=0;i<8;i++)
  {
    SCL=0;   
    delay_us(2);
    SCL=1;     //SCL�ɵ͵��ߺ󣬿��Դ�SDA��ȡ����
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
//��I2C�豸д��һ���ֽ�����
//**************************************
void Single_WriteI2C(u8 REG_Address,u8 REG_data)
{
  I2C_Start();                  //��ʼ�ź�
  I2C_WriteByte(SlaveAddress);   //�����豸��ַ+д�ź�
  I2C_WriteByte(REG_Address);    //�ڲ��Ĵ�����ַ��
  I2C_WriteByte(REG_data);       //�ڲ��Ĵ������ݣ�
  I2C_Stop();                   //����ֹͣ�ź�
}
//**************************************
//��I2C�豸��ȡһ���ֽ�����
//**************************************
u8 Single_ReadI2C(u8 REG_Address)
{
	u8 REG_data;
	I2C_Start();                   //��ʼ�ź�
	I2C_WriteByte(SlaveAddress);    //�����豸��ַ+д�ź�
	I2C_WriteByte(REG_Address);     //���ʹ洢��Ԫ��ַ����0��ʼ	
	I2C_Start();                   //��ʼ�ź�
	I2C_WriteByte(SlaveAddress+1);  //�����豸��ַ+���ź�
	REG_data=I2C_ReadByte();       //�����Ĵ�������
	I2C_SendACK(1);                //Ӧ���ź�
	I2C_Stop();                    //ֹͣ�ź�
	return REG_data;
}
//**************************************
//��ʼ��MPU3050
//**************************************
void InitMPU3050(void)
{
	I2C_GPIOInit();
	Single_WriteI2C(PWR_MGMT_1,0x80);
	Single_WriteI2C(SMPLRT_DIV,0x07);
	Single_WriteI2C(CONFIG,0x06);
	Single_WriteI2C(GYRO_CONFIG,0x18);		//2000deg/s
	Single_WriteI2C(ACCEL_CONFIG,0x09);		//4g 5Hz
	Single_WriteI2C(PWR_MGMT_1,0x00);	    //�������״̬
	Single_WriteI2C(SIGNAL_PATH_RESET,0x07);
}
//**************************************
//��ȡ���ϳ�����
//**************************************
s16 GetData(u8 REG_Address)
{
	s16 read=0;
	char H,L;
	H=Single_ReadI2C(REG_Address);
	L=Single_ReadI2C(REG_Address+1);
	read=(H<<8)+L;
	read/=16;
	return read;   //�ϳ�����
}
//******��ȡMPU3050����****************************************
void READ_MPU3050(void)
{
	u8 BUF[12];                   //�������ݻ�����

	BUF[0]=Single_ReadI2C(GYRO_XOUT_L); 
	BUF[1]=Single_ReadI2C(GYRO_XOUT_H);
	T_X=(BUF[1]<<8)|BUF[0];
	T_X/=16; 						   		//��ȡ������ٶ�X������
	
	BUF[2]=Single_ReadI2C(GYRO_YOUT_L);
	BUF[3]=Single_ReadI2C(GYRO_YOUT_H);
	T_Y=(BUF[3]<<8)|BUF[2];
	T_Y/=16;									//��ȡ������ٶ�Y������
					   
	BUF[4]=Single_ReadI2C(GYRO_ZOUT_L);
	BUF[5]=Single_ReadI2C(GYRO_ZOUT_H);
	T_Z=(BUF[5]<<8)|BUF[4];
	T_Z/=16; 					       	//��ȡ������ٶ�Z������

//	BUF[6]=Single_ReadI2C(ACCEL_XOUT_L);
//	BUF[7]=Single_ReadI2C(ACCEL_XOUT_H);
//	A_X=(BUF[7]<<8)|BUF[6];
//	A_X/=16; 					       	//��ȡ������ٶ�X������
//
//	BUF[8]=Single_ReadI2C(ACCEL_YOUT_L);
//	BUF[9]=Single_ReadI2C(ACCEL_YOUT_H);
//	A_Y=(BUF[9]<<8)|BUF[8];
//	A_Y/=16; 					       	//��ȡ������ٶ�Y������
//
//	BUF[10]=Single_ReadI2C(ACCEL_ZOUT_L);
//	BUF[11]=Single_ReadI2C(ACCEL_ZOUT_H);
//	A_Z=(BUF[11]<<8)|BUF[10];
//	A_Z/=16; 					       	//��ȡ������ٶ�Z������
}
















