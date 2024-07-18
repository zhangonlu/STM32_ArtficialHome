#ifndef Ringbuffer_H
#define Ringbuffer_H

#include <stdbool.h>        //bool

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
#define BUFFER_SIZE 20
typedef struct {
    u8 buffer[BUFFER_SIZE];    //存储区大小
    u32 head;   //写位置
    u32 tail;   //读位置
	bool flag;
}Ringbuffer_t;
void ring_buffer_Init(Ringbuffer_t *rb);
bool ring_buffer_is_empty(Ringbuffer_t *rb);
bool ring_buffer_is_full(Ringbuffer_t *rb);
bool ring_buffer_write(Ringbuffer_t *rb,u8 data);
bool ring_buffer_read(Ringbuffer_t *rb,u8 *data);
u32 ring_buffer_read_all(Ringbuffer_t *rb,u8 *data,u32 len);
#endif
