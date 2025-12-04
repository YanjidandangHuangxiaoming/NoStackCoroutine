

#include "task.h"
#include "task_port.h"
#include "string.h"

AL_STATIC           TIMER_DATA  TimerData[MAX_TIMER]    ;
AL_STATIC volatile  uint32_t    TMP1msCount             = 0;
AL_STATIC volatile  uint32_t    AllTickCount            = 0;


#if(TASK_WITH_PT) 
PT_THREAD(TaskTimer(struct pt* pt1));
#else
AL_STATIC int8_t TaskTimer(void);
#endif

 
AL_STATIC TASK_DATA TaskDb[] = {
    {1, TASK_MODE_PERIOD,TASK_ID(TaskTimer),0, 
#if TASK_WITH_NAME 
    TASK_NAME(TaskTimer),
#endif // TASK_WITH_NAME
    1L,1L,TaskTimer,
#if TASK_WITH_PT
    {0}
#endif 
    },
    #include "task_container.h"
};


void task_systick_handler(void)
{
    TMP1msCount++;
    AllTickCount++; 
    //printf("Interrupt simulated! Count = %d\n", AllTickCount);
}


void TaskFunc(void)
{ 
    int8_t ret = 0;
    uint8_t j = 0;
    for (j = 0; j < sizeof(TaskDb) / sizeof(TaskDb[0]); j++)
    {
        if (!TaskDb[j].cActive)
            continue;

        TaskDb[j].lLastTime -= TMP1msCount;
        if (TaskDb[j].lLastTime <= 0)
        { 
            if (TaskDb[j].taskproc != NULL)
            {
                #if(TASK_WITH_PT)
                struct pt* pt2 = &(TaskDb[j].pt1);
                ret = TaskDb[j].taskproc(pt2);
                #else  
                ret = TaskDb[j].taskproc();
                #endif
            }
            TaskDb[j].lLastTime += TaskDb[j].lPeriod;
            if (TaskDb[j].cMode == TASK_MODE_ONCE)
                TaskDb[j].cActive = 0;
        }
    }
    TMP1msCount = 0;
    UNUSED(ret); 
}

// Periodic task processing function declaration
void TaskProc(void)
{
    while (1)
    {
        if (!TMP1msCount)
        {
            Idle();
        }
        else
        {
            TaskFunc();
        }
    }
}

// Activate task
int8_t ActiveTask(Task_id taskid, int32_t lPeriod)
{
    int8_t result = -1;
    unsigned int i;

    for (i = 0; i < sizeof(TaskDb) / sizeof(TaskDb[0]); i++)
    {
        if (TaskDb[i].cTaskID == taskid)
        {
            if (TaskDb[i].cActive == 0)
            {
                TaskDb[i].cActive = 1;
                TaskDb[i].lPeriod = lPeriod;
                TaskDb[i].lLastTime = taskid<lPeriod? taskid:0;  // first run immediately 避免公倍数问题
            }
            result = 0;
            break;
        }
    }
    return result;
}

// Suspend task
int8_t SuspendTask(Task_id taskid)
{
    int8_t result = -1;
    int8_t i;
    for (i = 0; i < sizeof(TaskDb) / sizeof(TaskDb[0]); i++)
    {
        if (TaskDb[i].cTaskID == taskid)
        {
            result = i;
            TaskDb[i].cActive = 0;
            break;
        }
    }
    return result;
}

// Start timer (variant with TimerID)
int8_t StartTimer2(int8_t *TimerID, int8_t cMode, int32_t lPeriod, timerProc timerProc, void *Param1)
{
    int8_t id = StartTimer(cMode, lPeriod, timerProc, Param1);
    TimerData[id].TimerID = TimerID;
    *TimerID = id;
    return id;
}

// Start timer
int8_t StartTimer(int8_t cMode, int32_t lPeriod, timerProc timerProc, void *Param1)
{ 
    int8_t  i;
    TIMER_DATA* ptimer;

    ptimer = &TimerData[0];
    for (i = 0; i < MAX_TIMER; i++)
    {
        if (!ptimer[i].cUse)
        {
            switch (cMode)
            {
            case TIMER_MODE_TIMER:              // timer only
            case TIMER_MODE_ONCEROUTINE:        // timely routine
            case TIMER_MODE_CYCROUTINE:         // cyclely routine
                break;
            default:
                ptimer[i].cUse = 0;
                return -1;                      // error
            }
            ptimer[i].lPeriod = lPeriod;              // set timer period
            ptimer[i].lLastTime = lPeriod;                // get current system time as start timer
            ptimer[i].timerProc = timerProc;              // user timer's routine
            ptimer[i].cMode = cMode;                // timer's work mode
            ptimer[i].cUse = 1;
            ptimer[i].Param1 = Param1;
            return (i);                             // return timer number
        }
    }
    //if(i== MAX_TIMER);
#ifdef _DEBUG
    printf("timer id not found!");
#endif
    return (-1);                                    // no timer available
}


// Stop timer
int8_t StopTimer(int8_t *pcTimerNumber )
{
    if (*pcTimerNumber >= MAX_TIMER || *pcTimerNumber == -1)
        return -1;                            // timer number error

    TimerData[*pcTimerNumber].cUse = 0;
    *pcTimerNumber = -1;

    return 0;
}


// Stop all timers
void StopAllTimer(void)
{
    unsigned char i;
    memset(TimerData, 0, sizeof(TimerData));
    for (i = 0; i < MAX_TIMER; i++)
    {
        TimerData[i].cUse = 0;
    }
}

// Initialize task module
void InitTask(void)
{
    task_systick_init();
    StopAllTimer();
}


// Get system time (milliseconds)
int32_t get_system_ms(void)
{
    return AllTickCount;
}

 


// The callback timer executes a callback function one or more times after a specified time
#if(TASK_WITH_PT) 
PT_THREAD(TaskTimer(struct pt* pt1))
#else
AL_STATIC int8_t TaskTimer(void)
#endif
{
    TIMER_DATA* pTimer;
    unsigned char i;
    pTimer = &TimerData[0];                                         // pointer to the first timer
    for (i = 0; i < MAX_TIMER; i++)
    {
        if (!pTimer[i].cUse)
            continue;
        if ((--(pTimer[i].lLastTime)) <= 0)
        {
            switch (pTimer[i].cMode)
            {
            case TIMER_MODE_TIMER:                          // timer only
                pTimer[i].cUse = 0;
                if (pTimer[i].TimerID != NULL)
                    *pTimer[i].TimerID = -1;
                break;
            case TIMER_MODE_ONCEROUTINE:                    // timer routine once
                pTimer[i].cUse = 0;                          //prevents the start of timer after stoptimer in the routine from causing an error stop. 
                if (pTimer[i].TimerID != NULL)
                    *pTimer[i].TimerID = -1;

                pTimer[i].lLastTime += pTimer[i].lPeriod;
                if (pTimer[i].timerProc != NULL)
                    pTimer[i].timerProc(pTimer[i].Param1);    // start user's routine
                break;
            case TIMER_MODE_CYCROUTINE:                     // cyclely routine
                pTimer[i].lLastTime += pTimer[i].lPeriod;
                if (pTimer[i].timerProc != NULL)
                    pTimer[i].timerProc(pTimer[i].Param1);    // start user's routine
                break;
            default:
                pTimer[i].cUse = 0;
                break;
            }
        }
    }
#if(TASK_WITH_PT) 
    UNUSED(pt1);
#endif
    return 0;
}
