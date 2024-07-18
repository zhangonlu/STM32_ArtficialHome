#include "voltage.h"
#include "adc.h"
/*************************************************
函数名:void Voltage_Init(void)
功能:初始化光敏电阻引脚为模拟
参数:无
返回值:无
说明:
	GPIOC.0
**************************************************/
void Voltage_Init(void)
{
	/*----- 0.结构体变量定义 ------------------*/
	GPIO_InitTypeDef GPIO_InitStruct;
	/*----- 1.使能 RCC_AHB1外设总线的 GPIOC ---*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	/*----- 2.初始化GPIO引脚 ------------------*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;					//模式:模拟
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	
	GPIO_Init(GPIOC, &GPIO_InitStruct);
}

/****************************************************
函数:u16 Voltage_Get(void)
功能:获取电压值并写转换浮点数
返回值:数值范围
范围:0~3.3
****************************************************/
double Voltage_Get(void)
{
	u32 temp_value = 0;
	for(int i = 0; i < 10;i++)
	{
		temp_value += Get_ADC(ADC1,10);	//ADC1的通道10采集
	}
	
	u16 value = temp_value / 10;
	return value * 3.3 / 4096;
}

