#pragma once

typedef void (*timeout_callback_ptr)(void *arg);

//计时器处理模式
enum TimerOptions {
    TIMER_OPT_NONE,
    TIMER_OPT_ONCE,
    TIMER_OPT_REPEAT
};


//计时器结构体
typedef struct timer_ {
    int time_out;
    int time_left;
    enum TimerOptions type;
    timeout_callback_ptr callback;
    void *arg;
}timer;

typedef struct timer_manager_t {
    timer*   top;
    timer**  queue0;
    timer**  queue1;
    int      index;
    int      size;
    int      cap;
    int      top_index;
}timer_manager;

timer_manager* timer_manager_create();

void timer_manager_free(timer_manager* manager);

timer* timer_new(int time);

void timer_manager_add(timer_manager* manager, timer tm);

timer timer_manager_pop(timer_manager* manager);

timer timer_manager_get_top(timer_manager* manager);

void timer_manager_update(timer_manager* manager, int timeout);