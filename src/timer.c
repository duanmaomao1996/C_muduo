#include<string.h>
#include "macro.h"
#include "timer.h"

timer_manager* timer_manager_create()
{
    int size = sizeof(timer_manager);
    timer_manager* m = (timer_manager*)duo_malloc(size);
    if (m == NULL) {
        return NULL;
    }
    memset(m, 0, size);
    m->cap = MAX_TIME_EVENT;
    m->queue0 = (timer**)duo_malloc(size * m->cap);
    m->queue1 = (timer**)duo_malloc(size * m->cap);
    memset(m->queue1, 0, size * m->cap);
    memset(m->queue0, 0, size * m->cap);
    return m;
}

void timer_manager_free(timer_manager* manager)
{
    if (manager) {
        timer** queue = manager->queue0;
        if (manager->index == 1) {
            queue = manager->queue1;
        }
        for (int i = 0; i < manager->cap; i++) {
            if (queue[i]) {
                duo_free(queue[i]);
            }
            else {
                break;
            }
        }
        if (manager->queue0)  duo_free(manager->queue0);
        if (manager->queue1)  duo_free(manager->queue1);
    }
}

timer timer_manager_get_top(timer_manager* manager) 
{
    return *manager->top;
}