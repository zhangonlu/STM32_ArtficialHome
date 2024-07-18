#include "pwm.h"

void pwm_init(void)
{
	/*--- ����ʱ�� ---*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	/*---- 2.���Ÿ���ӳ�� ---*/
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_TIM12);
	
	/*---- 3.��ʼ��GPIO���� ----*/
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;			//ģʽ:����
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;	//����:9|10
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	/*--- ѡ��ʱ��Դ������ѡ�����Ĭ�ϵ�RCC�ڲ�ʱ�ӣ����Կ��Բ�д�� ---*/
	TIM_InternalClockConfig(TIM12);
	/*--- ����ʱ����Ԫ ---*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 100-1;//���Զ���װ�ؼ��������ó�10000������10000�����һ��
	TIM_TimeBaseInitStruct.TIM_Prescaler = 8400-1;//��Ƶ8400�κ�Ƶ�ʾ���10000
	TIM_TimeBaseInit(TIM12 ,&TIM_TimeBaseInitStruct);
	
	//����pwm
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCInitStruct.TIM_OCMode =TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 0;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM12, &TIM_OCInitStruct);
	/*--- ������ʱ�� ---*/
	TIM_Cmd(TIM12,ENABLE);
}
//�رշ���
void pwm_stop(void)
{
	TIM_SetCompare2(TIM12,0);
}
//�򿪷���
void pwm_start1(void)
{
	TIM_SetCompare2(TIM12,10);
}
//��������
void pwm_start2(void)
{
	TIM_SetCompare2(TIM12,40);
}
//��������
void pwm_start3(void)
{
	TIM_SetCompare2(TIM12,70);
}

