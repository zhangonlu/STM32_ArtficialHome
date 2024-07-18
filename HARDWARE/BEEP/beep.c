#include "stm32f4xx.h"                  // Device header
#include "beep.h"
/*--------------------------
����:void BEEP_Init(void)
����:��������ʼ��
����:��
����ֵ:��
˵����
ͨ��ԭ��ͼ
	BEEP ----- GPIOB.10
ͨ��оƬ�ֲ�
	GPIOB ������ AHB1 ������
---------------------------*/
void BEEP_Init(void)
{
	/*----- 0.�ṹ��������� ----*/
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/*----- 1.ʹ��RCC_AHB1��������GPIOB ---*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	/*----- 2.��ʼ��GPIO���� --------------*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;		//ģʽ:ͨ�����
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; 	//�������:����
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;			//����:10
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;		//������:����
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;	//�ٶ�:����
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	/*----- 3.Ĭ���豸״̬:�� -------------*/
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
}


