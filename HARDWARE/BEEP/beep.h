#ifndef __BEEP_H__
#define __BEEP_H__
#include "sys.h"
/*--------- BEEP位带宏-----------*/
#define BEEP PBout(10)

void BEEP_Init(void);		//BEEP蜂鸣器初始化函数

#endif

