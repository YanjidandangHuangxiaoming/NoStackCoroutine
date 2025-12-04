#ifndef _PT_TIMER_H_
#define _PT_TIMER_H_
#include "pt.h"


struct timer
{
    unsigned long start;     // Records the tick value at start time
    unsigned long interval;  // Number of ticks to be timed
};

void timer_set(struct timer *t, unsigned long interval);
unsigned char timer_expired(struct timer *t);


// Do not disable interrupts when using these two functions
void delay_ms(int32_t nMs);
void delay_us(int32_t nUs);




#define delay_ms_noblock(pt1,t_ms)\
    do\
    {\
        static struct timer t2;\
        timer_set(&t2,t_ms);\
        PT_WAIT_UNTIL(pt1,timer_expired(&t2));\
    }\
    while(0);

#endif
