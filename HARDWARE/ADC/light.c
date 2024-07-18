#include "light.h"
#include "adc.h"
/*************************************************
������:void Light_Init(void)
����:��ʼ��������������Ϊģ��
����:��
����ֵ:��
˵��:
	GPIOA.0
**************************************************/
void Light_Init(void)
{
	/*----- 0.�ṹ��������� ------------------*/
	GPIO_InitTypeDef GPIO_InitStruct;
	/*----- 1.ʹ�� RCC_AHB1�������ߵ� GPIOE ---*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	/*----- 2.��ʼ��GPIO���� ------------------*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;					//ģʽ:ģ��
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/****************************************************
����:u16 Light_Get(void)
����:��ȡ����ֵ��дת�����㷨
����ֵ:��ֵ��Χ
��Χ:0-100
��ֵ:0   = �
��ֵ:100 = ����
�����ӽ�:40 - 76��Χ
****************************************************/
u16 Light_Get(void)	//��ȡ����ǿ��
{
	u32 temp_value = 0;
	for(int i = 0; i < 10;i++)
	{
		temp_value += Get_ADC(ADC1,0);	//ADC1��ͨ��0�ɼ�
	}
	
	u16 value = temp_value / 10;
	return 100 - (value / 40.96);
}

