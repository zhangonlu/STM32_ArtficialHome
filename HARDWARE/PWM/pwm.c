#include "pwm.h"

void pwm_init(void)
{
	/*--- 开启时钟 ---*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	/*---- 2.引脚复用映射 ---*/
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_TIM12);
	
	/*---- 3.初始化GPIO引脚 ----*/
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;			//模式:复用
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;	//引脚:9|10
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	/*--- 选择时钟源（这里选择的是默认的RCC内部时钟，所以可以不写） ---*/
	TIM_InternalClockConfig(TIM12);
	/*--- 配置时基单元 ---*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 100-1;//将自动重装载计数器设置成10000，计数10000后更新一次
	TIM_TimeBaseInitStruct.TIM_Prescaler = 8400-1;//分频8400次后频率就是10000
	TIM_TimeBaseInit(TIM12 ,&TIM_TimeBaseInitStruct);
	
	//配置pwm
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCInitStruct.TIM_OCMode =TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 0;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM12, &TIM_OCInitStruct);
	/*--- 开启定时器 ---*/
	TIM_Cmd(TIM12,ENABLE);
}
//关闭风扇
void pwm_stop(void)
{
	TIM_SetCompare2(TIM12,0);
}
//打开风扇
void pwm_start1(void)
{
	TIM_SetCompare2(TIM12,10);
}
//二挡风扇
void pwm_start2(void)
{
	TIM_SetCompare2(TIM12,40);
}
//三挡风扇
void pwm_start3(void)
{
	TIM_SetCompare2(TIM12,70);
}

