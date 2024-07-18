#include "oledmenu.h"
#include "key.h"
#include "led.h"
#include "pwm.h"
enum page oled_page;

u32 flag_chooes=0;//����λ�ñ�־
u32 LED1_stat=0;//������״̬��־
//��ҳ����ʾ����
void  OLED_page_show()
{
    OLED_ShowChineseString(30,0,"Զ��ϵͳ",16,1);
    OLED_ShowChineseString(30,16,"����ϵͳ",16,1);
    OLED_ShowChineseString(30,32,"�ƹ�ϵͳ",16,1);
    OLED_ShowChineseString(30,48,"��֪ϵͳ",16,1);
    OLED_ShowChineseString(110,5,"��",16,1);
    OLED_ShowChineseString(110,22,"��",16,1);
    OLED_ShowChineseString(110,37,"��",16,1);
    OLED_ShowChar(10,0,'*',16,1);
    OLED_Refresh();
}
//WiFi��ʾ����
void  OLED_page_show_wifi()
{
    OLED_ShowChineseString(30,0,"����WiFi",16,1);
    OLED_ShowChineseString(30,16,"�Ͽ�WiFi",16,1);
    OLED_ShowChineseString(30,32,"���ӷ�����",16,1);
    OLED_ShowChineseString(30,48,"�Ͽ�������",16,1);
    OLED_ShowChar(10,0,'*',16,1);
    OLED_Refresh();
}
//������ʾ����
void  OLED_page_show_pwm(void)
{
    OLED_ShowChineseString(30,0,"�򿪷���",16,1);
    OLED_ShowChineseString(30,16,"һ������",16,1);
    OLED_ShowChineseString(30,32,"��������",16,1);
    OLED_ShowChineseString(30,48,"��������",16,1);
    OLED_ShowChar(10,0,'*',16,1);
    OLED_Refresh();
}
//�ƹ�����ʾ����
void  OLED_page_show_led(void)
{
    OLED_ShowChineseString(30,0,"�����ƿ�",16,1);
    OLED_ShowChineseString(30,16,"�����ƹ�",16,1);
    OLED_ShowChineseString(30,32,"�򿪵ƶ�",16,1);
    OLED_ShowChineseString(30,48,"�򿪵���",16,1);
    OLED_ShowChar(10,0,'*',16,1);
    OLED_Refresh();
}
//������ʾ����
void  OLED_page_show_per(void)
{
    OLED_ShowChar(10,0,'*',16,1);
    OLED_Refresh();
}

//�����ƶ�
void flag_chooes_up(void)
{
    OLED_My_clear_choose(flag_chooes/16);//�������
    if(flag_chooes==0)
    {

        flag_chooes=48;
    }
    else
    {
        flag_chooes-=16;
    }
    OLED_ShowChar(10,flag_chooes,'*',16,1);
    OLED_Refresh();
}

//�����ƶ�
void flag_chooes_down(void)
{
    OLED_My_clear_choose(flag_chooes/16);//�������
    if(flag_chooes==48)
    {
        flag_chooes=0;
    }
    else
    {
        flag_chooes+=16;
    }
    OLED_ShowChar(10,flag_chooes,'*',16,1);
    OLED_Refresh();
}


//����ѡ���ҳ��
void flag_chooes_page(void)
{
    if(flag_chooes==0)
    {
        flag_chooes=0;
        OLED_MyClear();//���ҳ��
        oled_page=page_wifi;//�л����к���
        OLED_page_show_wifi();//��ʾWiFiҳ��
    }
    else if(flag_chooes==16)
    {
        flag_chooes=0;
        OLED_MyClear();//���ҳ��
        oled_page=page_pwm;//�л����к���
        OLED_page_show_pwm();//��ʾ����ҳ��
    }
    else if(flag_chooes==32)
    {
        flag_chooes=0;
        OLED_MyClear();//���ҳ��
        oled_page=page_led;//�л����к���
        OLED_page_show_led();//��ʾledҳ��
    }
    else if(flag_chooes==48)
    {
        flag_chooes=0;
        OLED_MyClear();//���ҳ��
        oled_page=page_Perce;//�л����к���
        OLED_page_show_per();//��ʾ��֪ҳ��
    }
}



//��ҳ�����к���
void OLED_page_main(void)
{
    if(KEY1==0)
    {
        //�����ƶ�����ˢ��
        flag_chooes_up();
        delay_ms(40);
    }
    if(KEY2==0)
    {
        delay_ms(40);
        //�����ƶ�����ˢ��
        flag_chooes_down();
        delay_ms(40);
        delay_ms(40);

    }
    if(KEY3==0)
    {
        delay_ms(40);
        //����ѡ���ҳ��
        flag_chooes_page();
        delay_ms(40);

    }

}
//������ҳ����
void OLED_page_main_return(void)
{
    flag_chooes=0;
    OLED_MyClear();//���ҳ��
    oled_page=page_main;//�л����к���
    OLED_page_show();//��ʾ��֪ҳ��
}





//����124��װ
void OLED_KEY(void)
{
    if(KEY1==0)
    {
		delay_ms(40);
        //�����ƶ�����ˢ��
        flag_chooes_up();
        delay_ms(40);
    }
    if(KEY2==0)
    {
        delay_ms(40);
        //�����ƶ�����ˢ��
        flag_chooes_down();
        delay_ms(40);
    }
    if(KEY4==0)
    {
        delay_ms(40);
        OLED_page_main_return();//�л�����ҳ
        delay_ms(40);
    }
}




//WiFi���к���
void OLED_page_wifi(void)
{
    //����124�Ĺ��ܷ�װ
    OLED_KEY();
}


//�������к���
void OLED_page_pwm(void)
{
	    if(KEY3==0)
    {
		 delay_ms(100);
        switch(flag_chooes)
        {
        case 0:
			if (TIM_GetCapture2(TIM12)==0)
            {
                pwm_start1();
				//��������ѡ������
				OLED_My_clear_cinese(flag_chooes/16);
				OLED_ShowChineseString(30,0,"�رշ���",16,0);
				OLED_Refresh();
            }
            else
            {
                pwm_stop();
				//��������ѡ������
				OLED_My_clear_cinese(flag_chooes/16);
				OLED_ShowChineseString(30,0,"�򿪷���",16,1);
				OLED_Refresh();
            }
            break;
        case 16:
			pwm_start1();
            break;
        case 32:
			pwm_start2();
            break;
        case 48:
			pwm_start3();
            break;
        default:
            break;
        }
		 delay_ms(100);
    }
    //����124�Ĺ��ܷ�װ
    OLED_KEY();
}


//�ƹ����к���
void OLED_page_led(void)
{
    if(KEY3==0)
    {
		 delay_ms(100);
        switch(flag_chooes)
        {
        case 0:
			LED1_stat=1;
            break;
        case 16:
			LED1_stat=0;
            break;
        case 32:
            if (LED2 == 0)
            {
                LED2 = 1;
				//��������ѡ������
				OLED_My_clear_cinese(flag_chooes/16);
				OLED_ShowChineseString(30,32,"�򿪵ƶ�",16,1);
				OLED_Refresh();
            }
            else
            {
                LED2 = 0;
				//��������ѡ������
				OLED_My_clear_cinese(flag_chooes/16);
				OLED_ShowChineseString(30,32,"�رյƶ�",16,1);
				OLED_Refresh();
            }
            break;
        case 48:
			if (LED3 == 0)
            {
                LED3 = 1;
				//��������ѡ������
				OLED_My_clear_cinese(flag_chooes/16);
				OLED_ShowChineseString(30,48,"�򿪵���",16,1);
				OLED_Refresh();
            }
            else
            {
                LED3 = 0;
				//��������ѡ������
				OLED_My_clear_cinese(flag_chooes/16);
				OLED_ShowChineseString(30,48,"�رյ���",16,1);
				OLED_Refresh();
            }
            break;
        default:
            break;
        }
		 delay_ms(100);
    }
    //����124�Ĺ��ܷ�װ
    OLED_KEY();
}
//�������к���
void OLED_page_Per(void)
{
    if(KEY4==0)
    {
        delay_ms(40);
        OLED_page_main_return();//�л�����ҳ
        delay_ms(40);
    }
}








