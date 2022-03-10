#pragma once

typedef struct buffer_node_t {
    char* msg;
    int sz;
    struct buffer_node_t* next;
    char* buf;
    int cap;
}buffer_node;

typedef struct buffer_pool_t {
    buffer_node* head;
    int len;
}buffer_pool;

typedef struct socket_buffer_t {
    int size;
    int offset;
    buffer_node* head;
    buffer_node* tail;
    buffer_pool* pool;
}socket_buffer;

socket_buffer* socket_buffer_new();
void socket_buffer_free(socket_buffer* sb);
int buffer_push_data(socket_buffer* sb, char* msg, int size);
void buffer_node_release(socket_buffer* sb);
char* buffer_read_spec(socket_buffer* sb, int size, int* realSz);
char* buffer_read_all(socket_buffer* sb, int* retNum);

int buffer_get_size(socket_buffer* sb);