

#include "adc.h"
/*--------------------- ADC公共默认配置 --------------------------
ADC_CommonInitStruct->ADC_Mode = ADC_Mode_Independent;						//模式:独立模式
ADC_CommonInitStruct->ADC_Prescaler = ADC_Prescaler_Div2;					//分频:2分频 , 84M / 2 则为 42M 
ADC_CommonInitStruct->ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;		//DMA功能:失能
ADC_CommonInitStruct->ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;	//采样时间:5个时钟周期
---------------------- ADC参数默认配置 --------------------------
ADC_InitStruct->ADC_Resolution = ADC_Resolution_12b;						//分辨率:12位
ADC_InitStruct->ADC_ScanConvMode = DISABLE;									//扫描功能:失能
ADC_InitStruct->ADC_ContinuousConvMode = DISABLE;							//连续转换：失能
ADC_InitStruct->ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;	//触发方式：禁止
ADC_InitStruct->ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;			//触发方式：
ADC_InitStruct->ADC_DataAlign = ADC_DataAlign_Right;						//对其方式：右对齐
ADC_InitStruct->ADC_NbrOfConversion = 1;									//采集次数：单次采集
------------------------------------------------------------------*/
/*************************************************
函数名:void ADCx_Init(ADC_TypeDef* ADCx)
功能:初始化ADC
参数:	
	ADCx：ADC号
返回值:无
说明:
	ADC1初始化 APB2 总线
**************************************************/
void ADCx_Init(ADC_TypeDef* ADCx)		//ADC初始化
{
	//1.使能ADC1 总线时钟
	if(ADCx == ADC1)RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	if(ADCx == ADC2)RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	if(ADCx == ADC3)RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
	
	//2.配置ADC公共设置 (很多通道都是用的是同一个ADC)
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	ADC_CommonStructInit(&ADC_CommonInitStruct);
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	//3.ADC参数配置
	ADC_InitTypeDef ADC_InitStruct;
	ADC_StructInit(&ADC_InitStruct);
	ADC_Init(ADCx, &ADC_InitStruct);
	
	//4.使能ADC功能
	ADC_Cmd(ADCx, ENABLE);
}

u16 Get_ADC(ADC_TypeDef* ADCx,u8 ch)
{
	//1.指定ADC通道规则:规则组通道,一个序列采样时间 ---- 单次
	ADC_RegularChannelConfig(ADCx, ch, 1, ADC_SampleTime_480Cycles);	
	
	//2.指定ADC开启转换
	ADC_SoftwareStartConv(ADCx);
	
	//3.等待EOC清0表示转换成功
	while(ADC_GetFlagStatus(ADCx,ADC_FLAG_EOC) != SET);	//死等转换成功
	
	//4.读取转换结果并返回
	return ADC_GetConversionValue(ADCx);
}







