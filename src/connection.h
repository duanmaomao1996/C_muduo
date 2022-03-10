#pragma once

typedef struct connection_t connection;
typedef void (*connection_callback_ptr)(connection* conn);
typedef void (*message_callback_ptr)(connection* conn);
typedef struct event_t event;
typedef struct event_loop_t event_loop;

typedef struct ring_buffer_t   ring_buffer;
typedef struct socket_buffer_t socket_buffer;
typedef struct buffer_pool_t   buffer_pool;

enum {
    State_Closing = 1,
    State_Closed = 2,
};

struct connection_t {
    int connfd;
    event* conn_event;
    message_callback_ptr      message_callback;
    connection_callback_ptr   connection_callback;
    connection_callback_ptr   close_connection_callback;

    ring_buffer*              ring_buffer_in;
    ring_buffer*              ring_buffer_out;

    int                       state;
    void*                     handler;
    char*                     client_ip;
    int                       client_port;
    int                       time_on_connect;
};

connection* connection_create(int fd, message_callback_ptr callback);
void connection_start(connection* conn, event_loop* loop);
void connection_established(connection* conn);
void connection_active_close(connection* conn);
void connection_free(connection* conn);
void connection_send(connection *conn, char* buf, size_t len);
int connection_send_buffer(connection* conn);
void connection_set_close_connection_callback(connection* conn, connection_callback_ptr callback);