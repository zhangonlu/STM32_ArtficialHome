#ifndef __OLEDMENU_H__
#define __OLEDMENU_H__
#include "oled.h"
extern enum page oled_page;
/*-------------------------------------
 * 创建一个枚举
 * 存放界面变量
---------------------------------------*/
extern u32 LED1_stat;
enum page
{
    page_main,   //主界面
    page_wifi,   //远程系统
    page_pwm,    //风扇系统
    page_led,    //灯光系统
    page_Perce,  //感知系统
    page_door    //门禁系统

};
/*------------- 函数声明 --------------*/
void OLED_page_show(void);
void OLED_page_main(void);
void OLED_page_wifi(void);
void OLED_page_pwm(void);
void OLED_page_led(void);
void OLED_page_Per(void);
#endif

