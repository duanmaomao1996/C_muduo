#pragma once
#include<sys/time.h>

typedef struct event_t event;

typedef struct server_manager_t server_manager;

typedef void(*event_callback_ptr)(int fd, event* ev, void* arg);

struct event_t{
    int fd;
    short event_flag;
    int active_event;
    struct timeval time;
    event_callback_ptr event_read_handler;
    event_callback_ptr event_write_handler;
    void* r_arg;
    void* w_arg;

    int is_working;
    int epoll_fd;
};

event* event_create(int fd, short event_flag, event_callback_ptr read_handler, 
                    void* r_arg, event_callback_ptr write_handler, void* w_arg);

void event_start(event* ev);
void event_stop(event* ev);
void event_free(event* ev);
void event_add_io(int epoll, event* ev);
void event_enable_writing(event* ev);
void event_disable_writing(event* ev);
void event_handler(event* ev);