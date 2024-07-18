#include "spi.h"
/************************************
函数:void SPI1_Init(void)
功能:初始化 SPI1 总线
参数:无
返回值:无
说明:
	.通过原理图
	SCLK1 时钟线 ------ PB3 
	MISO1 接收线 ------ PB4
	MOSI1 发送线 ------ PB5
	.通过芯片手册
	GPIOB ---- AHB1
	SPI1  ---- APB2
**************************************/
void SPI1_Init(void)
{
	/********* 1.使能 GPIOB 和 SPI1 外设时钟总线 *********/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	/********* 2.初始化GPIOB 相关引脚为复用模式 **********/
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;	//引脚:3,4,5
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;						//模式:复用
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_8;	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;						//模式:输出
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	PBout(2) = 1;
	PBout(8) = 1;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;						//模式:输出
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	PEout(1) = 1;
	
	/********* 3.引脚复用成SPI1功能 **********************/
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);
	
	/********* 4.初始化SPI相关参数  ***********************/
	SPI_InitTypeDef SPI_InitStruct;
	
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;	//波特率(分频系数)4
	SPI_InitStruct.SPI_CPHA = 0;									//极性:0	前时钟沿采样,后时钟沿输出
	SPI_InitStruct.SPI_CPOL = 0;									//相位:0	低电平开始采集
	SPI_InitStruct.SPI_CRCPolynomial = 7;							//CRC校验多项式:7级 x1+x2+x3
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;					//数据位:8位
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//方向:全双工
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;					//数据发送方式:高位先发
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;						//模式:主机
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;							//NNS管理:软件
	
	SPI_Init(SPI1, &SPI_InitStruct);
	
	/********** 5.使能SPI1功能 ****************************/
	SPI_Cmd(SPI1, ENABLE);
}

/**********************************************************
函数:u8 SPI1_Write_Read(u8 data)
功能:发送 | 接收 1字节数据
参数:
	@data:数据
返回值:数据
说明: 发送和接收都是同步操作的
	1.发送数据时,可以忽略接收到数据.
	2.接收数据时,可以发送一个空字节 0x00 或 0xFF
***********************************************************/
u8 SPI1_Write_Read(u8 data)
{
	/****** 1.查看SPI1总线是否忙碌,如果忙碌则等待空闲(TXE发送标志位) ******/
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != SET);	//死等发送完成标志
	
	/****** 2.将数据写入SPI1移位寄存器,帮我们发送 ************************/
	SPI_I2S_SendData(SPI1, data);
	
	/****** 3.查看SPI1总线是否有可读数据(RXNE接收标志位) *****************/
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != SET);	//死等接收完成标志
	
	/****** 4.将移位寄存器中的数据读取出来 *******************************/
	return SPI_I2S_ReceiveData(SPI1);
}










