#ifndef __W25QXX_H__
#define __W25QXX_H__
#include "sys.h"

void W25QXX_Init(void);			//初始化W25Q64
u8 W25QXX_Read_Status(void);	//读取寄存器状态
void W25QXX_Wait_Busy(void);	//等待忙碌结束		
void W25QXX_Enable(void);		//写使能
void W25QXX_Sector_Erase(u32 addr);					//扇区擦除(4K)
void W25QXX_Write_Data(u32 addr,u8 *data,u32 len);	//写数据
void W25QXX_Read_Data(u32 addr,u8 *data,u32 len);	//读数据
uint16_t W25QXX_ReadID(void);						//读取ID
uint16_t W25QXX_Read_JEDID(void);	//芯片ID
#endif
