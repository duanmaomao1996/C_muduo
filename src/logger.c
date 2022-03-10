#include<stdarg.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>

#define MAXLINE 128

#define DBG_PRINTF debug_print

static void debug_print(int, const char*, va_list);

void debug_ret(const char* fmt,...)
{
    va_list ap;
    va_start(ap, fmt);
    DBG_PRINTF(1, fmt, ap);
    va_end(ap);
    exit(1);
}

void debug_sys(const char* fmt,...)
{
    va_list ap;
    va_start(ap, fmt);
    DBG_PRINTF(1, fmt, ap);
    va_end(ap);
    exit(1);
}

void debug_qui(const char* fmt,...)
{
    va_list ap;
    va_start(ap, fmt);
    DBG_PRINTF(0, fmt, ap);
    va_end(ap);
    exit(1);
}

void debug_msg(const char* fmt,...)
{
    va_list ap;
    va_start(ap, fmt);
    DBG_PRINTF(0, fmt, ap);
    va_end(ap);
    exit(1);
}


static void debug_print(int errnoflag, const char* fmt, va_list ap)
{
    int errno_save, n;
    char buf[MAXLINE + 1];
    errno_save = errno;
    vsnprintf(buf, MAXLINE, fmt, ap);
    n = strlen(buf);
    if (errnoflag) {
        snprintf(buf + n, MAXLINE - n, ": %s", strerror(errno_save));
    }
    strcat(buf, "\n");
    fflush(stderr);
    fputs(buf, stderr);
    fflush(stderr);
}