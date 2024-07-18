#ifndef __PWM__
#define __PWM__
#include "sys.h"
#define beep PBout(10)
void pwm_init(void);//pwm
//关闭风扇
void pwm_stop(void);
//打开风扇
void pwm_start1(void);
//打开风扇二档位
void pwm_start2(void);
//打开风扇三档位
void pwm_start3(void);
//打开风扇四档位
void pwm_start4(void);
//打开风扇五档位
void pwm_start5(void);

#endif
