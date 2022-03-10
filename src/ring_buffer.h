#pragma once

typedef struct ring_buffer_t {
    int start;
    int end;
    int cap;
    char* msg;
}ring_buffer;

ring_buffer* ring_buffer_new();

void ring_buffer_free(ring_buffer* rb);
void ring_buffer_add_data(ring_buffer* rb, char*msg, size_t size);
char* ring_buffer_readable_start(ring_buffer* rb);
int ring_buffer_readable_bytes(ring_buffer* rb);
int ring_buffer_available_bytes(ring_buffer* rb);
char* ring_buffer_get_msg(ring_buffer* rb, int len);
void ring_buffer_release_bytes(ring_buffer* rb, int size);