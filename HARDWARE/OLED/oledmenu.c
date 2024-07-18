#include "oledmenu.h"
#include "key.h"
#include "led.h"
#include "pwm.h"
enum page oled_page;

u32 flag_chooes=0;//按键位置标志
u32 LED1_stat=0;//呼吸灯状态标志
//主页面显示函数
void  OLED_page_show()
{
    OLED_ShowChineseString(30,0,"远程系统",16,1);
    OLED_ShowChineseString(30,16,"风扇系统",16,1);
    OLED_ShowChineseString(30,32,"灯光系统",16,1);
    OLED_ShowChineseString(30,48,"感知系统",16,1);
    OLED_ShowChineseString(110,5,"主",16,1);
    OLED_ShowChineseString(110,22,"菜",16,1);
    OLED_ShowChineseString(110,37,"单",16,1);
    OLED_ShowChar(10,0,'*',16,1);
    OLED_Refresh();
}
//WiFi显示函数
void  OLED_page_show_wifi()
{
    OLED_ShowChineseString(30,0,"连接WiFi",16,1);
    OLED_ShowChineseString(30,16,"断开WiFi",16,1);
    OLED_ShowChineseString(30,32,"连接服务器",16,1);
    OLED_ShowChineseString(30,48,"断开服务器",16,1);
    OLED_ShowChar(10,0,'*',16,1);
    OLED_Refresh();
}
//风扇显示函数
void  OLED_page_show_pwm(void)
{
    OLED_ShowChineseString(30,0,"打开风扇",16,1);
    OLED_ShowChineseString(30,16,"一档动力",16,1);
    OLED_ShowChineseString(30,32,"二挡动力",16,1);
    OLED_ShowChineseString(30,48,"三挡动力",16,1);
    OLED_ShowChar(10,0,'*',16,1);
    OLED_Refresh();
}
//灯光面显示函数
void  OLED_page_show_led(void)
{
    OLED_ShowChineseString(30,0,"呼吸灯开",16,1);
    OLED_ShowChineseString(30,16,"呼吸灯关",16,1);
    OLED_ShowChineseString(30,32,"打开灯二",16,1);
    OLED_ShowChineseString(30,48,"打开灯三",16,1);
    OLED_ShowChar(10,0,'*',16,1);
    OLED_Refresh();
}
//环境显示函数
void  OLED_page_show_per(void)
{
    OLED_ShowChar(10,0,'*',16,1);
    OLED_Refresh();
}

//向上移动
void flag_chooes_up(void)
{
    OLED_My_clear_choose(flag_chooes/16);//清除符号
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

//向下移动
void flag_chooes_down(void)
{
    OLED_My_clear_choose(flag_chooes/16);//清除符号
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


//进入选择的页面
void flag_chooes_page(void)
{
    if(flag_chooes==0)
    {
        flag_chooes=0;
        OLED_MyClear();//清空页面
        oled_page=page_wifi;//切换运行函数
        OLED_page_show_wifi();//显示WiFi页面
    }
    else if(flag_chooes==16)
    {
        flag_chooes=0;
        OLED_MyClear();//清空页面
        oled_page=page_pwm;//切换运行函数
        OLED_page_show_pwm();//显示风扇页面
    }
    else if(flag_chooes==32)
    {
        flag_chooes=0;
        OLED_MyClear();//清空页面
        oled_page=page_led;//切换运行函数
        OLED_page_show_led();//显示led页面
    }
    else if(flag_chooes==48)
    {
        flag_chooes=0;
        OLED_MyClear();//清空页面
        oled_page=page_Perce;//切换运行函数
        OLED_page_show_per();//显示感知页面
    }
}



//主页面运行函数
void OLED_page_main(void)
{
    if(KEY1==0)
    {
        //向上移动并且刷新
        flag_chooes_up();
        delay_ms(40);
    }
    if(KEY2==0)
    {
        delay_ms(40);
        //向下移动并且刷新
        flag_chooes_down();
        delay_ms(40);
        delay_ms(40);

    }
    if(KEY3==0)
    {
        delay_ms(40);
        //进入选择的页面
        flag_chooes_page();
        delay_ms(40);

    }

}
//返回主页函数
void OLED_page_main_return(void)
{
    flag_chooes=0;
    OLED_MyClear();//清空页面
    oled_page=page_main;//切换运行函数
    OLED_page_show();//显示感知页面
}





//按键124封装
void OLED_KEY(void)
{
    if(KEY1==0)
    {
		delay_ms(40);
        //向上移动并且刷新
        flag_chooes_up();
        delay_ms(40);
    }
    if(KEY2==0)
    {
        delay_ms(40);
        //向下移动并且刷新
        flag_chooes_down();
        delay_ms(40);
    }
    if(KEY4==0)
    {
        delay_ms(40);
        OLED_page_main_return();//切换到主页
        delay_ms(40);
    }
}




//WiFi运行函数
void OLED_page_wifi(void)
{
    //按键124的功能封装
    OLED_KEY();
}


//风扇运行函数
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
				//重新设置选项中文
				OLED_My_clear_cinese(flag_chooes/16);
				OLED_ShowChineseString(30,0,"关闭风扇",16,0);
				OLED_Refresh();
            }
            else
            {
                pwm_stop();
				//重新设置选项中文
				OLED_My_clear_cinese(flag_chooes/16);
				OLED_ShowChineseString(30,0,"打开风扇",16,1);
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
    //按键124的功能封装
    OLED_KEY();
}


//灯光运行函数
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
				//重新设置选项中文
				OLED_My_clear_cinese(flag_chooes/16);
				OLED_ShowChineseString(30,32,"打开灯二",16,1);
				OLED_Refresh();
            }
            else
            {
                LED2 = 0;
				//重新设置选项中文
				OLED_My_clear_cinese(flag_chooes/16);
				OLED_ShowChineseString(30,32,"关闭灯二",16,1);
				OLED_Refresh();
            }
            break;
        case 48:
			if (LED3 == 0)
            {
                LED3 = 1;
				//重新设置选项中文
				OLED_My_clear_cinese(flag_chooes/16);
				OLED_ShowChineseString(30,48,"打开灯三",16,1);
				OLED_Refresh();
            }
            else
            {
                LED3 = 0;
				//重新设置选项中文
				OLED_My_clear_cinese(flag_chooes/16);
				OLED_ShowChineseString(30,48,"关闭灯三",16,1);
				OLED_Refresh();
            }
            break;
        default:
            break;
        }
		 delay_ms(100);
    }
    //按键124的功能封装
    OLED_KEY();
}
//环境运行函数
void OLED_page_Per(void)
{
    if(KEY4==0)
    {
        delay_ms(40);
        OLED_page_main_return();//切换到主页
        delay_ms(40);
    }
}








