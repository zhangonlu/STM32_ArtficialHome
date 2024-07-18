#ifndef __OLEDMENU_H__
#define __OLEDMENU_H__
#include "oled.h"
extern enum page oled_page;
/*-------------------------------------
 * ����һ��ö��
 * ��Ž������
---------------------------------------*/
extern u32 LED1_stat;
enum page
{
    page_main,   //������
    page_wifi,   //Զ��ϵͳ
    page_pwm,    //����ϵͳ
    page_led,    //�ƹ�ϵͳ
    page_Perce,  //��֪ϵͳ
    page_door    //�Ž�ϵͳ

};
/*------------- �������� --------------*/
void OLED_page_show(void);
void OLED_page_main(void);
void OLED_page_wifi(void);
void OLED_page_pwm(void);
void OLED_page_led(void);
void OLED_page_Per(void);
#endif

