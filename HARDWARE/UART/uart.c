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

    //1.ʱ��ʹ�� :IO�� �� UART
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    //2.����������Ϊ���ù���
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

    //GPIOA��ʼ��
    GPIO_InitTypeDef gpio_initstruct;
    gpio_initstruct.GPIO_Mode = GPIO_Mode_AF;
    gpio_initstruct.GPIO_Pin =   GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_Init(GPIOA, &gpio_initstruct);

	//USART1��ʼ��
    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate =  bound;   //���ò�����
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None ;  //�����ƣ���
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;    //ģʽ ��|��
    USART_InitStruct.USART_Parity = USART_Parity_No;                  //��У��λ
    USART_InitStruct.USART_StopBits =    USART_StopBits_1;          //1ֹͣλ
    USART_InitStruct.USART_WordLength =  USART_WordLength_8b;      //8����λ
    USART_Init(USART1,&USART_InitStruct);

	//ʹ�ܴ���1
    USART_Cmd(USART1, ENABLE);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//���ý����ж�
    NVIC_InitTypeDef  NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;  //����1�ж�ͨ��
    NVIC_InitStruct.NVIC_IRQChannelCmd =   ENABLE;    //�ж�ͨ��ʹ��
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;     //��ռ���ȼ�
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;            //��Ӧ���ȼ�
    NVIC_Init(&NVIC_InitStruct);

}




#include "led.h"
void USART1_IRQHandler()
{
     static	u8 ret;
	//�жϻ������Ƿ�Ϊ��
    if( USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
		//��������
         ret = USART_ReceiveData(USART1);
        
		 USART_SendData(USART2, ret);
    }


}





