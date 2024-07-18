#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
void SPI1_Init(void);			//初始化
u8 SPI1_Write_Read(u8 data);	//收发函数
#endif

