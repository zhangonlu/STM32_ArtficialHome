#include "led.h"
/*---------------------
函数原型: led_init()
功能:   LED初始化
参数:     无
返回值:   无
其他:根据开发板原理图知 
LED1 --> PE8
LED2 --> PE9
LED3 --> PE10
根据开发手册寄存器映射知
GPIOE 挂载在AHB1总线上
-----------------------*/
void LED_Init()
{
	//时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	//GPIO初始化
	 GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType =  GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin =    GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Medium_Speed;
	 GPIO_Init(GPIOE,&GPIO_InitStruct);

	//默认高电平关闭
	LED1 = 1;
	LED2 = 1;
	LED3 = 1;
}


