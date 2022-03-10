#pragma once

typedef struct event_loop_t {
    int epoll_fd;
}event_loop;

event_loop* event_loop_create();
void event_loop_run(event_loop* e);