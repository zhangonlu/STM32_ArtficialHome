#include "led.h"
/*---------------------
����ԭ��: led_init()
����:   LED��ʼ��
����:     ��
����ֵ:   ��
����:���ݿ�����ԭ��ͼ֪ 
LED1 --> PE8
LED2 --> PE9
LED3 --> PE10
���ݿ����ֲ�Ĵ���ӳ��֪
GPIOE ������AHB1������
-----------------------*/
void LED_Init()
{
	//ʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	//GPIO��ʼ��
	 GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType =  GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin =    GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Medium_Speed;
	 GPIO_Init(GPIOE,&GPIO_InitStruct);

	//Ĭ�ϸߵ�ƽ�ر�
	LED1 = 1;
	LED2 = 1;
	LED3 = 1;
}


