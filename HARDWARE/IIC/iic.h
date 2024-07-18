#ifndef __IIC_H__
#define __IIC_H__
#include "sys.h"
/*---------- λ���� -----------------------*/
#define I2C_SCL		PBout(6)	// ʱ�������

#define I2C_SDA		PBout(7)	// ���������
#define READ_SDA	PBin(7)		// ����������
/***************************************
���巽��ı�����ģʽ�ĺ�
****************************************/
#define I2C_IN()	GPIOB->MODER &= ~(0x3 << 14)	//����ģʽ 00
#define I2C_OUT()	GPIOB->MODER |= 0x1 << 14		//���ģʽ 01
/*---------- �������� ---------------------*/
void IIC_Init(void);			//IIC��ʼ��
void IIC_Start(void);			//��ʼ�ź�
void IIC_Stop(void);			//ֹͣ�ź�
void IIC_Ack(void);				//Ӧ���ź�
void IIC_NAck(void);			//��Ӧ���ź�
int IIC_Wait_Ack(void);			//�ȴ�Ӧ��
void IIC_Send_Byte(u8 data);	//����1�ֽ�
u8 IIC_Read_Byte(void);			//����1�ֽ�
/*---------- �߼����� ---------------------*/
void IIC_Send_Data(u8 Addr,u8 addr,u8 *data,u8 len);	//��������
void IIC_Read_Data(u8 Addr,u8 addr,u8 *data,u8 len);	//��������
#endif
