#include<stdlib.h>
#include<string.h>
#include "buffer.h"
#include "macro.h"

socket_buffer* socket_buffer_new()
{
    socket_buffer* sb = (socket_buffer*)duo_malloc(sizeof(socket_buffer));
    sb->size = 0;
    sb->offset = 0;
    sb->head = sb->tail = NULL;
    sb->pool = (buffer_pool*)duo_malloc(sizeof(buffer_pool));
    sb->pool->head = NULL;
    sb->pool->len = 0;
    return sb;
}

void socket_buffer_free(socket_buffer* sb)
{
    buffer_node* head = sb->pool->head;
    for(; head != NULL; head = head->next) {
        if (head->msg) {
            duo_malloc(head->msg);
            head->msg = NULL;
        }
    }
}

buffer_node* new_buffer_node(int size)
{
    buffer_node** pool = (buffer_node**)duo_malloc(sizeof(buffer_node*) * size);
    int i = 0;
    for (i = 0; i < size; i++)
    {
        pool[i] = (buffer_node*)duo_malloc(sizeof(buffer_node));
    }
    for (i = 0; i < size - 1; i++)
    {
        pool[i]->msg = NULL;
        pool[i]->sz = 0;
        pool[i]->next = pool[i + 1];
    }
    pool[size - 1]->next = 0;
    return pool[0];
}