#ifndef __ADC_H__
#define __ADC_H__
#include "sys.h"
void ADCx_Init(ADC_TypeDef* ADCx);		//ADC初始化
u16 Get_ADC(ADC_TypeDef* ADCx,u8 ch);	//获取通道转换结果

#endif
