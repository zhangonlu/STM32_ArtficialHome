#include "key.h"
/*--------------------------
函数:void KEY_Init(void)
功能:按键初始化
参数:无
返回值:无
说明：
查看原理图
	KEY1 ---- PE4
	KEY2 ---- PE5
	KEY3 ---- PE6
	KEY4 ---- 没看
查看芯片手册
	GPIOE 挂载在 AHB1 外设总线
---------------------------*/
void KEY_Init(void)
{
	/*----- 0.结构体变量定义 -----*/
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/*----- 1.使能 RCC_AHB1 外设总线 GPIOE ---*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	/*----- 2.初始化GPIO引脚 -----------------*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;							//模式:输入模式
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;		//引脚:4|5|6
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;							//上下拉:上拉
	
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	/*----- 2.初始化GPIO引脚 -----------------*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;							//模式:输入模式
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;		//引脚:13
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;							//上下拉:上拉
	
	GPIO_Init(GPIOC, &GPIO_InitStruct);
}

/*----------------------
函数:u8 KEY_Scan(void)
功能:扫描按键
参数:无
返回值:	按键值
	KEY1 按下返回 1
	KEY2 按下返回 2
	KEY3 按下返回 3
	KEY4 按下返回 4

	若都没按下则返回 0
说明:
	当按键按下时,引脚会被拉低 (0)
	所以只要检测 引脚电平为0 则表示按下
------------------------*/
#include "delay.h"
u8 KEY_Scan(void)
{
	static u8 flag = 0;		//用于存储上次按键状态
	if((KEY1 == 0 || KEY2 == 0 || KEY3 == 0|| KEY4 == 0) && flag == 0)
	{
		flag = 1;		//按键按下保存
		delay_ms(10);	//延时消抖
		if(KEY1 == 0) return 1;
		if(KEY2 == 0) return 2;
		if(KEY3 == 0) return 3;
		if(KEY4 == 0) return 4;
	}
	
	if(KEY1 != 0 && KEY2 != 0 && KEY3 != 0&& KEY4 != 0)
	{
		flag = 0;		//按键没有按下保存
	}
	
	return 0;
}	





