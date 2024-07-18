#include "ringbuffer.h"
void ring_buffer_Init(Ringbuffer_t *rb)
{
    rb->head = 0;
    rb->tail = 0;
	rb->flag = false;
}

inline bool ring_buffer_is_empty(Ringbuffer_t *rb)
{
    return rb->head == rb->tail;
}

inline bool ring_buffer_is_full(Ringbuffer_t *rb)
{
    u32 next_head = (rb->head + 1) % BUFFER_SIZE;   //计算下一个写的位置
    return  next_head == rb->tail;
}

inline bool ring_buffer_write(Ringbuffer_t *rb,u8 data)
{
    if(ring_buffer_is_full(rb) == true) return false;   //失败

    rb->buffer[rb->head] = data;    //写入数据
    rb->head = (rb->head + 1) % BUFFER_SIZE;    //更新写的位置

    return true;    //成功
}

inline bool ring_buffer_read(Ringbuffer_t *rb,u8 *data)
{
    if(ring_buffer_is_empty(rb) == true) return false;  //失败

    *data = rb->buffer[rb->tail];
    rb->tail = (rb->tail+1) % BUFFER_SIZE;    //更新读的位置

    return true;    //成功
}

u32 ring_buffer_read_all(Ringbuffer_t *rb,u8 *data,u32 len)
{
    u32 i = 0;
    for(i = 0; i < len; i++)
    {
        if(ring_buffer_read(rb,data++) == false) break;
    }
    return i;
}
