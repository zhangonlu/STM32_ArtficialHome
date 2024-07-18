#include "spi.h"
#include "w25qxx.h"
/****** ���λ�β����� ******/
#define W25QXX_CS PBout(2)		//����Ƭѡ
/*************************************************
������:void W25QXX_Init(void)
����:��ʼ��W25Q64
����:��
����ֵ:��
˵��:
	W25Q64_CS ---- PB.2	Ƭѡ����
	GPIOB������RCC��AHB1����
**************************************************/
void W25QXX_Init(void)
{
	/*----- 0.�ṹ��������� ------------------*/
	GPIO_InitTypeDef GPIO_InitStruct;
	/*----- 1.ʹ�� RCC_AHB1�������ߵ� GPIOB ---*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	/*----- 2.��ʼ��GPIO���� ------------------*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;					//ģʽ:ͨ�����
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;					//����:�������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;						//����:2
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;					//������:����
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;				//�ٶ�:100Mhz
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);		//ע��:ֻ�ܲ���ͬһ��
	/*----- 3.Ĭ�ϵ�ƽ(��)	-------------------*/
	W25QXX_CS = 1;
}

/*************************************************
������:u8 W25Q64_Read_Status_Register(void)
����:��ȡ״̬�Ĵ��� W25Q64(S0λ)
����:��
����ֵ:
	����:0
	����:1
˵��:	������:05h
	BIT7  6   5   4   3   2   1   0
	SPR   RV  TB BP2 BP1 BP0 WEL BUSY
	SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
*************************************************/
u8 W25QXX_Read_Status(void)
{
	u8 status = 0;
	/*----- 1.��Ƭѡ������ ------*/
	W25QXX_CS = 0;
	/*----- 2.���Ͷ�ȡ������ ----*/
	SPI1_Write_Read(0x05);
	/*----- 3.��ȡ�Ĵ���ֵ ------*/
	status = SPI1_Write_Read(0xFF);	//��ȡ��״̬�Ĵ�����ֵ
	/*----- 4.��Ƭѡ������ ------*/
	W25QXX_CS = 1;
	/*----- 5.����״ֵ̬ --------*/
	return status;
}

/*************************************************
������:u8 W25Q64_Wait_Busy(void)
����:�ȴ�д�����
BIT7 == 0000 0001 ���� 0000 0000
0x01 == 0000 0001	   0000 0001
**************************************************/
void W25QXX_Wait_Busy(void)
{
	while((W25QXX_Read_Status() & 0x01) == 0x01);	//���ȿ��� 
}

/*************************************************
������:void W25Q64_Write_Enable(void)
����:дʹ�� W25Q64
����:��
����ֵ:��
˵��:	
������:06h
**************************************************/
void W25QXX_Enable(void)
{
	/*----- 1.��Ƭѡ������ ------*/
	W25QXX_CS = 0;
	/*----- 2.���Ͷ�ȡ������ ----*/
	SPI1_Write_Read(0x06);
	/*----- 3.��Ƭѡ������ ------*/
	W25QXX_CS = 1;
}
/*************************************************
������:void W25Q64_Sector_Erase(u32 addr)
����:��������(4K) W25Q64
����:
	@addr:Ҫ�������׵�ַ
����ֵ:��
˵��:	������:20h
	����������Ҫ���������׵�ַ
**************************************************/
void W25QXX_Sector_Erase(u32 addr)
{
	/*----- 0.���������׵�ַ ----*/
	addr = addr - (addr % 4096);
	/*----- 0.�ȴ�֮ǰ�������---*/
	W25QXX_Wait_Busy();
	/*----- 1.дʹ�� ------------*/
	W25QXX_Enable();
	/*----- 2.��Ƭѡ������ ------*/
	W25QXX_CS = 0;
	/*----- 3.���Ͷ�ȡ������ ----*/
	SPI1_Write_Read(0x20);
	/*----- 4.����������ַ ----*/
	SPI1_Write_Read(addr >> 16);	//A23-A16
	SPI1_Write_Read(addr >> 8);		//A15-A8
	SPI1_Write_Read(addr >> 0);		//A7-A0
	/*----- 5.��Ƭѡ������ ------*/
	W25QXX_CS = 1;
	/*----- 6.�ȴ�������� ------*/
	W25QXX_Wait_Busy();
}

/************************************************
����:void W25Q64_Write_Data(u32 addr, u8 *data, u32 len)
����:ҳ��� �������� = 02h
����:
	@addr����ַ
	@data��Ҫд�������
	@len��Ҫд��ĳ���
����:�ɹ�д������ݳ���
*************************************************/
void W25QXX_Write_Data(u32 addr,u8 *data,u32 len)
{
	/*----- 0.�ȴ�֮ǰ�������---*/
	W25QXX_Wait_Busy();
	/*----- 1.дʹ�� ------------*/
	W25QXX_Enable();
	/*----- 2.��Ƭѡ������ ------*/
	W25QXX_CS = 0;
	/*----- 3.���͹����� --------*/
	SPI1_Write_Read(0x02);
	/*----- 4.����Ҫд�ĵ�ַ ----*/
	SPI1_Write_Read(addr >> 16);	//A23-A16
	SPI1_Write_Read(addr >> 8);		//A15-A8
	SPI1_Write_Read(addr >> 0);		//A7-A0
	
	/*----- 5.ѭ�����ͳ����� ----*/
	for(int i = 0; i < len && i < 256;i++)
	{
		SPI1_Write_Read(data[i]);
	}
	
	/*----- 6.Ƭѡ������ --------*/
	W25QXX_CS = 1;
	/*----- 7.�ȴ�д����� ------*/
	W25QXX_Wait_Busy();
}

/******************************************************
����:void W25Q64_Read_Data(u32 addr, u8 *data, u32 len)
����:������ = 03h
����:
	@addr����ַ
	@data����ȡ�����ݴ�ŵĵ�ַ
	@len��Ҫ���ĳ���
����:�ɹ���ȡ�����ݳ���
*******************************************************/
void W25QXX_Read_Data(u32 addr,u8 *data,u32 len)
{
	/*----- 1.��Ƭѡ������ ------*/
	W25QXX_CS = 0;
	/*----- 2.���͹����� --------*/
	SPI1_Write_Read(0x03);
	/*----- 4.����Ҫ���ĵ�ַ ----*/
	SPI1_Write_Read(addr >> 16);	//A23-A16
	SPI1_Write_Read(addr >> 8);		//A15-A8
	SPI1_Write_Read(addr >> 0);		//A7-A0
	
	/*------ 5.ѭ����ȡ���� -----*/
	for(int i = 0; i < len;i++)
	{
		data[i] = SPI1_Write_Read(0xFF);
	}
	
	/*------ 6.��Ƭѡ������ -----*/
	W25QXX_CS = 1;
}

uint16_t W25QXX_ReadID(void)	//����ID
{
	uint16_t w25qxxID=0;

	W25QXX_CS = 0;
	SPI1_Write_Read(0x90);
	SPI1_Write_Read(0x00);
	SPI1_Write_Read(0x00);
	SPI1_Write_Read(0x00);
	w25qxxID = SPI1_Write_Read(0xff);
	w25qxxID = w25qxxID<<8;
	w25qxxID |= SPI1_Write_Read(0xff);
	W25QXX_CS = 1;
	return w25qxxID;
}

uint16_t W25QXX_Read_JEDID(void)	//оƬID
{
	uint16_t w25qxxID=0;

	W25QXX_CS = 0;
	SPI1_Write_Read(0x9F);
	SPI1_Write_Read(0xff);	//����:
	
	w25qxxID |= SPI1_Write_Read(0xff);
	w25qxxID <<= 8;
	w25qxxID |= SPI1_Write_Read(0xff);
	W25QXX_CS = 1;
	
	return w25qxxID;
}

