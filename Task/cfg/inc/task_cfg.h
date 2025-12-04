#ifndef _TASK_CFG_H_
#define _TASK_CFG_H_

#include "task_def.h"

#define TASK_WITH_PT        1
#define TASK_WITH_NAME      1
#define TASK_NAME_LEN       16 


#define MAX_TIMER           5          // the number of timer can be used 
#define TICK_PER_S          1000       // 1000 ticks per second
#define DATA_DUMMY          0xFF


typedef enum Task_id
{
    TASK_ID(TaskTimer),
    TASK_ID(TaskBlinLed),
    TASK_ID(TASK_MAX_ID)
} Task_id;

 

#endif
