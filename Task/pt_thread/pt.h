
#ifndef __PT_H__
#define __PT_H__

#include "lc.h"
#include "stdint.h"

struct pt
{
    lc_t lc;
};


#ifndef UNUSED_PARAM
    #define UNUSED_PARAM(v) (void)(v)
#endif

#define PT_WAITING 0  // Waiting
#define PT_YIELDED 1  // Thread locked
#define PT_EXITED 2   // Exited
#define PT_ENDED  3   // Ended

#define PT_INIT(pt)  LC_INIT((pt)->lc)
#define PT_THREAD(name_args) int8_t name_args
#define PT_BEGIN(pt) { int8_t PT_YIELD_FLAG = 1; LC_RESUME((pt)->lc)
#define PT_END(pt) LC_END((pt)->lc); PT_YIELD_FLAG = 0; \
    UNUSED_PARAM(PT_YIELD_FLAG );\
    PT_INIT(pt); return PT_ENDED; }

#define PT_BIND(pt1) {static struct pt pt_hide ;pt1 = &pt_hide;} // Generate a PT flag

// Wait until condition is met, if not met, continue waiting
#define PT_WAIT_UNTIL(pt, condition)      \
    do {           \
        LC_SET((pt)->lc);        \
        if(!(condition)) {       \
            return PT_WAITING;      \
        }            \
    } while(0)

// Keep waiting until condition is not met
#define PT_WAIT_WHILE(pt, cond) PT_WAIT_UNTIL((pt), !(cond))
// Wait for child thread to exit
#define PT_WAIT_THREAD(pt, thread) PT_WAIT_WHILE((pt), PT_SCHEDULE(thread))

// Start child thread, wait for child thread to exit, if it doesn't exit, block here

#define PT_SPAWN(pt, child, thread)   \
    do {           \
        PT_INIT((child));        \
        PT_WAIT_THREAD((pt), (thread));   \
    } while(0)


// Restart thread from the beginning
#define PT_RESTART(pt)       \
    do {           \
        PT_INIT(pt);        \
        return PT_WAITING;     \
    } while(0)

// Exit thread
#define PT_EXIT(pt)       \
    do {           \
        PT_INIT(pt);        \
        return PT_EXITED;      \
    } while(0)
// Check if child thread has exited
#define PT_SCHEDULE(f) ((f) < PT_EXITED)

// Lock task
#define PT_YIELD(pt)        \
    do {           \
        PT_YIELD_FLAG = 0;       \
        LC_SET((pt)->lc);       \
        if(PT_YIELD_FLAG == 0) {      \
            return PT_YIELDED;     \
        }           \
    } while(0)

// Lock task until condition is met
#define PT_YIELD_UNTIL(pt, cond)    \
    do {           \
        PT_YIELD_FLAG = 0;       \
        LC_SET((pt)->lc);        \
        if((PT_YIELD_FLAG == 0) || !(cond)) {  \
            return PT_YIELDED;      \
        }            \
    } while(0)


#endif /* __PT_H__ */
