#ifndef __IIC_H__
#define __IIC_H__
#include "sys.h"
/*---------- 位带宏 -----------------------*/
#define I2C_SCL		PBout(6)	// 时钟线输出

#define I2C_SDA		PBout(7)	// 数据线输出
#define READ_SDA	PBin(7)		// 数据线输入
/***************************************
定义方便改变引脚模式的宏
****************************************/
#define I2C_IN()	GPIOB->MODER &= ~(0x3 << 14)	//输入模式 00
#define I2C_OUT()	GPIOB->MODER |= 0x1 << 14		//输出模式 01
/*---------- 基础函数 ---------------------*/
void IIC_Init(void);			//IIC初始化
void IIC_Start(void);			//起始信号
void IIC_Stop(void);			//停止信号
void IIC_Ack(void);				//应答信号
void IIC_NAck(void);			//非应答信号
int IIC_Wait_Ack(void);			//等待应答
void IIC_Send_Byte(u8 data);	//发送1字节
u8 IIC_Read_Byte(void);			//接收1字节
/*---------- 高级函数 ---------------------*/
void IIC_Send_Data(u8 Addr,u8 addr,u8 *data,u8 len);	//发送数据
void IIC_Read_Data(u8 Addr,u8 addr,u8 *data,u8 len);	//接收数据
#endif
