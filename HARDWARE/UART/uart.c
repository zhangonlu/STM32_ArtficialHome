#include "uart.h"
#include "ringbuffer.h"
/*
PA9 -- uart1_tx
PA10 -- uart1_rx

GPIOA -- AHB1
USART1 -- APB2
*/

void uart1_init()
{

    //1.时钟使能 :IO口 和 UART
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    //2.将引脚配置为复用功能
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

    //GPIOA初始化
    GPIO_InitTypeDef gpio_initstruct;
    gpio_initstruct.GPIO_Mode = GPIO_Mode_AF;
    gpio_initstruct.GPIO_Pin =   GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_Init(GPIOA, &gpio_initstruct);

	//USART1初始化
    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate =  bound;   //设置波特率
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None ;  //流控制：无
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;    //模式 收|发
    USART_InitStruct.USART_Parity = USART_Parity_No;                  //无校验位
    USART_InitStruct.USART_StopBits =    USART_StopBits_1;          //1停止位
    USART_InitStruct.USART_WordLength =  USART_WordLength_8b;      //8数据位
    USART_Init(USART1,&USART_InitStruct);

	//使能串口1
    USART_Cmd(USART1, ENABLE);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//启用接收中断
    NVIC_InitTypeDef  NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;  //串口1中断通道
    NVIC_InitStruct.NVIC_IRQChannelCmd =   ENABLE;    //中断通道使能
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;     //抢占优先级
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;            //响应优先级
    NVIC_Init(&NVIC_InitStruct);

}




#include "led.h"
void USART1_IRQHandler()
{
     static	u8 ret;
	//判断缓冲区是否为空
    if( USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
		//接收数据
         ret = USART_ReceiveData(USART1);
        
		 USART_SendData(USART2, ret);
    }


}





