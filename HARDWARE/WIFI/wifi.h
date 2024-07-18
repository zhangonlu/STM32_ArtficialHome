#ifndef __WIFI_H_
#define __WIFI_H_

#include "sys.h"
#include "ringbuffer.h"
#include "stdio.h"
#define bound   115200

typedef struct {
	u8 head;
	u8 tail;
	u8 Data_type;
	u8 data[32];
	u8 compare;
}Tcp_data;

void uart2_init(void);
extern Ringbuffer_t buffer;
void TCP_Init(void);
void wifi_init(void);
int wifi_sendData(char *data,u32 timer);
u8 TCP_send(u8 *data,u32 size,u32 timer);
#endif
