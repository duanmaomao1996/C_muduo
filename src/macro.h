#pragma once
#include<stdlib.h>
#ifndef bool
    #define bool int
#endif
#ifndef NULL
    #define NULL 0
#endif

#ifndef true
    #define true 1
#endif

#ifndef false
    #define false 0
#endif

#define duo_malloc malloc

#define duo_free free

#define MAX_PAGE_NODE    12
#define DEFAULT_PORT     2022
#define MAX_LOOP         8
#define MAX_TIME_EVENT   100
#define MAX_EVENT        32

