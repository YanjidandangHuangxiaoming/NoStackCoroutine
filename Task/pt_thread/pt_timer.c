#include "pt_timer.h"
#include "task.h"
 
#define clock_time get_system_ms
// Start timer
void timer_set(struct timer *t, unsigned long interval)
{
    t->interval = interval; // Store the required delay time in microseconds
    t->start = clock_time(); // Get current microsecond value
}

// Timer timeout check
unsigned char timer_expired(struct timer *t)
{
    unsigned long now = clock_time() ;
    unsigned long err = now - t->start;
    // clock_time() gets the current tick value, if current value - start value exceeds the delay tick count, it means timeout
    unsigned char overTime = err >= t->interval;
    //_nop_();
    return overTime; // What about overflow? // No problem, negative - positive is still a positive number
}



/*
// Delay, must be in interrupt-enabled state
void delay_ms(int t_ms)
{
 struct pt pt1;
 static struct timer input_timer;

 timer_set(&input_timer,t_ms);
 while(!timer_expired(&input_timer));
}
*/


