#include "spi.h"
#include "w25qxx.h"
/****** 相关位段操作宏 ******/
#define W25QXX_CS PBout(2)		//用于片选
/*************************************************
函数名:void W25QXX_Init(void)
功能:初始化W25Q64
参数:无
返回值:无
说明:
	W25Q64_CS ---- PB.2	片选引脚
	GPIOB挂载在RCC的AHB1总线
**************************************************/
void W25QXX_Init(void)
{
	/*----- 0.结构体变量定义 ------------------*/
	GPIO_InitTypeDef GPIO_InitStruct;
	/*----- 1.使能 RCC_AHB1外设总线的 GPIOB ---*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	/*----- 2.初始化GPIO引脚 ------------------*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;					//模式:通用输出
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;					//类型:推挽输出
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;						//引脚:2
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;					//上下拉:上拉
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;				//速度:100Mhz
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);		//注意:只能操作同一组
	/*----- 3.默认电平(高)	-------------------*/
	W25QXX_CS = 1;
}

/*************************************************
函数名:u8 W25Q64_Read_Status_Register(void)
功能:读取状态寄存器 W25Q64(S0位)
参数:无
返回值:
	空闲:0
	非闲:1
说明:	功能码:05h
	BIT7  6   5   4   3   2   1   0
	SPR   RV  TB BP2 BP1 BP0 WEL BUSY
	SPR:默认0,状态寄存器保护位,配合WP使用
*************************************************/
u8 W25QXX_Read_Status(void)
{
	u8 status = 0;
	/*----- 1.将片选线拉低 ------*/
	W25QXX_CS = 0;
	/*----- 2.发送读取功能码 ----*/
	SPI1_Write_Read(0x05);
	/*----- 3.读取寄存器值 ------*/
	status = SPI1_Write_Read(0xFF);	//读取读状态寄存器的值
	/*----- 4.将片选线拉高 ------*/
	W25QXX_CS = 1;
	/*----- 5.返回状态值 --------*/
	return status;
}

/*************************************************
函数名:u8 W25Q64_Wait_Busy(void)
功能:等待写入结束
BIT7 == 0000 0001 空闲 0000 0000
0x01 == 0000 0001	   0000 0001
**************************************************/
void W25QXX_Wait_Busy(void)
{
	while((W25QXX_Read_Status() & 0x01) == 0x01);	//死等空闲 
}

/*************************************************
函数名:void W25Q64_Write_Enable(void)
功能:写使能 W25Q64
参数:无
返回值:无
说明:	
功能码:06h
**************************************************/
void W25QXX_Enable(void)
{
	/*----- 1.将片选线拉低 ------*/
	W25QXX_CS = 0;
	/*----- 2.发送读取功能码 ----*/
	SPI1_Write_Read(0x06);
	/*----- 3.将片选线拉高 ------*/
	W25QXX_CS = 1;
}
/*************************************************
函数名:void W25Q64_Sector_Erase(u32 addr)
功能:扇区擦除(4K) W25Q64
参数:
	@addr:要擦除的首地址
返回值:无
说明:	功能码:20h
	擦除扇区需要计算扇区首地址
**************************************************/
void W25QXX_Sector_Erase(u32 addr)
{
	/*----- 0.计算扇区首地址 ----*/
	addr = addr - (addr % 4096);
	/*----- 0.等待之前操作完成---*/
	W25QXX_Wait_Busy();
	/*----- 1.写使能 ------------*/
	W25QXX_Enable();
	/*----- 2.将片选线拉低 ------*/
	W25QXX_CS = 0;
	/*----- 3.发送读取功能码 ----*/
	SPI1_Write_Read(0x20);
	/*----- 4.发送扇区地址 ----*/
	SPI1_Write_Read(addr >> 16);	//A23-A16
	SPI1_Write_Read(addr >> 8);		//A15-A8
	SPI1_Write_Read(addr >> 0);		//A7-A0
	/*----- 5.将片选线拉高 ------*/
	W25QXX_CS = 1;
	/*----- 6.等待擦除完成 ------*/
	W25QXX_Wait_Busy();
}

/************************************************
函数:void W25Q64_Write_Data(u32 addr, u8 *data, u32 len)
功能:页编程 发送数据 = 02h
参数:
	@addr：地址
	@data：要写入的数据
	@len：要写入的长度
返回:成功写入的数据长度
*************************************************/
void W25QXX_Write_Data(u32 addr,u8 *data,u32 len)
{
	/*----- 0.等待之前操作完成---*/
	W25QXX_Wait_Busy();
	/*----- 1.写使能 ------------*/
	W25QXX_Enable();
	/*----- 2.将片选线拉低 ------*/
	W25QXX_CS = 0;
	/*----- 3.发送功能码 --------*/
	SPI1_Write_Read(0x02);
	/*----- 4.发送要写的地址 ----*/
	SPI1_Write_Read(addr >> 16);	//A23-A16
	SPI1_Write_Read(addr >> 8);		//A15-A8
	SPI1_Write_Read(addr >> 0);		//A7-A0
	
	/*----- 5.循环发送出数据 ----*/
	for(int i = 0; i < len && i < 256;i++)
	{
		SPI1_Write_Read(data[i]);
	}
	
	/*----- 6.片选线拉高 --------*/
	W25QXX_CS = 1;
	/*----- 7.等待写入完成 ------*/
	W25QXX_Wait_Busy();
}

/******************************************************
函数:void W25Q64_Read_Data(u32 addr, u8 *data, u32 len)
功能:读数据 = 03h
参数:
	@addr：地址
	@data：读取后数据存放的地址
	@len：要读的长度
返回:成功读取的数据长度
*******************************************************/
void W25QXX_Read_Data(u32 addr,u8 *data,u32 len)
{
	/*----- 1.将片选线拉低 ------*/
	W25QXX_CS = 0;
	/*----- 2.发送功能码 --------*/
	SPI1_Write_Read(0x03);
	/*----- 4.发送要读的地址 ----*/
	SPI1_Write_Read(addr >> 16);	//A23-A16
	SPI1_Write_Read(addr >> 8);		//A15-A8
	SPI1_Write_Read(addr >> 0);		//A7-A0
	
	/*------ 5.循环读取数据 -----*/
	for(int i = 0; i < len;i++)
	{
		data[i] = SPI1_Write_Read(0xFF);
	}
	
	/*------ 6.将片选线拉高 -----*/
	W25QXX_CS = 1;
}

uint16_t W25QXX_ReadID(void)	//厂商ID
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

uint16_t W25QXX_Read_JEDID(void)	//芯片ID
{
	uint16_t w25qxxID=0;

	W25QXX_CS = 0;
	SPI1_Write_Read(0x9F);
	SPI1_Write_Read(0xff);	//名称:
	
	w25qxxID |= SPI1_Write_Read(0xff);
	w25qxxID <<= 8;
	w25qxxID |= SPI1_Write_Read(0xff);
	W25QXX_CS = 1;
	
	return w25qxxID;
}

