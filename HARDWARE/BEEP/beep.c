#include "stm32f4xx.h"                  // Device header
#include "beep.h"
/*--------------------------
函数:void BEEP_Init(void)
功能:蜂鸣器初始化
参数:无
返回值:无
说明：
通过原理图
	BEEP ----- GPIOB.10
通过芯片手册
	GPIOB 挂载在 AHB1 总线上
---------------------------*/
void BEEP_Init(void)
{
	/*----- 0.结构体变量定义 ----*/
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/*----- 1.使能RCC_AHB1外设总线GPIOB ---*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	/*----- 2.初始化GPIO引脚 --------------*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;		//模式:通用输出
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; 	//输出类型:推挽
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;			//引脚:10
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;		//上下拉:上拉
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;	//速度:高速
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	/*----- 3.默认设备状态:关 -------------*/
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
}


