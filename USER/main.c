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
	/*---- д���ⲿ�洢 ----*/
	device.led1 = LED1;
	device.led2 = LED2;
	device.led3 = LED3;
	device.beep = BEEP;
	
	AT24C02_Write(0x00,(u8 *)&device,sizeof(device));	//д������
}

void EEPROM_Read_Device(void)
{
	Device_t device;
	/*---- ��ȡEEPROM --------*/
	AT24C02_Read(0x00,(u8 *)&device,sizeof(device));	//��ȡ����
	/*---- ��ȡ����Ļ��ʾ ----*/
	LED1 = device.led1; 	OLED_ShowChineseString(40,16,(LED1 == 0 ? "��" : "��"),16,1);	
	LED2 = device.led2;	OLED_ShowChineseString(105,16,(LED2 == 0 ? "��" : "��"),16,1);
	LED3 = device.led3;	OLED_ShowChineseString(40,32,(LED3 == 0 ? "��" : "��"),16,1);	
	BEEP = device.beep;	OLED_ShowChineseString(105,32,(BEEP == 1 ? "��" : "��"),16,1);
	OLED_Refresh();		//д�뻺����
}




TaskHandle_t StartTask_Handler;//������
void TaskHandle_START(void *pvParameters);//��ʼ����������
void TaskHandle_OLED (void *pvParameters);//OLED�˵�������
void TaskHandle_WIFI (void *pvParameters);//WIFIͨ��������
void TaskHandle_THSE (void *pvParameters);//�¶�ʪ��������
void TaskHandle_PHOT (void *pvParameters);//��������������



int main(void)
{ 
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�жϷ���
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
	//��λ����ҳ�棬�ҳ�ʼ��
	OLED_page_show();
	oled_page=page_main;
	//������ʼ����
	xTaskCreate( (TaskFunction_t )TaskHandle_START,      //��ʼ������
				 (const char*    )"start",               //��������
				 (uint16_t       )256,        			 //�����ջ��С
				 (void*          )NULL,                  //���ݸ��������Ĳ���
				 (UBaseType_t    )1,      				 //�������ȼ�
				 (TaskHandle_t*  )&StartTask_Handler );  //������ 
				 
	vTaskStartScheduler();  //�����������	
}




//��ʼ����������
void TaskHandle_START(void *pvParameters)
{	
	/*------- �����ٽ��� -------------*/
	taskENTER_CRITICAL(); 
	
   	xTaskCreate(TaskHandle_OLED ,  "OLED�˵�����",  128,  NULL, 2,  NULL);
	xTaskCreate(TaskHandle_WIFI ,  "WiFiͨ������",  128,  NULL, 2,  NULL);
	xTaskCreate(TaskHandle_THSE ,  "�¶�ʪ������",  128,  NULL, 2,  NULL);
	xTaskCreate(TaskHandle_PHOT ,  "������������",  128,  NULL, 2,  NULL);

	
	vTaskDelete(StartTask_Handler);  //ɾ����ʼ���񣬵�����������ָ��
	
	/*------- �ٽ������� -------------*/
	taskEXIT_CRITICAL(); 
}



//OLED�˵�������
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

//WIFIͨ��������
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

//�¶�ʪ��������
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
	


//��������������
void TaskHandle_PHOT (void *pvParameters)
{
	u32 lastWakeTime = xTaskGetTickCount();
    while(1)
    {	
		vTaskDelayUntil(&lastWakeTime, 10);	
    }
}



//�Ž�ϵͳ������
void TaskHandle_door (void *pvParameters)
{
	u32 lastWakeTime = xTaskGetTickCount();
    while(1)
    {	
		vTaskDelayUntil(&lastWakeTime, 10);	
    }
}


