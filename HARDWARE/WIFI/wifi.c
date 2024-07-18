#include "wifi.h"

#include "uart.h"
#include "ringbuffer.h"
/*
PD5 -- uart2_tx
PD6 -- uart2_rx

GPIOA -- AHB1
USART2 -- APB1
*/

Ringbuffer_t buffer;
void uart2_init()
{

    //1.时钟使能 :IO口 和 UART
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	ring_buffer_Init(&buffer);
    //2.将引脚配置为复用功能
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);

    //GPIOA初始化
    GPIO_InitTypeDef gpio_initstruct;
    gpio_initstruct.GPIO_Mode = GPIO_Mode_AF;
    gpio_initstruct.GPIO_Pin =   GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_Init(GPIOD, &gpio_initstruct);

	//USART2初始化
    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate =  bound;   //设置波特率
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None ;  //流控制：无
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;    //模式 收|发
    USART_InitStruct.USART_Parity = USART_Parity_No;                  //无校验位
    USART_InitStruct.USART_StopBits =    USART_StopBits_1;          //1停止位
    USART_InitStruct.USART_WordLength =  USART_WordLength_8b;      //8数据位
    USART_Init(USART2,&USART_InitStruct);

	//使能串口1
    USART_Cmd(USART2, ENABLE);

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//启用接收中断
    NVIC_InitTypeDef  NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;  //串口1中断通道
    NVIC_InitStruct.NVIC_IRQChannelCmd =   ENABLE;    //中断通道使能
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;     //抢占优先级
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;            //响应优先级
    NVIC_Init(&NVIC_InitStruct);

}


#include "led.h"
void USART2_IRQHandler()
{
     static	u8 ret,old;
	//判断缓冲区是否为空
    if( USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
    {
		//接收数据
         ret = USART_ReceiveData(USART2);
        
		USART_SendData(USART1, ret);
			
		ring_buffer_write(&buffer,ret);
		
		if(old == '\r' && ret == '\n')                                                              
			buffer.flag = true;	
		old = ret;
		
		 //清空标志位
        USART_ClearITPendingBit(USART2,USART_IT_RXNE)  ;
    }
}

u8 USART2_SendData(u8 *data,u8 size)
{
   for(int i = 0;i < size;i++)
	{
		while((USART2->SR >> 6 & 0x1) == 0 );//死等发送完成
		
		USART2->DR = data[i];
	
	}
	
	return size;
}

#include "string.h"
#include "delay.h"


int WIFI_Wait_OK(char *OKStr,u32 timer)
{
	while(timer > 0)
	{
		timer--;
		delay_ms(1);
		if(buffer.flag == true)
		{
			/*---- 判断数据 --------*/
			int len = 0;	char buf[200] = "";
			len = ring_buffer_read_all(&buffer,(u8 *)buf,sizeof(buf));
			if(strstr(buf,OKStr) != NULL) return 0;
		}
	}
	return -1;
}
/*********************************************************
函数:int WIFI_SendData(char *data,u32 timer)
参数:
	@data:要发送的数据(字符串)
	@timer:超时时间,毫秒,一般1000ms,前提:需要测带有OK返回值
功能:发送AT指令
返回:
	成功:0
	失败:!0
**********************************************************/
int wifi_sendData(char *data,u32 timer)
{
		USART2_SendData((u8*)data,strlen(data));
	/*---- 读取环形缓冲区数据并处理 ----*/
	return WIFI_Wait_OK("OK",timer);
}

void wifi_init()
{
	//1设置模式为SPA
	wifi_sendData("AT+CWMODE=2\r\n",1000);
	wifi_sendData("AT+CWSAP=\"zhang\",\"12345678\",5,3\r\n",5000);


}

void TCP_Init()
{
	
		/*----- 1.设置多连接方式 ---*/
	wifi_sendData("AT+CIPMUX=1\r\n",5000);

		/*----- 1.开启TCP服务器 ----*/                                                                  
	wifi_sendData("AT+CIPSERVER=1,10000\r\n",5000);		//ip地址位 192.168.4.1 端口为 10000
}




#include "dht11.h"
#include "stdio.h"
u8 TCP_send(u8 *data,u32 size,u32 timer)
{
		char buf[200] = "AT\r\n";
	WIFI_Wait_OK("OK",50);
	sprintf(buf,"AT+CIPSEND=0,%d\r\n",size);
	if(wifi_sendData(buf,timer) != 0) return -1;
	USART2_SendData((u8*)data,size);
	/*---- 读取环形缓冲区数据并处理 ----*/
	return WIFI_Wait_OK("SEND OK",timer);	
}

//重定义fputc函数
int fputc(int ch, FILE *f)
{
 while((USART1->SR & 0x40) == 0); //循环死等发送完成。
 USART1->DR = (u8)ch;
 return ch;
}




