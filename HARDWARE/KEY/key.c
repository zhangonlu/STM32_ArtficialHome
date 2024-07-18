#include "key.h"
/*--------------------------
����:void KEY_Init(void)
����:������ʼ��
����:��
����ֵ:��
˵����
�鿴ԭ��ͼ
	KEY1 ---- PE4
	KEY2 ---- PE5
	KEY3 ---- PE6
	KEY4 ---- û��
�鿴оƬ�ֲ�
	GPIOE ������ AHB1 ��������
---------------------------*/
void KEY_Init(void)
{
	/*----- 0.�ṹ��������� -----*/
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/*----- 1.ʹ�� RCC_AHB1 �������� GPIOE ---*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	/*----- 2.��ʼ��GPIO���� -----------------*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;							//ģʽ:����ģʽ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;		//����:4|5|6
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;							//������:����
	
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	/*----- 2.��ʼ��GPIO���� -----------------*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;							//ģʽ:����ģʽ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;		//����:13
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;							//������:����
	
	GPIO_Init(GPIOC, &GPIO_InitStruct);
}

/*----------------------
����:u8 KEY_Scan(void)
����:ɨ�谴��
����:��
����ֵ:	����ֵ
	KEY1 ���·��� 1
	KEY2 ���·��� 2
	KEY3 ���·��� 3
	KEY4 ���·��� 4

	����û�����򷵻� 0
˵��:
	����������ʱ,���Żᱻ���� (0)
	����ֻҪ��� ���ŵ�ƽΪ0 ���ʾ����
------------------------*/
#include "delay.h"
u8 KEY_Scan(void)
{
	static u8 flag = 0;		//���ڴ洢�ϴΰ���״̬
	if((KEY1 == 0 || KEY2 == 0 || KEY3 == 0|| KEY4 == 0) && flag == 0)
	{
		flag = 1;		//�������±���
		delay_ms(10);	//��ʱ����
		if(KEY1 == 0) return 1;
		if(KEY2 == 0) return 2;
		if(KEY3 == 0) return 3;
		if(KEY4 == 0) return 4;
	}
	
	if(KEY1 != 0 && KEY2 != 0 && KEY3 != 0&& KEY4 != 0)
	{
		flag = 0;		//����û�а��±���
	}
	
	return 0;
}	





