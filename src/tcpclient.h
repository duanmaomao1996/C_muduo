#pragma once


typedef struct event_loop_t event_loop;
typedef struct connection_t connection;
typedef void (*message_callback_ptr)(connection* conn);
typedef struct tcpclient_t {
    int connfd;
    char* ip;
    short   port;
    event_loop*  loop;
}tcpclient;

tcpclient* tcpclient_create(const char* ip, short port);

typedef void (*connected_callback_ptr)(connection* conn);

int tcpclient_connect(tcpclient* cli, connected_callback_ptr connected_callback, message_callback_ptr  msg_callback);

int tcpclient_send(tcpclient* cli);

void tcpclient_run(event_loop* loop);