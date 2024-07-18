#include "delay.h"

/*---------------------
����ԭ��:void delay_init()
����: ʹ�ö�ʱ��������ʱ
����: ��
����ֵ: ��
-----------------------*/


//TIM7������APB1���� ʱ��Ƶ��Ϊ84MHZ
void TIM7_Init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);

    //ʱ�ӳ�ʼ��
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;      //1��Ƶ
    TIM_TimeBaseInitStruct.TIM_CounterMode =  TIM_CounterMode_Up; //���ϼ���
    TIM_TimeBaseInitStruct.TIM_Period =  0xFFFF -1;         //�Զ���װ��ֵ
    TIM_TimeBaseInitStruct.TIM_Prescaler =  84-1;    //Ԥ����ֵ
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter  = 0; //�߼���ʱ���ſ�
	
    TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStruct);

    //ʹ�ܿ�����ʱ��
    TIM_Cmd(TIM7,ENABLE);
}

void delay_us(u32 us)
{
    u32 start_time = TIM7->CNT;
    u32 end_time = start_time + us;

    if(end_time > (0xffff-1))
    {
        while(TIM7->CNT < (0xffff - 1));
        end_time -= 0xffff - 1;
    }
	while(TIM7->CNT < end_time );

}

void delay_ms(u32 ms)
{
    while(ms--)
    {
        delay_us(1000);
    }

}
