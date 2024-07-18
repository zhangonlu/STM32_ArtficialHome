#include "at24c02.h"
#include "delay.h"
/*******************************************************
函数:void AT24C02_Write(u8 addr,u8 *data,u8 len)
功能:IIC写入数据到AT24C02
参数:
	@addr:设备写入首内存地址
	@data:数据地址
	@len:数据长度
返回值:无
说明:
	如果循环发送数据,则产生停止信号,直到发完,则产生停止信号
	注意:理解像文件操作,有一个文件读写指针,会往后偏移
	0x00 - 0x07		0x10 - 0x17
	0x08 - 0x0f		0x18 - 0x1f
	
	案例：向 0x05 写入35字节数据 
	1.将当前地址页剩余空间写入。
	2.将整页空间数据写入。
	3.将剩余字节写入下一个空间。
	
	123 12345678 12345678 12345678 9
********************************************************/
void AT24C02_Write(u8 addr, u8 *data, u8 len)
{
//	IIC_Send_Data(AT24C02, addr, data, len);   // 写入数据
//	delay_ms(10);
   /*--------- 发送当前页剩余的字节数据 --------------------------*/
	u8 offset = addr % 8;			//计算是否是页地址
	if(offset != 0){
		 offset = 8 - offset;						   //当前页剩余空间	123
		 IIC_Send_Data(AT24C02, addr, data, offset);   // 写入数据
		 len -= offset;    addr += offset;        data += offset;
		 delay_ms(10);
	}
	
	/*--------- 发送完整页字节的数据 ------------------------------*/
	offset = 8;
	while(len / 8 > 0)
	{
		IIC_Send_Data(AT24C02, addr, data, offset);   // 写入数据
		len -= offset;    addr += offset;        data += offset;
		delay_ms(10);
	}
	
	/*--------- 发送剩余字节的数据 --------------------------------*/
	IIC_Send_Data(AT24C02, addr, data, len);   // 写入数据
}


/*******************************************************
函数:void AT24C02_Read(u8 addr,u8 *data,u8 len)
功能:IIC读取AT24C02数据
参数:
	@addr:设备读取首内存地址
	@data:数据地址
	@len:数据长度
返回值:
说明:随机读取
********************************************************/
void AT24C02_Read(u8 addr,u8 *data,u8 len)
{
	IIC_Read_Data(AT24C02,addr,data,len);
}


