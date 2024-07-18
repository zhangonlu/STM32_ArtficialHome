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

    //1.ʱ��ʹ�� :IO�� �� UART
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	ring_buffer_Init(&buffer);
    //2.����������Ϊ���ù���
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);

    //GPIOA��ʼ��
    GPIO_InitTypeDef gpio_initstruct;
    gpio_initstruct.GPIO_Mode = GPIO_Mode_AF;
    gpio_initstruct.GPIO_Pin =   GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_Init(GPIOD, &gpio_initstruct);

	//USART2��ʼ��
    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate =  bound;   //���ò�����
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None ;  //�����ƣ���
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;    //ģʽ ��|��
    USART_InitStruct.USART_Parity = USART_Parity_No;                  //��У��λ
    USART_InitStruct.USART_StopBits =    USART_StopBits_1;          //1ֹͣλ
    USART_InitStruct.USART_WordLength =  USART_WordLength_8b;      //8����λ
    USART_Init(USART2,&USART_InitStruct);

	//ʹ�ܴ���1
    USART_Cmd(USART2, ENABLE);

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//���ý����ж�
    NVIC_InitTypeDef  NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;  //����1�ж�ͨ��
    NVIC_InitStruct.NVIC_IRQChannelCmd =   ENABLE;    //�ж�ͨ��ʹ��
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;     //��ռ���ȼ�
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;            //��Ӧ���ȼ�
    NVIC_Init(&NVIC_InitStruct);

}


#include "led.h"
void USART2_IRQHandler()
{
     static	u8 ret,old;
	//�жϻ������Ƿ�Ϊ��
    if( USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
    {
		//��������
         ret = USART_ReceiveData(USART2);
        
		USART_SendData(USART1, ret);
			
		ring_buffer_write(&buffer,ret);
		
		if(old == '\r' && ret == '\n')                                                              
			buffer.flag = true;	
		old = ret;
		
		 //��ձ�־λ
        USART_ClearITPendingBit(USART2,USART_IT_RXNE)  ;
    }
}

u8 USART2_SendData(u8 *data,u8 size)
{
   for(int i = 0;i < size;i++)
	{
		while((USART2->SR >> 6 & 0x1) == 0 );//���ȷ������
		
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
			/*---- �ж����� --------*/
			int len = 0;	char buf[200] = "";
			len = ring_buffer_read_all(&buffer,(u8 *)buf,sizeof(buf));
			if(strstr(buf,OKStr) != NULL) return 0;
		}
	}
	return -1;
}
/*********************************************************
����:int WIFI_SendData(char *data,u32 timer)
����:
	@data:Ҫ���͵�����(�ַ���)
	@timer:��ʱʱ��,����,һ��1000ms,ǰ��:��Ҫ�����OK����ֵ
����:����ATָ��
����:
	�ɹ�:0
	ʧ��:!0
**********************************************************/
int wifi_sendData(char *data,u32 timer)
{
		USART2_SendData((u8*)data,strlen(data));
	/*---- ��ȡ���λ��������ݲ����� ----*/
	return WIFI_Wait_OK("OK",timer);
}

void wifi_init()
{
	//1����ģʽΪSPA
	wifi_sendData("AT+CWMODE=2\r\n",1000);
	wifi_sendData("AT+CWSAP=\"zhang\",\"12345678\",5,3\r\n",5000);


}

void TCP_Init()
{
	
		/*----- 1.���ö����ӷ�ʽ ---*/
	wifi_sendData("AT+CIPMUX=1\r\n",5000);

		/*----- 1.����TCP������ ----*/                                                                  
	wifi_sendData("AT+CIPSERVER=1,10000\r\n",5000);		//ip��ַλ 192.168.4.1 �˿�Ϊ 10000
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
	/*---- ��ȡ���λ��������ݲ����� ----*/
	return WIFI_Wait_OK("SEND OK",timer);	
}

//�ض���fputc����
int fputc(int ch, FILE *f)
{
 while((USART1->SR & 0x40) == 0); //ѭ�����ȷ�����ɡ�
 USART1->DR = (u8)ch;
 return ch;
}




