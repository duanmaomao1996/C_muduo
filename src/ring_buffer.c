#include<string.h>
#include<stdlib.h>
#include "ring_buffer.h"
#include "macro.h"

ring_buffer* ring_buffer_new()
{
    ring_buffer* rb = (ring_buffer*)duo_malloc(sizeof(ring_buffer));

    memset(rb, 0, sizeof(ring_buffer));
    return rb;
}

void ring_buffer_free(ring_buffer* rb)
{
    if (rb->msg) {
        duo_free(rb->msg);
    }
}

void ring_buffer_add_data(ring_buffer* rb, char* msg, size_t size)
{

}

char* ring_buffer_readable_start(ring_buffer* rb)
{
    return rb->msg + rb->start;
}
char* ring_buffer_get_msg(ring_buffer* rb, int* len)
{
    char* msg = rb->msg + rb->start;
    if (len) {
        int ocuSize = rb->end - rb->start;
       *len = ocuSize;
    }
    if (rb->end == rb->start) {
        return NULL;
    }
    return msg;
}

int ring_buffer_readable_bytes(ring_buffer* rb)
{
    int ocuSize = rb->end - rb->start;
    return ocuSize;
}

int ring_buffer_release_bytes(ring_buffer* rb, size_t size)
{
    if(rb->msg + rb->start) {
        memset(rb->msg + rb->start, 0, size);
    }
    rb->start += size;
    if(rb->start == rb->end) {
        rb->start = rb->end = 0;
    }
}