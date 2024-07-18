#include "dht11.h"
#include "delay.h"
/*********** 定义一些宏,改变SDA引脚方向 *******/
#define DHT11_Init_IN()		GPIOA->MODER &= ~(0x3 << 6)		//输入模式00
#define DHT11_Init_OUT()	GPIOA->MODER |=  (0x1 << 6)		//输出模式01
/*********** 定义一些宏,位操作 ****************/
#define DHT11_OUT	PAout(3) 	//用于电平输出[高 | 低] 电平	---- 主发,从收 ----
#define DHT11_IN	PAin(3)		//用于电平读取[高 | 低] 电平	---- 主收,从发 ----

/******************************************************
函数:void DHT11_Init(void)
功能:初始化DHT11温湿度模块
参数:无
返回值:无
说明:
	.通过原理图
	DHT11_DATA ---- PA3
	.通过芯片手册
	GPIOA --------- AHB1
********************************************************/
void DHT11_Init(void)
{
	/*********1.使能GPIOA外设时钟 ********/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	/*********2.初始化GPIOA相关引脚 ******/
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;			//引脚:3
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;		//模式:输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;		//输出类型:推挽|开漏
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;		//上下拉:上拉
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;	//输出速度:50MHz
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/******************************************************
函数:void DHT11_Reset(void)
功能:起始信号|复位信号   (主控)
参数:无
返回值:无
******************************************************/
void DHT11_Reset(void)
{
	//1.设置引脚的模式:输出
	DHT11_Init_OUT();
	
	//2.输出低电平[18:30]ms,经典:20ms
	DHT11_OUT = 0;
	delay_ms(20);
	
	//3.输出高电平[10:35]us,经典:13us
	DHT11_OUT = 1;
	delay_us(13);
}

/******************************************************
函数:u8 DHT11_Check(void)
功能:检查响应信号   (从控)
参数:无
返回值:	
	存在:0
	不存在:!0
******************************************************/
u8 DHT11_Check(void)
{
	int timer = 0;
	/******1.设置引脚模式:输入 *******/
	DHT11_Init_IN();
	
	/******2.等待从设备拉低数据线,我们做一个等待100us,否则超时 ****/
	timer = 0;
	while(DHT11_IN == 1)	//死等拉低
	{
		delay_us(1);
		timer++;
		if(timer > 100) return 1;	//不存在
	}
	
	/******3.等待从设备响应(拉高)时间[78:88] us,经典:83us,我只接等待最大时间 ****/
	timer = 0;
	while(DHT11_IN == 0)	//死等拉高
	{
		delay_us(1);
		timer++;
		if(timer > 88) return 1;	//不存在
	}
	
	/******4.等待从设备高电平[80:92]us,经典87us我们直接等待最大时间92us ********/
	delay_us(87);
	return 0;	//存在
}

/******************************************************
函数:u8 DHT11_Read_Bit(void)
功能:读取数据 1bit   (从控)
参数:无
返回值:数据
******************************************************/
u8 DHT11_Read_Bit(void)
{
	int timer = 0;
	/******1.设置引脚模式:输入 *******/
	DHT11_Init_IN();
	
	/******2.死等DHT11拉低,等待100us ***********/
	timer = 0;
	while(DHT11_IN == 1) //死等拉低
	{
		delay_us(1);
		timer++;
		if(timer > 100) return 0;	//超时
	}
	
	/******3.会拉低[50:58]us,经典:54us,然后拉高,等待最大时间58us *****/
	while(DHT11_IN == 0) //死等拉高
	{
		delay_us(1);
		timer++;
		if(timer > 100) return 0;	//超时
	}
	
	/******4.记录高电平持续时间 0值:[23-27]us, 1值:[68-74] 我们选择最长时间74us, *******/
	timer = 0;
	while(DHT11_IN == 1)	//死等拉低
	{
		delay_us(1);
		timer++;
		if(timer > 100)  return 0;	//超时
	}
	/******5.判断高电平持续时间属于哪个范围 0值:[23-27]us, 1值:[68-74]***/
	return timer < 50 ? 0 : 1;
}

/******************************************************
函数:u8 DHT11_Read_Byte(void)
功能:读取数据 1Byte   (从控)
参数:无
返回值:数据
说明:高位先出,每读1位,左移1次
******************************************************/
u8 DHT11_Read_Byte(void)
{
	u8 data = 0;	//存储读取的数据
	/******* 循环8次读取数据 ******/
	for(int i = 0; i < 8; i++)
	{
		data <<= 1;					//先左移1位
		data |= DHT11_Read_Bit();	//读取1bit数据
	}
	return data;
}

/******************************************************************************
函数:u8 DHT11_Read_Data(u8 *HumiH,u8 *HumiL,u8 *TempH,u8 *TempL)
功能:读取DHT11温湿度
参数:
	@HumiH:湿度高位
	@HumiL:湿度低位
	@TempH:温度高位
	@TempL:温度低位
返回值:
	成功:0
	失败:!0
******************************************************************************/
#include "stdio.h"
u8 DHT11_Read_Data(DHT11_t *dht11)
{
	//1.初始化DHT11
	DHT11_Init();
	
	//2.发出起始|复位信号
	DHT11_Reset();
	
	//3.判断设备是否存在:响应信号
	if(DHT11_Check() != 0) {
		return 1;	//不存在
	}
	
	//4.循环读取湿度高位,低位,温度高位,低位,以及校验位 40Bit,5字节
	u8 Data[5] = {0};
	for(int i = 0; i < 5;i++)
	{
		Data[i] = DHT11_Read_Byte();
	}
	
	//5.判断数据是否完整[校验数据]
	if(Data[0] + Data[1] + Data[2] + Data[3] == Data[4])
	{
		dht11->HumiH = Data[0];	//湿度高位
		dht11->HumiL = Data[1];	//湿度低位	
		dht11->TempH = Data[2];	//温度高位
		dht11->TempL = Data[3];	//温度低位	
		dht11->Hum = dht11->HumiH + dht11->HumiL * 0.01;
		dht11->Tem = dht11->TempH + dht11->TempL * 0.01;
		
//		printf("湿度:%d.%d \t 温度:%d.%d \n",Data[0],Data[1],Data[2],Data[3]);
//		printf("湿度:%G \t 温度:%G \n",dht11->Hum,dht11->Tem);
		return 0;	//读取成功
	}
	
	return 1;	//读取失败
}
