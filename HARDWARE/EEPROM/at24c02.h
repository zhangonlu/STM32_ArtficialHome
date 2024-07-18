#ifndef __AT24C02_H__
#define __AT24C02_H__
#include "iic.h"
#define AT24C02 0xA0		//���豸��ַ
/*******************************************************
����:void AT24C02_Write(u8 addr,u8 *data,u8 len)
����:IICд�����ݵ�AT24C02
����:
	@addr:�豸д�����ڴ��ַ
	@data:���ݵ�ַ
	@len:���ݳ���
����ֵ:��
˵��:
	���ѭ����������,�����ֹͣ�ź�,ֱ������,�����ֹͣ�ź�
	ע��:�������ļ�����,��һ���ļ���дָ��,������ƫ��
********************************************************/
void AT24C02_Write(u8 addr,u8 *data,u8 len);
/*******************************************************
����:void AT24C02_Read(u8 addr,u8 *data,u8 len)
����:IIC��ȡAT24C02����
����:
	@addr:�豸��ȡ���ڴ��ַ
	@data:���ݵ�ַ
	@len:���ݳ���
����ֵ:
˵��:�����ȡ
********************************************************/
void AT24C02_Read(u8 addr,u8 *data,u8 len);
#endif
