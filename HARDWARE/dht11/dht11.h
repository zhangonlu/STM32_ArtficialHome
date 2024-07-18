#ifndef __DHT11_H__
#define __DHT11_H__
#include "sys.h"
typedef struct {
	u8 HumiH;	//湿度高位
	u8 HumiL;	//湿度低位
	u8 TempH;	//温度高位
	u8 TempL;	//温度地位
	double Hum;	//湿度
	double Tem;	//温度
}DHT11_t;

/******************************************************
函数:void DHT11_Init(void)
功能:初始化DHT11温湿度模块
参数:无
返回值:无
说明:
	.通过原理图
	DHT11_DATA ---- PA3
	.通过芯片手册
	GPIOA --------- AHB1
********************************************************/
void DHT11_Init(void);
/******************************************************************************
函数:u8 DHT11_Read_Data(u8 *HumiH,u8 *HumiL,u8 *TempH,u8 *TempL)
功能:读取DHT11温湿度
参数:
	@HumiH:湿度高位
	@HumiL:湿度低位
	@TempH:温度高位
	@TempL:温度低位
返回值:
	成功:0
	失败:!0
******************************************************************************/
u8 DHT11_Read_Data(DHT11_t *dht11);
void DHT11_Init(void);

#endif
