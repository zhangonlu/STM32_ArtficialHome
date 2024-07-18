#include "dht11.h"
#include "delay.h"
/*********** ����һЩ��,�ı�SDA���ŷ��� *******/
#define DHT11_Init_IN()		GPIOA->MODER &= ~(0x3 << 6)		//����ģʽ00
#define DHT11_Init_OUT()	GPIOA->MODER |=  (0x1 << 6)		//���ģʽ01
/*********** ����һЩ��,λ���� ****************/
#define DHT11_OUT	PAout(3) 	//���ڵ�ƽ���[�� | ��] ��ƽ	---- ����,���� ----
#define DHT11_IN	PAin(3)		//���ڵ�ƽ��ȡ[�� | ��] ��ƽ	---- ����,�ӷ� ----

/******************************************************
����:void DHT11_Init(void)
����:��ʼ��DHT11��ʪ��ģ��
����:��
����ֵ:��
˵��:
	.ͨ��ԭ��ͼ
	DHT11_DATA ---- PA3
	.ͨ��оƬ�ֲ�
	GPIOA --------- AHB1
********************************************************/
void DHT11_Init(void)
{
	/*********1.ʹ��GPIOA����ʱ�� ********/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	/*********2.��ʼ��GPIOA������� ******/
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;			//����:3
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;		//ģʽ:���ģʽ
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;		//�������:����|��©
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;		//������:����
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;	//����ٶ�:50MHz
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/******************************************************
����:void DHT11_Reset(void)
����:��ʼ�ź�|��λ�ź�   (����)
����:��
����ֵ:��
******************************************************/
void DHT11_Reset(void)
{
	//1.�������ŵ�ģʽ:���
	DHT11_Init_OUT();
	
	//2.����͵�ƽ[18:30]ms,����:20ms
	DHT11_OUT = 0;
	delay_ms(20);
	
	//3.����ߵ�ƽ[10:35]us,����:13us
	DHT11_OUT = 1;
	delay_us(13);
}

/******************************************************
����:u8 DHT11_Check(void)
����:�����Ӧ�ź�   (�ӿ�)
����:��
����ֵ:	
	����:0
	������:!0
******************************************************/
u8 DHT11_Check(void)
{
	int timer = 0;
	/******1.��������ģʽ:���� *******/
	DHT11_Init_IN();
	
	/******2.�ȴ����豸����������,������һ���ȴ�100us,����ʱ ****/
	timer = 0;
	while(DHT11_IN == 1)	//��������
	{
		delay_us(1);
		timer++;
		if(timer > 100) return 1;	//������
	}
	
	/******3.�ȴ����豸��Ӧ(����)ʱ��[78:88] us,����:83us,��ֻ�ӵȴ����ʱ�� ****/
	timer = 0;
	while(DHT11_IN == 0)	//��������
	{
		delay_us(1);
		timer++;
		if(timer > 88) return 1;	//������
	}
	
	/******4.�ȴ����豸�ߵ�ƽ[80:92]us,����87us����ֱ�ӵȴ����ʱ��92us ********/
	delay_us(87);
	return 0;	//����
}

/******************************************************
����:u8 DHT11_Read_Bit(void)
����:��ȡ���� 1bit   (�ӿ�)
����:��
����ֵ:����
******************************************************/
u8 DHT11_Read_Bit(void)
{
	int timer = 0;
	/******1.��������ģʽ:���� *******/
	DHT11_Init_IN();
	
	/******2.����DHT11����,�ȴ�100us ***********/
	timer = 0;
	while(DHT11_IN == 1) //��������
	{
		delay_us(1);
		timer++;
		if(timer > 100) return 0;	//��ʱ
	}
	
	/******3.������[50:58]us,����:54us,Ȼ������,�ȴ����ʱ��58us *****/
	while(DHT11_IN == 0) //��������
	{
		delay_us(1);
		timer++;
		if(timer > 100) return 0;	//��ʱ
	}
	
	/******4.��¼�ߵ�ƽ����ʱ�� 0ֵ:[23-27]us, 1ֵ:[68-74] ����ѡ���ʱ��74us, *******/
	timer = 0;
	while(DHT11_IN == 1)	//��������
	{
		delay_us(1);
		timer++;
		if(timer > 100)  return 0;	//��ʱ
	}
	/******5.�жϸߵ�ƽ����ʱ�������ĸ���Χ 0ֵ:[23-27]us, 1ֵ:[68-74]***/
	return timer < 50 ? 0 : 1;
}

/******************************************************
����:u8 DHT11_Read_Byte(void)
����:��ȡ���� 1Byte   (�ӿ�)
����:��
����ֵ:����
˵��:��λ�ȳ�,ÿ��1λ,����1��
******************************************************/
u8 DHT11_Read_Byte(void)
{
	u8 data = 0;	//�洢��ȡ������
	/******* ѭ��8�ζ�ȡ���� ******/
	for(int i = 0; i < 8; i++)
	{
		data <<= 1;					//������1λ
		data |= DHT11_Read_Bit();	//��ȡ1bit����
	}
	return data;
}

/******************************************************************************
����:u8 DHT11_Read_Data(u8 *HumiH,u8 *HumiL,u8 *TempH,u8 *TempL)
����:��ȡDHT11��ʪ��
����:
	@HumiH:ʪ�ȸ�λ
	@HumiL:ʪ�ȵ�λ
	@TempH:�¶ȸ�λ
	@TempL:�¶ȵ�λ
����ֵ:
	�ɹ�:0
	ʧ��:!0
******************************************************************************/
#include "stdio.h"
u8 DHT11_Read_Data(DHT11_t *dht11)
{
	//1.��ʼ��DHT11
	DHT11_Init();
	
	//2.������ʼ|��λ�ź�
	DHT11_Reset();
	
	//3.�ж��豸�Ƿ����:��Ӧ�ź�
	if(DHT11_Check() != 0) {
		return 1;	//������
	}
	
	//4.ѭ����ȡʪ�ȸ�λ,��λ,�¶ȸ�λ,��λ,�Լ�У��λ 40Bit,5�ֽ�
	u8 Data[5] = {0};
	for(int i = 0; i < 5;i++)
	{
		Data[i] = DHT11_Read_Byte();
	}
	
	//5.�ж������Ƿ�����[У������]
	if(Data[0] + Data[1] + Data[2] + Data[3] == Data[4])
	{
		dht11->HumiH = Data[0];	//ʪ�ȸ�λ
		dht11->HumiL = Data[1];	//ʪ�ȵ�λ	
		dht11->TempH = Data[2];	//�¶ȸ�λ
		dht11->TempL = Data[3];	//�¶ȵ�λ	
		dht11->Hum = dht11->HumiH + dht11->HumiL * 0.01;
		dht11->Tem = dht11->TempH + dht11->TempL * 0.01;
		
//		printf("ʪ��:%d.%d \t �¶�:%d.%d \n",Data[0],Data[1],Data[2],Data[3]);
//		printf("ʪ��:%G \t �¶�:%G \n",dht11->Hum,dht11->Tem);
		return 0;	//��ȡ�ɹ�
	}
	
	return 1;	//��ȡʧ��
}
