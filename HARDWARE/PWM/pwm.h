#ifndef __PWM__
#define __PWM__
#include "sys.h"
#define beep PBout(10)
void pwm_init(void);//pwm
//�رշ���
void pwm_stop(void);
//�򿪷���
void pwm_start1(void);
//�򿪷��ȶ���λ
void pwm_start2(void);
//�򿪷�������λ
void pwm_start3(void);
//�򿪷����ĵ�λ
void pwm_start4(void);
//�򿪷����嵵λ
void pwm_start5(void);

#endif
