#pragma once

typedef struct connection_t connection;
typedef void (*connection_callback_ptr)(connection* conn);
typedef void(*message_callback_ptr)(connection* conn);

typedef struct event_loop_t event_loop;
typedef struct timer_ timer;
typedef struct timer_manager_t timer_manager;


typedef struct server_manager_t {
    int         epoll_fd;
    int         listen_port;
    int         loop_num;
    event_loop* loop;
    connection_callback_ptr new_connection_callback;
    message_callback_ptr    msg_callback;

    timer_manager* timer_manager;
}server_manager;

server_manager* server_manager_create(int port, int thread_num);
void server_manager_run(server_manager* manager);
void server_manager_add_timer(server_manager* manager, timer ti);
