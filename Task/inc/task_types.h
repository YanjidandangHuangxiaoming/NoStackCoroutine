#ifndef _TASK_TYPES_H
#define _TASK_TYPES_H
#include "al_helper.h"


#include "task_cfg.h"
#if(TASK_WITH_PT)
#include "pt_header.h"
#endif




typedef enum TASK_MODE
{
    TASK_MODE_ONCE,
    TASK_MODE_PERIOD
} TASK_MODE;



typedef enum TIMER_MODE
{
    TIMER_MODE_TIMER = 0,
    TIMER_MODE_ONCEROUTINE,
    TIMER_MODE_CYCROUTINE
} TIMER_MODE;



#if(TASK_WITH_PT)
    typedef int8_t (*ptTaskProc)(struct pt *pt1);
#else
    typedef int8_t (*ptTaskProc)(void);
#endif

typedef int8_t (*timerProc)(void *Param1);


typedef struct mytask
{
    int8_t   cActive;      //
    TASK_MODE   cMode;
    int8_t   cTaskID;      //
    int8_t   reserve;      //
#if(TASK_WITH_NAME)
    int8_t   cTaskName[TASK_NAME_LEN];
#endif
    int32_t   lLastTime;
    int32_t   lPeriod;      //
    ptTaskProc taskproc;      //

#if(TASK_WITH_PT)
    struct pt  pt1; 
#else

#endif
} TASK_DATA, *PTASK_DATA;

// define timer data struct
typedef struct mytimer
{
    int8_t      cUse;
    int8_t*     TimerID;      //reference to timer id
    TIMER_MODE      cMode;         // timer work mode
    int8_t      reserve1;
    int32_t     lPeriod;        // timer period, unit=10ms.
    int32_t     lLastTime;       // timer count for reload
    timerProc   timerProc;       // user's timer routine
    void        *Param1;
} TIMER_DATA, *PTIMER_DATA;

#endif
