#ifndef __KEY_H__
#define __KEY_H__
#include "sys.h"
/*--------- KEYλ���� -----------*/
#define KEY1 PEin(4)
#define KEY2 PEin(5)
#define KEY3 PEin(6)
#define KEY4 PCin(13)

void KEY_Init(void);	//������ʼ��
u8 KEY_Scan(void);		//����ɨ��

#endif
