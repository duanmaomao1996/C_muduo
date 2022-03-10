#include<sys/epoll.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<signal.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include"macro.h"
#include"event.h"
#include"connection.h"
#include"logger.h"
#include"event_loop.h"
#include"tcpclient.h"

tcpclient* tcpclient_create(const char* ip, short port)
{
    tcpclient* cli = (tcpclient*)duo_malloc(sizeof(tcpclient));
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        debug_ret("creat socket failed, file: %s, line: %d", __FILE__, __LINE__);
        return NULL;
    }
    cli->connfd = sockfd;
    cli->ip = (char*)ip;
    cli->port = port;
    signal(SIGPIPE, SIG_IGN);
    return cli;
}

int tcpclient_connect(tcpclient* cli, connected_callback_ptr connected_callback, message_callback_ptr message_callback)
{
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, cli->ip, &server_address.sin_addr);
    server_address.sin_port = htons(cli->port);

    int ret = connect(cli->connfd, (struct sockaddr*)&server_address, sizeof(server_address));
    if (ret = -1) {
        debug_sys("connection create failed: %s , line: %d", __FILE__, __LINE__);
    }
    connection* conn = connection_create(cli->connfd, message_callback);
    if (conn == NULL) {
        debug_qui("connection create failed: %s, line: %d", __FILE__, __LINE__);
    }
    connected_callback(conn);
    return 0;
}

void tcpclient_run(event_loop* loop)
{
    event_loop_run(loop);
}