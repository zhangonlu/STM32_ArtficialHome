#include "spi.h"
/************************************
����:void SPI1_Init(void)
����:��ʼ�� SPI1 ����
����:��
����ֵ:��
˵��:
	.ͨ��ԭ��ͼ
	SCLK1 ʱ���� ------ PB3 
	MISO1 ������ ------ PB4
	MOSI1 ������ ------ PB5
	.ͨ��оƬ�ֲ�
	GPIOB ---- AHB1
	SPI1  ---- APB2
**************************************/
void SPI1_Init(void)
{
	/********* 1.ʹ�� GPIOB �� SPI1 ����ʱ������ *********/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	/********* 2.��ʼ��GPIOB �������Ϊ����ģʽ **********/
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;	//����:3,4,5
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;						//ģʽ:����
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_8;	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;						//ģʽ:���
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	PBout(2) = 1;
	PBout(8) = 1;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;						//ģʽ:���
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	PEout(1) = 1;
	
	/********* 3.���Ÿ��ó�SPI1���� **********************/
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);
	
	/********* 4.��ʼ��SPI��ز���  ***********************/
	SPI_InitTypeDef SPI_InitStruct;
	
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;	//������(��Ƶϵ��)4
	SPI_InitStruct.SPI_CPHA = 0;									//����:0	ǰʱ���ز���,��ʱ�������
	SPI_InitStruct.SPI_CPOL = 0;									//��λ:0	�͵�ƽ��ʼ�ɼ�
	SPI_InitStruct.SPI_CRCPolynomial = 7;							//CRCУ�����ʽ:7�� x1+x2+x3
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;					//����λ:8λ
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//����:ȫ˫��
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;					//���ݷ��ͷ�ʽ:��λ�ȷ�
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;						//ģʽ:����
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;							//NNS����:���
	
	SPI_Init(SPI1, &SPI_InitStruct);
	
	/********** 5.ʹ��SPI1���� ****************************/
	SPI_Cmd(SPI1, ENABLE);
}

/**********************************************************
����:u8 SPI1_Write_Read(u8 data)
����:���� | ���� 1�ֽ�����
����:
	@data:����
����ֵ:����
˵��: ���ͺͽ��ն���ͬ��������
	1.��������ʱ,���Ժ��Խ��յ�����.
	2.��������ʱ,���Է���һ�����ֽ� 0x00 �� 0xFF
***********************************************************/
u8 SPI1_Write_Read(u8 data)
{
	/****** 1.�鿴SPI1�����Ƿ�æµ,���æµ��ȴ�����(TXE���ͱ�־λ) ******/
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != SET);	//���ȷ�����ɱ�־
	
	/****** 2.������д��SPI1��λ�Ĵ���,�����Ƿ��� ************************/
	SPI_I2S_SendData(SPI1, data);
	
	/****** 3.�鿴SPI1�����Ƿ��пɶ�����(RXNE���ձ�־λ) *****************/
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != SET);	//���Ƚ�����ɱ�־
	
	/****** 4.����λ�Ĵ����е����ݶ�ȡ���� *******************************/
	return SPI_I2S_ReceiveData(SPI1);
}










