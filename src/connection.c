#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<sys/epoll.h>
#include<sys/ioctl.h>
#include<sys/socket.h>
#include<errno.h>
#include<string.h>
#include<sys/uio.h>
#include"connection.h"
#include"event_loop.h"
#include"event.h"
#include"macro.h"
#include"ring_buffer.h"
#include"logger.h"


static void connection_passive_close(connection* conn);
static void connection_disconnect(connection* conn);
static void event_readable_callback(int fd, event* ev, void* arg);
static void event_writable_callback(int fd, event* ev, void* arg);

connection* connection_create(int connfd, message_callback_ptr msg_callback) 
{
    connection* conn = (connection*)duo_malloc(sizeof(connection));
    if (conn == NULL) {
        debug_ret("create connection failed, file: %s, line: %d", __FILE__, __LINE__);
        return NULL;
    }
    memset(conn, 0, sizeof(connection));
    conn->connfd = connfd;
    conn->message_callback = msg_callback;

    event* ev = (event*) event_create(conn, EPOLLIN | EPOLLPRI, event_readable_callback, conn, event_writable_callback, conn);
    if (ev == NULL) {
        debug_ret("create event failed , file: %s, line: %d", __FILE__, __LINE__);
        duo_free(conn);
        return NULL;
    }
    conn->conn_event = ev;
    return conn;
}

void connection_start(connection* conn, event_loop* loop)
{
    if (! conn->ring_buffer_in) {
        conn->ring_buffer_in = ring_buffer_new();
    }
    if (! conn->ring_buffer_out) {
        conn->ring_buffer_out = ring_buffer_new();
    }
}

static int read_buffer(int fd, connection* conn)
{
    int read_byte = 65536;
    char extrabuf2[read_byte];
    struct iovec vec[2];

    char* start = ring_buffer_readable_start(conn->ring_buffer_in);
    int available_bytes = ring_buffer_available_bytes(conn->ring_buffer_in);
    vec[0].iov_base = start;
    vec[0].iov_len = available_bytes;
    vec[1].iov_base = extrabuf2;
    vec[1].iov_len = read_byte;

    ssize_t nread = readv(fd, vec, 2);
    if (nread == 0) {
        return 0;
    }
    else if (nread < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return -1;
        }
        else {
            debug_msg("read n < 0, fd : %d, file: %s, line: %d", conn->connfd, __FILE__, __LINE__);
            return -1;
        }
    }
    else {
        conn -> ring_buffer_in -> end += available_bytes;
        ring_buffer_add_data(conn->ring_buffer_in, extrabuf2, nread - available_bytes);
        return nread;
    }
    return -1;
}

static void event_readable_callback(int fd, event* ev, void* arg)
{
    connection* conn = (connection*)arg;
    int nread = read_buffer(fd, conn);
    if (nread > 0 && conn->message_callback) {
        conn->message_callback(conn);
    }
    else if (nread <= 0) {
        connection_passive_close(conn);
    }
}

static void event_writable_callback(int fd, event* ev, void* arg)
{
    int len = 0;
    connection* conn = (connection*)arg;
    char* msg = ring_buffer_get_msg(conn->ring_buffer_out, &len);
    if (msg && len > 0) {
        int n = send(conn->connfd, msg, len, 0);
        if (n > 0) {
            ring_buffer_release_bytes(conn->ring_buffer_out, n);
            len = ring_buffer_readable_bytes(conn->ring_buffer_out);
            if (len == 0) {
                event_disable_writing(conn->conn_event);
                if (conn->state == State_Closing) {
                    connection_free(conn);
                    conn->state = State_Closed
                }
            }
        }
    }
}


static void connection_passive_close(connection* conn)
{
    connection_disconnect(conn);
}

void connection_established(connection* conn)
{
    if (conn->connection_callback)
        conn->connection_callback(conn);
}

void connection_active_close(connection* conn)
{
    connection_disconnect(conn);
}

void connection_set_close_callback(connection* conn, connection_callback_ptr callback)
{   
    conn->close_connection_callback = callback;
}

static void connection_disconnect(connection* conn)
{
    conn->state = State_Closing;
    if (ring_buffer_readable_bytes(conn->ring_buffer_out) > 0) {
        event_enable_writing(conn->conn_event);
    }
    else {
        connection_free(conn);
        conn->state = State_Closed;
    }
}

void connection_free(connection* conn)
{
    if (conn->close_connection_callback) {
        conn->close_connection_callback(conn);
    }

    event_free(conn->conn_event);

    if (conn->ring_buffer_in) {
        ring_buffer_free(conn->ring_buffer_in);
    }
    if (conn->ring_buffer_out) {
        ring_buffer_free(conn->ring_buffer_out);
    }
    duo_free(conn);
}

void connection_send(connection* conn, char* buf, size_t len)
{
    if (ring_buffer_readable_bytes(conn->ring_buffer_out) == 0) {
        int ret = send(conn->connfd, buf, len, 0);
    }
    else {
        printf("connection_send %d \n", len);
        ring_buffer_add_data(conn->ring_buffer_out, buf, len);
        event_enable_writing(conn->conn_event);
    }
}

int connection_send_buffer(connection* conn)
{
    int len = 0;
    char* msg = ring_buffer_get_msg(conn->ring_buffer_out, &len);
    if (msg && len > 0) {
        int n = send(conn->connfd, msg, len, 0);
        if (n == -1) {
            return -1;
        }
        if (n > 0) {
            ring_buffer_release_bytes(conn->ring_buffer_out, n);
            if (n < len) {
                event_enable_writing(conn->conn_event);
                return 1;
            }
            else {
                return 0;
            }
        }
    }
    return -1;
}