#include "stm32f4xx.h"
#include "FreeRTOSConfig.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "delay.h"
#include "uart.h"
#include "wifi.h"
#include "dht11.h"
#include "string.h"
#include "beep.h"
#include "key.h"
#include "oled.h"
#include "at24c02.h"
#include "oledmenu.h"

typedef struct{
	u8 led1:1;
	u8 led2:1;
	u8 led3:1;
	u8 beep:1;
	u8 fan:2;
}Device_t;

void EEPROM_Save_Device(void)
{
	Device_t device;
	/*---- 写入外部存储 ----*/
	device.led1 = LED1;
	device.led2 = LED2;
	device.led3 = LED3;
	device.beep = BEEP;
	
	AT24C02_Write(0x00,(u8 *)&device,sizeof(device));	//写入数据
}

void EEPROM_Read_Device(void)
{
	Device_t device;
	/*---- 读取EEPROM --------*/
	AT24C02_Read(0x00,(u8 *)&device,sizeof(device));	//读取数据
	/*---- 读取到屏幕显示 ----*/
	LED1 = device.led1; 	OLED_ShowChineseString(40,16,(LED1 == 0 ? "开" : "关"),16,1);	
	LED2 = device.led2;	OLED_ShowChineseString(105,16,(LED2 == 0 ? "开" : "关"),16,1);
	LED3 = device.led3;	OLED_ShowChineseString(40,32,(LED3 == 0 ? "开" : "关"),16,1);	
	BEEP = device.beep;	OLED_ShowChineseString(105,32,(BEEP == 1 ? "开" : "关"),16,1);
	OLED_Refresh();		//写入缓冲区
}




TaskHandle_t StartTask_Handler;//任务句柄
void TaskHandle_START(void *pvParameters);//开始任务任务函数
void TaskHandle_OLED (void *pvParameters);//OLED菜单任务函数
void TaskHandle_WIFI (void *pvParameters);//WIFI通信任务函数
void TaskHandle_THSE (void *pvParameters);//温度湿度任务函数
void TaskHandle_PHOT (void *pvParameters);//光敏电阻任务函数



int main(void)
{ 
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断分组
    LED_Init();
    TIM7_Init();
	BEEP_Init();
    uart1_init();
    DHT11_Init();
    KEY_Init();
    IIC_Init();
    OLED_Init();
	
	uart1_init();
    uart2_init();
	wifi_init();
    TCP_Init();
	//定位到主页面，且初始化
	OLED_page_show();
	oled_page=page_main;
	//创建开始任务
	xTaskCreate( (TaskFunction_t )TaskHandle_START,      //开始任务函数
				 (const char*    )"start",               //任务名称
				 (uint16_t       )256,        			 //任务堆栈大小
				 (void*          )NULL,                  //传递给任务函数的参数
				 (UBaseType_t    )1,      				 //任务优先级
				 (TaskHandle_t*  )&StartTask_Handler );  //任务句柄 
				 
	vTaskStartScheduler();  //开启任务调度	
}




//开始任务任务函数
void TaskHandle_START(void *pvParameters)
{	
	/*------- 进入临界区 -------------*/
	taskENTER_CRITICAL(); 
	
   	xTaskCreate(TaskHandle_OLED ,  "OLED菜单任务",  128,  NULL, 2,  NULL);
	xTaskCreate(TaskHandle_WIFI ,  "WiFi通信任务",  128,  NULL, 2,  NULL);
	xTaskCreate(TaskHandle_THSE ,  "温度湿度任务",  128,  NULL, 2,  NULL);
	xTaskCreate(TaskHandle_PHOT ,  "光敏电阻任务",  128,  NULL, 2,  NULL);

	
	vTaskDelete(StartTask_Handler);  //删除开始任务，调用了任务句柄指针
	
	/*------- 临界区结束 -------------*/
	taskEXIT_CRITICAL(); 
}



//OLED菜单任务函数
void TaskHandle_OLED (void *pvParameters)
{
	u32 lastWakeTime = xTaskGetTickCount();
    while(1)
    {
		vTaskDelayUntil(&lastWakeTime, 10);
		if	( oled_page == page_main )  OLED_page_main();
		else if( oled_page== page_wifi ) OLED_page_wifi();
		else if( oled_page== page_pwm ) OLED_page_pwm();
		else if( oled_page== page_led ) OLED_page_led();
		else if( oled_page== page_Perce ) OLED_page_Per();
		
    }
}

//WIFI通信任务函数
void TaskHandle_WIFI (void *pvParameters)
{
	u32 lastWakeTime = xTaskGetTickCount();
    while(1)
    {
		vTaskDelayUntil(&lastWakeTime, 10);
		if(buffer.flag==true)
		{

			
		}
		
    }
}

//温度湿度任务函数
void TaskHandle_THSE (void *pvParameters)
{
	u32 lastWakeTime = xTaskGetTickCount();
    while(1)
    {
		
		if(LED1_stat==1)
		{
			for(u32 i=0;i<=100;i++)
			{
				vTaskDelay(8);	
				TIM_SetCompare1(TIM1,i);
			}
			for(u32 i=0;i<=100;i++)
			{
				vTaskDelay(8);	
				TIM_SetCompare1(TIM1,100-i);
			}
		}
		else
		{
		TIM_SetCompare1(TIM1,0);
		}
		vTaskDelay(100);	
    }
}
	


//光敏电阻任务函数
void TaskHandle_PHOT (void *pvParameters)
{
	u32 lastWakeTime = xTaskGetTickCount();
    while(1)
    {	
		vTaskDelayUntil(&lastWakeTime, 10);	
    }
}



//门禁系统任务函数
void TaskHandle_door (void *pvParameters)
{
	u32 lastWakeTime = xTaskGetTickCount();
    while(1)
    {	
		vTaskDelayUntil(&lastWakeTime, 10);	
    }
}


