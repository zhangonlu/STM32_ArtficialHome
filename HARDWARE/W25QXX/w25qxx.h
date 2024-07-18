#ifndef __W25QXX_H__
#define __W25QXX_H__
#include "sys.h"

void W25QXX_Init(void);			//��ʼ��W25Q64
u8 W25QXX_Read_Status(void);	//��ȡ�Ĵ���״̬
void W25QXX_Wait_Busy(void);	//�ȴ�æµ����		
void W25QXX_Enable(void);		//дʹ��
void W25QXX_Sector_Erase(u32 addr);					//��������(4K)
void W25QXX_Write_Data(u32 addr,u8 *data,u32 len);	//д����
void W25QXX_Read_Data(u32 addr,u8 *data,u32 len);	//������
uint16_t W25QXX_ReadID(void);						//��ȡID
uint16_t W25QXX_Read_JEDID(void);	//оƬID
#endif
