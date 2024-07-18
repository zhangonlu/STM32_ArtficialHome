#include "voltage.h"
#include "adc.h"
/*************************************************
������:void Voltage_Init(void)
����:��ʼ��������������Ϊģ��
����:��
����ֵ:��
˵��:
	GPIOC.0
**************************************************/
void Voltage_Init(void)
{
	/*----- 0.�ṹ��������� ------------------*/
	GPIO_InitTypeDef GPIO_InitStruct;
	/*----- 1.ʹ�� RCC_AHB1�������ߵ� GPIOC ---*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	/*----- 2.��ʼ��GPIO���� ------------------*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;					//ģʽ:ģ��
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	
	GPIO_Init(GPIOC, &GPIO_InitStruct);
}

/****************************************************
����:u16 Voltage_Get(void)
����:��ȡ��ѹֵ��дת��������
����ֵ:��ֵ��Χ
��Χ:0~3.3
****************************************************/
double Voltage_Get(void)
{
	u32 temp_value = 0;
	for(int i = 0; i < 10;i++)
	{
		temp_value += Get_ADC(ADC1,10);	//ADC1��ͨ��10�ɼ�
	}
	
	u16 value = temp_value / 10;
	return value * 3.3 / 4096;
}

