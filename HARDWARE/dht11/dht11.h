#ifndef __DHT11_H__
#define __DHT11_H__
#include "sys.h"
typedef struct {
	u8 HumiH;	//ʪ�ȸ�λ
	u8 HumiL;	//ʪ�ȵ�λ
	u8 TempH;	//�¶ȸ�λ
	u8 TempL;	//�¶ȵ�λ
	double Hum;	//ʪ��
	double Tem;	//�¶�
}DHT11_t;

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
void DHT11_Init(void);
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
u8 DHT11_Read_Data(DHT11_t *dht11);
void DHT11_Init(void);

#endif
