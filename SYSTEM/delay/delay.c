#include "delay.h"

/*---------------------
函数原型:void delay_init()
功能: 使用定时器进行延时
参数: 无
返回值: 无
-----------------------*/


//TIM7挂载在APB1总线 时钟频率为84MHZ
void TIM7_Init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);

    //时钟初始化
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;      //1分频
    TIM_TimeBaseInitStruct.TIM_CounterMode =  TIM_CounterMode_Up; //向上计数
    TIM_TimeBaseInitStruct.TIM_Period =  0xFFFF -1;         //自动重装载值
    TIM_TimeBaseInitStruct.TIM_Prescaler =  84-1;    //预分配值
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter  = 0; //高级定时器才开
	
    TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStruct);

    //使能开启定时器
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
