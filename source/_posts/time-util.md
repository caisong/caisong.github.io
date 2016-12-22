---
title: 时间相关函数
date: 2016-12-19 22:44:11
tags: time
category: program
---

```c
#incldue <time.h>

struct tm  //calendar time type
{
  int tm_sec; //[0,60](since C99)
  int tm_min; //[0,59]
  int tm_hour; //[0,23]
  int tm_mday; //[1,31]
  int tm_mon; //[0,11]
  int tm_year; //year since 1900
  int tm_wday; //days since Sunday:[0~6]
  int tm_wday; //day since Jan 1:[0,365]
  int tm_isdst;//夏令时时间，>0:有效	=0：无效	<0: 不可用
};

time_t; //calendar time since epoch type

clock_t; //processor time since era type

struct timespce{  //time in seconds and nanoseconds
  time_t tv_sec;
  long tv_nsec;
};

time_t time(time_t *calptr); //returns the current calendar time of the system as  ime since epoch
double difftime(time_t time_end, time_t time_beg); //computes the difference between times

/*
* format conversions
*/
char* asctime( const struct tm *time_ptr); //converts a tm object to a textual representation
char* ctime(const time_t *time); //converts a time_t object to a textual representation; as if by calling asctime(localtime(time));
struct tm *gmtime(const time_t *time); //convert time since epoch calendar time expressedd as UTC
struct tm *localtime(const time_t *time); //converts time since epoch calendar time expressed as local time 
time_t mktime(struct tm *time); //converts calendar time to time since epoch

size_t strftime(char *str, size_t count, const char *format, const struct tm *time); //converts a tm object to custom textual representation
char *strptime(const char *s, const char *format, struct tm *tm); //converse of strftime

#include <sys/time.h>
/*
clockid_t :
CLOCK_REALTIME: 实时系统时间
CLOCK_MONOTONIC: 不带负跳数的实时系统时间
CLOCK_PROCESS_CPUTIME_ID: 调用进程CPU时间 
CLOCK_THREAD_CPUTIME_ID: 调用线程CPU时间
*/
int clock_gettime(clockid_t clock_id, struct timespec *tsp); //获取指定时钟的时间
int clock_getres(clockid_t clock_id, struct timespec *tsp); //设置指定时钟精度

int clock_settime(clockid_t clodk_id, const struct timespec *tsp);//设置时钟时间
int gettimeofday(struct timeval *restrict tp, void *restrict tzp);//获取当前时间，与其他相同作用函数相比，精度较高
```

![time_util](/images/time_util.jpg)